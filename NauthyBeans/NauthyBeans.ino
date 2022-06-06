/* ****************************************************************************
 *
 *
 *
 * rsa3072
 * ***************************************************************************/

// ============================================================================
//
//  INCLUDES
//
// ============================================================================


// UTIL
#define UTIL_DBG
#include "src/util/util.hpp"

// Gestion du bus I2C
// Outil général
#include "src/i2c/scani2c.h"
#include "src/i2c/I2CDevice.hpp"
#include "src/i2c/MCPDevice.hpp"

// Défintiion pour les afficheur 7 Segment
#include "src/Afficheur/disp2b7Seg.h"

// Définition pour l'écran LCD
// Gestion d'un LCD 20 colonne et 4 ligne
#include "src/lcd/lcd20_4.h"
#include "src/lcd/LCDMenuItem.hpp"

// Gestion Bouton de commande de rotation
//
#include "src/Bouton/rotationcommand.h"

// NauthyBeans App
#include "src/App/NauthyBeans.hpp"

// ACS 712 Current Input Measure
#include "Irms_Calc.h"

/// Ruban management
#include "src/FastLed/ruban.hpp"

/// Test App
#include "src/App/Test.hpp"

/// FightGame
#include "src/App/FightGame.hpp"



// ============================================================================
//
//  DEFINES
//
// ============================================================================

//
// NodeMCU GPIO Definition
//
#define RXD1      0                 // GPIO0
#define TXD1      2                 // GPIO2
#define SD3       10
#define SD2       9
#define SD1       8              
#define SC        11        
#define S0        7      
#define SK        6     

#define ADC       A0
#define sensorIn  A0;



#define INTERRUPTED_PIN D4          // Interrupt Pin


/// HARDWARE DIVICE
#define harwareCnt    7

// 7SEGMENT ADDR
#define DISP_SYSTEM   1
#define DISP_PLAYER1  0
#define DISP_PLAYER2  2




// DEGGING
#define _DBG
// Allow to DEBUG UNDERDESCRIPTION
//#define _DBGA





/* ============================================================================
 * Global Data
 *
 *
 * ==========================================================================*/
enum broche {
  pl100 =  0,
  pl101 =  1,
  pl102 =  2,
  pl103 =  3,
  pl104 =  4,
  pl105 =  5,
  pl106 =  6,
  pl107 =  7,
  pl108 =  8,
  pl109 =  9,
  Start =  10,
  free0 =  11,
  free1 =  12,
  free2 =  13,
  pl200 =  14,
  pl201 =  15,
  pl202 =  0,
  pl203 =  1,
  pl204 =  2,
  pl205 =  3,
  pl206 =  4,
  pl207 =  5,
  pl208 =  6,
  pl209 =  7,
};


// ============================================================================
//
// GLOBAL DATA
//
// ============================================================================

/* *************************
    GLOBAL SET
 * ************************/

/* *************************
    SOME TIME SET
 * ************************/
unsigned long dt01 = 5;
unsigned long dt02 = 50;
unsigned long dt03 = 100;
unsigned long dt04 = 250;
unsigned long dt05 = 500;
unsigned long dt06 = 750;
unsigned long dt07 = 1000;
unsigned long dt08 = 1500;
unsigned long dt09 = 2000;
unsigned long dt10 = 3000;
unsigned long dt11 = 5000;

/* *************************
   ELAPSE TIME
 * ************************/
unsigned long   elapseTime;
unsigned long   elapseTimePrev;

unsigned long   secs;
unsigned long   secsd;
unsigned long   mins;
unsigned long   hour;

/* *************************
    APPLICATION DATA
 * ************************/
NauthyBeans beans = NauthyBeans();



/* **************************
    LCD
 * **************************/
LCD20_4 lcd;
/// @brief Device for LCD management
I2CDevice lcdDevice;

LCDMenuItem *_menu;

/* **************************
    DISPLAY 2 BYTES 7 SEG GROUPE IN SPI
 * **************************/
Disp2b7Seg _lc;

/* **************************
    Gestion à l'aide du bouton menu
 * **************************/
RotationCommand _rtC = RotationCommand();


/* **************************
    PORT EXTENSION
 * **************************/
