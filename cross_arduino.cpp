#include "cross_arduino.h"

// #define DEBUG_MODE

Arduboy2Base arduboy;
uint16_t buffer[8]; 
ArduboyTonesFX sound(arduboy.audio.enabled, buffer);
#define ARDBITMAP
#ifdef ARDBITMAP
#define ARDBITMAP_SBUF arduboy.getBuffer()
#include "ArdBitmap.h"
ArdBitmap<WIDTH, HEIGHT> ardbitmap;
#endif

unsigned long currentTime, frameTime, fps, frameMs = 0;
const unsigned int FRAME_RATE = 60; // Frame rate in frames per second

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

uint8_t checkDesired(uint8_t desired) {
    uint8_t desiredKeyPresses[11] = {1,1,2,2,3,4,3,4,6,5,5};
    if (arduboy.justPressed(UP_BUTTON)) {
        return desiredKeyPresses[desired] == 1 ? 2 : 1;
    } else if (arduboy.justPressed(DOWN_BUTTON)) {
        return desiredKeyPresses[desired] == 2 ? 2 : 1;
    } else if (arduboy.justPressed(LEFT_BUTTON)) {
        return desiredKeyPresses[desired] == 3 ? 2 : 1;
    } else if (arduboy.justPressed(RIGHT_BUTTON)) {
        return desiredKeyPresses[desired] == 4 ? 2 : 1;
    } else if (arduboy.justPressed(A_BUTTON)) {
        return desiredKeyPresses[desired] == 5 ? 2 : 1;
    } else if (arduboy.justPressed(B_BUTTON)) {
        return desiredKeyPresses[desired] == 6 ? 2 : 1;
    };
    
    return 0;
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
        FX::drawBitmap(lineX, lineY, FX_DATA_FONT46, fontId, dbmNormal);
        lineX += (sizeX+spacingX);
   }
}

void cross_print(int x, int y, int size, __uint24 address) {
    lineX = x;
    lineY = y;
    int i = 0;
  for(;;)
  {
    FX::seekData(address++);
    uint8_t c = FX::readEnd();
    if (!c) break;
    if (c =='\n') {
        lineY += sizeY + spacingY;
        lineX = x;
        continue;
    }

    if (i > 0) {
        cross_drawVLine(lineX-spacingX,lineY,sizeY, 0);
    }

    if (c == 95) c=32;
    int fontId = FXFONT(c);
    FX::drawBitmap(lineX, lineY, FX_DATA_FONT46, fontId, dbmNormal);
    lineX += (sizeX+spacingX);
    i++;
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

void cross_set_leds(uint8_t red, uint8_t green, uint8_t blue) 
{
    arduboy.setRGBled(red,green,blue);
}

static void cross_drawBitmapTile(int x, int y, int width, int height, int colour, int mirror, float zoom, __uint24 fximage)
{
    #ifdef ARDBITMAP
    ardbitmap.drawFXCompressedResized(x, y, fximage, colour, 0, mirror, zoom);
    // ardbitmap.drawFXBitmapResized(x, y, fximage, width, height, colour, 0, mirror, zoom);
    #endif
}

bool cross_getPixel(int x, int y) {
    return arduboy.getPixel(x,y);
}

void cross_playSound(bool makeSound, uint16_t hertz, uint8_t duration) {
    if (makeSound) sound.tone(hertz, duration); // play a 1000Hz tone for 500ms
};

void cross_play_audio(bool makeSound, uint24_t fxsound) {
    if (makeSound && !sound.playing()) {
        sound.tonesFromFX(fxsound);
    }
}

void cross_stop_audio(bool makeSound) {
    if (makeSound) {
        sound.noTone();
    }
}

void cross_setup()
{
    arduboy.boot();
    FX::begin(FX_DATA_PAGE, FX_SAVE_PAGE); // // initialise FX chip
    arduboy.initRandomSeed();
    arduboy.setFrameRate(FRAME_RATE);
    arduboy.audio.on();

#ifdef DEBUG_MODE
    Serial.begin(9600);
#endif
}

bool cross_loop_start()
{
    if (!(arduboy.nextFrame()))
        return false;
    sound.fillBufferFromFX();

    frameTime = currentTime;
    currentTime = millis();
    frameMs = currentTime - frameTime;
    if (frameMs == 0)
         frameMs = 1;
    
    arduboy.pollButtons();

    return true;
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

#ifdef PERF_RENDER
unsigned long getMs()
{
    return millis();
}
#endif
