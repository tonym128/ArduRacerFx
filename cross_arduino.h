#ifndef _CROSS_ARDUINO_H_
#define _CROSS_ARDUINO_H_
#include "structs.h"

// #define ARDUBOYG

#ifdef ARDUBOYG
#include "ArduboyG.h"
extern ArduboyGBase_Config<ABG_Mode::L4_Triplane> arduboy;
#else
#include "Arduboy2.h"
#include <ArduboyTones.h>
#endif

#include <ArduboyFX.h>                // Required library for accessing the FX flash chip
#include "fxdata/fxdata.h"            // this file contains all the references to FX data
                                      // Check out fxdata.txt to see how this is done.

#define FXFONT(n) (((int)n)-32)

bool cross_input_up();
bool cross_input_down();
bool cross_input_left();
bool cross_input_right();
bool cross_input_a();
bool cross_input_b();

bool cross_input_up_now();
bool cross_input_down_now();
bool cross_input_left_now();
bool cross_input_right_now();
bool cross_input_a_now();
bool cross_input_b_now();

void cross_print(int x, int y, int size, char *string);
void cross_drawPixel(int x, int y, bool colour);
void cross_drawVLine(int x, int y, int length, bool colour);
void cross_drawHLine(int x, int y, int length, bool colour);
void cross_drawBitmapTile(int x, int y, int width, int height, int colour, int mirror, float zoom, __uint24 fximage);
bool cross_getPixel(int x, int y);
void cross_setup();
bool cross_loop_start();
void cross_loop_end();
unsigned long getFrameMs();
unsigned long getCurrentMs();

void cross_save(SaveData saveData);
SaveData cross_load();

void cross_playSound(bool makeSound, uint16_t hertz, uint8_t duration);

#endif