MCPPIN _bus24[16] = {{PA0, INPUT, "PL1B01", HIGH},
                {PA1, INPUT, "PL1B02", HIGH},
                {PA2, INPUT, "PL1B03", HIGH},
                {PA3, INPUT, "PL1B04", HIGH},
                {PA4, INPUT, "PL1B05", HIGH},
                {PA5, INPUT, "PL1B06", HIGH},
                {PA6, INPUT, "PL1B07", HIGH},
                {PA7, INPUT, "PL1B08", HIGH},
                {PB0, INPUT, "PL1B09", HIGH},
                {PB1, INPUT, "PL1B10", HIGH},
                {PB2, INPUT, "STARTB", HIGH},
                {PB3, INPUT, "SPARB1", HIGH},
                {PB4, INPUT, "SPARB2", HIGH},
                {PB5, INPUT, "SPARB3", HIGH},
                {PB6, INPUT, "PL2B01", HIGH},
                {PB7, INPUT, "PL2B02", HIGH}
                 };
MCPPIN _bus25[16] = {{PA0, INPUT, "PL2B03", HIGH},
                {PA1, INPUT, "PL2B04", HIGH},
                {PA2, INPUT, "PL2B05", HIGH},
                {PA3, INPUT, "PL2B06", HIGH},
                {PA4, INPUT, "PL2B07", HIGH},
                {PA5, INPUT, "PL2B08", HIGH},
                {PA6, INPUT, "PL2B09", HIGH},
                {PA7, INPUT, "PL2B10", HIGH},
                {PB0, OUTPUT, "PL1L01"},
                {PB1, OUTPUT, "PL1L02"},
                {PB2, OUTPUT, "PL1L03"},
                {PB3, OUTPUT, "PL1L04"},
                {PB4, OUTPUT, "PL1L05"},
                {PB5, OUTPUT, "PL1L06"},
                {PB6, OUTPUT, "PL1L07"},
                {PB7, OUTPUT, "PL1L08"}
                 };
MCPPIN _bus26[16] = {{PA0, OUTPUT, "PL1L09"},
                {PA1, OUTPUT, "PL1L10"},
                {PA2, OUTPUT, "STARTL"},
                {PA3, OUTPUT, "SPARE1"},
                {PA4, OUTPUT, "SPARE2"},
                {PA5, OUTPUT, "SPARE3"},
                {PA6, OUTPUT, "PL2L01"},
                {PA7, OUTPUT, "PL2L02"},
                {PB0, OUTPUT, "PL2L03"},
                {PB1, OUTPUT, "PL2L04"},
                {PB2, OUTPUT, "PL2L05"},
                {PB3, OUTPUT, "PL2L06"},
                {PB4, OUTPUT, "PL2L07"},
                {PB5, OUTPUT, "PL2L08"},
                {PB6, OUTPUT, "PL2L09"},
                {PB7, OUTPUT, "PL2L10"}
                 };

 
MCPDevice IO24;
MCPDevice IO25;
MCPDevice IO26;



/// Current Management
int sensorPin = A0;    // select the input pin for the current
int sensorValue = 0;  // variable to store the value coming from the sensor
ACS712_Irms acs712;
/// Amps measure consume on main power 220VAC
double _AmpsRMS = 0;      // Valeur de courant en Ampère RMS



/// FightGame
FightGame *_fgame ;
/// Specify need to be start and initiate game
bool _fightGame_start = true;
/// Specify game is in running mode
bool _fightGame_running = false;



int _looper = 0;
unsigned long _elapse_time_looper = millis();
#define TIEMOUT_I2C_VALIDATION  5000

// ============================================================================
//
// SYSTEM TOOL
//
// ============================================================================


/* ****************************************************************************
   out

   Use serial to print out message
 * ***************************************************************************/
unsigned long onElapse(unsigned long &prev, bool enableChange = true) {
  unsigned long elapsedTime = millis() - prev;
  if (enableChange)  prev = millis();
  return elapsedTime;
}





// ============================================================================
//
// MAIN PROGRAM
//
// ============================================================================



/** ***************************************************************************
 * @brief Set the up lcd object
 * 
 * Prepare LCD for display.
 * 
 * ****************************************************************************/
void setup_lcd(){
  // Init. LCD
  lcd.linit();
  lcd.linitSpecialChar();
  lcd.lpush("* **************** *");
  lcd.lpush("*   Nauthy Beans   *");
  lcd.lpush("*    Let's Play    *");
  lcd.lpush("* **************** *");
  delay(1000);
  lcd.lclear();
}


