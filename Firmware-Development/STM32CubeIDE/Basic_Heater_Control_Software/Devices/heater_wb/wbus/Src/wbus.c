#include "wbus.h"
#include "wbus_const.h"

#define WBMSGLEN_MAX 32

#include "usbd_cdc_if.h"  // For CDC_Transmit_FS
#include <stdio.h>
#include <stdarg.h>

#define CDC_TX_BUFFER_SIZE 256
#define CDC_MAX_PACKET_SIZE 64  // USB FS CDC max packet size

void CDC_Printf(const char *fmt, ...)
{
    char buffer[CDC_TX_BUFFER_SIZE];
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    if (len < 0) return; // encoding error
    if (len > sizeof(buffer)) len = sizeof(buffer);

    int offset = 0;
    uint8_t result;

    while (offset < len) {
        int chunk = (len - offset > CDC_MAX_PACKET_SIZE) ? CDC_MAX_PACKET_SIZE : (len - offset);

        do {
            result = CDC_Transmit_FS((uint8_t *)(buffer + offset), chunk);
        } while (result == USBD_BUSY);

        offset += chunk;
    }
}
/*
 * \param buf pointer to ACK message part
 * \param len length of data
 * \param chk initial value of the checksum. Useful for concatenating.
 */
static unsigned char checksum(unsigned char *buf, unsigned char len, unsigned char chk)
{	 
  for (;len!=0; len--) {
    chk ^= *buf++;
  }
 
  return chk;
}

/**
 * Send request to heater and one or two consecutive buffers.
 * \param wbus wbus handle
 * \param cmd wbus command to be sent
 * \param data pointer for first buffer.
 * \param len length of first data buffer.
 * \param data pointer to an additional buffer.
 * \param len length the second data buffer.
 * \return 0 if success, 1 on error.
 */
 int wbus_msg_send( UART_HandleTypeDef uartHandle,
                            unsigned char addr,
                          unsigned char cmd,
                          unsigned char *data,
                          int len,
                          unsigned char *data2,
                          int len2)
{
  unsigned char bytes, chksum;
  unsigned char buf[4];
	 
  /* Assemble packet header */
  buf[0] = addr;
  buf[1] = len + len2 + 2;
  buf[2] = cmd;

  chksum = checksum(buf, 3, 0);
  chksum = checksum(data, len, chksum);
  chksum = checksum(data2, len2, chksum);

  /* Send message */
  uint8_t dummy;
  while (HAL_UART_Receive(&uartHandle, &dummy, 1, 10) == HAL_OK) {
      // flush input buffer by reading out garbage
      CDC_Printf("flushed buffer\n");
  }

  HAL_UART_Transmit(&uartHandle, buf, 3, HAL_MAX_DELAY);
  HAL_UART_Transmit(&uartHandle, data, len, HAL_MAX_DELAY);
  HAL_UART_Transmit(&uartHandle, data2, len2, HAL_MAX_DELAY);
  HAL_UART_Transmit(&uartHandle,  &chksum, 1, HAL_MAX_DELAY);

  CDC_Printf("Packet Sent: ");
  for (int i = 0; i < 3; i++) { // header
      CDC_Printf("%02X ", buf[i]);
  }
  for (int i = 0; i < len; i++) { // first data block
      CDC_Printf("%02X ", data[i]);
  }
  if (data2 != NULL && len2 > 0) { // optional second data block
      for (int i = 0; i < len2; i++) {
          CDC_Printf("%02X ", data2[i]);
      }
  }
  CDC_Printf("%02X\n", chksum); // checksum at the end

  
  // /* Read and check echoed header */
  // HAL_StatusTypeDef status = HAL_UART_Receive(&uartHandle, buf, 3, HAL_MAX_DELAY);
  // if (status != HAL_OK) {
  //     CDC_Printf("wbus_msg_send() K-Line error. UART receive failed (header)\n");
  //     return -1;
  // }
  // if (buf[0] != addr || buf[1] != (len + len2 + 2) || buf[2] != cmd) {
  //     CDC_Printf("wbus_msg_send() K-Line error: %02X %02X %02X\n", buf[0], buf[1], buf[2]);
  //     return -1;
  // }
  
  // /* Read and check echoed data */
  // for (int i = 0; i < len; i++) {
  //     status = HAL_UART_Receive(&uartHandle, buf, 1, HAL_MAX_DELAY);
  //     if (status != HAL_OK || buf[0] != data[i]) {
  //         CDC_Printf("wbus_msg_send() K-Line error. Data mismatch at %d: expected %02X, got %02X\n", i, data[i], buf[0]);
  //         return -1;
  //     }
  // }
  
  // for (int i = 0; i < len2; i++) {
  //     status = HAL_UART_Receive(&uartHandle, buf, 1, HAL_MAX_DELAY);
  //     if (status != HAL_OK || buf[0] != data2[i]) {
  //         CDC_Printf("wbus_msg_send() K-Line error. Data2 mismatch at %d: expected %02X, got %02X\n", i, data2[i], buf[0]);
  //         return -1;
  //     }
  // }
  
  // /* Check echoed checksum */
  // status = HAL_UART_Receive(&uartHandle, buf, 1, HAL_MAX_DELAY);
  // if (status != HAL_OK || buf[0] != chksum) {
  //     CDC_Printf("wbus_msg_send() K-Line error. Checksum mismatch: expected %02X, got %02X\n", chksum, buf[0]);
  //     return -1;
  // }
  
  return 0;
}

