/* ****************************************************************************
 * PCF8574 GPIO Port Expand
 * http://nopnop2002.webcrow.jp/WeMos/WeMos-25.html
 *
 * PCF8574    ----- WeMos
 * A0         ----- GRD
 * A1         ----- GRD
 * A2         ----- GRD
 * VSS        ----- GRD
 * VDD        ----- 5V/3.3V
 * SDA        ----- GPIO_4(PullUp)
 * SCL        ----- GPIO_5(PullUp)
 *
 * P0     ----------------- LED0
 * P1     ----------------- LED1
 * P2     ----------------- LED2
 * P3     ----------------- LED3
 * P4     ----------------- LED4
 * P5     ----------------- LED5
 * P6     ----------------- LED6
 * P7     ----------------- LED7
 *
 * ***************************************************************************/
#ifndef I2CDEVICE_H
#define I2CDEVICE_H
// ============================================================================
//
//  INCLUDES
//
// ============================================================================

#include <string>
#include "Arduino.h"
// PCF8574
// https://github.com/xreef/PCF8574_library
#include "PCF8574.h"  


#define CLASS   String("I2CDevice")

// ============================================================================
//
//  STRUCTURE
//
// ============================================================================
struct PCFSTATE {
			bool p0 = false;
			bool p1 = false;
			bool p2 = false;
			bool p3 = false;
			bool p4 = false;
			bool p5 = false;
			bool p6 = false;
			bool p7 = false;
		} ;

struct PCFPIN {
  uint8_t pin;
  uint8_t mode = OUTPUT;
  std::string name = "undef bus name";
  PCFSTATE state;
  uint8_t addr;
  bool  pending = false;
};
#define PCFPINSIZE  sizeof(PCFPIN);



// PCF8574 *_b21;




class I2CDevice {

  public:
    // ============================================================================
    //
    //  CONSTRUCTORS
    //
    // ============================================================================
    I2CDevice(uint8_t addr, PCFPIN *bus, std::string identification = "undef device", uint8_t interruptPin = NULL,  void (*inputInterrupt)() = NULL,  void (*pendingFunction)() = NULL) {
      this->_addr = addr;
      _bus = bus;
      _name = identification;


      if(interruptPin!=NULL){
        pinMode(interruptPin, INPUT);
        _inputInterruptDefined = true;
        _inputInterrupt = inputInterrupt;
        _pendingFunction = pendingFunction;
        _pcf8574 = new PCF8574(_addr, interruptPin, inputInterrupt);
        Serial.println("I2CDevice : create with ON interrupt on addr = " + String(_addr) + " device(" + _name.c_str() + ")");
        
        // _b21 = new PCF8574(0x21, interruptPin, onInputInterrupt);
        // _b21->pinMode(P0, OUTPUT);
        // _b21->pinMode(P1, INPUT);
        // _b21->begin();

      }else{
        _pcf8574 = new PCF8574(_addr);
        Serial.println("I2CDevice : create with NO interrupt on addr = " + String(_addr) + " device(" + _name.c_str() + ")");
      }

      
      for(int i=0; i < 8; i++){
        _pcf8574->pinMode(i, _bus[i].mode);
        Serial.println(String(_addr) + " : " + String(i) + " Mode " + String(_bus[i].mode == INPUT ? "INPUT" : "OUTPUT"));
      }  

      _pcf8574->begin();

      _stdDevice = false;
    }


    I2CDevice(uint8_t addr, std::string identification = "undef device") {
      this->_addr = addr;
      _name = identification;
      _stdDevice = true;

    }

    I2CDevice(){}

    // ============================================================================
    //
    //  FUNCTION
    //
    // ============================================================================
    void On(uint8_t pin){
      _pcf8574->digitalWrite(pin, HIGH);
      setState(pin, HIGH);      
    }

    void Off(uint8_t pin){
      _pcf8574->digitalWrite(pin, LOW);
      setState(pin, LOW);
    }




    /**
     * @brief 
     * 
     * @param pin 
     */
    void invert(uint8_t pin){
      // if(getState(pin)){
      //   Serial.println("I2CDevice >> invert : pin was On now set to Off");
      //   Off(pin);
      // }else{
      //   Serial.println("I2CDevice >> invert : pin was Off now set to On");
      //   On(pin);
      // }
      getState(pin)?Off(pin):On(pin);
    }