/** ***************************************************************************
 * @brief Setup i2c Devices
 * 
 * Prepare definition of all i2c
 * 
 * ****************************************************************************/
void setup_i2cDevices(){

  // Init. Device
  beans.clearI2CDevices();
  beans.clearMCPDevices();
  
  // Scan device online
  ScanI2C scan = ScanI2C();
  if(scan.isAvalaible(0x24)){
    IO24 = MCPDevice(4, _bus24, "MCP23017 BUS24 A/B");
    beans.addDevice(IO24);
    Serial.println("NauthyBeans >> setup_i2cDevices : IO24 Create and add !====");
  }
  if(scan.isAvalaible(0x25)){
    IO25 = MCPDevice(5, _bus25, "MCP23017 BUS25 A/B");
    beans.addDevice(IO25);
    Serial.println("NauthyBeans >> setup_i2cDevices : IO25 Create and add !====");
  }
  if(scan.isAvalaible(0x26)){
    IO26 = MCPDevice(6, _bus26, "MCP23017 BUS26 A/B");
    beans.addDevice(IO26);
    Serial.println("NauthyBeans >> setup_i2cDevices : IO26 Create and add !====");
  }

  if(scan.isAvalaible(0x27)){
    lcdDevice = I2CDevice(0x27, "LCD2004 20x4");
    beans.addDevice(lcdDevice);
    Serial.println("NauthyBeans >> setup_i2cDevices : IO24 Create and add !====");
  }



  lcd.lreturn();
  lcd.lpush("Initialisation...");
  std::string msg = "Defined I2C device = " + std::to_string(beans.getI2CDevices().size());
              msg = "Defined MCP device = " + std::to_string(beans.getMCPDevices().size());
  lcd.lpush(msg);

  // Desplay defined device
  std::list<MCPDevice> m = beans.getMCPDevices();
  int i = 1;
  for (auto it = m.begin(); it != m.end(); ++it) {
    msg = std::to_string(i) +" : " + (*it).getName().c_str();
    lcd.lpush(msg);
    char hex_string[4];
    sprintf(hex_string, "%X", (*it).getAddr());
    msg = "    Dec(" + std::to_string((*it).getAddr()) + ", 0x" + hex_string + ")";
    lcd.lpush(msg);

    i++;
    delay(125);
  }

    // Desplay defined device
  std::list<I2CDevice> t = beans.getI2CDevices();
  i = 1;
  for (auto it = t.begin(); it != t.end(); ++it) {
    msg = std::to_string(i) + " : " + (*it).getName();
    lcd.lpush(msg);
    char hex_string[4];
    sprintf(hex_string, "%X", (*it).getAddr());
    msg = "    Dec(" + std::to_string((*it).getAddr()) + ", 0x" + hex_string + ")";
    lcd.lpush(msg);

    i++;
    delay(125);
  }
}



/** ***************************************************************************
 * @brief Setup i2c Validation
 * 
 * Check on i2c available device and count it. Compare it to number of i2c
 * devices defined. If result is deferent operate identification missing devices
 * Otherwise go forward !
 * 
 * If missing device go 
 * 
 * ****************************************************************************/