/*
 * Read answer from wbus
 * addr: source/destination address pair to be expected or returned in case of host I/O
 * cmd: if pointed location is zero, received client message, if not send client message
        with command *cmd.
 * data: buffer to either receive client data, or sent its content if client (*cmd!=0).
         If *data is !=0, then *data amount of data bytes will  be skipped.
 * dlen: out: length of data.
 */


static int wbus_msg_recv(UART_HandleTypeDef uartHandle,
                         unsigned char *addr,
                         unsigned char *cmd,
                         unsigned char *data,
                         int *dlen,
                         int skip)
{
    unsigned char header[3];       // addr, len, cmd
    unsigned char checksum_rx;     // checksum from sender
    unsigned char chksum_calc;     // calculated checksum
    unsigned char packet_bytes[256]; // store entire packet for printing
    int packet_index = 0;          // track how many bytes stored
    int total_len;                 // total length from header
    int data_len;                  // actual payload length
    HAL_StatusTypeDef status;

    // --- Step 1: Read header ---
    status = HAL_UART_Receive(&uartHandle, header, 3, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        CDC_Printf("wbus_msg_recv(): Failed to read header\n");
        return -1;
    }

    // Store header in packet_bytes
    memcpy(packet_bytes, header, 3);
    packet_index = 3;

    *addr = header[0];
    total_len = header[1]; // total length = cmd + data + checksum
    *cmd  = header[2];

    // --- Step 2: Start checksum calculation ---
    chksum_calc = header[0] ^ header[1] ^ header[2];

    // --- Step 3: Calculate payload size ---
    data_len = total_len - 2 - skip; // remove cmd & checksum
    if (data_len < 0) data_len = 0;

    // --- Step 4: Skip bytes if needed ---
    for (int i = 0; i < skip; i++) {
        unsigned char dummy;
        status = HAL_UART_Receive(&uartHandle, &dummy, 1, HAL_MAX_DELAY);
        if (status != HAL_OK) {
            CDC_Printf("wbus_msg_recv(): Error skipping bytes\n");
            return -1;
        }
        chksum_calc ^= dummy;
        packet_bytes[packet_index++] = dummy;
    }

    // --- Step 5: Read payload data ---
    if (data_len > 0) {
        status = HAL_UART_Receive(&uartHandle, data, data_len, HAL_MAX_DELAY);
        if (status != HAL_OK) {
            CDC_Printf("wbus_msg_recv(): Failed to read payload\n");
            return -1;
        }
        for (int i = 0; i < data_len; i++) {
            chksum_calc ^= data[i];
            packet_bytes[packet_index++] = data[i];
        }
    }
    *dlen = data_len;

    // --- Step 6: Read received checksum ---
    status = HAL_UART_Receive(&uartHandle, &checksum_rx, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        CDC_Printf("wbus_msg_recv(): Failed to read checksum\n");
        return -1;
    }
    packet_bytes[packet_index++] = checksum_rx;

    // --- Step 7: Verify checksum ---
    if (checksum_rx != chksum_calc) {
        CDC_Printf("wbus_msg_recv(): Checksum mismatch! RX=0x%02X CALC=0x%02X\n",
               checksum_rx, chksum_calc);
        return -1;
    }

    // --- Step 8: Print full packet ---
    CDC_Printf("Packet Received: ");
    for (int i = 0; i < packet_index; i++) {
        CDC_Printf("%02X ", packet_bytes[i]);
    }
    CDC_Printf("\n");

    return 0; // success
}

