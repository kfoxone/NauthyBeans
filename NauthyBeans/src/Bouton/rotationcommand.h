/* ***********************************************************
    Note :

    RotationCommand
    
*/

#ifndef ROTATIONCOMMAND
#define ROTATIONCOMMAND


#include <iostream>










class RotationCommand {

public: 
  RotationCommand(uint8_t sw, uint8_t dt, uint8_t clk):_swpin(sw), _dtpin(dt), _clkpin(clk){
    pinMode(_swpin, INPUT_PULLUP);
    pinMode(_dtpin, INPUT);
    pinMode(_clkpin, INPUT);
  }


  void initialize(){
    // pinMode(_swpin, INPUT_PULLUP);
    // pinMode(_dtpin, INPUT);
    // pinMode(_clkpin, INPUT);
    // //_encoderCounter = digitalRead(CLK);
  }


  bool isSWOn(){
    _switchLastSate = digitalRead(_swpin)==HIGH?false:true;
    return _switchLastSate;
  }



  /** *************************************************************************
   * @brief Do run is main running process if btn is turning
   * 
   * *************************************************************************/
  void doRun(){
    int vclk = digitalRead(_clkpin);
    int vdt  = digitalRead(_dtpin);

    if (vclk != _vclk or vdt != _vdt) {
      ndbg("RotationCommand >> doRun <<  [CLK, DT] = [" + String(vclk) + "," + String(vdt) + "]");
      _encoderChanged = true;
    }else{
      _encoderChanged = false;
      return;
    }

    // -----------
    // STEP 1 : Check which is first enable clk or dt with priority on clk
    // -----------
    if (vclk == 0 and vdt == 1) {
      clkFirst = true;
      dtFirst = false;
      ndbg("RotationCommand >> doRun << try to go right");
    } else if (vclk == 1 and vdt == 0) {
      dtFirst = true;
      clkFirst = false;
      ndbg("RotationCommand >> doRun << try to go left");
    }

    // -----------
    // STEP 2 : With priority on clock wise, increment rotary or décrement roty
    // -----------
    if(clkFirst and vdt ==0){
      ndbg("RotationCommand >> doRun << Go Right !");
      clkFirst = false;
      _encoderCounter++;
      _encoderMemory = _encoderCounter -1;
    }else if(dtFirst and vclk==0){
      ndbg("RotationCommand >> doRun << Go Left !");
      dtFirst = false;
      _encoderCounter--;
      _encoderMemory = _encoderCounter +1;
    }


    
    // -----------
    // STEP  : Save actual value as last
    // -----------
    _vclk = vclk;
    _vdt = vdt;
  }





  /** *************************************************************************
   * @brief Reset the encoder 
   * 
   * Default reset is set to zero you can change val of reset
   * 
   * @param val defaut 0 otherwise if change rest to val defined
   * *************************************************************************/
  void resetEncoderCounter (int val = 0){
    _encoderCounter = val;
  }


  //===========================================================================
  //===========================================================================
  //  GETTER / SETTER
  //===========================================================================
  //===========================================================================

  /// Get encoder counter  
  int getEncoderCounter() const {
    return _encoderCounter;
  }
  /// Set encoder counter
  void setEncoderCounter(int encoderCounter = 0){
    _encoderCounter = encoderCounter;
  }

  /// Get encoder Changed
  bool isEncoderChanged() const {
    return _encoderChanged;
  }
  bool getEncoderChanged() const {
    return _encoderChanged;
  }
  /// Set encoder Changed
  void setEncoderChanged(bool encoderChanged = true){
    _encoderChanged = encoderChanged;
  }

  /// Get Last Switch State
  bool getSwitchLastState() const {
    return _switchLastSate;
  }
  /// Set last switch state
  void setSwitchLastState(bool state = false){
    _switchLastSate = state;
  }



private:
  /// IDENTIFIY SWITCH INPUT PIN
  uint8_t _swpin;
  /// IDENTIFY CLK PIN
  uint8_t _clkpin;
  /// IDENTIFY DT PIN
  uint8_t _dtpin;

  /// LAST SWITCH INPUT READ
  bool _switchLastSate;

  /// Main encoder 
  int _encoderCounter = 0;
  /// Encoder Memory save last value of encoder
  int _encoderMemory = 0;
  /// Encoder Changed indicate if encoder as change
  bool _encoderChanged = false;
  
  int _vclk, _vdt;
  bool clkFirst = false;
  bool dtFirst = false;
};

#endif   // ROTATIONCOMMAND
