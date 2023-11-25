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
    case 11: return FX_LEVEL11;
    case 12: return FX_LEVEL12;
    case 13: return FX_LEVEL13;
    case 14: return FX_LEVEL14;
    case 15: return FX_LEVEL15;
    case 16: return FX_LEVEL16;
    case 17: return FX_LEVEL17;
    case 18: return FX_LEVEL18;
    case 19: return FX_LEVEL19;
    case 20: return FX_LEVEL20;
  }
}

static uint8_t getLevelMapSize(uint8_t level) {
  if (level <= 10) return 10;
  if (level <= 18) return 16;
  return 30;
}

static uint8_t getLevelTile(__uint24 level, uint8_t x, uint8_t y, uint8_t levelmapsize) {
  uint16_t pos = x + (y * levelmapsize);
  // return FX::readIndexedUInt8(level,pos);
  return FX::readIndexedUInt8(level+pos,0);
}

#endif
