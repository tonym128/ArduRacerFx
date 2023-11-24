#pragma once

/**** FX data header generated by fxdata-build.py tool version 1.15 ****/

using uint24_t = __uint24;

// Initialize FX hardware using  FX::begin(FX_DATA_PAGE); in the setup() function.

constexpr uint16_t FX_DATA_PAGE  = 0xff3f;
constexpr uint24_t FX_DATA_BYTES = 45247;

constexpr uint16_t FX_SAVE_PAGE  = 0xfff0;
constexpr uint24_t FX_SAVE_BYTES = 30;

constexpr uint24_t FX_STR_PAUSED = 0x000000;
constexpr uint24_t FX_STR_CONTINUE = 0x000007;
constexpr uint24_t FX_STR_TIMES = 0x000010;
constexpr uint24_t FX_STR_START = 0x000016;
constexpr uint24_t FX_STR_OPTIONS = 0x00001C;
constexpr uint24_t FX_STR_ABOUT = 0x000024;
constexpr uint24_t FX_STR_SOUND = 0x00002A;
constexpr uint24_t FX_STR_MUSIC = 0x00002E;
constexpr uint24_t FX_STR_MAP = 0x000034;
constexpr uint24_t FX_STR_AB_CONT = 0x000038;
constexpr uint24_t FX_STR_LEVEL_SELECT = 0x000049;
constexpr uint24_t FX_STR_MADE_BY = 0x00005D;
constexpr uint24_t FX_STR_GITHUB = 0x000065;
constexpr uint24_t FX_STR_TWITTER = 0x00006C;
constexpr uint24_t FX_STR_TONYM128 = 0x00006E;
constexpr uint24_t FX_STR_PRESSA = 0x000077;
constexpr uint24_t FX_STR_TOSTART = 0x00007F;
constexpr uint24_t FX_STR_NEXTRETRY = 0x000088;
constexpr uint24_t FX_STR_LTIMES = 0x00009B;
constexpr uint24_t FX_STR_RGOAL = 0x0000A5;
constexpr uint24_t FX_STR_LEFT = 0x0000AE;
constexpr uint24_t FX_STR_TOTUNE = 0x0000B6;
constexpr uint24_t FX_DATA_1 = 0x0000BF;
constexpr uint24_t FX_DATA_2 = 0x00011A;
constexpr uint24_t FX_DATA_3 = 0x000197;
constexpr uint24_t FX_LEVEL1 = 0x00021B;
constexpr uint24_t FX_LEVEL2 = 0x00027F;
constexpr uint24_t FX_LEVEL3 = 0x0002E3;
constexpr uint24_t FX_LEVEL4 = 0x000347;
constexpr uint24_t FX_LEVEL5 = 0x0003AB;
constexpr uint24_t FX_LEVEL6 = 0x00040F;
constexpr uint24_t FX_LEVEL7 = 0x000473;
constexpr uint24_t FX_LEVEL8 = 0x0004D7;
constexpr uint24_t FX_LEVEL9 = 0x00053B;
constexpr uint24_t FX_LEVEL10 = 0x00059F;
constexpr uint24_t FX_LEVEL11 = 0x000603;
constexpr uint24_t FX_LEVEL_TIMES = 0x000703;
constexpr uint24_t FX_DATA_FONT46 = 0x000745;
constexpr uint16_t FX_DATA_FONT46_WIDTH  = 4;
constexpr uint16_t FX_DATA_FONT46HEIGHT  = 6;
constexpr uint8_t  FX_DATA_FONT46_FRAMES = 96;

constexpr uint24_t FX_DATA_LOGO = 0x0008C9;
constexpr uint16_t FX_DATA_LOGO_WIDTH  = 128;
constexpr uint16_t FX_DATA_LOGOHEIGHT  = 64;

constexpr uint24_t FX_DATA_LOGO_FX = 0x000CCD;
constexpr uint16_t FX_DATA_LOGO_FX_WIDTH  = 32;
constexpr uint16_t FX_DATA_LOGO_FXHEIGHT  = 20;

constexpr uint24_t FX_DATA_TROPHY = 0x000D91;
constexpr uint16_t FX_DATA_TROPHY_WIDTH  = 64;
constexpr uint16_t FX_DATA_TROPHYHEIGHT  = 64;

