#ifndef _CAR_H_
#define _CAR_H_
#include "fixpoint.h"

#ifndef ARDUINO
#define PROGMEM 
#endif

static uint8_t getCarDirection(FIXPOINT radians) {
  if (radians < 5. / 8. * PI)
    return 0;
  else if (radians < 15. / 8. * PI)
    return 1;
  else if (radians < 25. / 8. * PI)
    return 2;
  else if (radians < 35. / 8. * PI)
    return 3;
  else if (radians < 45. / 8. * PI)
    return 4;
  else if (radians < 55. / 8. * PI)
    return 5;
  else if (radians < 65. / 8. * PI)
    return 6;
  else if (radians < 75. / 8. * PI)
    return 7;
  else if (radians < 85. / 8. * PI)
    return 8;
  else if (radians < 95. / 8. * PI)
    return 9;
  else if (radians < 105. / 8. * PI)
    return 10;
  else if (radians < 115. / 8. * PI)
    return 11;
  else if (radians < 125. / 8. * PI)
    return 12;
  else if (radians < 135. / 8. * PI)
    return 13;
  else if (radians < 145. / 8. * PI)
    return 14;
  else if (radians < 155. / 8. * PI)
    return 15;

  return 0;
}

#endif
