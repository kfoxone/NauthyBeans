#ifndef     LCDSCHAR_H
#define     LCDSCHAR_H




// https://maxpromer.github.io/LCD-Character-Creator/



/// Caractère de retour 
static uint8_t customChar0[8] = {
  0B11011,
  0B10001,
  0B00000,
  0B11011,
  0B11011,
  0B11011,
  0B00011,
  0B11111
};

/// Flèche haute
static uint8_t customChar1[8] = {
  0b00100,
  0b01110,
  0b11111,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100
};

/// Flèche basse
static uint8_t customChar2[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b11111,
  0b01110,
  0b00100
};



#endif      // LCDSCHAR_H