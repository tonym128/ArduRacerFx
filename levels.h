#ifndef _LEVELS_H
#define _LEVELS_H
#include <ArduboyFX.h>                // Required library for accessing the FX flash chip
#include "fxdata/fxdata.h"            // this file contains all the references to FX data
                                      // Check out fxdata.txt to see how this is done.

static __uint24 getLevelMap(uint8_t level) {
  switch(level) {
    case 1: return FX_LEVEL1;
    case 2: return FX_LEVEL2;
    case 3: return FX_LEVEL3;
    case 4: return FX_LEVEL4;
    case 5: return FX_LEVEL5;
    case 6: return FX_LEVEL6;
    case 7: return FX_LEVEL7;
    case 8: return FX_LEVEL8;
    case 9: return FX_LEVEL9;
    case 10: return FX_LEVEL10;
    default:
      return FX_LEVEL10;
  }
}
static uint8_t getLevelMapSize(uint8_t level) {
  switch(level) {
    case 16: return 20;
    default:
      return 10;
  }
}

static uint8_t getLevelTile(__uint24 level, uint8_t x, uint8_t y) {
  int pos = x + y * 10;
  return FX::readIndexedUInt8(level,pos);
}

#endif
