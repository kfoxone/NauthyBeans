/* ****************************************************************************
 *
 *
 *
 *
 * ***************************************************************************/
#ifndef GAME_H
#define GAME_H
// ============================================================================
//
//  INCLUDES
//
// ============================================================================

// String management
//
#include <string>

// Player of the Game
//
#include "Player.hpp"

// Ruban
//
#include "../FastLED/ruban.hpp"

// 7SEGMENT
//
#include "../Afficheur/disp2b7Seg.h"


// 7SEGMENT ADDR
#define DISP_SYSTEM   1
#define DISP_PLAYER1  0
#define DISP_PLAYER2  2



class FightGame {

  public:
    FightGame(){
      _lc = new Disp2b7Seg();
    }
    FightGame(Player player1, Player player2 = Player(), Disp2b7Seg *lc = new Disp2b7Seg()){  
      _player1 = player1; 
      _player2 = player2; 

      defaultUser();

      _lc = lc;
    }
    // FightGame(std::string namePlayer1, std::string namePlayer2 = "U" + std::to_string(millis())){
    //    _player1 = Player(namePlayer1); 
    //    _player2 = Player(namePlayer2);  
    //    defaultUser();
    
    // }

  public:
    Player* getPlayer1(){ return & _player1; }
    Player* getPlayer2(){ return & _player2; }

    /**
     * @brief Processing main core function
     * 
     */
    void process(){
      for(int i = 0; i < 10; i++){
        
        // Run On Player 1
        if(_player1.getInput(i)==true and _player1.getLed(i)==true){
          // Displayer new defined vector
          dbgnl("FightGame >> run >> Player 1 BEFORE : ");
          showVector(_player1.getLeds());
          dbg("");

          // Update the vector with new value
          updateVectorLight(_player1.getLeds());
          _player1.setLed(i, false);
          _player1.scoreAdd();

          // Displayer new defined vector
          dbgnl("FightGame >> run >> Player 1 AFTER : ");
          showVector(_player1.getLeds());
          dbg("");
        }else if(_player1.getInput(i)==true and _player1.getLed(i)==false){
          // Wrong activage remove point
          _player1.scoreSub();
        }



        // Run On Player 2
        if(_player2.getInput(i)==true and _player2.getLed(i)==true){
          // Displayer new defined vector
          dbgnl("FightGame >> run >> Player 2 BEFORE : ");
          showVector(_player2.getLeds());
          dbg("");

          // Update the vector with new value
          updateVectorLight(_player2.getLeds());
          _player2.setLed(i, false);
          _player2.scoreAdd();

          // Displayer new defined vector
          dbgnl("FightGame >> run >> Player 2 AFTER : ");
          showVector(_player2.getLeds());
          dbg("");
        }else if(_player2.getInput(i)==true and _player2.getLed(i)==false){
          // Wrong activage remove point
          _player2.scoreSub();
        }
      }

      
    }


    /**
     * @brief Initialize user with new value
     * 
     */
    void init(){
      Serial.println("FightGame >> init >> start initialisation players ! ");
      clear();
      Serial.println("FightGame >> init >> ChangeLight invoke start...");
      _player1.changeLight(initLightWithRandom());
      _player2.changeLight(initLightWithRandom());
      
    }


    /** ***************************************************************************
     * @brief Initialise Fight Game
     * 
     * 
     * ***************************************************************************/
    void prepare(String namePlayer1, String namePlayer2) {
      dbg("FightGame >> prepare << Start...");
      _player1.setFirstname(namePlayer1.c_str());
      _player1.setScore(0);
      dbg("FightGame >> prepare << Reset Player 1 finished...");
      _player2.setFirstname(namePlayer2.c_str());
      _player2.setScore(0);
      dbg("FightGame >> prepare << Reset Player 2 finished...");
      
      dbg("FightGame >> prepare << Refill content with init...");
      init();
    }



    /** ***************************************************************************
     * @brief  fightGame_pullIn
     * From input readed affected player input state.
     * 
     * Only first byte on portB is managed
     * ***************************************************************************/
    void pushIn(MCPDevice inA, MCPDevice inB){
      if(!inA.isNull()){
        for(int i = 0; i < 16; i++){
          if(!inA.isNull()){
            if(i<10){
              _player1.setInput(i, inA.isOnRise(i));
            }else if(i >=14){
              _player2.setInput(i-14, inA.isOnRise(i));
            }
          }

          if(!inB.isNull()){
            if(i<8){
              _player2.setInput(i+2, inB.isOnRise(i));
            }
          }
        }
      }
    }

