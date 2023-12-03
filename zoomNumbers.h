#ifndef _ZOOM_NUMBERS_H_
#define _ZOOM_NUMBERS_H_
#include <ArduboyFX.h>                // Required library for accessing the FX flash chip
#include "fxdata/fxdata.h"            // this file contains all the references to FX data
                                      // Check out fxdata.txt to see how this is done.

static __uint24 getNumber(uint8_t number) {
  return FX_DATA_NUMBER + (number-1) * sizeof(__uint24);
}

#endif