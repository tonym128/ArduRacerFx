#ifndef _LEVELS_H
#define _LEVELS_H
#include <ArduboyFX.h>                // Required library for accessing the FX flash chip
#include "fxdata/fxdata.h"            // this file contains all the references to FX data
                                      // Check out fxdata.txt to see how this is done.

static __uint24 getLevelMap(uint8_t level) {
  return FX::readIndexedUInt24(FX_DATA_LEVELS,level-1);
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

static __uint24 getTileSet(int pixelSize) {
  if (pixelSize<7) { return FX_DATA_TILES_6; }
  switch ((pixelSize-1)/8) {
    case 0:
      return FX_DATA_TILES_8;
    case 1:
      return FX_DATA_TILES_16;
    case 2:
      return FX_DATA_TILES_24;
    case 3:
      return FX_DATA_TILES_32;
    case 4:
      return FX_DATA_TILES_40;
    case 5:
      return FX_DATA_TILES_48;
    case 6:
      return FX_DATA_TILES_56;
    default:
      return FX_DATA_TILES_64;
  }
}

#endif