    /** ***************************************************************************
     * @brief Push result at the output
     * 
     * @param outB Only use byte B on this port
     * @param outC Use all byte on this port
     * ***************************************************************************/
    void pushOut(MCPDevice outB, MCPDevice outC){
      if(!_running){
        return;
      }

      // Player 1
      for(int i = 0; i < 10; i++){
        bool ledState = _player1.getLed(i);
        if(ledState==true){
          if(i<8){
            outB.onB(i);
          }else{
            outC.onA(i-8);
          }
        }else{
          if(i<8){
            outB.offB(i);
          }else{
            outC.offA(i-8);
          }
        }
      }

      // Player 2
      for(int i = 0; i < 10; i++){
        bool ledState = _player2.getLed(i);
        if(ledState==true){
          if(i<2){
            outC.onA(i+6);
           }else{
            outC.onB(i-2);
          }
        }else{
          if(i<2){
            outC.offA(i+6);
          }else{
            outC.offB(i-2);
          }
        }
      }
    }


    /** ***************************************************************************
     * @brief  Fight Game running main process
     * 
     * ***************************************************************************/
    void run(MCPDevice inA, MCPDevice ioB, MCPDevice outC){
      static uint64_t elapseTimeToResetGame = millis();
      static uint64_t elapseTimeToFinishGame = millis();
      static uint64_t elapseDisplayTime = millis();
      static uint64_t elapseFinishGameTime = millis();
      static uint64_t elapseDisplayRstBlinkTime = millis();

      // 
      if(!inA.isNull() and !_start){
        if(!inA.isOn(broche::Start)){
          uint64_t elapseResetMs = millis()-elapseTimeToResetGame;
          
          if(millis()-elapseDisplayTime>=1000){
            dbg("NauthyBeans >> fightGame_run >> Reset count down " + String((5000-elapseResetMs)/1000));
          }
          if(elapseResetMs >= 5000){ // 5secs
            _start = true;
            elapseTimeToResetGame =  millis();
            if(millis()-elapseDisplayTime>=1000){
              dbg("NauthyBeans >> fightGame_run >> reset done by restarting the game !");
            }
          }
          // Affichage clignotement led central reset.
          if(!outC.isNull()){
            if(millis()-elapseDisplayRstBlinkTime<=500){
              outC.onA(2);
            }else{
              outC.offA(2);
              if(millis()-elapseDisplayRstBlinkTime>=1000){
                elapseDisplayRstBlinkTime=millis();
              }
            }
          }
        }else{
          elapseTimeToResetGame =  millis();
          elapseDisplayTime = millis();
          elapseDisplayRstBlinkTime=millis();
        }
      }else{
        // Indicate options is available
        if(!outC.isNull()){
          outC.onA(2);
        }
      }






      // 
      // Operate evrything while gamme is running
      //
      if(_running){
        //
        process();
        
        // Calculate remaining time
        elapseFinishGameTime = millis() - elapseTimeToFinishGame;
        _remainingTime = _presetGameTime - elapseFinishGameTime;
        ndbg("FightGame >> run >> ElpaseFinishGameTime = " + String(elapseFinishGameTime) + " ElapseTimeToFinishGame = " + String(elapseTimeToFinishGame));

        // 
        if(elapseFinishGameTime >= _presetGameTime){ // 60000
          _running = false;

          if(_player1.getScore() > _player2.getScore()){
            ws2812BColorFirstHalf(CRGB::Red);
            ws2812BColorSecondHalf(CRGB::Green);
          }else if(_player1.getScore()<_player2.getScore()){
            ws2812BColorFirstHalf(CRGB::Green);
            ws2812BColorSecondHalf(CRGB::Red);
          }else{
            ws2812BColorAll(CRGB::Orange);
          }
        }else{
          loop_ColorPalette();
        }

        // Process scoring
        int rtsec = (int)_remainingTime/1000;
        // Player 1
        _lc->printNumber(_player1.getScore(), DISP_PLAYER1);
        _lc->printNumber(rtsec, DISP_PLAYER1, true);

        // Player 2
        _lc->printNumber(_player2.getScore(), DISP_PLAYER2);
        _lc->printNumber(rtsec, DISP_PLAYER2, true);

        // System
        static uint64_t elapseTimeDisplaySystem = millis();
        static int fightGame_SysSequence = 0;
        if(millis()-elapseTimeDisplaySystem >= 1000){
          switch(fightGame_SysSequence){
            case 0 : // Score for player 1
              _lc->printTextLR("PL01", DISP_SYSTEM);
              _lc->printNumber(_player1.getScore(), DISP_SYSTEM);
              fightGame_SysSequence++;
              elapseTimeDisplaySystem = millis();
              break;
            case 1 : // Score for player 2
              _lc->printTextLR("PL02", DISP_SYSTEM);
              _lc->printNumber(_player2.getScore(), DISP_SYSTEM);
              fightGame_SysSequence++;
              elapseTimeDisplaySystem = millis();
              break;
            case 2 :
              _lc->printTextLR("SECS", DISP_SYSTEM);
              _lc->setRow(DISP_SYSTEM, 7, 0B01011011); // S
              _lc->setRow(DISP_SYSTEM, 5, 0B01001110); // C
              _lc->setRow(DISP_SYSTEM, 4, 0B01011011); // S
              _lc->printNumber(rtsec, DISP_SYSTEM);
              //fightGame_SysSequence++;
              fightGame_SysSequence = 0;
              elapseTimeDisplaySystem = millis();
              break;
            default:
              dbg("FightGame >> run >> Undefine switch sequence back to 0!");
              fightGame_SysSequence = 0;
              break;
          }
        }


      }




      //
      // SET Everything when game need to start
      //
      if(_start){
        prepare("Joleen", "Shelly");
        _start = false;
        _running = true;
        ws2812BTurnOff();
        elapseTimeToFinishGame = millis();
        scoreClear();
      }
    }



