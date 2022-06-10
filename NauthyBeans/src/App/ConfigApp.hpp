
#ifndef CONFIGAPP_H
#define CONFIGAPP_H

/// MCPDEVICE
//////////////////////////////////

/// BROCHE MCP DEVICE DESCRIPTION
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



/// Afficheur LCD
//////////////////////////////////

// 7SEGMENT ADDR
#define DISP_SYSTEM   1
#define DISP_PLAYER1  0
#define DISP_PLAYER2  2




#endif  // CONFIGAPP_H