void wbus_init(UART_HandleTypeDef uartHandle)
{
  /* Initialization sequence */
    HAL_LIN_SendBreak(&uartHandle);      // Start BREAK
    HAL_Delay(50);
   /* Empty all queues. BRK toggling may cause a false received byte (or more than one who knows). */
    uint8_t dummy;
    while (HAL_UART_Receive(&uartHandle, &dummy, 1, 10) == HAL_OK) {
        // flush input buffer by reading out garbage
    }
}

/*
 * Send a client W-Bus request and read answer from Heater.
 */
int wbus_io( UART_HandleTypeDef uartHandle,
             unsigned char cmd,
             unsigned char *out,
             unsigned char *out2,
             int len2,
             unsigned char *in,
             int *dlen,
             int skip)
{
  int err, tries;
  int len;
  unsigned char addr;
  
  len = *dlen;


  tries = 0;
  do {
    if (tries != 0) {
      CDC_Printf("wbus_io() retry: %d\n", tries);
      HAL_Delay(500);
      wbus_init(uartHandle);
    }
    
    tries++;
    /* Send Message */
    addr = (WBUS_CADDR<<4) | WBUS_HADDR;
    err = wbus_msg_send(uartHandle ,addr, cmd, out, len, out2, len2);
    
    if (err != 0) {
      CDC_Printf("wbus_msg_send returned %d\n", err);
      continue;
    }
     
    /* Receive reply */
    addr = (WBUS_HADDR<<4) | WBUS_CADDR;
    err = wbus_msg_recv(uartHandle ,&addr, &cmd, in, dlen, skip);
    if (err != 0) {
      CDC_Printf("wbus_msg_recv returned %d\n", err);
      continue;
    }
    
  } while (tries < 4 && err != 0);

  return err;
}


int wbus_host_listen( UART_HandleTypeDef uartHandle,
                      unsigned char *addr,
                      unsigned char *cmd,
                      unsigned char *data,
                      int *len)
{
  int err;
  
  do {
    *cmd = 0;
    *addr = WBUS_HADDR;
    // rs232_blocking(wbus->rs232, 1);
    err = wbus_msg_recv(uartHandle, addr, cmd, data, len, 0);
  } while (err);
    
  return err;
}

/*
 * Send answer to W-Bus client.
 */
int wbus_host_answer( UART_HandleTypeDef uartHandle, 
                      unsigned char addr,
                      unsigned char cmd,
                      unsigned char *data,
                      int len)
{
  addr = (WBUS_HADDR<<4) | (addr>>4); 
  //CDC_Printf("sending %x %x %x %x %x ... \n", addr, len+3, cmd, data[0], data[1] );
  return wbus_msg_send(uartHandle, addr, cmd|0x80, data, len, NULL, 0);
}


