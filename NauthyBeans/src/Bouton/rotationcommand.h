/* ***********************************************************
    Note :

    RotationCommand
    
*/

#ifndef ROTATIONCOMMAND
#define ROTATIONCOMMAND


#include <iostream>



#define SD3   9   // For NodeMCU V3
#define SD2   10  // For NodeMCU V3

#define CLK   D6   // encoder clk
#define DT    SD3  // encoder dt
#define SW    D0   // Rotari swtich






class RotationCommand {

public: 
  RotationCommand(){
    
  }


  void initialize(){
    pinMode( SW, INPUT_PULLUP );
    pinMode (CLK, INPUT);
    pinMode (DT, INPUT);
    //rotationCnt = digitalRead(CLK);
  }


  bool isSWOn(){
    //int value = digitalRead(SW);
    if (digitalRead(SW) == 0) {
      return true;
    }
    return false;
  }

  void resetRotationCnt (int val = 0){
    rotationCnt = val;
  }

  int getRotationCnt() const {
    return rotationCnt;
  }

  void doRun(){
    int vclk = digitalRead(CLK);
    int vdt  = digitalRead(DT);

#ifdef _DBG
    if (vclk != _vclk or vdt != _vdt) {
      std::string str = "RotationCommand >> doRun << Loop = " + std::to_string(looper) + " [CLK, DT] = [" + std::to_string(vclk) + "," + std::to_string(vdt) + "]";
      Serial.println(str.c_str());
    }
#endif

    // -----------
    // STEP 1 : Check which is first enable clk or dt with priority on clk
    // -----------
    if (vclk == 0 and vdt == 1) {
      clkFirst = true;
      dtFirst = false;
#ifdef _DBG
      std::string str = "RotationCommand >> doRun << try to go right";
      Serial.println(str.c_str());
#endif  
    } else if (vclk == 1 and vdt == 0) {
      dtFirst = true;
      clkFirst = false;
#ifdef _DBG
      std::string str = "RotationCommand >> doRun << try to go left";
      Serial.println(str.c_str());
#endif
    }

    // -----------
    // STEP 2 : With priority on clock wise, increment rotary or décrement roty
    // -----------
    if(clkFirst and vdt ==0){
#ifdef _DBG
      std::string str = "RotationCommand >> doRun << Go Right !";
      Serial.println(str.c_str());
#endif
      clkFirst = false;
      rotationCnt++;
    }else if(dtFirst and vclk==0){
#ifdef _DBG
      std::string str = "RotationCommand >> doRun << Go Left !";
      Serial.println(str.c_str());
#endif
      dtFirst = false;
      rotationCnt--;
    }


    
    // -----------
    // STEP  : Save actual value as last
    // -----------
    _vclk = vclk;
    _vdt = vdt;
  }

private:
  int rotationCnt = 0;
  
  int _vclk, _vdt;
  bool clkFirst = false;
  bool dtFirst = false;
};

#endif   // ROTATIONCOMMAND
