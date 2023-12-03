#ifndef _RACER_H_
#define _RACER_H_

#include "cross_arduino.h"

#include "structs.h"
#include "fixpoint.h"
#include "levels.h"
#include "car.h"

#include <ArduboyFX.h>                // Required library for accessing the FX flash chip
#include "fxdata/fxdata.h"            // this file contains all the references to FX data
                                      // Check out fxdata.txt to see how this is done.
#define ABG_TIMER1
#define ABG_SYNC_PARK_ROW

void update();
void render();

void racerSetup();
void racerLoop();

#endif
