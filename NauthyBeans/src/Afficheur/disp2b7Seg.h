/* ***********************************************************
    Note :

    LiquidCrystal_SC
    Allow to defined special character and more
*/

#ifndef DISP2B7SEG
#define DISP2B7SEG

//We always have to include the library
#include "LedControl.h"

// 7SEGMENT ADDR
#define DISP_SYSTEM   1
#define DISP_PLAYER1  0
#define DISP_PLAYER2  2




/* *************************
    7 SEGMENTS DISPLAY
 * ************************/
/*
   DIN = D7, CS = D8, CLK = D5, Number of MAX7219 chips = 1
   On NodeMCU Pin are like
   D5 GPIO14 HSCLK
   D6 GPIO15 HMISO
   D7 GPIO16 HMOSI
   D8 GPIO17 HCS

   Usage of LedControl see http://wayoda.github.io/LedControl/pages/software

 * */
LedControl lc = LedControl(D7, D5, D8, 3);



class Disp2b7Seg {

  public :
    Disp2b7Seg() {

    }


    void initialize() {
      for (int index = 0; index < lc.getDeviceCount(); index++) {
        lc.shutdown(index, false);  // wakeup call
        lc.setIntensity(index, 8);  // Set brightness level (0 is min, 15 is max)
        lc.clearDisplay(index);     // Clear display register
      }
    }


    /**
     * @brief Count number of device available on bus
     * 
     * @return int 
     */
    int dispCounter() const {
      return lc.getDeviceCount();
    }



    /* ****************************************************************************
      printNumber

      print a number between -999 and 999
    *****************************************************************************/
    bool printNumber(int v, int deviceAddr = 0, bool firstbyte = false) {
      // Data
      int ones;
      int tens;
      int hundreds;
      int nk = 0;
      int vo = v;
      int nbyte = 3;
      boolean negative = false;


      // If write on second byte
      if (firstbyte)  nbyte = 7;

      // Limit display value
      if (v < -999 || v > 9999) {
        return false;
      }

      // Manage negative number
      if (v < 0) {
        negative = true;
        v = v * -1;
      }

      // Get Digits !
      ones = v % 10;
      v = v / 10;

      tens = v % 10;
      v = v / 10;

      hundreds = v % 10;
      v = v / 10;

      nk = v % 10;


      //out("Décomposition en digit  : " + std::to_string(hundreds) + " / "  + std::to_string(tens) + " / "  + std::to_string(ones) + " )");

      if (negative) {
        //print character '-' in the leftmost column
        lc.setChar(deviceAddr, nbyte, '-', false);
      }
      else {
        //print a blank in the sign column
        if (nk == 0)
          lc.setChar(deviceAddr, nbyte, ' ', false);
        else
          lc.setDigit(deviceAddr, nbyte, (byte)nk, false);
      }
      //Now print the number digit by digit
      if (hundreds == 0 and vo < 100) lc.setChar(deviceAddr, nbyte - 1, ' ', false);
      else      lc.setDigit(deviceAddr, nbyte - 1, (byte)hundreds, false);

      if (tens == 0 and vo < 10) lc.setChar(deviceAddr, nbyte - 2, ' ', false);
      else      lc.setDigit(deviceAddr, nbyte - 2, (byte)tens, false);

      lc.setDigit(deviceAddr, nbyte - 3 , (byte)ones, false);

      //  lc.setDigit(0, nbyte-1, (byte)hundreds, false);
      //  lc.setDigit(0, nbyte-2, (byte)tens, false);
      //  lc.setDigit(0, nbyte-3, (byte)ones, false);

      return true;
    }


    /**
     * @brief Print a text to a 7 digits display
     * 
     * @param text text to be display 
     * @param addr device address to be dispaly
     */
    void printText(String text, int addr = 0){
      String str = text;
      str = str.substring(0, 8);
      //Serial.println("Text  = [" + text + "] Str = [" + str + "] Length Str = " + String(str.length()));
      str.toUpperCase();

      int j = 0;
      for (int i=str.length()-1; i >= 0; i--){
        char c = str.charAt(j);
        lc.setChar(addr, i, c, false);
        j++;
      }
    }

    /**
     * @brief Print a text to a 7 digits display
     * 
     * @param text text to be display 
     * @param addr device address to be dispaly
     * @param from from which point to start writing text default 0. from can 
     * not be more than 7 if so will be limited to 7
     * @param to when to we need to stop display writing
     */
    void printTextLR(String text, int deviceAddr = 0, int from = 0, int to = 7){
      String str = text;
      str = str.substring(0, 8);
      //dbg("Text  = [" + text + "] Str = [" + str + "] Length Str = " + String(str.length()));
      str.toUpperCase();

      // Secure parameter from 
      int j = 7 - from;
      if(j < 0){
        j = 0;
      }
      // Secure parameter from 
      int k = 7 - to;
      if(k < 0 or j <= k){
        k=0;
      }

      // Start printing text
      for (int i=0; i <= str.length()-1; i++){
        char c = str.charAt(i);
        lc.setChar(deviceAddr, j, c, false);
        j--;
        // Stop loop if out off context
        if(j<0 or j<k){
          i = str.length();
        }
      }
    }


    void setChar(int addr, int digit, char value, boolean dp){
      lc.setChar(addr, digit, value, dp);
    }

    void setRow(int addr, int row, byte value){
      lc.setRow(addr, row, value);
    }

  private:


};


#endif // DISP2B7SEG
