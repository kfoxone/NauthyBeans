#include "MCPDevice.hpp"


/** ***************************************************************************
 * @brief Construct a new MCPDevice object
 *
 * Do Not Use only For NULL pointer
 *
 * ***************************************************************************/
MCPDevice::MCPDevice(){
    _nullMCP = true;
}


/** ***************************************************************************
 * @brief Construct a new MCPDevice object
 * 
 * @param addr the param can be 0 to 7,the default param is 7.means
 * the dafault device address 0x27.
 *    Addr(BIN)  Addr(hex)   param
 *    010 0111    0x27        7
 *    010 0110    0x26        6
 *    010 0101    0x25        5
 *    010 0100    0x24        4
 *    010 0011    0x23        3
 *    010 0010    0x22        2
 *    010 0001    0x21        1
 * @param bus  a convenient description of the bus containing all pin of 
 * MCPDevice. 
 * @param name of the current device default "undef device"
 * @param interruptPin 
 * @param pendingFunction 
 * @param portInterrupt 
 * ***************************************************************************/
MCPDevice::MCPDevice(uint8_t addr, MCPPIN *bus, String name,
     uint8_t interruptPin, void (*pendingFunction)(), 
     PORTINTERRUPT portInterrupt):_addr(addr), _bus(bus),_name(name),
     _interruptPin(interruptPin)
{
    // Specify no input pending
    _inputPending = false;
 
    // Create MCP Object and Begin it at address 0 to 7
    _mcp = new MCP23017();
    _mcp->begin(_addr);

    // Manage Pin Interrupt
    if (interruptPin != NULL){
        pinMode(_interruptPin, INPUT);

        // We mirror INTA and INTB, so that only one line is required between MCP and Arduino for int reporting
        // The INTA/B will not be Floating
        // INTs will be signaled with a LOW
        if (portInterrupt == PORTINTERRUPT::INTA){
            _mcp->setupInterrupts(false, false, LOW);
        }else if (portInterrupt == PORTINTERRUPT::INTB){
            _mcp->setupInterrupts(true, false, LOW);
        }else{
            _mcp->setupInterrupts(true, false, LOW);
        }

        // Define Pending function to call in case of interrupt
        if (pendingFunction != NULL){
            _pendingFunction = pendingFunction;
        }
    }

    // configuration for type (INPUT/OUTPUT) on PORT
    // interrupt Pint will be setup 
    for (int i = 0; i < 16; i++){
        // Control Pin Mode 
        _mcp->pinMode(i, _bus[i].mode);

        // turn on/off a 100K pullup internally
        if(_bus[i].mode == INPUT){
            _mcp->pullUp(i, _bus[i].pullUp); 
        }
        
        // If Interrupt Manage Interrupt
        if (interruptPin != NULL and _bus[i].mode == INPUT){
            _mcp->setupInterruptPin(i, FALLING);
        }
        dbg("MCPDevice >> Constructor >> Device(" + String(_addr) + ", 0x"
            + String(identifiyAddr(_addr), HEX) +  ") : P" +
            + (i<8?"A":"B") +  String(i<8?i:i-8) + " Mode " + String(_bus[i].mode == INPUT ? "INPUT" : "OUTPUT")
            + " ==> " + _bus[i].name.c_str());
    }

    // Device is not null
    _nullMCP = false;

}



/** ***************************************************************************
 * @brief 
 * 
 * ***************************************************************************/
void MCPDevice::readInput(){
    for(int i = 0; i < 16; i++){
        if(_bus[i].mode==INPUT){
            _bus[i].inputActual = _mcp->digitalRead(i)==HIGH?true:false;
            if(_bus[i].inputPrev != _bus[i].inputActual){
                _bus[i].inputPrev = _bus[i].inputActual;
                _bus[i].pending = true;
                //handleInterrupt();
                dbg("MCPDevice >> readInput >> Dev(" + String(_addr) + ", " + String(identifiyAddr(_addr), HEX) + ") = (" +
                    (i<8?String("PA" + String(i)):String("PB"+String(i-8))) + "," + String(_bus[i].inputActual) + ")");
            }
        }
    }
}




/** ***************************************************************************
 * @brief 
 * 
 * ***************************************************************************/
void MCPDevice::runOnInterrupt()
{
    if (!_interruptAttached)
    {
        attachInterrupt(_interrupt, intCallBack, FALLING);
        dbg("MCPDevice >> run >> attach Interrupt ...");
        _interruptAttached = true;
    }
    attachInterrupt(_interrupt, intCallBack, FALLING);
    detachInterrupt(_interrupt);
    if (_inputPending)
    {
        dbg("MCPDevice >> run >> input pending ...");
        // disable interrupts while handling them.
        detachInterrupt(_interrupt);
        //
        handleInterrupt();
        //
        _interruptAttached = false;
    }
}