    /**
     * @brief 
     * 
     */
    void scoreClear(){
      _player1.scoreInit(0);
      _player2.scoreInit(0);
    }
    
    /**
     * @brief Allow to clear all led state
     * 
     */
    void clear(){
      Serial.println("FightGame >> Clear >> start clearing all users ! ");
      _player1.clear();
      _player2.clear();
    }

    /** ***************************************************************************
     * @brief Init Lights Vector with random state depend on simultanated
     * activate led allow. 
     * 
     * @return table containing a vector of 10 boolean light state 
     * ***************************************************************************/
    bool* initLightWithRandom(){
      dbg("FightGame >> initLightWithRandom >> start...");
      bool *lights = new bool[10]{false, false, false, false, false, false, false, false, false, false};

      for(int i = 0; i < _simultanatedLed; i++){
        dbg("FightGame >> initLightWithRandom >> start loop for s=" + String(i));
        int val =  random(0, 10);
        while(lights[val]==true){
          val = random(0,10);
        }
        lights[val]=true;
      }

      Serial.print("FightGame >> initLightWithRandom >> display initiated tabl : ");
      showVector(lights);
      Serial.println("");

      return lights;
    }


    int countActivate(bool * leds){
      int activate = 0;
      for(int i=0; i < sizeof(leds); i++){
        if(leds[i]){
          activate++;
        }
      }
      return activate;
    }

    /** ***************************************************************************
     * @brief Update a vector with new random value
     * 
     * @param vector is a led vector
     * ***************************************************************************/
    void updateVectorLight(bool vector[10]){
      // Pre-process
      dbg("FightGame >> updateVectorLight >> before : ");
      showVector(vector);
      dbg("");

      // Process
      int val =  random(0, 10);
      while(vector[val]==true){
        val = random(0,10);
      }
      vector[val]=true;

      // Post-process
      dbg("FightGame >> updateVectorLight >> after : ");
      showVector(vector);
      dbg("");
    }

    /** ***************************************************************************
     * @brief Display a vector of 10 boolean value.
     * 
     * @param vector is input or led 
     * ***************************************************************************/
    void showVector(bool vector[10]){
      for(int i = 0; i < 10; i++){
        dbgnl(String(vector[i]==true?1:0) + ", ");
      }
    }




    /* **************************************************************************
    *
    *
    * Getter / Setter
    * 
    * 
    * *************************************************************************/
    int getSimultanateLed(){
      return _simultanatedLed;
    }

    void setSimultanatedLed(int simultanatedLed = 5) {
      _simultanatedLed = simultanatedLed;
    }

    uint64_t getRemainingTime() const {
      return _remainingTime;
    }
    void setRemainingTime(uint64_t remainingTime = 0){
      _remainingTime = remainingTime;
    }

    uint64_t getPresetGameTime() const {
      return _presetGameTime;
    }
    void setPresetGameTime(uint64_t presetGameTime = 60000){
      _presetGameTime = presetGameTime;
    }


  private:
    void defaultUser(){
      Serial.println("FightGame >> prepare >> Start preparing player !");
      if(_player1.operator==(Player())){
        _player1 = Player("P01");
      }
      if(_player2.operator==(Player())){
        _player2 = Player("P02");
      }
    }

  private:
    Player _player1;
    Player _player2;

    /// Number of simultanated led active at same time
    int _simultanatedLed = 5; 
    /// Remaining time for the game to end in ms
    uint64_t _remainingTime = 0;
    /// Preset game time to play (run) in ms
    uint64_t _presetGameTime = 60000; //ms

    /// Specify need to be start and initiate game
    bool _start = true;
    /// Specify game is in running mode
    bool _running = false;
    /// 7SegDisplay
    Disp2b7Seg *_lc;
    
};







#endif // GAME_H