    /**
     * @brief set state in PCFSTATE of current bus
     * 
     * @param pin   pin concernt to change
     * @param value value to be change LOW or HIGH
     */
    void setState(uint8_t pin, uint8_t value){
      switch(pin){
        case P0:{
          _bus->state.p0 = value == LOW ? false : true;
          //Serial.println("I2CDevice >> setState << value = " + String(_bus->state.p0 ? "On" : "Off"));
        }
        break;
        case P1:{
          _bus->state.p1 = value == LOW ? false : true;
          //Serial.println("I2CDevice >> setState << value = " + String(_bus->state.p1 ? "On" : "Off"));
        }
        break;
        case P2:{
          _bus->state.p2 = value == LOW ? false : true;
          //Serial.println("I2CDevice >> setState << value = " + String(_bus->state.p2 ? "On" : "Off"));
        }
        break;
        case P3:{
          _bus->state.p3 = value == LOW ? false : true;
          //Serial.println("I2CDevice >> setState << value = " + String(_bus->state.p3 ? "On" : "Off"));
        }
        break;
        case P4:{
          _bus->state.p4 = value == LOW ? false : true;
          //Serial.println("I2CDevice >> setState << value = " + String(_bus->state.p4 ? "On" : "Off"));
        }
        break;
        case P5:{
          _bus->state.p5 = value == LOW ? false : true;
          //Serial.println("I2CDevice >> setState << value = " + String(_bus->state.p5 ? "On" : "Off"));
        }
        break;
        case P6:{
          _bus->state.p6 = value == LOW ? false : true;
          //Serial.println("I2CDevice >> setState << value = " + String(_bus->state.p6 ? "On" : "Off"));
        }
        break;
        case P7:{
          _bus->state.p7 = value == LOW ? false : true;
          //Serial.println("I2CDevice >> setState << value = " + String(_bus->state.p7 ? "On" : "Off"));
        }
        break;
        default:
          Serial.println(CLASS + " >> setState : unknow pin ! ");
        break;
      }
    }

    /**
     * @brief Get the State object
     * 
     * @param pin 
     * @return true 
     * @return false 
     */
    bool getState(uint8_t pin){
      switch(pin){
        case P0:{
          return _bus->state.p0; 
        }
        break;
        case P1:{
          return _bus->state.p1;
        }
        break;
        case P2:{
          return _bus->state.p2;
        }
        break;
        case P3:{
          return _bus->state.p3;
        }
        break;
        case P4:{
          return _bus->state.p4;
        }
        break;
        case P5:{
          return _bus->state.p5;
        }
        break;
        case P6:{
          return _bus->state.p6;
        }
        break;
        case P7:{
          return _bus->state.p7;
        }
        break;
        default:
          Serial.println(CLASS + " >> getState : unknow pin ! ");
        break;
      }
      return false;
    }


    /**
     * @brief Set Device Has Input Pending
     * 
     */
    void inputInterruptPending(bool pending = true){
      _inputPending = pending;
    }

    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    bool isPending(){
      return _inputPending;
    }

    PCF8574::DigitalInput readAll(){
      return _pcf8574->digitalReadAll();
    }

    bool readPin(uint8_t pin){
      return _pcf8574->digitalRead(pin)==LOW?false:true;
    }

    void writeAll(bool state = true){
      uint8_t val = state==true?HIGH:LOW;
      for(int i = 0; i <= 7; i++)
        _pcf8574->digitalWrite(i, val);
    }

    void allOff(){
      writeAll(false);
    }

    void allOn(){
      writeAll(true);
    }

    // ============================================================================
    //
    //  GETTER / SETTER
    //
    // ============================================================================
    uint8_t getAddr() const {
      return _addr;
    }
    std::string getName() const {
      return _name;
    }


    /**
     * @brief 
     * 
     */
    void doPending(){
      // if(this->_pendingInput){
      //   this->_inputInterrupt();
      //   this->_pendingInput = false;
      //   Serial.println("Pending done for addr " + String(_addr));
      // }
      // if(_inputPending){
      //   _pendingFunction();
      //   _inputPending = false;
      // }
    }





  private:
    uint8_t _addr;
    std::string _name;
    bool _stdDevice = true; 

    // BUS DEF
    PCFPIN *_bus;

    // 
    PCF8574 *_pcf8574;


    // Manage function
    typedef void (*InputInterrupt)(void);   /// type for conciseness input interrupt function
    InputInterrupt _inputInterrupt = NULL;  /// Interrupt function memory
    bool          _inputInterruptDefined;   /// Specify class with interrupt function defined
    bool          _inputPending = false;    /// Identify if pending data at input are available

    typedef void  (*PendingFunction)(void);  /// type for conciseness pending function
    PendingFunction  _pendingFunction = NULL;/// Pending function  

};



#endif  // I2CDEVICE_H