/** ***************************************************************************
 * @brief Read original pin
 *
 * @param pin
 * @return true
 * @return false
 * ***************************************************************************/
bool MCPDevice::isOn(uint8_t pin)
{
    _memory.actual[pin] = _mcp->digitalRead(pin) == HIGH ? true : false;
    return _memory.actual[pin];
}


/** ***************************************************************************
 * @brief Read original pin
 *
 * @param pin
 * @return true
 * @return false
 * ***************************************************************************/
bool MCPDevice::isOnRise(uint8_t pin){
    _memory.actual[pin] = _mcp->digitalRead(pin) == HIGH ? true : false;
    dbg("MCPDevice >> isOnRise >> pin:" + String(pin) + " actual =" + String(_memory.actual[pin]) +  " prev = " + String(_memory.prev[pin]));
    if(_memory.actual[pin]!=_memory.prev[pin]){
        dbg("MCPDevice >> isOnRise >> Value new value arrive ! ");

        // Save preview state
        bool prev = _memory.prev[pin];

        // Save new state
        _memory.prev[pin] = _memory.actual[pin];
        dbg("MCPDevice >> isOnRise >> Previw state is now equal to actual state (actual, prev) = (" + String(_memory.actual[pin]) + ", " + String(_memory.prev[pin]) + ")");

        // In case of rising
        if(prev==false){
            dbg("MCPDevice >> isOnRise >> change prev value and return true for rising ! ");
            return true;
        }else{
            dbg("MCPDevice >> isOnRise >> change prev value and return false for non rising value ! ");
            return false;
        }
    }
    dbg("MCPDevice >> isOnRise >> No new value so return false ! ");
    return false;
}


/** ***************************************************************************
 * @brief Write state to specify port
 *
 * @param state true mean HIGH and false low
 * @param port 0 for all, 1 for port A, 2 for port B
 * ***************************************************************************/
void MCPDevice::writeAll(bool state, int port)
{
    uint8_t val = state == true ? HIGH : LOW;
    if (port == 0 or port == 1)
    {
        for (int i = 0; i <= 7; i++)
        {
            _mcp->digitalWrite(i, val);
            setState(i, val, PORTA);
        }
    }

    if (port == 0 or port == 2)
    {
        for (int i = 8; i <= 15; i++)
        {
            _mcp->digitalWrite(i, val);
            setState(i - 8, val, PORTB);
        }
    }
}

/** ***************************************************************************
 * @brief write all port A and port B to false
 *
 * ***************************************************************************/
void MCPDevice::offAll()
{
    writeAll(false);
}

void MCPDevice::onAll()
{
    writeAll(true);
}

void MCPDevice::offPort(int port)
{
    writeAll(false, port);
}

void MCPDevice::onPort(int port)
{
    writeAll(true, port);
}

void MCPDevice::offPA()
{
    writeAll(false, 1);
}
void MCPDevice::onPA()
{
    writeAll(true, 1);
}

void MCPDevice::offPB()
{
    writeAll(false, 2);
}
/** ***************************************************************************
 * @brief
 *
 * ***************************************************************************/
void MCPDevice::onPB()
{
    writeAll(true, 2);
}


/** ***************************************************************************
 * @brief 
 * 
 * @param pin 
 * @param state 
 * ***************************************************************************/
void MCPDevice::writePinA(uint8_t pin, bool state)
{
    if (pin < 0 or pin > 7){
        dbg("MCPDevice >> writePinA : Error pin not between 0 and 7 !");
        return;
    }
    _mcp->digitalWrite(pin, state == true ? HIGH : LOW);
    setState(pin, state == true ? HIGH : LOW, PORTA);
}


/** ***************************************************************************
 * @brief 
 * 
 * @param pin 
 * @param state 
 * ***************************************************************************/
void MCPDevice::writePinB(uint8_t pin, bool state)
{
    if (pin < 0 or pin > 7)
    {
        dbg("MCPDevice >> writePinB : Error pin not between 0 and 7 !");
        return;
    }
    _mcp->digitalWrite(pin + 8, state == true ? HIGH : LOW);
    setState(pin, state == true ? HIGH : LOW, PORTB);
}


/** ***************************************************************************
 * @brief
 *
 * @param pin
 * ***************************************************************************/
void MCPDevice::offA(uint8_t pin){
    writePinA(pin, false);
}
/** ***************************************************************************
 * @brief
 *
 * @param pin
 * ***************************************************************************/
