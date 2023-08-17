#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver(2000);
// TX DATA arduino pin 12
void setup()
{
    Serial.begin(9600);    // Debugging only
    if (!driver.init())
         Serial.println("init failed");
    Serial.println("abcd");
}

void loop()
{   
    const char *msg = "HELLO WORLD";
    driver.send((uint8_t *)msg, strlen(msg));
    
    driver.waitPacketSent();
    Serial.println("Transmitted");
    delay(1000);
    
}
