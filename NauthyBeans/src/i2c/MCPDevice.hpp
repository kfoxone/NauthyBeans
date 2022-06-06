/* ****************************************************************************
 *
 *  the param can be 0 to 7,the default param is 7.means the dafault device address 0x27.
 *  Addr(BIN)  Addr(hex)   param
 *  010 0111    0x27        7
 *  010 0110    0x26        6
 *  010 0101    0x25        5
 *  010 0100    0x24        4
 *  010 0011    0x23        3
 *  010 0010    0x22        2
 *  010 0001    0x21        1
 *  010 0000    0x20        0
 *
 * ***************************************************************************/
#ifndef MCPDEVICE_H
#define MCPDEVICE_H
// ============================================================================
//
//  INCLUDES
//
// ============================================================================

#include <Arduino.h>
#include <Wire.h>


#ifndef UTIL_H
    #include "./../util/util.hpp"
#endif

//
//  MCP23017
//
#include "MCP23017.h"

#define MCPDEVICECLASS "MCPDEVICE >> "

#ifndef PA0
    #define PA0 0
    #define PA1 1
    #define PA2 2
    #define PA3 3
    #define PA4 4
    #define PA5 5
    #define PA6 6
    #define PA7 7
    #define PB0 8
    #define PB1 9
    #define PB2 10
    #define PB3 11
    #define PB4 12
    #define PB5 13
    #define PB6 14
    #define PB7 15
#endif

#ifndef PORT_
    #define PORT_ 0
#endif
#ifndef PORTA
    #define PORTA 1
#endif
#ifndef PORTB
    #define PORTB 2
#endif

#ifndef INPUT
    #define INPUT 0x00
#endif

#ifndef LOW
    #define LOW 0x0
#endif



enum PORTINTERRUPT
{
    INTA = 1,
    INTB = 2,
    MIROR = 3
};


// ============================================================================
//
//  STRUCTURE
//
// ============================================================================
struct MCPSTATE{
    bool pa0 = false;
    bool pa1 = false;
    bool pa2 = false;
    bool pa3 = false;
    bool pa4 = false;
    bool pa5 = false;
    bool pa6 = false;
    bool pa7 = false;
    bool pb0 = false;
    bool pb1 = false;
    bool pb2 = false;
    bool pb3 = false;
    bool pb4 = false;
    bool pb5 = false;
    bool pb6 = false;
    bool pb7 = false;
};

struct MCPPIN
{
    uint8_t pin;
    uint8_t mode = INPUT;
    String name = "undef bus name";
    uint8_t pullUp = LOW;
    MCPSTATE state;
    bool inputActual = false;
    bool inputPrev = false;
    uint8_t addr;
    bool pending = false;
    bool stateChanged = false;
};

struct MCPINPUTMEMORY {
    bool actual[16] = {true, true, true, true, true, true, true, true, true, true,  true, true, true, true, true, true};
    bool prev[16] = {true, true, true, true, true, true, true, true, true, true,  true, true, true, true, true, true};
};

#define MCPPINSIZE sizeof(MCPPIN);


static bool _inputPending;
// The int handler will just signal that the int has happen
// we will do the work from the main loop.
static void IRAM_ATTR intCallBack(){
    _inputPending = true;
}


// ============================================================================
//
//  CLASS
//
// ============================================================================
class MCPDevice
{

public:
    /// @brief Construct a new MCPDevice object
    MCPDevice(uint8_t addr, MCPPIN *bus, String name = "undef device",
     uint8_t interruptPin = NULL, void (*pendingFunction)() = NULL, PORTINTERRUPT portInterrupt = MIROR);

    /// @brief Construct a new MCPDevice object
    MCPDevice();





    /// @brief brief description
    void readInput();

    /// @brief brief description
    void runOnInterrupt();
    
    /// @brief Read original pin
    bool isOn(uint8_t pin);

    /// @brief Read original pin identify if rising
    bool isOnRise(uint8_t pin);

    /// @brief Write state to specify port
    void writeAll(bool state = true, int port = 0);

    /// @brief write all port A and port B to false
    void offAll();
    /// @brief brief description
    void onAll();

    /// @brief brief description
    void offPort(int port);
    /// @brief brief description
    void onPort(int port);

    /// @brief brief description
    void offPA();
    /// @brief brief description
    void onPA();
    /// @brief brief description
    void offPB();
    /// @brief brief description
    void onPB();