void MCPDevice::onA(uint8_t pin){
    writePinA(pin, true);
    if(pin==8){
        dbg("MCPDevice >> onA >> Pin 8 Port A is Activated !");
    }
}
/** ***************************************************************************
 * @brief
 *
 * @param pin
 * ***************************************************************************/
void MCPDevice::offB(uint8_t pin){
    writePinB(pin, false);
}
/** ***************************************************************************
 * @brief
 *
 * @param pin
 * ***************************************************************************/
void MCPDevice::onB(uint8_t pin){
    writePinB(pin, true);
}

/** ***************************************************************************
 * @brief
 *
 * @param pin
 * ***************************************************************************/
void MCPDevice::invert(uint8_t pin, int port)
{
    getState(port == PORTB ? pin + 8 : pin) ? (port == PORTA ? offA(pin) : offB(pin)) : (port == PORTA ? onA(pin) : onB(pin));
}

/** ***************************************************************************
 * @brief set state in PCFSTATE of current bus
 *
 * @param pin   pin concernt to change
 * @param value value to be change LOW or HIGH
 * ***************************************************************************/
void MCPDevice::setState(uint8_t pin, uint8_t value, uint8_t port)
{
    uint8_t pincpy = pin;
    if (port == 2)
    {
        pincpy = pin + 8;
    }

    switch (pincpy)
    {
    case PA0:
    {
        _bus->state.pa0 = value == LOW ? false : true;
        // dbg("I2CDevice >> setState << value = " + String(_bus->state.p0 ? "On" : "Off"));
    }
    break;
    case PA1:
    {
        _bus->state.pa1 = value == LOW ? false : true;
        // dbg("I2CDevice >> setState << value = " + String(_bus->state.p1 ? "On" : "Off"));
    }
    break;
    case PA2:
    {
        _bus->state.pa2 = value == LOW ? false : true;
        // dbg("I2CDevice >> setState << value = " + String(_bus->state.p2 ? "On" : "Off"));
    }
    break;
    case PA3:
    {
        _bus->state.pa3 = value == LOW ? false : true;
        // dbg("I2CDevice >> setState << value = " + String(_bus->state.p3 ? "On" : "Off"));
    }
    break;
    case PA4:
    {
        _bus->state.pa4 = value == LOW ? false : true;
        // dbg("I2CDevice >> setState << value = " + String(_bus->state.p4 ? "On" : "Off"));
    }
    break;
    case PA5:
    {
        _bus->state.pa5 = value == LOW ? false : true;
        // dbg("I2CDevice >> setState << value = " + String(_bus->state.p5 ? "On" : "Off"));
    }
    break;
    case PA6:
    {
        _bus->state.pa6 = value == LOW ? false : true;
        // dbg("I2CDevice >> setState << value = " + String(_bus->state.p6 ? "On" : "Off"));
    }
    break;
    case PA7:
    {
        _bus->state.pa7 = value == LOW ? false : true;
        // dbg("I2CDevice >> setState << value = " + String(_bus->state.p7 ? "On" : "Off"));
    }
    break;
    case PB0:
    {
        _bus->state.pb0 = value == LOW ? false : true;
        // dbg("I2CDevice >> setState << value = " + String(_bus->state.p0 ? "On" : "Off"));
    }
    break;
    case PB1:
    {
        _bus->state.pb1 = value == LOW ? false : true;
        // dbg("I2CDevice >> setState << value = " + String(_bus->state.p1 ? "On" : "Off"));
    }
    break;
    case PB2:
    {
        _bus->state.pb2 = value == LOW ? false : true;
        // dbg("I2CDevice >> setState << value = " + String(_bus->state.p2 ? "On" : "Off"));
    }
    break;
    case PB3:
    {
        _bus->state.pb3 = value == LOW ? false : true;
        // dbg("I2CDevice >> setState << value = " + String(_bus->state.p3 ? "On" : "Off"));
    }
    break;
    case PB4:
    {
        _bus->state.pb4 = value == LOW ? false : true;
        // dbg("I2CDevice >> setState << value = " + String(_bus->state.p4 ? "On" : "Off"));
    }
    break;
    case PB5:
    {
        _bus->state.pb5 = value == LOW ? false : true;
        // dbg("I2CDevice >> setState << value = " + String(_bus->state.p5 ? "On" : "Off"));
    }
    break;
    case PB6:
    {
        _bus->state.pb6 = value == LOW ? false : true;
        // dbg("I2CDevice >> setState << value = " + String(_bus->state.p6 ? "On" : "Off"));
    }
    break;
    case PB7:
    {
        _bus->state.pb7 = value == LOW ? false : true;
        // dbg("I2CDevice >> setState << value = " + String(_bus->state.p7 ? "On" : "Off"));
    }
    break;
    default:
        dbg("MCPDevice >> setState : unknow pin ! ");
        break;
    }
}

