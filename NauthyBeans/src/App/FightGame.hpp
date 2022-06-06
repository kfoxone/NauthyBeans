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





class FightGame {

  public:
    FightGame(){}
    FightGame(Player player1, Player player2 = Player()){  
      _player1 = player1; 
      _player2 = player2; 

      prepare();}
    FightGame(std::string namePlayer1, std::string namePlayer2 = "U" + std::to_string(millis())){ _player1 = Player(namePlayer1); _player2 = Player(namePlayer2);  prepare();}

  public:
    Player* getPlayer1(){ return & _player1; }
    Player* getPlayer2(){ return & _player2; }


    void run(){
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


    
    void init(){
      Serial.println("FightGame >> init >> start initialisation players ! ");
      clear();
      Serial.println("FightGame >> init >> ChangeLight invoke start...");
      _player1.changeLight(initLightWithRandom());
      _player2.changeLight(initLightWithRandom());
      
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
    void prepare(){
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

   
    
};







#endif // GAME_H
