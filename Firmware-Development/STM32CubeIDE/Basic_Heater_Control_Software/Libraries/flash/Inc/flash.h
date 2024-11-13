#pragma once

class FlashStorage{
public:
	uint32_t getPage(uint32_t Address);
	void float2Bytes(uint8_t * ftoa_bytes_temp,float float_variable);
	float bytes2Float(uint8_t * ftoa_bytes_temp);
	uint32_t writeData (uint32_t StartPageAddress, uint32_t *Data, uint16_t numberofwords);
	void readData (uint32_t StartPageAddress, uint32_t *RxBuf, uint16_t numberofwords);
	void convertToStr (uint32_t *Data, char *Buf);
	void writeNum (uint32_t StartSectorAddress, float Num);
	float readNum (uint32_t StartSectorAddress);
};
