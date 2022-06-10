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

#define CLK   D6   // encoder clk
#define DT    SD3  // encoder dt
#define SW    D0   // Rotari swtich


#define INTERRUPTED_PIN D4          // Interrupt Pin


/// HARDWARE DIVICE
#define harwareCnt    7






// DEGGING
#define _DBG
// Allow to DEBUG UNDERDESCRIPTION
//#define _DBGA
//#define _DBGB





/* ============================================================================
 * Global Data
 *
 *
 * ==========================================================================*/



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
RotationCommand *_rotationCommand;


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
FightGame *_fightGame;// = new FightGame("Joleen", "Shelly", &_lc) ;



/// Loop counter
int _looper = 0;
/// Elapse time over loops
unsigned long _elapse_time_looper = millis();
/// Measure of the loop duration
double _loop_duration_ms = 0.000;


/// I2C Timeout vaidation
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
  // Initialize rotary command for NodeMCU
  _rotationCommand = new RotationCommand(SW, DT, CLK);
  _rotationCommand->initialize();
  
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


/** ***************************************************************************
 * @brief Set the up ws2812B object
 * 
 * ****************************************************************************/
void setup_games(){
  // FightGame
  _fightGame = new FightGame("Joleen", "Shelly", &_lc) ;
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
  // 
  setup_games();
  dbg("NauthyBeans >> Setup << setup games finished ! ");

  LCDMenuItem *infos = new LCDMenuItem("INFORMATIONS");
  LCDMenuItem *voltage = new LCDMenuItem("SOURCES (V)");
  LCDMenuItem *light = new LCDMenuItem("TEST");
  _menu = new LCDMenuItem("MENU [ISM]");
  _menu->setCurrentMenuItem();
  _menu->enableChanged();
  _menu->add(infos);
  _menu->add(voltage);
  _menu->add(light);
  dbg("NauthyBeans >> Setup << Setup Menu Finished !");


  elapseTime = onElapse(elapseTimePrev);
  lcd.lpush("Ready after " + std::to_string(elapseTime / 1000));

  _elapse_time_looper = millis();
}






/** ***************************************************************************
 * @brief Loop read input Image
 * 
 * Convenient method to read input value from start
 * ***************************************************************************/
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
  _fightGame->pushIn(&IO24, &IO25);


  //
  // Measure Current
  //
  processAmps();


}




/** ***************************************************************************
 * @brief Output Loop to write output
 * 
 * Convenient method to set output value at end
 * ***************************************************************************/
void loop_output(){
  // 
  // FightGame
  //
  _fightGame->pushOut(IO25, IO26);

}




/** ***************************************************************************
 * @brief Main Loop
 * 
 * Main Program
 *
 * ***************************************************************************/
void loop() {
  //
  // RESET LOOP CONTROLLEUR
  //
  _looper++;
  if(millis()-_elapse_time_looper >= 5000){ // 5s
    _loop_duration_ms = (millis()-_elapse_time_looper)/5000.000;
    log("NauthyBeans >> loop >> Average loop time = " + String(_loop_duration_ms) + "ms");
    _looper = 1;
    _elapse_time_looper = millis();
  }


  //
  // Go to initialisation system
  //
  static uint64_t i2cValidationTimeout = millis();
  //while (!setup_i2cValidation() and millis()-i2cValidationTimeout <= TIEMOUT_I2C_VALIDATION)  delay(1000);


  //
  // READ INPUT STATE
  // 
  loop_input();
  ndbg("NauthyBeans >> loop >> Loop input finished !");



  //
  // Process Fight Game
  //
  _fightGame->run(IO24, IO25, IO26);



  //
  // Manage RotationCommand for menu ! 
  //
  if(_rotationCommand->isSWOn()){
    dbg("NauthyBeans >> Loop >> rtC push down !");
  }
  _rotationCommand->doRun();
  if(_rotationCommand->isEncoderChanged()){
    dbg("NauthyBeans >> Loop >> rtC encoder changed = " + String(_rotationCommand->getEncoderCounter()));
  }
  
  //
  // LCD display loop
  //
  _menu->on_show(&lcd);


  //
  // TESTING TOOL
  //
  //testOnState();
  //testBlinkAll(500);
  //testSequence(250);
  //testSequenceBtn(5000, IO24, IO25, IO26);
  // Test WS2812B
  //loop_ws2812B();
  //loop_ColorPalette();














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














//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================




