/* Overall info*/
int wbus_get_wbinfo(UART_HandleTypeDef uartHandle ,HANDLE_WBINFO i)
{
      int err, len;
      unsigned char tmp, tmp2[2];
    
      wbus_init(uartHandle);
    
      tmp = IDENT_WB_VER; len = 1; err = wbus_io( uartHandle , WBUS_CMD_IDENT, &tmp, NULL, 0, &i->wbus_ver, &len, 1);
      if (err) goto bail;
    
      tmp = IDENT_DEV_NAME; len = 1; err = wbus_io(uartHandle , WBUS_CMD_IDENT, &tmp, NULL, 0, (unsigned char*)i->dev_name, &len, 1);
      if (err) goto bail;
      i->dev_name[len] = 0; /* Hack: Null terminate this string */
    #if 0
      tmp = 3;              len = 1; err = wbus_io(uartHandle , WBUS_CMD_SL_RD, &tmp, NULL, 0, i->strange2, &len, 1);
    #else
      tmp2[0]=3; tmp2[1]=7; len = 2; err = wbus_io( uartHandle ,WBUS_CMD_SL_RD, tmp2, NULL, 0, i->strange2, &len, 2);
    #endif
      tmp = IDENT_WB_CODE;  len = 1; err = wbus_io(uartHandle , WBUS_CMD_IDENT, &tmp, NULL, 0, i->wbus_code, &len, 1);
                            len = 0; err = wbus_io( uartHandle ,WBUS_CMD_U1,  NULL, NULL, 0, i->strange, &len, 0);
    
      if (err) goto bail;
      tmp = IDENT_DEV_ID;  len = 1; err = wbus_io( uartHandle ,WBUS_CMD_IDENT, &tmp, NULL, 0, i->dev_id, &len, 1);
      if (err) goto bail;
      tmp = IDENT_HWSW_VER;len = 1; err = wbus_io(uartHandle , WBUS_CMD_IDENT, &tmp, NULL, 0, i->hw_ver, &len, 1);
      if (err) goto bail;
      tmp = IDENT_DATA_SET;len = 1; err = wbus_io(uartHandle , WBUS_CMD_IDENT, &tmp, NULL, 0, i->data_set_id, &len, 1);
      if (err) goto bail;
      tmp = IDENT_DOM_CU;  len = 1; err = wbus_io( uartHandle ,WBUS_CMD_IDENT, &tmp, NULL, 0, i->dom_cu, &len, 1);
      if (err) goto bail;
      tmp = IDENT_DOM_HT;  len = 1; err = wbus_io( uartHandle ,WBUS_CMD_IDENT, &tmp, NULL, 0, i->dom_ht, &len, 1);
      if (err) goto bail;
      tmp = IDENT_U0;      len = 1; err = wbus_io( uartHandle ,WBUS_CMD_IDENT, &tmp, NULL, 0, i->u0, &len, 1);
      if (err) goto bail;
      tmp = IDENT_CUSTID;  len = 1; err = wbus_io( uartHandle ,WBUS_CMD_IDENT, &tmp, NULL, 0, i->customer_id, &len, 1);
      if (err) goto bail;
      tmp = IDENT_SERIAL;  len = 1; err = wbus_io( uartHandle ,WBUS_CMD_IDENT, &tmp, NULL, 0, i->serial, &len, 1);
      if (err) goto bail;
      tmp = IDENT_SW_ID;   len = 1; err = wbus_io( uartHandle ,WBUS_CMD_IDENT, &tmp, NULL, 0, i->sw_id, &len, 1);
    
    bail:
      return err;
}

static char* hexdump(char *str, unsigned char *d, int l)
{
  for (l--;l!=-1;l--)
    str += sprintf(str, "%02x", *d++);
  /* str += sprintf(str, "\n"); */
	
  return str;
}

static const char *_wd[6] = { "StrangeDay", "Monday ", "Tuesday ", "Wednesday ", "Thursday ", "Friday " };

static char* getVersion(char *str, unsigned char *d)
{
  strcpy(str, _wd[d[0]]);
  str += strlen(str);
  str += sprintf(str, "%x/%x  %x.%x", d[1], d[2], d[3], d[4]);

  return str;
}

