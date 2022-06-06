#ifndef LCD20_4_h
#define LCD20_4_h
/* ***********************************************************
    Note :


*/


#include <iostream>
// Include the libraries:
// LiquidCrystal_I2C.h: https://github.com/johnrickman/LiquidCrystal_I2C
#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD


#include <iostream>
#include <iterator>
#include <list>

#include <algorithm>
#include <cctype>
#include <locale>



#include "../util/util.hpp"
#include "lcdSChar.hpp"


/* *************************
    LIQUID CRYSTAL DISPLAY
 * ************************/
#define LCD_I2C_ADDR 0x27
#define LCD_COL 20
#define LCD_ROW 4






class LCD20_4 {

public:
    
    LCD20_4();

    void backlightEnabled(bool en);
    void print(int row, int col, std::string text) {
      lcd->setCursor(row, col);
      lcd->print(text.c_str());
    }


    /// Initialaze lcd and enable backlight
    void linit() {
      lcd->init();
      lcd->backlight();
      linitSpecialChar();
    }



    /// Initialise les caractère spéciaux pour le lcd
    void linitSpecialChar() {
      //uint8_t h05c[8] { B10000, B01000, B01000, B00100, B00100, B00010, B00010, B00001 }; // Sym : 
      
      lcd->createChar(0, customChar0); // create a new custom character (index 0)
      lcd->createChar(1, customChar1); // create a new custom character (index 1)
      lcd->createChar(2, customChar2); // create a new custom character (index 2)
    }

    /// Using liquidcrystal on i2c to print text at position row and col.
    void lprint(int row, int col, std::string text) {
      lcd->setCursor(col, row);
      lcd->print(text.c_str());
      
    }

    /* ****************************************************************************
      lprint

      surcharge function to print a msg list to a screen.
      / ! \ this function not managing row limit so will try to write over max
      lcd row if msg list is not properly managed
    *****************************************************************************/
    void lprint(std::list<std::string> msg) {
      int row = 0;
      for (auto it = msg.begin(); it != msg.end(); ++it) {
        lprint(row, 0, *it);
        row++;
      }
    }

/* ****************************************************************************
   lpush

   from list display already created "ltext", the function unsure padding to
   the back for lcd max column.
   Then erase previous first text before adding new text at the back of the
   list.
   Finaly full list is printed
 *****************************************************************************/
void lpush(std::string text) {
  std::string a = text;
  int i = 0;

  while (a.size() > 0) {
    if (i >= 1)  delay(100);

    a = text.substr(i * (LCD_COL), LCD_COL);
#ifdef _DBG_F // if full debug
    Serial.print("i = "); Serial.print(i); Serial.print(" a1: "); Serial.print(a.c_str()); Serial.print(" size: "); Serial.print(a.size());
#endif
    padTo(a, LCD_COL);
#ifdef _DBG_F // if full debug
    Serial.print("i = "); Serial.print(i); Serial.print(" a2: "); Serial.print(a.c_str()); Serial.print(" size: "); Serial.print(a.size());
#endif
    std::list<std::string>::iterator it = ltext.begin();
    it = ltext.erase(it);
    ltext.push_back(a);
#ifdef _DBG_F // if full debug
    Serial.print("i = "); Serial.print(i); Serial.print(" a3: "); Serial.print(a.c_str()); Serial.print(" size: "); Serial.print(a.size());
#endif
    lprint(ltext);
#ifdef _DBG_F // if full debug
    Serial.print("i = "); Serial.print(i); Serial.print(" a4: "); Serial.print(a.c_str()); Serial.print(" size: "); Serial.print(a.size());
#endif
    i++;
    if ((i * (LCD_COL)) <= text.size()) {
      a = text.substr(i * LCD_COL, text.size());
    } else {
      a = "";
    }
#ifdef _DBG_F // if full debug
    Serial.print("i = "); Serial.print(i); Serial.print(" a5: "); Serial.print(a.c_str()); Serial.print(" size: "); Serial.print(a.size());
#endif
  }

}

/* ****************************************************************************
   lreturn

   convenient function to add a empty line (row) in lcd display
 *****************************************************************************/
void lreturn() {
  std::string str("");
  lpush(str);
}

/* ****************************************************************************
   lclear

   erease lcd screen
 *****************************************************************************/
void lclear() {
  lcd->clear();
}









LiquidCrystal_I2C* getLCD(){
  return lcd;
}



/* ****************************************************************************
   padTo

   add some paddingChar at the end of str to fill caracters to num size.
   if padToFront is enable pad will be at front.
 *****************************************************************************/
void padTo(std::string &str, const size_t num, bool padToFront = false, const char paddingChar = ' ') {
  if (num > str.size()) {
    if (padToFront)
      str.insert(str.begin(), num - str.size(), paddingChar);
    else
      str.insert(str.end(), num - str.size(), paddingChar);
  }
}












  private:
    bool initialized;

    LiquidCrystal_I2C *lcd;
    std::list<std::string> ltext{"", "", "", ""}; // list of display text

};

#endif  //LCD20_4_h
