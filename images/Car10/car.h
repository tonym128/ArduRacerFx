#ifndef _CAR_H_
#define _CAR_H_
#include "fixpoint.h"

const unsigned char PROGMEM car1_00[] ={
1,1,1,1,1,1,1,1,1,1,
1,1,1,0,0,0,0,1,1,1,
1,0,0,0,1,1,0,0,0,1,
1,0,1,0,1,1,0,1,0,1,
1,0,0,0,1,1,0,0,0,1,
1,1,0,0,0,0,0,0,1,1,
1,0,0,0,0,0,0,0,0,1,
1,0,1,0,0,0,0,1,0,1,
1,0,0,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1,1,1,
};

const unsigned char PROGMEM car1_01[] ={
1,1,1,1,1,1,1,1,1,1,
1,1,1,0,0,0,0,0,0,1,
1,1,1,0,1,0,1,1,0,1,
1,0,0,0,0,0,1,1,0,1,
1,0,1,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,1,0,1,
1,1,0,0,0,0,0,0,0,1,
1,1,0,0,0,1,0,1,1,1,
1,1,1,1,0,0,0,1,1,1,
1,1,1,1,1,1,1,1,1,1,
};

const unsigned char PROGMEM car1_02[] ={
1,1,1,1,1,1,1,1,1,1,
1,0,0,0,1,0,0,0,1,1,
1,0,1,0,0,0,1,0,1,1,
1,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,1,1,1,0,1,
1,0,0,0,0,1,1,1,0,1,
1,0,0,0,0,0,0,0,0,1,
1,0,1,0,0,0,1,0,1,1,
1,0,0,0,1,0,0,0,1,1,
1,1,1,1,1,1,1,1,1,1,
};

const unsigned char PROGMEM car1_03[] ={
1,1,1,1,1,1,1,1,1,1,
1,1,1,1,0,0,0,1,1,1,
1,1,0,0,0,1,0,1,1,1,
1,1,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,1,0,1,
1,0,1,0,0,0,0,0,0,1,
1,0,0,0,0,0,1,1,0,1,
1,1,1,0,1,0,1,1,0,1,
1,1,1,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1,1,1,
};

const unsigned char PROGMEM car1_04[] ={
1,1,1,1,1,1,1,1,1,1,
1,0,0,0,0,0,0,0,0,1,
1,0,1,0,0,0,0,1,0,1,
1,0,0,0,0,0,0,0,0,1,
1,1,0,0,0,0,0,0,1,1,
1,0,0,0,1,1,0,0,0,1,
1,0,1,0,1,1,0,1,0,1,
1,0,0,0,1,1,0,0,0,1,
1,1,1,0,0,0,0,1,1,1,
1,1,1,1,1,1,1,1,1,1,
};

const unsigned char PROGMEM car1_05[] ={
1,1,1,1,1,1,1,1,1,1,
1,1,1,0,0,0,1,1,1,1,
1,1,1,0,1,0,0,0,1,1,
1,0,0,0,0,0,0,0,1,1,
1,0,1,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,1,0,1,
1,0,1,1,0,0,0,0,0,1,
1,0,1,1,0,1,0,1,1,1,
1,0,0,0,0,0,0,1,1,1,
1,1,1,1,1,1,1,1,1,1,
};

const unsigned char PROGMEM car1_06[] ={
1,1,1,1,1,1,1,1,1,1,
1,1,0,0,0,1,0,0,0,1,
1,1,0,1,0,0,0,1,0,1,
1,0,0,0,0,0,0,0,0,1,
1,0,1,1,1,0,0,0,0,1,
1,0,1,1,1,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,1,
1,1,0,1,0,0,0,1,0,1,
1,1,0,0,0,1,0,0,0,1,
1,1,1,1,1,1,1,1,1,1,
};

const unsigned char PROGMEM car1_07[] ={
1,1,1,1,1,1,1,1,1,1,
1,0,0,0,0,0,0,1,1,1,
1,0,1,1,0,1,0,1,1,1,
1,0,1,1,0,0,0,0,0,1,
1,0,0,0,0,0,0,1,0,1,
1,0,1,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,1,1,
1,1,1,0,1,0,0,0,1,1,
1,1,1,0,0,0,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,
};

const unsigned char PROGMEM car1_00mask[] ={
1,1,0,0,0,0,0,0,1,1,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,1,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
};

const unsigned char PROGMEM car1_01mask[] ={
1,1,0,0,0,0,0,0,0,0,
1,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,1,1,
1,1,1,0,0,0,0,0,1,1,
};

const unsigned char PROGMEM car1_02mask[] ={
0,0,0,0,1,0,0,0,0,1,
0,0,0,0,0,0,0,0,0,1,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,1,
0,0,0,0,1,0,0,0,0,1,
};

const unsigned char PROGMEM car1_03mask[] ={
1,1,1,0,0,0,0,0,1,1,
1,0,0,0,0,0,0,0,1,1,
1,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
1,1,0,0,0,0,0,0,0,0,
1,1,0,0,0,0,0,0,0,0,
};

const unsigned char PROGMEM car1_04mask[] ={
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,1,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
1,1,0,0,0,0,0,0,1,1,
};

const unsigned char PROGMEM car1_05mask[] ={
1,1,0,0,0,0,0,1,1,1,
1,1,0,0,0,0,0,0,0,1,
0,0,0,0,0,0,0,0,0,1,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,1,1,
0,0,0,0,0,0,0,0,1,1,
};

const unsigned char PROGMEM car1_06mask[] ={
1,0,0,0,0,1,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,1,0,0,0,0,
};

const unsigned char PROGMEM car1_07mask[] ={
0,0,0,0,0,0,0,0,1,1,
0,0,0,0,0,0,0,0,1,1,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,1,
1,1,0,0,0,0,0,0,0,1,
1,1,0,0,0,0,0,1,1,1,
};

unsigned char *getCar(FIXPOINT radians) {
  if (radians < 10. / 8. * PI)
    return car1_00;
  else if (radians < 30. / 8. * PI)
    return car1_01;
  else if (radians < 50. / 8. * PI)
    return car1_02;
  else if (radians < 70. / 8. * PI)
    return car1_03;
  else if (radians < 90. / 8. * PI)
    return car1_04;
  else if (radians < 110. / 8. * PI)
    return car1_05;
  else if (radians < 130. / 8. * PI)
    return car1_06;
  else if (radians < 150. / 8. * PI)
    return car1_07;

  return car1_00;
}

unsigned char *getCarMask(FIXPOINT radians) {
  if (radians < 10. / 8. * PI)
    return car1_00mask;
  else if (radians < 30. / 8. * PI)
    return car1_01mask;
  else if (radians < 50. / 8. * PI)
    return car1_02mask;
  else if (radians < 70. / 8. * PI)
    return car1_03mask;
  else if (radians < 90. / 8. * PI)
    return car1_04mask;
  else if (radians < 110. / 8. * PI)
    return car1_05mask;
  else if (radians < 130. / 8. * PI)
    return car1_06mask;
  else if (radians < 150. / 8. * PI)
    return car1_07mask;

  return car1_00;
}

#endif