bool setup_i2cValidation(){
  
  std::string msg = "";
  

  // Scan device online
  ScanI2C scan = ScanI2C();
  scan.listToMonitor();
  int nDevices = scan.availableDevices();
  Serial.println("NauthyBeans >> setup_i2cValidation >> nombre d'appareil trouvé sur le bus = " + String(nDevices));

  std::list<I2CDevice> i2cDevices = beans.getI2CDevices();
  std::list<MCPDevice> mcpDevices = beans.getMCPDevices();

  int i2cDeviceCnt = i2cDevices.empty()?0:i2cDevices.size();
  int mcpDeviceCnt = mcpDevices.empty()?0:mcpDevices.size();


  Serial.println("NauthyBeans >> setup_i2cValidation >> Nombre d'appareil définit (i2cDeviceCnt, mcpDeviceCnt) = (" + String(i2cDeviceCnt) + ", " + String(mcpDeviceCnt) + ")");

  if (nDevices == (i2cDeviceCnt + mcpDeviceCnt)) {
    msg = "All device founded.";
    lcd.lpush(msg);
    delay(250);
    Serial.println("Tous les appareils sont disponible !");
    return true;
  } else {
    lcd.lpush("/!\\ " + std::to_string((i2cDeviceCnt + mcpDeviceCnt)-nDevices) + " manque(nt) ! ");
    Serial.println("NauthyBeans >> setup_i2cValidation >> /!\\ Un ou des appareils manquent : " + String((i2cDeviceCnt + mcpDeviceCnt) - nDevices) + " ! ");
    delay(3000);


    // Display missing device
    int i = 1;
    for (auto it = mcpDevices.begin(); it != mcpDevices.end(); ++it) {
      if (!scan.isAvalaible((*it).getAddr(1))) {
        msg = std::to_string(i) + " : " + (*it).getName().c_str();
        lcd.lpush(msg);
        char hex_string[4];
        sprintf(hex_string, "%X", (*it).getAddr(1));
        msg = "    Dec(" + std::to_string((*it).getAddr(1)) + ", 0x" + hex_string + ")";
        lcd.lpush(msg);
        msg = "    !!! Missing !!!";
        Serial.println(String(i) + " : " +  (*it).getName().c_str() + "    Dec(" + std::to_string((*it).getAddr(1)).c_str() + "0x" + std::string("%X", (*it).getAddr(1)).c_str()+ ")    !!! Missing !!!");
        lcd.lpush(msg);
      }
      i++;
      delay(1000);
    }


    // Display missing device
    i = 1;
    for (auto it = i2cDevices.begin(); it != i2cDevices.end(); ++it) {
      if (!scan.isAvalaible((*it).getAddr())) {
        msg = std::to_string(i) + " : " + (*it).getName();
        lcd.lpush(msg);
        char hex_string[4];
        sprintf(hex_string, "%X", (*it).getAddr());
        msg = "    Dec(" + std::to_string((*it).getAddr()) + ", 0x" + hex_string + ")";
        lcd.lpush(msg);
        msg = "    !!! Missing !!!";
        Serial.println(String(i) + " : " +  (*it).getName().c_str() + "    Dec(" + std::to_string((*it).getAddr()).c_str() + "0x" + std::string("%X", (*it).getAddr()).c_str()+ ")    !!! Missing !!!");
        lcd.lpush(msg);
      }
      i++;
      delay(1000);
    }
  }

  Serial.println("NauthyBeans >> i2cValidation : end...");

}

/** ***************************************************************************
 * @brief Set the up SPI object
 * 
 * ****************************************************************************/
void setup_SPI(){
  // The MAX7219 chip is in power-saving mode on startup
  _lc.initialize();
  lcd.lreturn();
  lcd.lpush("SPI device found...");
  lcd.lpush("==> " + std::to_string(_lc.dispCounter()) + " of SPI Dev");
  Serial.println("SPI Device found... ===> " + String(_lc.dispCounter()) + " of SPI DEV");

  _lc.printText("HELLOP01", DISP_PLAYER1);
  _lc.printText("HELLOSYS", DISP_SYSTEM);
  _lc.printText("HELLOP02", DISP_PLAYER2);
  _lc.setRow(DISP_PLAYER1, 3, 0B01111110); // 0, O, D
  _lc.setRow(DISP_SYSTEM, 3, 0B01111110); // 0, O, D
  _lc.setRow(DISP_SYSTEM, 2, 0B01011011); // S
  _lc.setRow(DISP_SYSTEM, 1, 0B00111011); // Y
  _lc.setRow(DISP_SYSTEM, 0, 0B01011011); // S
  _lc.setRow(DISP_PLAYER2, 3, 0B01111110); // 0, O, D
  delay(2000);
}


/** ***************************************************************************
 * @brief Set the up rotationCommand object
 * 
 * ****************************************************************************/
void setup_rotationCommand(){
  elapseTime = onElapse(elapseTimePrev);
  lcd.lpush("Ready after " + std::to_string(elapseTime / 1000));
  
  // Initialize rotary command for NodeMCU
  _rtC.initialize();
  
}


/** ***************************************************************************
 * @brief Set the up ACS712 object
 * 
 * ****************************************************************************/
void setup_ACS712(){
  acs712.ADCSamples = 1024.0; //1024 samples
  acs712.mVperAmp = scaleFactor::ACS712_30A; // use 100 for 20A Module and 66 for 30A Module and 185 for 5A Module
  acs712.maxADCVolt = 3.3; //5 Volts
  acs712.ADCIn = ADC;
  acs712.Init(); 
}


