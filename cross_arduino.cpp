#include "cross_arduino.h"

// #define DEBUG_MODE

#ifndef ARDUBOYG
Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);
#endif
#define ARDBITMAP_SBUF arduboy.getBuffer()
#include "ArdBitmap.h"
ArdBitmap<WIDTH, HEIGHT> ardbitmap;
unsigned long currentTime, frameTime, fps, frameMs = 0;
const unsigned int FRAME_RATE = 60; // Frame rate in frames per second
const unsigned int FRAME_RATE_MENU = 30; // Frame rate in frames per second

void cross_save(SaveData saveData) {
    FX::saveGameState(saveData);
}

SaveData cross_load() {
    SaveData saveData;
    if (FX::loadGameState(saveData)) {
        return saveData;
    }
    // If no data is found, save blank data
    FX::loadGameState(saveData);
    return saveData;
}

bool cross_input_up()
{
    return arduboy.pressed(UP_BUTTON);
}
bool cross_input_down()
{
    return arduboy.pressed(DOWN_BUTTON);
}
bool cross_input_left()
{
    return arduboy.pressed(LEFT_BUTTON);
}

bool cross_input_right()
{
    return arduboy.pressed(RIGHT_BUTTON);
}
bool cross_input_a()
{
    return arduboy.pressed(A_BUTTON);
}
bool cross_input_b()
{
    return arduboy.pressed(B_BUTTON);
}

bool cross_input_up_now()
{
    return arduboy.justPressed(UP_BUTTON);
}
bool cross_input_down_now()
{
    return arduboy.justPressed(DOWN_BUTTON);
}
bool cross_input_left_now()
{
    return arduboy.justPressed(LEFT_BUTTON);
}

bool cross_input_right_now()
{
    return arduboy.justPressed(RIGHT_BUTTON);
}
bool cross_input_a_now()
{
    return arduboy.justPressed(A_BUTTON);
}
bool cross_input_b_now()
{
    return arduboy.justPressed(B_BUTTON);
}

constexpr int sizeX = 4;
constexpr int sizeY = 6;
constexpr int spacingX = 1;
constexpr int spacingY = 0;
int lineX;
int lineY;

void cross_print(int x, int y, int size, char *string) {
    lineX = x;
    lineY = y;

   for (int i = 0; i < strlen(string); i++) {
        if (string[i]=='\n') {
            lineY += sizeY + spacingY;
            lineX = x;
            continue;
        }
        
        if (i > 0) {
            cross_drawVLine(lineX-spacingX,lineY,sizeY, 0);
        }

        int fontId = FXFONT(string[i]);
        FX::drawBitmap(lineX,                      // although this function is called drawBitmap it can also draw masked sprites
                    lineY, 
                    FX_DATA_FONT46,                  // the ball sprites masked bitmap offset in external flash memory
                    fontId,                              // the fxdata was build using the single ball sprite.png image so there's only frame 0
                    //i % 16,                       // comment above and uncomment this one if the fxdata is rebuild using the ball_16x16.png image
                    dbmNormal /* | dbmReverse */ ); // remove the '/*' and '/*' to reverse the balls into white balls
        lineX += (sizeX+spacingX);
   }
}

void cross_drawPixel(int x, int y, bool colour)
{
    arduboy.drawPixel(x, y, colour);
}

void cross_drawHLine(int x, int y, int length, bool colour)
{
    arduboy.drawFastHLine(x,y,length,colour);
}

void cross_drawVLine(int x, int y, int length, bool colour)
{
    arduboy.drawFastVLine(x,y,length,colour);
}

static void cross_drawBitmapTile(int x, int y, int width, int height, int colour, int mirror, float zoom, __uint24 fximage)
{
    ardbitmap.drawFXCompressedResized(x, y, fximage, colour, 0, mirror, zoom);
    // ardbitmap.drawFXBitmapResized(x, y, fximage, width, height, colour, 0, mirror, zoom);
}

bool cross_getPixel(int x, int y) {
    return arduboy.getPixel(x,y);
}

void cross_playSound(bool makeSound, uint16_t hertz, uint8_t duration) {
#ifndef ARDUBOYG
    if (makeSound) sound.tone(hertz, duration); // play a 1000Hz tone for 500ms
#endif
};

void cross_setup()
{
    arduboy.begin();
    FX::begin(FX_DATA_PAGE, FX_SAVE_PAGE); // // initialise FX chip
    arduboy.initRandomSeed();

#ifdef ARDUBOYG
    arduboy.startGray();
#else
    arduboy.setFrameRate(FRAME_RATE);
    arduboy.audio.on();
#endif

#ifdef DEBUG_MODE
    Serial.begin(9600);
#endif
}

bool cross_loop_start()
{
#ifdef ARDUBOYG
    arduboy.waitForNextPlane();
    if(arduboy.needsUpdate()) {
        frameTime = currentTime;
        currentTime = millis();
        frameMs = currentTime - frameTime;

        if (frameMs == 0)
            frameMs = 1;
        
        arduboy.pollButtons();
        return true;
    }

    return false;
#else
    if (!(arduboy.nextFrame()))
        return false;
    
    frameTime = currentTime;
    currentTime = millis();
    frameMs = currentTime - frameTime;
    if (frameMs == 0)
         frameMs = 1;
    
    arduboy.pollButtons();

    return true;

#endif
}

void cross_loop_end()
{
    FX::display(CLEAR_BUFFER); // Using CLEAR_BUFFER will clear the display buffer after it is displayed
}

unsigned long getFrameMs()
{
    return frameMs;
}

unsigned long getCurrentMs()
{
    return currentTime;
}
