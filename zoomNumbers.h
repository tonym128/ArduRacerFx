#ifndef _ZOOM_NUMBERS_H_
#define _ZOOM_NUMBERS_H_
#include <ArduboyFX.h>                // Required library for accessing the FX flash chip
#include "fxdata/fxdata.h"            // this file contains all the references to FX data
                                      // Check out fxdata.txt to see how this is done.

static __uint24 getNumber(uint8_t number) {
  switch(number) {
    case 1: return FX_DATA_1;
    case 2: return FX_DATA_2;
    case 3: return FX_DATA_3;
  }
  
  return FX_DATA_1;
}

static __uint24 getMapTile(uint8_t number) {
  switch(number) {
    case 1: return FX_DATA_MAP1;
    case 2: return FX_DATA_MAP2;
    case 3: return FX_DATA_MAP3;
    case 4: return FX_DATA_MAP4;
    case 5: return FX_DATA_MAP5;
    case 6: return FX_DATA_MAP6;
    case 7: return FX_DATA_MAP7;
    case 8: return FX_DATA_MAP8;
    case 9: return FX_DATA_MAP9;
    case 10: return FX_DATA_MAP10;
    case 11: return FX_DATA_MAP11;
    case 12: return FX_DATA_MAP12;
    case 13: return FX_DATA_MAP13;
    case 14: return FX_DATA_MAP14;
    case 15: return FX_DATA_MAP15;
    case 16: return FX_DATA_MAP16;
    case 17: return FX_DATA_MAP17;
    case 18: return FX_DATA_MAP18;
    case 19: return FX_DATA_MAP19;
    case 20: return FX_DATA_MAP20;
    case 21: return FX_DATA_MAP21;
    case 22: return FX_DATA_MAP22;
    case 23: return FX_DATA_MAP23;
    case 24: return FX_DATA_MAP24;
  }
  
  return FX_DATA_MAP1;
}
#endif