/** ***************************************************************************
 * @brief Set the up ws2812B object
 * 
 * ****************************************************************************/
void setup_ws2812B(){
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(_crgbleds, NUM_LEDS);  // GRB ordering is typical
}


/* ****************************************************************************
     Setup

     Main program setup

 *****************************************************************************/
void setup() {

  delay(300);
  // Configuration du moniteur
  Serial.begin(115200);
  while (!Serial); // wait for serial monitor
  delay(3000);

  // Timing
  elapseTime = onElapse(elapseTimePrev);
  secs = 0;
  secsd  = 9999;
  
  

  // Components
  setup_lcd();
  Serial.println("NauthyBeans >> setup << setup lcd finished...");
  setup_i2cDevices();
  Serial.println("NauthyBeans >> setup << setup i2c Devices finished...");
  setup_SPI();
  Serial.println("NauthyBeans >> setup << setup SPI finished...");
  setup_rotationCommand();
  Serial.println("NauthyBeans >> setup << setup rotation command finished...");
  setup_i2cValidation();
  Serial.println("NauthyBeans >> setup << i2C validation is finished...");
  // 
  setup_ACS712();
  Serial.println("NauthyBeans >> setup << setup ACS712 Finished ! ");
  //
  setup_ws2812B();
  Serial.println("NauthyBeans >> Setup << setup WS2812B Finished !");


  LCDMenuItem *infos = new LCDMenuItem("INFORMATIONS");
  LCDMenuItem *voltage = new LCDMenuItem("SOURCES (V)");
  LCDMenuItem *light = new LCDMenuItem("TEST");
  _menu = new LCDMenuItem("MENU [ISM]");
  _menu->setCurrentMenuItem();
  _menu->enableChanged();
  _menu->add(infos);
  _menu->add(voltage);
  _menu->add(light);

  _elapse_time_looper = millis();
}






/**
 * @brief Loop read input Image
 * 
 * 
 */
void loop_input(){
  // if(!IO24.isNull()){
  //   IO24.readInput();
  // }else{
  //   Serial.println("Unable to read IO24 : isNUll");
  // }

  // if(!IO25.isNull()){
  //   IO25.readInput();
  // }else{
  //   Serial.println("Unable to read IO25 : isNUll");
  // }

  //
  // FightGame
  //
  if(_fightGame_running){
    fightGame_pullIn();
  }


  //
  // Measure Current
  //
  processAmps();


}




/**
 * @brief Output Loop to write output
 * 
 */
void loop_output(){
  // 
  // FightGame
  //
  if(_fightGame_running){
    fightGame_pushOut();
  }
}




//#define _DBGB


/** ***************************************************************************
 * @brief Main Loop
 * 
 * Main Program
 *
 * ***************************************************************************/
void loop() {

  #ifdef _DBGB
  Serial.println("Start loop " + looper);
  #endif

  // RESET LOOP CONTROLLEUR
  _looper++;
  if (_looper > 100000)  _looper = 0;

  if(millis()-_elapse_time_looper >= 5000){ // 5s
    dbg("NauthyBeans >> loop >> Average loop time = " + String((millis()-_elapse_time_looper)/5000.000) + "ms");
    _looper = 0;
    _elapse_time_looper = millis();
  }


  // Go to initialisation system
  static uint64_t i2cValidationTimeout = millis();
  //while (!setup_i2cValidation() and millis()-i2cValidationTimeout <= TIEMOUT_I2C_VALIDATION)  delay(1000);


  //
  // READ INPUT STATE
  // 
  #ifdef _DBGB
  Serial.println("Go to loop input ");
  #endif
  loop_input();



  //
  // Process Fight Game
  //
  fightGame_run();

  //
  // Oled display loop
  //
  _menu->on_show(&lcd);


  //
  // TESTING TOOL
  //
  #ifdef _DBGB
  Serial.println("Go to testing tool");
  #endif
  //testOnState();
  //testBlinkAll(500);
  //testSequence(250);
  //testSequenceBtn(5000, IO24, IO25, IO26);






  // WS2812B
  //loop_ws2812B();
  //loop_ColorPalette();










  //
  // Manage RotationCommand for menu ! 
  //
  if(_rtC.isSWOn()){
    //lpush("rtC Pushed! L=" + std::to_string(looper));
    _rtC.resetRotationCnt();
  }
  _rtC.doRun();
  //_lc.printNumber(_rtC.getRotationCnt(), DISP_SYSTEM, false);
  



  //
  // MANAGE OUTPUT
  //
  loop_output();

}








