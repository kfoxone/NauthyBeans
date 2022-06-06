#include "scani2c.h"
#include <Wire.h>

/**
 * Constructor
 * @param address: i2c address
 */
void ScanI2C::listToMonitor(){
  Serial.println("ScanI2C >> listToMonitor << Scanning I2C To monitor address over 1 to 127 : ");

  byte address;
  nDevices = 0;
  for (address = 1; address < 127; address++ ){
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("    >> I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.print(" | Dec("); Serial.print(address); Serial.print(")");
      Serial.println("  !");

      nDevices++;
    }else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n"); 
}






/***
 * 
 */
bool ScanI2C::isAvalaible(byte addr){
  Wire.beginTransmission(addr);
  error = Wire.endTransmission();
  if (error == 0){
    return true;
  }

  return false;
}
