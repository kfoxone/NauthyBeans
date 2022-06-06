

#ifndef TEST_H
#define TEST_H



#include <iostream>
#include "./../i2c/MCPDevice.hpp"





//
// TEST TOOLS
//
int _testSequenceActiveSequence = 0;




/** ***************************************************************************
 * @brief Test tool for checking one state
 * 
 * State on of false
 * 
 * @param msBlink 
 *
 * ***************************************************************************/
void testOnState(bool state, MCPDevice IOb, MCPDevice IOc){
  if(!IOb.isNull()){
    if(state==true){
      IOb.onPB();
    }else{
      IOb.offPB();
    }
  }

  
  if(!IOc.isNull()){
    if(state==true){
      IOc.onAll();
    }else{
      IOc.offAll();
    }
  }
}



/** ***************************************************************************
 * @brief Test tool Blink
 * 
 * All led will blink to indicate good working at msBlink frequence
 * 
 * @param msBlink 
 *
 * ***************************************************************************/
void testBlinkAll(unsigned long msBlink, MCPDevice IOb, MCPDevice IOc){
    static uint64_t switchLedOutputTimeout = millis();
    if(millis()-switchLedOutputTimeout >= 500){
      for(int i=0; i <= 7; i++){
        if(!IOb.isNull()){
          IOb.invert(i, PORTB);
        }
        if(!IOc.isNull()){
          IOc.invert(i, PORTA);
          IOc.invert(i, PORTB);
        }
        
      }
      switchLedOutputTimeout = millis();
    }

}


/** ***************************************************************************
 * @brief Test tool Sequence
 * 
 * Each led will blink one after another after msBlink time 
 * 
 * @param msBlink time during which light will stay activated
 *
 * ***************************************************************************/
void testSequence(unsigned long msBlink, MCPDevice IOb, MCPDevice IOc){
  
  static uint64_t testSequenceTimeoutNext = millis();

  if(millis()-testSequenceTimeoutNext<=msBlink)
    return;

  if(!IOb.isNull()){
    IOb.offPB();
  }
  if(!IOc.isNull()){
    IOc.offAll();
  }
  
  
  if(_testSequenceActiveSequence < 8){
    IOb.onB(_testSequenceActiveSequence);    
    //lcd.lpush("Seq=" + std::to_string(_testSequenceActiveSequence) + " L:" + _bus25[_testSequenceActiveSequence].name.c_str());
    //Serial.println("Seq=" + String(_testSequenceActiveSequence) + " L:" + _bus25[_testSequenceActiveSequence].name.c_str());
  }else if(_testSequenceActiveSequence>=8 and _testSequenceActiveSequence<16 and !IOc.isNull()){
    IOc.onA(_testSequenceActiveSequence-8);
    //Serial.println("Seq=" + String(_testSequenceActiveSequence) + " L:" +  _bus26[_testSequenceActiveSequence-8].name.c_str());
  }else if(_testSequenceActiveSequence>=16 and _testSequenceActiveSequence<24  and !IOc.isNull()){
    IOc.onB(_testSequenceActiveSequence-16);
    //lcd.lpush("Seq=" + std::to_string(_testSequenceActiveSequence) + " L:" +  _bus26[_testSequenceActiveSequence-8].name.c_str());
    //Serial.println("Seq=" + String(_testSequenceActiveSequence) + " L:" +  _bus26[_testSequenceActiveSequence-8].name.c_str());
  }
  _testSequenceActiveSequence++;
  if(_testSequenceActiveSequence>=24){
    _testSequenceActiveSequence=0;
  }
  // Jump over unused output
  if(_testSequenceActiveSequence>=11 and _testSequenceActiveSequence<=13){
    //lcd.lpush("Pin Q21.3 Q21.4 Q21.5 not used !");
    _testSequenceActiveSequence = 14;
  }
  // Update New Time 
  testSequenceTimeoutNext=millis();


}



/** ***************************************************************************
 * @brief 
 * 
 * @param errorTime 
 * ***************************************************************************/
bool testSequenceBtnWait = false;
bool testSequenceBtnProcessing = false;
bool testSequenceBtnOnePush = true;
void testSequenceBtn(unsigned long errorTime, MCPDevice IOa, MCPDevice IOb, MCPDevice IOc){
  
  static uint64_t testSequenceBtnTimeoutNext = millis();

  if(!testSequenceBtnProcessing){
    testSequenceBtnTimeoutNext = millis();
    testSequenceBtnProcessing = true;
    _testSequenceActiveSequence = 0;
  }

  // Managing Led state
  if(_testSequenceActiveSequence < 8 and !IOb.isNull()){
    // 
    if(!IOc.isNull()){
      IOc.offAll();
    }

    // 
    for(int i = 0; i < 8; i++){
      if(_testSequenceActiveSequence == i){
        IOb.onB(i);
      }else{
        IOb.offB(i);
      }
    }

    
    if(testSequenceBtnOnePush){
      //lcd.lpush(std::string("S=" + std::to_string(_testSequenceActiveSequence) + " Btn_") + _bus24[_testSequenceActiveSequence].name.c_str());
      testSequenceBtnOnePush=false;
    }

    // Wait for corresponding button
    if(!IOa.isOn(_testSequenceActiveSequence)){
      _testSequenceActiveSequence++;
      testSequenceBtnOnePush = true;
    }

  }else if(_testSequenceActiveSequence >= 8 and _testSequenceActiveSequence<16 and !IOc.isNull()){
    // Turn the rest off
    if(!IOb.isNull()){
      IOb.offPB();
    }
    IOc.offPB();

    
    for(int i = 0; i < 8; i++){
      if(_testSequenceActiveSequence-8 == i){
        IOc.onA(i);
      }else{
        IOc.offA(i);
      }
    }

    //
    if(testSequenceBtnOnePush){
      //lcd.lpush(std::string("S=" + std::to_string(_testSequenceActiveSequence) + " Btn_") + _bus24[_testSequenceActiveSequence].name.c_str());
      testSequenceBtnOnePush=false;
    }

    // Wait for corresponding button
    if(!IOa.isOn(_testSequenceActiveSequence)){
      _testSequenceActiveSequence++;
      testSequenceBtnOnePush = true;
    }

  }else if(_testSequenceActiveSequence>=16 and _testSequenceActiveSequence<24  and !IOc.isNull()){
    // Turn the rest off
    if(!IOb.isNull()){
      IOb.offPB();
    }
    IOc.offPA();
    
    
    for(int i = 0; i < 8; i++){
      if(_testSequenceActiveSequence-16 == i){
        IOc.onB(i);
      }else{
        IOc.offB(i);
      }
    }

    //
    if(testSequenceBtnOnePush){
      //lcd.lpush(std::string("S=" + std::to_string(_testSequenceActiveSequence) + " Btn_") + _bus25[_testSequenceActiveSequence-16].name.c_str());
      testSequenceBtnOnePush=false;
    }

    // Wait for corresponding button
    if(!IOb.isOn(_testSequenceActiveSequence-16)){
      _testSequenceActiveSequence++;
      testSequenceBtnOnePush = true;
    }
  }


  if(_testSequenceActiveSequence>=24){
    _testSequenceActiveSequence=0;
  }
  // Jump over unused output
  if(_testSequenceActiveSequence>=11 and _testSequenceActiveSequence<=13){
    //lcd.lpush("Pin Q21.3 Q21.4 Q21.5 not used !");
    _testSequenceActiveSequence = 14;
  }
}



















#endif // TEST_H