/** ***************************************************************************
 * @brief Get the State object
 *
 * @param pin
 * @param port 1 is port A 2 is port B
 * @return true
 * @return false
 * ***************************************************************************/
bool MCPDevice::getState(uint8_t pin, int port)
{
    uint8_t pincpy = pin;
    if (port == 2)
    {
        pincpy = pin + 8;
    }

    switch (pincpy)
    {
    case PA0:
    {
        return _bus->state.pa0;
    }
    break;
    case PA1:
    {
        return _bus->state.pa1;
    }
    break;
    case PA2:
    {
        return _bus->state.pa2;
    }
    break;
    case PA3:
    {
        return _bus->state.pa3;
    }
    break;
    case PA4:
    {
        return _bus->state.pa4;
    }
    break;
    case PA5:
    {
        return _bus->state.pa5;
    }
    break;
    case PA6:
    {
        return _bus->state.pa6;
    }
    break;
    case PA7:
    {
        return _bus->state.pa7;
    }
    break;
    case PB0:
    {
        return _bus->state.pb0;
    }
    break;
    case PB1:
    {
        return _bus->state.pb1;
    }
    break;
    case PB2:
    {
        return _bus->state.pb2;
    }
    break;
    case PB3:
    {
        return _bus->state.pb3;
    }
    break;
    case PB4:
    {
        return _bus->state.pb4;
    }
    break;
    case PB5:
    {
        return _bus->state.pb5;
    }
    break;
    case PB6:
    {
        return _bus->state.pb6;
    }
    break;
    case PB7:
    {
        return _bus->state.pb7;
    }
    break;

    default:
        dbg("MCPDevice >> getState : unknow pin ! ");
        break;
    }
    return false;
}


/** ***************************************************************************
 * @brief 
 * 
 * @param addr 
 * @return uint8_t 
 * ***************************************************************************/
uint8_t MCPDevice::identifiyAddr(int addr)
{
    uint8_t val;
    String str;

    switch (addr)
    {
    case 0:
        val = 0x20;
        str = "010 0000    0x20        0";
        break;
    case 1:
        val = 0x21;
        str = "010 0000    0x21        1";
        break;
    case 2:
        val = 0x22;
        str = "010 0010    0x22        2";
        break;
    case 3:
        val = 0x23;
        str = "010 0011    0x23        3";
        break;
    case 4:
        val = 0x24;
        str = "010 0100    0x24        4";
        break;
    case 5:
        val = 0x25;
        str = "010 0101    0x25        5";
        break;
    case 6:
        val = 0x26;
        str = "010 0110    0x26        6";
        break;
    case 7:
        val = 0x27;
        str = "010 0111    0x27        7";
        break;

    default:
        val = 0x20;
        str = "Erreur return default : 010 0000    0x20        0";
        break;
    }
    // dbg(MCPDEVICECLASS + String("identifiyAddr : ") + str);

    return val;
}



/** ***************************************************************************
 * @brief 
 * 
 * ***************************************************************************/
void MCPDevice::handleInterrupt()
{
    // Get more information from the MCP from the INT
    uint8_t pin = _mcp->getLastInterruptPin();
    uint8_t val = _mcp->getLastInterruptPinValue();

    dbg("Device(" + String(identifiyAddr(_addr)) + ") = (" +
                    (pin < 8 ? String("PA" + String(pin)) : String("PB" + String(pin - 8))) + "," + String(val) + ")");

    // dbg("_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-");
    // dbg("the key has been pressed!!!");
    // dbg("the last interrupt (Pin, Value) are (" + String(pin) + ", " + String(val) + ")");
    // dbg("_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-");

    // we have to wait for the interrupt condition to finish
    // otherwise we might go to sleep with an ongoing condition and never wake up again.
    // as, an action is required to clear the INT flag, and allow it to trigger again.
    // see datasheet for datails.
    // for(int i = 0; i < 16; i++){
    //     if(_bus[i].mode == INPUT && _interrupt!=NULL){
    //         while(!(_mcp->digitalRead(i)));
    // //while(!(mcp.digitalRead(mcpPinA) && mcp.digitalRead(mcpPinB) ));
    //     }
    // }

    // and clean queued INT signal
    cleanInterrupts();
}


