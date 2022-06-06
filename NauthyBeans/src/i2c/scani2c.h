/* ***********************************************************
 *  Note : 
 *  
 *  Serial must be initialize before using ScanI2C
 */

#ifndef ScanI2C_h
#define ScanI2C_h

#include <Arduino.h>
#include <Wire.h>



//#define Sr Serial     // Serial must be initialise beforee



/**
 * 
 */
class ScanI2C {

public:
  ScanI2C(){
    Wire.begin();
    #ifdef _DBG
      Sr.println("ScanI2C >> constructor : Wire beginning...");
    #endif
  }
  ScanI2C(uint8_t sda, uint8_t scl){
    Wire.begin(sda, scl);
    #ifdef _DBG
      Sr.println("ScanI2C >> constructor : Wire beginning...");
    #endif
  }

  /**
   * Print to monitor serie available list ! 
   */
  void listToMonitor();

  int availableDevices() const { return nDevices;  }

  bool isAvalaible(byte addr);


private: 
  byte error;
  int nDevices;

};

#endif