//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================

/** ***************************************************************************
 * @brief Porcessing input current consomution measurment
 * 
 * 
 * ***************************************************************************/
void processAmps(){
    // static uint64_t timeToReadCurrent = millis();
  // if(millis()-timeToReadCurrent>= 1000){
  //   double AmpsRMS = acs712.Process();
  //   Serial.println("Amps RMS: " + String(AmpsRMS));
  //   timeToReadCurrent = millis();
  // }

  // Normal 
  static uint64_t adcTimeOut = millis();
  if(millis()-adcTimeOut >= 5000){
    sensorValue = analogRead(sensorPin);
    _AmpsRMS = 1.00*sensorValue/1024.0;
    Serial.println("Loop >> Sensor Input = " + String(sensorValue) + " Value converte by 1024 = " + String(_AmpsRMS));
    adcTimeOut = millis();
  }
}







//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================


/** ***************************************************************************
 * @brief Initialise Fight Game
 * 
 * 
 * ***************************************************************************/
void fightGame_init(){
  // 
  /// FightGame
  Player p1("Joleen", 0);
  Player p2("Shelly", 0);
  Serial.println("NauthyBeans >> fightGame_init << Create Player finished !");
  _fgame = new FightGame(p1, p2);
  Serial.println("NauthyBeans >> fightGame_init << fight game creation finished !");
  _fgame->init();
  Serial.println("NauthyBeans >> fightGame_init << fight game initialisation finished !");
}


/** ***************************************************************************
 * @brief 
 * 
 * ***************************************************************************/
void fightGame_pushOut(){
  // Player 1
  for(int i = 0; i < 10; i++){
    bool ledState = _fgame->getPlayer1()->getLed(i);
    if(ledState==true){
      if(i<8){
        IO25.onB(i);
      }else{
        IO26.onA(i-8);
      }
    }else{
      if(i<8){
        IO25.offB(i);
      }else{
        IO26.offA(i-8);
      }
    }
  }

  // Player 2
  for(int i = 0; i < 10; i++){
    bool ledState = _fgame->getPlayer2()->getLed(i);
    if(ledState==true){
      if(i<2){
        IO26.onA(i+6);
        //dbg("NauthyBeans >> fightGame_pushOUt >> i = " + String(i) + " state : true and write on IO26.PA" + String(i+6));
      }else{
        IO26.onB(i-2);
      }
    }else{
      if(i<2){
        IO26.offA(i+6);
        //dbg("NauthyBeans >> fightGame_pushOUt >> i = " + String(i) + " state : false and write off IO26.PA" + String(i+6));
      }else{
        IO26.offB(i-2);
      }
    }
  }
}

/** ***************************************************************************
 * @brief  fightGame_pullIn
 * From input readed affected player input state.
 * 
 * ***************************************************************************/
void fightGame_pullIn(){
  // dbg("NauthyBeans >> FightGame_pullIn >> Start ...");

  if(!IO24.isNull()){
    for(int i = 0; i < 16; i++){
      if(!IO24.isNull()){
        if(i<10){
          _fgame->getPlayer1()->setInput(i, IO24.isOnRise(i));
        }else if(i >=14){
          _fgame->getPlayer2()->setInput(i-14, IO24.isOnRise(i));
        }
      }

      if(!IO25.isNull()){
        if(i<8){
          _fgame->getPlayer2()->setInput(i+2, IO25.isOnRise(i));
        }
      }
    }
  }
}




/** ***************************************************************************
 * @brief  Fight Game running main process
 * 
 * ***************************************************************************/