    /// @brief brief description
    void writePinA(uint8_t pin, bool state = true);
    /// @brief brief description
    void writePinB(uint8_t pin, bool state = true);

    /// @brief brief description
    void offA(uint8_t pin);
    /// @brief brief description
    void onA(uint8_t pin);
    /// @brief brief description
    void offB(uint8_t pin);
    /// @brief brief description
    void onB(uint8_t pin);
    /// @brief brief description

    
    /// @brief brief description
    void invert(uint8_t pin, int port = 1);

    /// @brief set state in PCFSTATE of current bus
    void setState(uint8_t pin, uint8_t value, uint8_t port = 1);
    /// @brief Get the State object
    bool getState(uint8_t pin, int port = 1);

    
    /// @brief brief description
    uint8_t identifiyAddr(int addr);





    bool* readPortA(){    
        bool input[8] = {false, false, false, false, false, false, false, false};
        dbg("MCPDevice >> readPortA >> read GPIO 0...");
        unsigned char v = _mcp->readGPIO(0);
        unsigned char b = 0x01;
        dbg("MCPDevice >> readPortA >> read input : ");
        for(int i =0; i < 8; i++){
            unsigned char a = v&b;
            dbg("a = [" + String(a) + ", " + String(a, BIN) + "] from " + String(v, BIN) + " & " + String(b, BIN));
            input[i] = a==0?true:false;
            dbg("a as input = " + String(input[i]));
            b=b<<1;
        }
        return input;
    }

    bool* readPortB(){
        bool input[8] = {false, false, false, false, false, false, false, false};
        dbg("MCPDevice >> readPortB >> read GPIO 1...");
        unsigned char v = _mcp->readGPIO(1);
        unsigned char b = 0x01;
        dbg("MCPDevice >> readPortB >> read input : ");
        for(int i =0; i < 8; i++){
            unsigned char a = v&b;
            dbg("a = [" + String(a) + ", " + String(a, BIN) + "] from " + String(v, BIN) + " & " + String(b, BIN));
            input[i] = a==0?true:false;
            b=b<<1;
        }
        return input;
    }

    // ============================================================================
    //
    //  GETTER / SETTER
    //
    // ============================================================================
    uint8_t getAddr(bool type = 0){
        if (type == 0)
        {
            return _addr;
        }
        int addr = identifiyAddr(_addr);
        return addr;
    }

    String getName() const{
        return _name;
    }

    bool getInterruptAttached() const
    {
        return _interruptAttached;
    }

    void setInterruptAttached(bool interruptAttached)
    {
        _interruptAttached = interruptAttached;
    }


    /// @brief brief description
    bool isNull()
    {
        return _nullMCP;
    }

    
    /// @brief brief description
    uint8_t getLastInterruptPin()
    {
        _mcp->getLastInterruptPin();
    }

    /// @brief brief description
    uint8_t getLastInterruptPinValue()
    {
        _mcp->getLastInterruptPinValue();
    }


    /// @brief brief description
    void handleInterrupt();





private:
    // handy for interrupts triggered by buttons
    // normally signal a few due to bouncing issues
    void cleanInterrupts()
    {
        // EIFR=0x01; pour arduino ATMEGA
        _inputPending = false;
    }




private:
    // Connect pin SCL of the expander to SCL
    // Connect pin SDA of the expander to SDA
    // don't solder A0,A1,A2 (default)
    // Connect MCP23017 pin INTA or INTB to Arduino Pin 3
    // Connect PA7 and PB7 to a button(when the button pressed,it connect to GND)
    MCP23017 *_mcp;

    /// MCP Address
    int _addr = 0;

    // Interrupts from the MCP23017 will be handled by pin3 or On NodeMCU D4 or 2U
    uint8_t _interruptPin = (uint8_t)2U;

    // Interrupt vector
    uint8_t _interrupt = 1;

    /// Not define
    bool _nullMCP = true;

    typedef void (*DoPending)(void); /// type for conciseness input interrupt function
    DoPending _doPending = NULL;     /// Interrupt function memory

    //
    // Device name
    //
    String _name;

    //
    // BUS DEF
    //
    MCPPIN *_bus;

    //
    // Keep Last State
    //
    MCPINPUTMEMORY _memory;

    //
    //
    //
    bool _interruptAttached = false;

    ///
    /// Pending function to call from output
    ///
    typedef void(IRAM_ATTR *PendingFunction)(void); /// type for conciseness pending function
    PendingFunction _pendingFunction = NULL;        /// Pending function

public:
};




#endif // MCPDEVICE_H