void wbus_ident_print(char *str, HANDLE_WBINFO i, int line)
{
  switch (line) {
    case 0: str += sprintf(str, "W-Bus version: %d.%d", (i->wbus_ver>>4)&0x0f, (i->wbus_ver&0x0f)); break;
    case 1: str += sprintf(str, "Device Name: %s", i->dev_name); break;
    case 2: str += sprintf(str, "W-Bus code: "); str=hexdump(str, i->wbus_code, 7); break;
    case 3: str += sprintf(str, "Device ID Number: "); str=hexdump(str, i->dev_id, 5); break;
    case 4: str += sprintf(str, "Data set ID Number: "); str=hexdump(str, i->data_set_id, 6); break;
    case 5: str += sprintf(str, "Software ID Number: "); str=hexdump(str, i->sw_id, 5); break;
    case 6: str += sprintf(str, "Hardware version: %x/%x", i->hw_ver[0], i->hw_ver[1] ); break;
    case 7: str += sprintf(str, "Software version: "); str=getVersion(str, i->sw_ver); break;
    case 8: str += sprintf(str, "Software version EEPROM: "); str=getVersion(str, i->sw_ver_eeprom); break;
    case 9: str += sprintf(str, "Date of Manufacture Control Unit: %x.%x.%x", i->dom_cu[0], i->dom_cu[1], i->dom_cu[2] ); break;
    case 10: str += sprintf(str, "Date of Manufacture Heater: %x.%x.%x", i->dom_ht[0], i->dom_ht[1], i->dom_ht[2] ); break;
    case 11: str += sprintf(str, "Customer ID Number: %s", i->customer_id ); break;
    case 12: str += sprintf(str, "Serial Number: "); str=hexdump(str, i->serial, 5); break;
    case 13: str += sprintf(str, "Test signature: "); str=hexdump(str, i->test_signature, 2); break;
    case 14: str += sprintf(str, "Sensor8: "); str=hexdump(str, i->u0, 7); break;
    case 15: str += sprintf(str, "U1: "); str=hexdump(str, i->strange, 7); break;
    case 16: str += sprintf(str, "System Level: "); str=hexdump(str, i->strange2, 1); break;
  }
}

/* Sensor access */
int wbus_sensor_read(UART_HandleTypeDef uartHandle, HANDLE_WBSENSOR sensor, int idx)
{
  int err  = 0;
  int len;
  unsigned char sen;
	
  /* Tweak: skip some addresses since reading them just causes long delays. */
  switch (idx) {
    default:
    break;
    case 0:
    case 1:
    case 8:
    case 9:
    case 13:
    case 14:
    case 16:
      sensor->length = 0;
      sensor->idx = 0xff;
      return -1;
      break;	
  }
	
  wbus_init(uartHandle);
	
  sen = idx; len=1;
  err = wbus_io(uartHandle, WBUS_CMD_QUERY, &sen, NULL, 0, sensor->value, &len, 1);
  if (err != 0)
  {
    CDC_Printf("Reading sensor %d failed\n", idx);
    sensor->length = 0;
    return -1;
  }
  
  /* Store length of received value */
  sensor->length = len;
  sensor->idx=idx;	
  return err;
}

#define BOOL(x) (((x)!=0)?1:0)