//  static uint64_t elapseTimeToStartGame = 
void fightGame_run(){
  static uint64_t elapseTimeToResetGame = millis();
  static uint64_t elapseTimeToFinishGame = millis();
  static uint64_t elapseDisplayTime = millis();
  static uint64_t elapseFinishGameTime = millis();
  static uint64_t elapseDisplayRstBlinkTime = millis();

  // 
  if(!IO24.isNull() and !_fightGame_start){
    if(!IO24.isOn(broche::Start)){
      uint64_t elapseResetMs = millis()-elapseTimeToResetGame;
      
      if(millis()-elapseDisplayTime>=1000){
        dbg("NauthyBeans >> fightGame_run >> Reset count down " + String((5000-elapseResetMs)/1000));
      }
      if(elapseResetMs >= 5000){ // 5secs
        _fightGame_start = true;
        elapseTimeToResetGame =  millis();
        if(millis()-elapseDisplayTime>=1000){
          dbg("NauthyBeans >> fightGame_run >> reset done by restarting the game !");
        }
      }
      // Affichage clignotement led central reset.
      if(!IO26.isNull()){
        if(millis()-elapseDisplayRstBlinkTime<=500){
          IO26.onA(2);
        }else{
          IO26.offA(2);
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
    if(!IO26.isNull()){
      IO26.onA(2);
    }
  }






  // 
  // Operate evrything while gamme is running
  //
  if(_fightGame_running){
    //
    _fgame->run();
    
    // Calculate remaining time
    elapseFinishGameTime = millis() - elapseTimeToFinishGame;
    _fgame->setRemainingTime(_fgame->getPresetGameTime() - elapseFinishGameTime);
    //dbg("NauthyBeans >> FightGame_run >> ElpaseFinishGameTime = " + String(elapseFinishGameTime) + " ElapseTimeToFinishGame = " + String(elapseTimeToFinishGame));

    // 
    if(elapseFinishGameTime >= _fgame->getPresetGameTime()){ // 60000
      _fightGame_running = false;

      if(_fgame->getPlayer1()->getScore()>_fgame->getPlayer2()->getScore()){
        ws2812BColorFirstHalf(CRGB::Red);
        ws2812BColorSecondHalf(CRGB::Green);
      }else if(_fgame->getPlayer1()->getScore()<_fgame->getPlayer2()->getScore()){
        ws2812BColorFirstHalf(CRGB::Green);
        ws2812BColorSecondHalf(CRGB::Red);
      }else{
        ws2812BColorAll(CRGB::Orange);
      }
    }else{
      loop_ColorPalette();
    }

    // Process scoring
    // Player 1
    _lc.printNumber(_fgame->getPlayer1()->getScore(), DISP_PLAYER1);
    _lc.printNumber((int)_fgame->getRemainingTime()/1000, DISP_PLAYER1, true);

    // Player 2
    _lc.printNumber(_fgame->getPlayer2()->getScore(), DISP_PLAYER2);
    _lc.printNumber((int)_fgame->getRemainingTime()/1000, DISP_PLAYER2, true);

    // System
    static uint64_t elapseTimeDisplaySystem = millis();
    static int fightGame_SysSequence = 0;
    if(millis()-elapseTimeDisplaySystem >= 1000){
      switch(fightGame_SysSequence){
        case 0 : // Score for player 1
          _lc.printTextLR("PL01", DISP_SYSTEM);
          _lc.printNumber(_fgame->getPlayer1()->getScore(), DISP_SYSTEM);
          fightGame_SysSequence++;
          elapseTimeDisplaySystem = millis();
          break;
        case 1 : // Score for player 2
          _lc.printTextLR("PL02", DISP_SYSTEM);
          _lc.printNumber(_fgame->getPlayer2()->getScore(), DISP_SYSTEM);
          fightGame_SysSequence++;
          elapseTimeDisplaySystem = millis();
          break;
        case 2 :
          _lc.printTextLR("SECS", DISP_SYSTEM);
          _lc.setRow(DISP_SYSTEM, 7, 0B01011011); // S
          _lc.setRow(DISP_SYSTEM, 5, 0B01001110); // C
          _lc.setRow(DISP_SYSTEM, 4, 0B01011011); // S
          _lc.printNumber((int)_fgame->getRemainingTime()/1000, DISP_SYSTEM);
          //fightGame_SysSequence++;
          fightGame_SysSequence = 0;
          elapseTimeDisplaySystem = millis();
          break;
        default:
          dbg("NauthyBeans >> fightGame_run >> Undefine switch sequence back to 0!");
          fightGame_SysSequence = 0;
          break;
      }
    }


  }




  //
  // SET Everything when game need to start
  //
  if(_fightGame_start){
    fightGame_init();
    _fightGame_start = false;
    _fightGame_running = true;
    ws2812BTurnOff();
    elapseTimeToFinishGame = millis();
    _fgame->scoreClear();
  }
}





//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================




















