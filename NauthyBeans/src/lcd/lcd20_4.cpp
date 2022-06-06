#include "lcd20_4.h"
#include <Wire.h>

/**
   Constructor
   @param address: i2c address
*/
LCD20_4::LCD20_4(){
  LiquidCrystal_I2C a(LCD_I2C_ADDR, LCD_COL, LCD_ROW);
  this->lcd = &a;
  lcd->init();

  // lcd->backlight();  //output message
  // lcd->setCursor(0, 0);
  // lcd->print(" joy-IT");
  // lcd->setCursor(0, 1);
  // lcd->print(" ");
  // lcd->setCursor(0, 2);
  // lcd->print(" I2C Serial");
  // lcd->setCursor(0, 3);
  // lcd->print(" LCD");
}


void LCD20_4::backlightEnabled(bool en) {
  if (en)
    lcd->backlight();
  else
    lcd->noBacklight();
}