void wbus_sensor_print(char *str, HANDLE_WBSENSOR s)
{	
  unsigned char v;

  if (s->idx == 0xff)
  {
    strcpy(str, "skipped");
    return; 
  }
  
  switch (s->idx) {
     case QUERY_STATUS0:
       str += sprintf(str, "SHR: %d, MS: %d, S: %d, D: %d, BOOST: %d AD: %d, T15: %d",
         s->value[0]&STA00_SHR, s->value[0]&STA00_MS,
         s->value[0]&STA01_S, s->value[0]&STA02_D,
         s->value[0]&STA03_BOOST, s->value[0]&STA03_AD,
         s->value[0]&STA04_T15);
       break;
     case QUERY_STATUS1:
        v=s->value[0]; str += sprintf(str, "State CF=%d GP=%d CP=%d VFR=%d", 
		BOOL(v&STA10_CF), BOOL(v&STA10_GP), BOOL(v&STA10_CP), BOOL(v&STA10_VF));
        break;
     case QUERY_OPINFO0:
       str += sprintf(str, "Fuel type %x, Max heating time %d [min], factors %d %d",
         s->value[0], s->value[1]*10, s->value[2], s->value[3]);
       break;
     case QUERY_SENSORS:
	str += sprintf(str, "Temp = %d, Volt = ", BYTE2TEMP(s->value[0]));
        str += WORD2VOLT_TEXT(str, s->value+1);
        str += sprintf(str, ", FD = %d, HE = %d, GPR = %d", 
		s->value[3], WORD2WATT(s->value+4), s->value[6]);
        str += BYTE2GPR_TEXT(str, s->value[7]);
        break;
     case QUERY_COUNTERS1:
        str += sprintf(str, "Working hours %d:%d  Operating hours %u:%u Start Count %u",
          WORD2HOUR(s->value), s->value[2], WORD2HOUR(s->value+3), s->value[5], twobyte2word(s->value+6));
        break;
     case QUERY_STATE:
       str += sprintf(str, "OP state: 0x%x, N: %d, Dev state: 0x%x", s->value[0], s->value[1], s->value[2]);
       break;
     case QUERY_DURATIONS0:
        /* 
           PH=parking heating, SH= supplemental heatig.
           format: hours:minutes, 1..33%,34..66%,67..100%,>100%
         */
        str += sprintf(str, "Burning duration PH %u:%u %u:%u %u:%u %u:%u SH %u:%u %u:%u %u:%u %u:%u",
          WORD2HOUR(s->value), s->value[2], WORD2HOUR(s->value+3), s->value[5], WORD2HOUR(s->value+6), s->value[8], 
          WORD2HOUR(s->value+9), s->value[11],
          WORD2HOUR(s->value+12), s->value[14], WORD2HOUR(s->value+15), s->value[17], WORD2HOUR(s->value+18), s->value[20],
          WORD2HOUR(s->value+21), s->value[23]);
       break;
     case QUERY_DURATIONS1:
        str += sprintf(str, "Working duration PH %u:%u SH %u:%u",
          WORD2HOUR(s->value), s->value[2], WORD2HOUR(s->value+3), s->value[5]);
       break; 
     case QUERY_COUNTERS2:
        str += sprintf(str, "Start Counter PH %d SH %d other %d",
          twobyte2word(s->value), twobyte2word(s->value+2), twobyte2word(s->value+4));
       break;
     case QUERY_STATUS2:
        /* Level in percent. Too bad the % sign cant be printed reasonably on an 14 segment LCD. */
        str += sprintf(str, "Level GP:%d FP:%d Hz CF:%d %02x CP: %02x",
           s->value[0]>>1, s->value[1]/20, s->value[2]>>1,
           s->value[3], s->value[4]>>1);
        break;
     case QUERY_OPINFO1:\
       str += sprintf(str, "Low temp thres %d, High temp thres %d, Unknown 0x%x",
          BYTE2TEMP(s->value[OP1_TLO]),
          BYTE2TEMP(s->value[OP1_THI]),
          s->value[OP1_U0]);
       break;
     case QUERY_DURATIONS2:
        str += sprintf(str, "Ventilation duration %d:%d",
          WORD2HOUR(s->value), s->value[2]);
        break;
     case QUERY_FPW:
	str += sprintf(str, "FPW = ");
#if 0
	str += WORD2FPWR_TEXT(str, s->value);
	str += sprintf(str, "Ohm, %d Watt", WORD2WATT(s->value+2)); 
#else
        str += sprintf(str, "%d 'C, %d Watt", WORD2WATT(s->value), WORD2WATT(s->value+2)); 
#endif
        break;
     default:
	str += sprintf(str, "Sensor %d, value = ", s->idx); str=hexdump(str, s->value, s->length);
	break;
  }	
}

/* error code handling */
int wbus_errorcodes_read(UART_HandleTypeDef uartHandle, HANDLE_WBERR e)
{
  int err  = 0, i, nErr, len;
  unsigned char list[32];
  unsigned char tmp[2];
	
//   if (uartHandle == 0)
//   {
//     CDC_Printf("wbus_errorcodes_read() Invalid handle\n");
//     return -1;
//   }

  wbus_init(uartHandle); 
	
  tmp[0] = ERR_LIST;
  len = 1;
  err = wbus_io(uartHandle, WBUS_CMD_ERR, tmp, NULL, 0, list, &len, 1);
  
  nErr = list[0];
  CDC_Printf("found %d errors\n", nErr);

  /* Extract info from list and read data of each error */
  for (i=0; i<nErr; i++)
  {
    len = 2;
    tmp[0] = ERR_READ;
    tmp[1] = list[i*2+1];
    err = wbus_io(uartHandle, WBUS_CMD_ERR, tmp, NULL, 0, (unsigned char*)&e->errors[i].info, &len, 1);
  }
  e->nErr = nErr;
	 
  return err;
}

