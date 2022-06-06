/* ****************************************************************************
 *
 *
 *
 *
 * ***************************************************************************/
#ifndef NAUTHYBEANS_H
#define NAUTHYBEANS_H
// ============================================================================
//
//  INCLUDES
//
// ============================================================================

// Manage String
#include <string>
#include <list>

// Manage I2C
#include "../i2c/I2CDevice.hpp"
#include "../i2c/MCPDevice.hpp"




class NauthyBeans {



  public:
    std::list<I2CDevice> getI2CDevices() {
      return _i2cDevices;
    }

    std::list<MCPDevice> getMCPDevices() {
      return _mcpDevices;
    }

    I2CDevice getI2CDevice(uint8_t addr){
      int i = 1;
      for (auto it = _i2cDevices.begin(); it != _i2cDevices.end(); ++it) {
        if((*it).getAddr()==addr){
          return (*it);
        }
      }
      Serial.println("NauthyBeans >> getDevice >> Device address " + String(addr) + " not found ! ");
      return NULL;
    }

    MCPDevice getMCPDevice(uint8_t addr){
      int i = 1;
      for (auto it = _mcpDevices.begin(); it != _mcpDevices.end(); ++it) {
        if((*it).getAddr()==addr){
          return (*it);
        }
      }
      Serial.println("NauthyBeans >> getMCPDevice >> Device address " + String(addr) + " not found ! ");
      return MCPDevice();
    }

    void addDevice(I2CDevice i2cDevice) {
      _i2cDevices.push_back(i2cDevice);
    }

    void addDevice(MCPDevice mcpDevice){
      _mcpDevices.push_back(mcpDevice);
    }

    void clearI2CDevices() {
      _i2cDevices.clear();
    }

    void clearMCPDevices(){
      _mcpDevices.clear();
    }


  private :
    std::list<I2CDevice> _i2cDevices;
    std::list<MCPDevice> _mcpDevices;
};








#endif // NAUTHYBEANS_H
