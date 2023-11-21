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

#endif