int wbus_errorcodes_clear(UART_HandleTypeDef uartHandle)
{
  int err, len;
  unsigned char tmp;
	 
  wbus_init(uartHandle); 
	
  tmp = ERR_DEL; len = 1; err = wbus_io(uartHandle, WBUS_CMD_ERR, &tmp, NULL, 0, &tmp, &len, 0);
	 
  return err;
}

void wbus_errorcode_print(char *str, HANDLE_WBERR e, int i)
{
  /* Print info about given error index */
  if (e->nErr == 0)
  {
    strcpy(str, "No Errors :)");
  }
  else
  {
    err_info_t *info = &e->errors[i].info;
		  
    str += sprintf(str, "Code 0x%x", info->code);
    str += sprintf(str, " Flags %x", info->flags);
    str += sprintf(str, " Counter %d", info->counter);
    str += sprintf(str, " OP State %d, %d", info->op_state[0], info->op_state[1] );
    str += sprintf(str, " Temp %d C Supply ", BYTE2TEMP(info->temp));
    str += WORD2VOLT_TEXT(str, info->volt);
    str += sprintf(str, " Volt OP time %d:%d", WORD2HOUR(info->hour), info->minute); 
  }
}


/* Test */
int wbus_test(UART_HandleTypeDef uartHandle, unsigned char test, unsigned char secds, float value)
{
  int err = 0;
  unsigned char tmp[4];
  int len = 4;

  wbus_init(uartHandle);
  	
  tmp[0] = test;
  tmp[1] = secds;
	
  if (test == WBUS_TEST_FP)
    FREQ2WORD(tmp[2], value);
  else
    PERCENT2WORD(tmp[2], value);
	
  err = wbus_io(uartHandle, WBUS_CMD_TEST, tmp, NULL, 0, tmp, &len, 0);
	
  return err;
}

/* Turn heater on for time minutes */
int wbus_turnOn(UART_HandleTypeDef uartHandle, WBUS_TURNON mode, unsigned char  time)
{
  int err = 0;
  unsigned char cmd;
  unsigned char tmp[1];
  int len = 1; 

  switch (mode) {
    case WBUS_VENT: cmd = WBUS_CMD_ON_VENT; break;
    case WBUS_SH: cmd = WBUS_CMD_ON_SH; break;
    case WBUS_PH: cmd = WBUS_CMD_ON_PH; break;
    default:
       return -1;
  }
  
  wbus_init(uartHandle); 
  
  tmp[0] = time;
  err = wbus_io(uartHandle, cmd, tmp, NULL, 0, tmp, &len, 0);
	  
  return err;
}

/* Turn heater off */
int wbus_turnOff(UART_HandleTypeDef uartHandle)
{
  int err = 0;
  int len = 0;
  unsigned char tmp[2];
	
  wbus_init(uartHandle); 
	
  err = wbus_io(uartHandle, WBUS_CMD_OFF, tmp, NULL, 0, tmp, &len, 0);
	  
  return err;
}

/* Check current command */
int wbus_check(UART_HandleTypeDef uartHandle, WBUS_TURNON mode)
{
  int err = 0;
  int len = 2;
  unsigned char tmp[3];

  switch (mode) {
    case WBUS_VENT: tmp[0] = WBUS_CMD_ON_VENT; break;
    case WBUS_SH: tmp[0] = WBUS_CMD_ON_SH; break;
    case WBUS_PH: tmp[0] = WBUS_CMD_ON_PH; break;
    default:
       return -1;
  }
  tmp[1] = 0;
  
  wbus_init(uartHandle); 
  
  err = wbus_io(uartHandle, WBUS_CMD_CHK, tmp, NULL, 0, tmp, &len, 0);
	  
  return err;
}

/*
 * Fuel Priming
 */