constexpr uint24_t FX_DATA_CAR = 0x000F95;
constexpr uint16_t FX_DATA_CAR_WIDTH  = 10;
constexpr uint16_t FX_DATA_CARHEIGHT  = 10;
constexpr uint8_t  FX_DATA_CAR_FRAMES = 8;

constexpr uint24_t FX_DATA_CAR2 = 0x0010D9;
constexpr uint16_t FX_DATA_CAR2_WIDTH  = 16;
constexpr uint16_t FX_DATA_CAR2HEIGHT  = 16;
constexpr uint8_t  FX_DATA_CAR2_FRAMES = 16;

constexpr uint24_t FX_DATA_TILES_64 = 0x0014DD;
constexpr uint16_t FX_DATA_TILES_64_WIDTH  = 64;
constexpr uint16_t FX_DATA_TILES_64HEIGHT  = 64;
constexpr uint8_t  FX_DATA_TILES_64_FRAMES = 24;

constexpr uint24_t FX_DATA_TILES_56 = 0x0044E1;
constexpr uint16_t FX_DATA_TILES_56_WIDTH  = 56;
constexpr uint16_t FX_DATA_TILES_56HEIGHT  = 56;
constexpr uint8_t  FX_DATA_TILES_56_FRAMES = 24;

constexpr uint24_t FX_DATA_TILES_48 = 0x0069A5;
constexpr uint16_t FX_DATA_TILES_48_WIDTH  = 48;
constexpr uint16_t FX_DATA_TILES_48HEIGHT  = 48;
constexpr uint8_t  FX_DATA_TILES_48_FRAMES = 24;

constexpr uint24_t FX_DATA_TILES_40 = 0x0084A9;
constexpr uint16_t FX_DATA_TILES_40_WIDTH  = 40;
constexpr uint16_t FX_DATA_TILES_40HEIGHT  = 40;
constexpr uint8_t  FX_DATA_TILES_40_FRAMES = 24;

constexpr uint24_t FX_DATA_TILES_32 = 0x00976D;
constexpr uint16_t FX_DATA_TILES_32_WIDTH  = 32;
constexpr uint16_t FX_DATA_TILES_32HEIGHT  = 32;
constexpr uint8_t  FX_DATA_TILES_32_FRAMES = 24;

constexpr uint24_t FX_DATA_TILES_24 = 0x00A371;
constexpr uint16_t FX_DATA_TILES_24_WIDTH  = 24;
constexpr uint16_t FX_DATA_TILES_24HEIGHT  = 24;
constexpr uint8_t  FX_DATA_TILES_24_FRAMES = 24;

constexpr uint24_t FX_DATA_TILES_16 = 0x00AA35;
constexpr uint16_t FX_DATA_TILES_16_WIDTH  = 16;
constexpr uint16_t FX_DATA_TILES_16HEIGHT  = 16;
constexpr uint8_t  FX_DATA_TILES_16_FRAMES = 24;

constexpr uint24_t FX_DATA_TILES_8 = 0x00AD39;
constexpr uint16_t FX_DATA_TILES_8_WIDTH  = 8;
constexpr uint16_t FX_DATA_TILES_8HEIGHT  = 8;
constexpr uint8_t  FX_DATA_TILES_8_FRAMES = 24;

constexpr uint24_t FX_DATA_TILES_6 = 0x00ADFD;
constexpr uint16_t FX_DATA_TILES_6_WIDTH  = 6;
constexpr uint16_t FX_DATA_TILES_6HEIGHT  = 6;
constexpr uint8_t  FX_DATA_TILES_6_FRAMES = 24;

constexpr uint24_t FX_DATA_TILES_4 = 0x00AE91;
constexpr uint16_t FX_DATA_TILES_4_WIDTH  = 4;
constexpr uint16_t FX_DATA_TILES_4HEIGHT  = 4;
constexpr uint8_t  FX_DATA_TILES_4_FRAMES = 24;

constexpr uint24_t FX_DATA_ABOUT = 0x00AEF5;
constexpr uint16_t FX_DATA_ABOUT_WIDTH  = 48;
constexpr uint16_t FX_DATA_ABOUTHEIGHT  = 48;

constexpr uint24_t FX_SOUND_INTRO = 0x00B019;
