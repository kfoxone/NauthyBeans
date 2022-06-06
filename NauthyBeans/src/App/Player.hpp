/* ****************************************************************************
 *
 *
 *
 *
 * ***************************************************************************/
#ifndef PLAYER_H
#define PLAYER_H
// ============================================================================
//
//  INCLUDES
//
// ============================================================================
#include <string>
#include <iostream>




class Player {
  public:
    Player(){ 
      _pseudo = "U" + std::to_string(millis()); 
      _firstname = ""; 
      _lastname = ""; 
      _score = -1; 
      _inputs = new bool[10]{true, true, true, true, true, true, true, true, true, true};
      _leds = new bool[10]{false, false, false, false, false, false, false, false, false, false};
    }
    Player(std::string pseudo){ 
      _pseudo = "U" + std::to_string(millis()); 
      _firstname = ""; 
      _lastname = ""; 
      _score = -1; 
      _inputs = new bool[10]{true, true, true, true, true, true, true, true, true, true};
      _leds = new bool[10]{false, false, false, false, false, false, false, false, false, false};
    }
    Player(std::string pseudo, int score):_pseudo(pseudo), _firstname(""), _lastname(""), _score(score){
      _inputs = new bool[10]{true, true, true, true, true, true, true, true, true, true};
      _leds = new bool[10]{false, false, false, false, false, false, false, false, false, false};
      Serial.println("Player >> Player << pseudo " + String(_pseudo.c_str()) + " created !");
    }



    /**
     * @brief Set the Leds object
     * 
     * @param leds 
     */
    void changeLight(bool *ledTab){
      dbg("Player >> ChangeLight >> Start Change Light for player " + String(_pseudo.c_str()) + " Avec taille " + String(sizeof(ledTab)));
      dbgnl("Player >> ChangeLight >> Led for player " + String(_pseudo.c_str()));
      for(int i = 0; i < 10; i++){
        Serial.print(String(ledTab[i]==true?1:0) + ", ");
         _leds[i] = ledTab[i];
      }
      dbg("");
    }

    /**
     * @brief 
     * 
     * @param light 
     * @param state 
     */
    void updateLight(int light, bool state){
      _leds[light] = state;
    }


    /**
     * @brief Clear all led state to false
     * 
     */
    void clear(){
      for(int i =0; i < 10; i++){
         _leds[i] = false;
         _inputs[i] = true;
      }
    }


    void scoreAdd(int addVal = 1){
      _score += addVal;
    }

    void scoreSub(int subVal = 1){
      _score -= subVal;
    }

    void scoreInit(int initVal = 0){
      _score = initVal;
    }


  /* **************************************************************************
  *
  *
  * SURGCHARGE DES OPERATEURS
  * 
  * 
  * *************************************************************************/
  public:
    // Surcharge de l'opérateur d'affectation
    void operator=(const Player &player) { 
      _pseudo = player._pseudo;
      _firstname = player._firstname;
      _lastname = player._lastname;
      _score = player._score;
      _inputs = player._inputs;
      _leds = player._leds;
    }

    // Surcharge de l'opérateur d'égalité
    bool operator==(const Player &player){
      if(_pseudo != player._pseudo){
        return false;
      };
      if(_firstname != player._firstname) return false;
      if(_lastname != player._lastname) return false;
      if(_score != player._score) return false;
      if(_leds != player._leds) return false;
      if(_inputs != player._inputs) return false;

      return true;
    }

    // Surcharge de l'opérateur test égalité
    friend bool operator==(const Player &player1, const Player &player2){
      if(player1._pseudo != player2._pseudo)   return false;
      if(player1._firstname != player2._firstname) return false;
      if(player1._lastname != player2._lastname) return false;
      if(player1._score != player2._score) return false;
      if(player1._leds != player2._leds) return false;
      if(player1._inputs != player2._inputs) return false;

      return true;
    }



  public :
    std::string getPseudo() const { return _pseudo; }
    void setPseudo(std::string pseudo){ _pseudo = pseudo;  }

    std::string getFirstname() const { return _firstname; }
    void setFirstname(std::string firstname){ _firstname = firstname;  }

    std::string getLastname() const { return _lastname; }
    void setLastname(std::string lastname){ _lastname = lastname;  }

    int getScore() const { return _score; }
    void setScore(int score){ _score = score;  }

    bool* getLeds(){  return _leds;   }
    void setLeds(bool *leds){   _leds = leds;    }

    bool* getInputs(){  return _inputs;   }
    void setInputs(bool *inputs){   _inputs = inputs;    }




    void setLed(int i, bool state=true){
      _leds[i] = state;
    }
    bool getLed(int i){
      return _leds[i];
    }
    void setInput(int i, bool state=true){
      _inputs[i] = state;
    }
    bool getInput(int i){
      return _inputs[i];
    }

  private : 
    std::string _pseudo;
    std::string _firstname;
    std::string _lastname;
    int _score;

    /// Control Input/Output
    //HardwareControl _hwCtrl;
    
    bool *_inputs;
    bool *_leds;
};








#endif // PLAYER_H