int wbus_fuelPrime(UART_HandleTypeDef uartHandle, unsigned char time)
{
  unsigned char tmp[5];
  int len;
  
  wbus_init(uartHandle); 
  
  tmp[0] = 0x03;
  tmp[1] = 0x00;
  tmp[2] = time>>1;
  len = 3;
  
  return wbus_io(uartHandle, WBUS_CMD_X, tmp, NULL, 0, tmp, &len, 0);
}

int wbus_eeprom_read(UART_HandleTypeDef uartHandle, int addr, unsigned char *eeprom_data)
{
  unsigned char tmp[1];
  int len;
  
  wbus_init(uartHandle); 
  
  tmp[0] = addr;
  len = 1;
  
  return wbus_io(uartHandle, WBUS_TS_ERD, tmp, NULL, 0, eeprom_data, &len, 2);
}

int wbus_eeprom_write(UART_HandleTypeDef uartHandle, int addr, unsigned char *eeprom_data)
{
  unsigned char tmp[3];
  int len;
  
  wbus_init(uartHandle); 
  
  tmp[0] = addr;
  len = 1;
  
  return wbus_io(uartHandle, WBUS_TS_EWR, tmp, eeprom_data, 1, tmp, &len, 0);
}


/*
 * Library Self test
 */
#ifdef WBUS_TEST


#include <string.h>
/*
 * Static tests
 */
void wbus_teststatic(void)
{
  const unsigned char msg1[] = {0xf4, 0x04, 0x56, 0x02, 0x98, 0x3c};
  const unsigned char msg2[] = {0x4f, 0x0e, 0xd6, 0x02, 0x98, 0x00, 0x01, 0x04,
		  0x00, 0x46, 0x35, 0x7a, 0x03, 0x97, 0x1b, 0x8e};
	 
  CDC_Printf("Testing checksum\n");
  if (checksum(msg1, 5, 0) != 0x3c)
    CDC_Printf("Checksum on msg1 failed! chksum = %x\n", checksum(msg1, 5, 0));
  if (checksum(msg2, 15, 0) != 0x8e)
    CDC_Printf("Checksum on msg2 failed! chksum = %x\n", checksum(msg2, 15, 0));

}
#endif

int wbus_data_set_load(UART_HandleTypeDef uartHandle, void *seq, unsigned char idx)
{
  int err = 0;
  int len = 2;
  unsigned char *tmp = seq;

  tmp[0] = DATASET_READ;
  tmp[1] = idx;

  wbus_init(uartHandle);
  
  err = wbus_io(uartHandle, WBUS_CMD_DATASET, tmp, NULL, 0, seq, &len, 2);

  return err;

}

int wbus_data_set_store(UART_HandleTypeDef uartHandle, void *seq, unsigned char idx)
{
  int err = 0;
  int len = 2;
  unsigned char tmp[2];

  tmp[0] = DATASET_WRITE;
  tmp[1] = idx;

  wbus_init(uartHandle);
  
  err = wbus_io(uartHandle, WBUS_CMD_DATASET, tmp, seq, 96, NULL, &len, 96+2);

  return err;
}

/* Overall handling stuff */

// #ifndef WBUS_USE_MALLOC
// static struct WBUS _wbus[NO_RS232];
// #endif
 
// int wbus_open(HANDLE_WBUS *pWbus, unsigned char dev_idx)
// {
//   UART_HandleTypeDef uartHandle;
//   int err;

// #ifdef WBUS_USE_MALLOC
//   wbus = malloc(sizeof(struct WBUS));
// #else
//   wbus = &_wbus[dev_idx];
// #endif
//   err = rs232_open(&wbus->rs232, dev_idx, 2400, RS232_FMT_8E1);
    
//   if (err == 0)
//     *pWbus = wbus;
    	 
// #ifdef WBUS_TEST
//   wbus_teststatic();
// #endif
	 
//   return err;
// }

// void wbus_close(UART_HandleTypeDef uartHandle)
// {
//   if (uartHandle == 0)
//     return;
	 
//   rs232_close(wbus->rs232);
// #ifdef WBUS_USE_MALLOC
//   free(uartHandle);
// #endif
// }
