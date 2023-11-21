#ifndef _RACER_H_
#include "racer.h"
#endif

char string[30];
SaveData saveData;
GameState *gameState;
unsigned long tFrameMs = 0;
constexpr int ZOOM_TIME = 6000;

// #define DEBUG_MODE

void setTimeout(int timeout) {
  gameState->timeout = timeout;
}

void freeUp(CheckPoint *p)
{
  if (p != nullptr && p->nextcheckpoint != nullptr)
    freeUp(p->nextcheckpoint);
  if (p != nullptr)
    free(p);
}

bool doTimeout() {
  if (gameState->timeout > 0) {
    gameState->timeout -= getFrameMs();
    return true;
  }
  return false;
}

void setLevelDetails()
{
  gameState->levelMap = getLevelMap(gameState->level);

  gameState->lasttile = 100;
  gameState->lastx = 100;
  gameState->lasty = 100;
  gameState->paused = 0;
  gameState->player1.acceleration.force = FIXP_TO_INT(0);
  gameState->player1.acceleration.direction = FIXP_TO_INT(0);

  bool done = false;
  gameState->checkpoints = 0;
  gameState->checkpointpassed = 0;

  freeUp(gameState->levelCheckPoints);
  CheckPoint *checkpoint = nullptr;
  CheckPoint *firstcheckpoint = nullptr;

  for (int x = 0; x < 10 && !done; x++)
  {
    for (int y = 0; y < 10 && !done; y++)
    {
      uint8_t levelTile = getLevelTile(gameState->levelMap, x, y);
      if (levelTile == 24)
      {
        gameState->player1.X = FLOAT_TO_FIXP(x * 64.0f);
        gameState->player1.Y = FLOAT_TO_FIXP(y * 64.0f + 32 - 4);
        gameState->player1.rotation = PI / 2;
        //        done = true;
      }
      else if (levelTile == 25)
      {
        gameState->player1.X = FLOAT_TO_FIXP(x * 64.0f + 32 - 4);
        gameState->player1.Y = FLOAT_TO_FIXP(y * 64.0f);
        gameState->player1.rotation = PI;
        //        done = true;
      }
      else if (levelTile == 26 || levelTile == 27)
      {
        if (checkpoint == nullptr)
        {
          checkpoint = new CheckPoint();
          gameState->levelCheckPoints = checkpoint;
        }
        else
        {
          checkpoint->nextcheckpoint = new CheckPoint();
          checkpoint = checkpoint->nextcheckpoint;
        }

        checkpoint->x = x;
        checkpoint->y = y;
        checkpoint->passed = false;
        gameState->checkpoints++;
      }
    }
  }

  gameState->laptimer = false;
  gameState->curlap = 0;
  gameState->newbestLap = false;
  for (int i = 0; i < TIMED_LAPS; i++)
  {
    gameState->laptimes[i] = 0;
  }

  gameState->bestLap = saveData.BestLapTimes[gameState->level - 1];

  gameState->player1.offRoad = false;
}

void processGameMode()
{

  if (gameState->paused)
  {
    if (cross_input_down())
    {
      gameState->mode = 4;
    }
  }

  if (cross_input_a())
  {
    if (gameState->paused)
      gameState->paused = false;
  }

  if (cross_input_up())
  {
    gameState->paused = true;
    return;
  }

  if (cross_input_left())
  {
    gameState->player1.rotation -= gameState->max_turn_speed * tFrameMs;
  }

  if (cross_input_right())
  {
    gameState->player1.rotation += gameState->max_turn_speed * tFrameMs;
  }

  if (gameState->player1.rotation < 0)
    gameState->player1.rotation = PI * 2;
  if (gameState->player1.rotation > PI * 2)
    gameState->player1.rotation = 0;

  if (gameState->laptimer && !gameState->paused)
  {
    gameState->laptimes[(gameState->curlap)] += tFrameMs;
    if (gameState->laptimes[(gameState->curlap)] > 60000)
    {
      gameState->laptimes[(gameState->curlap)] = 60000;
    }
  }

  if (cross_input_a())
  {
    gameState->player1.acceleration.force += FLOAT_TO_FIXP(0.01 * tFrameMs);
  }

  if (cross_input_b())
  {
    gameState->player1.acceleration.force -= FLOAT_TO_FIXP(0.004 * tFrameMs);
  }
}

void updateGameMode()
{
  if (gameState->paused)
    return;

  gameState->player1.XVec = xVec2(gameState->player1.acceleration.force, FLOAT_TO_FIXP(gameState->player1.rotation));
  gameState->player1.YVec = yVec2(gameState->player1.acceleration.force, FLOAT_TO_FIXP(gameState->player1.rotation));
  gameState->player1.X += gameState->player1.XVec;
  gameState->player1.Y += gameState->player1.YVec;

  int x = FIXP_TO_INT(gameState->player1.X);
  int y = FIXP_TO_INT(gameState->player1.Y);

  bool collision = false;
  if (x > 640)
  {
    gameState->player1.X = FLOAT_TO_FIXP(640.0f);
    collision = true;
  }
  else if (x < 0)
  {
    gameState->player1.X = INT_TO_FIXP(0);
    collision = true;
  }

  if (y > 640)
  {
    gameState->player1.Y = FLOAT_TO_FIXP(640.0f);
    collision = true;
  }
  else if (y < 0)
  {
    gameState->player1.Y = INT_TO_FIXP(0);
    collision = true;
  }

  // Side Hard wall collisions
  if (collision)
  {
    if (gameState->player1.acceleration.force > 0)
      gameState->player1.acceleration.force -= gameState->acceleration * tFrameMs;
    if (gameState->player1.acceleration.force < 0)
      gameState->player1.acceleration.force += gameState->acceleration * tFrameMs;
  }
 
  if (gameState->player1.acceleration.force > gameState->max_speed)
    gameState->player1.acceleration.force = gameState->max_speed;
  else if (gameState->player1.acceleration.force < gameState->max_dec)
    gameState->player1.acceleration.force = gameState->max_dec;

  // Drag
  if (gameState->player1.acceleration.force > 0)
  {
    gameState->player1.acceleration.force -= gameState->drag * tFrameMs;
    if (gameState->player1.acceleration.force < 0)
      gameState->player1.acceleration.force = 0;
  }
  else if (gameState->player1.acceleration.force < 0)
  {
    gameState->player1.acceleration.force += gameState->drag * tFrameMs;
    if (gameState->player1.acceleration.force > 0)
      gameState->player1.acceleration.force = 0;
  }

  // Off Road
  if (gameState->player1.offRoad)
  {
    if (gameState->player1.acceleration.force > gameState->offroad)
    {
      gameState->player1.acceleration.force -= gameState->offroad_pen * tFrameMs;
    }
    else if (gameState->player1.acceleration.force < gameState->offroad_neg)
    {
      gameState->player1.acceleration.force += gameState->offroad_pen * tFrameMs;
    }
  }

  // Tile Logic
  int tilex = (x + 5) / 64;
  int tiley = (y + 5) / 64;
  uint8_t tile = getLevelTile(gameState->levelMap, tilex, tiley);

  if (gameState->lastx != tilex || gameState->lasty != tiley)
  {
    // Went through a check point
    if (gameState->lasttile == 26 || gameState->lasttile == 27)
    {
      bool done = false;
      CheckPoint *a = gameState->levelCheckPoints;
      while (a != nullptr && !done)
      {
        if (a->x == gameState->lastx && a->y == gameState->lasty)
        {
          if (!a->passed)
          {
            a->passed = true;
            gameState->checkpointpassed++;
          }
          done = true;
        }
        a = a->nextcheckpoint;
      }
    }

    // Was in start block and no longer in start block
    if (gameState->lasttile == 24 || gameState->lasttile == 25)
    {
      if (gameState->laptimer)
      {
        if (gameState->checkpoints == gameState->checkpointpassed)
        {
          // Finished a lap
          if (gameState->laptimes[(gameState->curlap)] < gameState->bestLap || gameState->bestLap == 0)
          {
            gameState->bestLap = gameState->laptimes[(gameState->curlap)];
            gameState->newbestLap = true;

            // Update Best Lap Time to Rom
            if (saveData.BestLapTimes[gameState->level - 1] == 0 || saveData.BestLapTimes[gameState->level - 1] > gameState->laptimes[(gameState->curlap)])
            {
              saveData.BestLapTimes[gameState->level - 1] = gameState->laptimes[(gameState->curlap)];
              unsigned int levelTime = FX::readIndexedUInt16(FX_LEVEL_TIMES, (gameState->level - 1) * 3 + 2);
              if (saveData.BestLapTimes[gameState->level - 1] < levelTime)
              { // If the time is better than 3rd place, we can go forwards
                if (saveData.maxLevel < gameState->level + 1 && gameState->level < 10)
                  saveData.maxLevel = gameState->level + 1;
              }

              if (gameState->level > saveData.maxLevel)
                saveData.maxLevel = gameState->level;

              cross_save(saveData);
            }
          }

          gameState->curlap++;
          gameState->checkpointpassed = 0;

          CheckPoint *point = gameState->levelCheckPoints;
          while (point != nullptr)
          {
            point->passed = false;
            point = point->nextcheckpoint;
          }
        }
      }
      else
      {
        gameState->laptimer = true;
      }
    }
  }

  gameState->lasttile = tile;
  gameState->lastx = tilex;
  gameState->lasty = tiley;
}

inline void setLevelTimeString(char number, unsigned int time)
{
  sprintf(string, ("%c-%2u.%02u"), number, time / 1000, time / 10 % 100);
}

inline void setLevelString()
{
  sprintf(string, ("Level %d"), gameState->level);
}

void displayGameMode()
{
  int x = FIXP_TO_INT(gameState->player1.X) - 60;
  int y = FIXP_TO_INT(gameState->player1.Y) - 30;

  if (x < 0)
    x = 0;
  else if (x > 640 - 128)
    x = 640 - 128;

  if (y < 0)
    y = 0;
  else if (y > 640 - 64)
    y = 640 - 64;

  int inlinex = x % 64 * -1;
  int inliney = y % 64 * -1;
  // Draw Road
  for (uint8_t j = y / 64; j < (y / 64) + 2 && j < 10; j++)
  {
    for (uint8_t i = x / 64; i < (x / 64) + 3 && i < 10; i++)
    {
      if (inlinex < -64 | inlinex > 640 | inliney < -64 | inlinex > 640){
        inlinex += 64;
        continue;
      }
      uint8_t levelTile = getLevelTile(gameState->levelMap, i, j);
      int tilemap;
      switch (levelTile) {
        case 24:
          tilemap = 0;
          break;
        case 25:
          tilemap = 1;
          break;
        case 26:
          tilemap = 0;
          break;
        case 27:
          tilemap = 1;
          break;
        default:
          tilemap = levelTile;
      }

      FX::drawBitmap(inlinex, inliney, FX_DATA_TILES_64, tilemap, dbmNormal);

      if (levelTile > 23)
      {
        switch (levelTile)
        {
        case 24:
          cross_drawVLine(inlinex + 63 - 8, inliney, 64, true);
          break;
        case 25:
          cross_drawHLine(inlinex, inliney + 63 - 8, 64, true);
          break;
        case 26:
          cross_drawHLine(inlinex, inliney + 12, 64, true);
          cross_drawHLine(inlinex, inliney + 13, 64, true);
          cross_drawHLine(inlinex, inliney + 64 - 13, 64, true);
          cross_drawHLine(inlinex, inliney + 64 - 14, 64, true);
          break;
        case 27:
          cross_drawVLine(inlinex + 12, inliney, 64, true);
          cross_drawVLine(inlinex + 13, inliney, 64, true);
          cross_drawVLine(inlinex + 64 - 13, inliney, 64, true);
          cross_drawVLine(inlinex + 64 - 14, inliney, 64, true);
          break;
        }
      }
      inlinex += 64;
    }

    inlinex = x % 64 * -1;
    inliney += 64;
  }

  // Off Road Check (Do it here because we rely on the populated screen
  int carx = FIXP_TO_INT(gameState->player1.X);
  int cary = FIXP_TO_INT(gameState->player1.Y);

  if (carx < 60)
  {
    // use x as is in direct to screen;
  }
  else if (carx > 650 - 68)
  {
    carx = 128 - (650 - carx);
  }
  else
  {
    carx = 60;
  }

  if (cary < 30)
  {
    // use y as is in direct to screen;
  }
  else if (cary > 650 - 36)
  {
    cary = 64 - (650 - cary);
  }
  else
  {
    cary = 30;
  }

  gameState->player1.offRoad = false;
  if (cross_getPixel(carx + 8, cary + 8) == 1)
  {
    gameState->player1.offRoad = true;
  }

  // Draw Car
  uint8_t carDirection = getCarDirection(((FIXPOINT)(gameState->player1.rotation * 10)));

  FX::drawBitmap(carx, cary, FX_DATA_CAR2, carDirection, dbmMasked);

  sprintf(string, ("%c-%2u.%02u %d/%d\n%c-%2u.%02u"), gameState->curlap + 1 + 48, gameState->laptimes[(gameState->curlap)] / 1000, gameState->laptimes[(gameState->curlap)] / 10 % 100, gameState->checkpointpassed, gameState->checkpoints, gameState->newbestLap ? '*' : 'B', saveData.BestLapTimes[gameState->level - 1]  / 1000, saveData.BestLapTimes[gameState->level - 1]  / 10 % 100);
  cross_print(0, 0, 1, string);

  for (int i = 0; i < 16; i++)
  {
    cross_drawVLine(128 - 10 - 6 + i, 0, 6, 0);
  }
  if (gameState->player1.offRoad && gameState->player1.acceleration.force != 0)
  {
    if (((gameState->laptimes[(gameState->curlap)] / 100) % 3) == 0)
      cross_playSound(saveData.sound > 0, 100, 30);
  }
 
  // Draw Map
  if (saveData.map) {
    uint8_t levelTile = 0;
    uint8_t mapX = 118;
    uint8_t mapY = 7;
    inlinex = FIXP_TO_INT(gameState->player1.X) / 64;
    inliney = FIXP_TO_INT(gameState->player1.Y) / 64;

    for (uint8_t j = 0; j < 10; j++) {
      for (uint8_t i = 0; i < 10; i++) {
        if (inlinex == i && inliney == j) {
            cross_drawPixel(mapX+i,mapY+j,(gameState->laptimes[(gameState->curlap)]/200)%2==0);
            continue;
        }

        levelTile = getLevelTile(gameState->levelMap, i, j);
        if ((levelTile <= 11) | (levelTile >= 16 && levelTile <= 19) || (levelTile >= 24)) {
            cross_drawPixel(mapX+i,mapY+j,1);
        } else {
            cross_drawPixel(mapX+i,mapY+j,0);
        }
      }
    }
  }
  // Display Speed
  for (int i = 0; i <= FIXP_TO_FLOAT(gameState->player1.acceleration.force * 8); i++)
  {
    cross_drawVLine(113 + i, 0, 7, 1);
  }
  cross_drawHLine(113, 6, 15, 0);

  if (gameState->paused)
  {
    cross_print(0, 20, 3, (" Paused "));
    for (int i = 0; i < 16; i++)
      cross_drawHLine(0, i + 48, 64, 0);

    cross_print(0, 48, 1, ("A / B - Continue"));
    cross_print(0, 56, 1, ("Down - Level Select"));
  }
}

void racerSetup()
{
  cross_setup();

  saveData = cross_load();
  if (saveData.maxLevel==0) {
    saveData.maxLevel = 1;
    cross_save(saveData);
  }
  gameState = new GameState();
  gameState->level = 1;
  gameState->levelMap = getLevelMap(gameState->level);
  gameState->laptimer = false;
  setTimeout(100);
  gameState->lastmode = -1;
  gameState->mode = 0;

  cross_loop_end();
}

void processMenu()
{
  // Debounce input
  if (doTimeout()) return;

  // Check for up button
  if (cross_input_up())
  {
    if (gameState->menuItem == 0)
      gameState->menuItem = 3;
    else
      gameState->menuItem -= 1;
    setTimeout(100);
  }
  // Check for down button
  else if (cross_input_down())
  {
    if (gameState->menuItem == 3)
      gameState->menuItem = 0;
    else
      gameState->menuItem += 1;
    setTimeout(100);
  }
  // Check for button press
  else if (cross_input_a())
  {
    gameState->enter = true;
    setTimeout(100);
  }
  else if (cross_input_b()) {
    gameState->mode = 0;
    gameState->menuItem = 0;
    gameState->enter = false;
  }
}

void displayOptionsMenu(int menuItem)
{
  FX::drawBitmap(0 ,0 , FX_DATA_LOGO   , 0, dbmNormal);
  FX::drawBitmap(50,15, FX_DATA_LOGO_FX, 0, dbmMasked);

  cross_print(90, 30 + 0, 1, ("About"));
  cross_print(90, 30 + 8, 1, ("Sound"));
  cross_print(90, 30 + 16, 1, ("Map"));
  cross_print(90, 30 + 24, 1, ("Delete"));
  if (saveData.sound > 0)
    cross_print(90 + 5 * 6, 30 + 8, 1, ("*"));
  if (saveData.map > 0)
    cross_print(90 + 5 * 6, 30 + 16, 1, ("*"));

  cross_print(84, 30 + menuItem * 8, 1, ("*"));
}

void displayAbout() {
  FX::drawBitmap(0, 0, FX_DATA_ABOUT, 0, dbmNormal);

  cross_print(60, 0, 1, ("Made By"));
  cross_print(60, 20, 1, ("github"));
  cross_print(60, 40, 1, ("twitter"));
  for (int i = 0; i < 3; i++) {
    cross_print(70, 10 + i * 20, 1, ("tonym128"));
  }
}

void updateOptionsMenu() {
  if (gameState->enter)
    switch (gameState->menuItem) {
      case 0:
        if (doTimeout()) return;
        gameState->mode = 3;
        gameState->enter = false;
        setTimeout(100);
      case 1:
        // Toggle Sound
        if (doTimeout()) return;
        if (saveData.sound == 0) { saveData.sound = 1;}
        else saveData.sound = 0;

        gameState->enter = false;
        setTimeout(100);
        cross_save(saveData);
        break;
      case 2:
        // Toggle Map
        if (doTimeout()) return;
        if (saveData.map == 0) { saveData.map = 1;}
        else saveData.map = 0;

        gameState->enter = false;
        setTimeout(100);
        cross_save(saveData);
        break;
      case 3:
        // Clear Data
        if (doTimeout()) return;
        gameState->mode = 99;
        break;
      case 99:
        //Back
        if (doTimeout()) return;
        gameState->mode = 1;
        gameState->enter = false;
        gameState->menuItem = 3;
        setTimeout(100);
        break;
    }
}

void updateMenu()
{
  if (gameState->enter)
  {
    switch (gameState->menuItem)
    {
    case 0:
      // Go To Last Level Finished
      gameState->level = saveData.maxLevel;
      gameState->mode = 4; // Level Start
      break;
    case 1:
      // Go To First Level
      gameState->level = 1;
      gameState->levelMap = getLevelMap(gameState->level);
      gameState->mode = 4; // Level Start
      break;
    case 2:
      // Times
      gameState->mode = 98;
      setTimeout(1000);
      break;
    case 3:
      // Options Menu
      gameState->mode = 2;
      setTimeout(100);
      gameState->menuItem = 0;
      gameState->enter = false;
      break;
    }
  }
}

void displayMenu(int menuItem)
{
  FX::drawBitmap(0 ,0 , FX_DATA_LOGO   , 0, dbmNormal);
  FX::drawBitmap(50,15, FX_DATA_LOGO_FX, 0, dbmMasked);

  cross_print(90, 30 + 0, 1, ("Continue"));
  cross_print(90, 30 + 8, 1, ("Start"));
  cross_print(90, 30 + 16, 1, ("Times"));
  cross_print(90, 30 + 24, 1, ("Options"));

  cross_print(84, 30 + menuItem * 8, 1, ("*"));
}

void displayMap()
{
  for (int y = 0; y < 10; y++)
  {
    for (int x = 0; x < 10; x++)
    {
      uint8_t levelTile = getLevelTile(gameState->levelMap, x, y);
      int tilemap;
      switch (levelTile) {
        case 24:
          tilemap = 0;
          break;
        case 25:
          tilemap = 1;
          break;
        case 26:
          tilemap = 0;
          break;
        case 27:
          tilemap = 1;
          break;
        default:
          tilemap = levelTile;
      }

      FX::drawBitmap(x * 6, y * 6, FX_DATA_TILES_6, tilemap, dbmNormal);
    }
  }
}

void drawBestTimes()
{
  for (int i = 0; i < 3; i++)
  {
    setLevelTimeString(i + 1 + 48, (unsigned int)FX::readIndexedUInt16(FX_LEVEL_TIMES,(gameState->level - 1)*3 + i));

    cross_print(74, 21 + 8 * i, 1, string);
  }
}

void displayLevelInfo()
{
  for (int x = 64; x < 128; x++)
  {
    cross_drawVLine(x, 0, 64, 0);
  }

  setLevelString();
  cross_print(74, 0, 1, string);
  cross_drawHLine(64, 8, 64, 1);
  setLevelTimeString('B', (unsigned int)saveData.BestLapTimes[gameState->level - 1]);

  cross_print(74, 10, 1, string);
  cross_drawHLine(64, 19, 64, 1);
  drawBestTimes();
  cross_drawHLine(64, 46, 64, 1);
  
  if (doTimeout()) {}
  else if ((getCurrentMs() / 1000) % 2 == 0)
    {
      cross_print(64, 64 - 16, 1, ("Press A"));
      cross_print(64, 64 - 8, 1, ("To Start"));
    }
};

void inputLevelInfo()
{
  if (cross_input_a())
  {
    gameState->mode++;
    setTimeout(100);
  }

  if (cross_input_b())
  {
    gameState->mode = 1;
    gameState->enter = false;
    setTimeout(100);
  }

  if (cross_input_up())
  {
    setTimeout(100);
    if (gameState->level < saveData.maxLevel)
       gameState->level++;
    gameState->lastmode = -1;
    if (gameState->level > 10)
      gameState->level = 10;
    gameState->levelMap = getLevelMap(gameState->level);
  }

  if (cross_input_down())
  {
      setTimeout(100);
    gameState->level--;
    gameState->lastmode = -1;
    if (gameState->level < 1)
      gameState->level = 1;
    gameState->levelMap = getLevelMap(gameState->level);
  }
}

static inline __uint24 getTileSet(int pixelSize) {
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

bool displayLevelZoom()
{
  float time = ((ZOOM_TIME - gameState->timeout) / (float)ZOOM_TIME);
  float zoom = 0.1f + 0.9f * time;
  int pixelSize = (int)floor(6.0f + 58.0f * time);

  float headtox = ((FIXP_TO_FLOAT(gameState->player1.X) - 64) * time * 2);
  float headtoy = ((FIXP_TO_FLOAT(gameState->player1.Y) - 32) * time * 2);

  if (headtox < 0)
    headtox = 0;
  if (headtoy < 0)
    headtoy = 0;

  if (headtox > FIXP_TO_FLOAT(gameState->player1.X) - 64)
    headtox = FIXP_TO_FLOAT(gameState->player1.X) - 64;

  if (headtoy > FIXP_TO_FLOAT(gameState->player1.Y) - 32)
    headtoy = FIXP_TO_FLOAT(gameState->player1.Y) - 32;

  headtox *= (pixelSize / 64.0f);
  headtoy *= (pixelSize / 64.0f);

  int endx = (int)(64 + 64 * (time * 7));
  if (endx > 128)
    endx = 128;

  // Choose map
  __uint24 tiles = getTileSet(pixelSize);

  for (int y = 0; y < 10; y++)
  {
    for (int x = 0; x < 10; x++)
    {
      // Basic Screen Check
      if ((x * pixelSize + pixelSize - headtox) > 0 && (y * pixelSize + pixelSize - headtoy) > 0 && (x * pixelSize - headtox) < endx && (y * pixelSize - headtoy < 64))
      {
        uint8_t levelTile = getLevelTile(gameState->levelMap, x, y);
        int tilemap;
        switch (levelTile) {
          case 24:
            tilemap = 0;
            break;
          case 25:
            tilemap = 1;
            break;
          case 26:
            tilemap = 0;
            break;
          case 27:
            tilemap = 1;
            break;
          default:
            tilemap = levelTile;
        }

        FX::drawBitmap((int)(x * pixelSize - headtox),  (int)(y * pixelSize - headtoy), tiles, tilemap, dbmNormal);
      }
    }
  }

  return false;
}

void drawContinueMenu()
{
  if ((getCurrentMs() / 1000) % 2 == 0)
  {
    if (saveData.maxLevel > gameState->level)
      cross_print(64, 64 - 16, 1, "A - Next\nB - Retry");
  }
}

void drawTrophySheet()
{
  for (int i = 13; i < 53; i += 13) 
    cross_drawHLine(0, i, 128, 1);

  cross_print(3, 17, 1, ("G"));
  cross_print(3, 31, 1, ("S"));
  cross_print(3, 43, 1, ("B"));

  for (int level = 0; level < 10; level++)
  {
    sprintf(string, ("%d"), level + 1);
    cross_print(16 + level * 11, 3, 1, string);
    cross_drawVLine(15 + level * 11, 0, 53, 1);
    if (saveData.BestLapTimes[level] > 0)
      for (int i = 0; i < 3; i++)
      {
        if (saveData.BestLapTimes[level] < (unsigned int)FX::readIndexedUInt16(FX_LEVEL_TIMES,level*3 + i))
        {
          cross_print(17 + level * 11, 4 + (1 + i) * 13, 1, ("X"));
          break;
        }
      }
  }
}

void drawGoalTimes()
{
  setLevelString();
  cross_print(74, 0, 1, string);
  cross_drawHLine(64, 8, 64, 1);

  setLevelTimeString('B', (unsigned int)saveData.BestLapTimes[gameState->level - 1]);
  cross_print(74, 10, 1, string);
  cross_drawHLine(64, 19, 64, 1);

  drawBestTimes();

  cross_drawHLine(64, 46, 64, 1);

  drawContinueMenu();
  cross_print(0, 64 - 7, 1, ("L - Times"));
}

void drawAllTimes()
{
  setLevelString();
  cross_print(74, 0, 1, string);
  cross_drawHLine(64, 8, 64, 1);

  for (int i = 0; i < TIMED_LAPS; i++)
  {
    setLevelTimeString(i + 1 + 48, gameState->laptimes[i]);
    cross_print(74, 9 + 8 * i, 1, string);
  }

  drawContinueMenu();
  cross_print(0, 64 - 7, 1, ("R - Goal"));
  cross_drawHLine(64, 64 - 17, 128 - 64, 1);
}

void drawTrophy()
{
  FX::drawBitmap(0, 0, FX_DATA_TROPHY, 0, dbmNormal);

  if (gameState->curlap == 0)
  {
    drawGoalTimes();
  }
  else if (gameState->curlap == 1)
  {
    drawAllTimes();
  };
};

void inputTrophy()
{
  if (cross_input_left())
  {
    gameState->curlap = 1;
  }

  if (cross_input_right())
  {
    gameState->curlap = 0;
  }

  if (cross_input_a())
  {
    if (gameState->level < saveData.maxLevel)
    {
      gameState->level += 1;
      if (gameState->level > 10)
        gameState->level = 10;
      gameState->mode = 4;
      gameState->levelMap = getLevelMap(gameState->level);
    }
  }

  if (cross_input_b())
  {
    gameState->mode = 4;
  }
};

void buildDeleteString(char button) {
  sprintf(string,("%c - Delete"),button);
}

void buildAbortString(char button) {
  sprintf(string,("%c - Abort"),button);
}

void displayIntro() {
  int x = 50-gameState->timeout/10;
  FX::drawBitmap(0 ,0 , FX_DATA_LOGO   , 0, dbmNormal);
  FX::drawBitmap(x, 15, FX_DATA_LOGO_FX, 0, dbmMasked);
}

void displayZoomer() {
      displayGameMode();
      int lastnumber = gameState->timeout == 3000 ? 4 : gameState->timeout / 1000 + 1;

      int distance = gameState->timeout % 1000;
      int number = gameState->timeout / 1000 + 1;
      float mod = (1000.0f - distance) / 1000.0f;
      int numMod = (int)(32 * mod);
      cross_drawBitmapTile(32 + numMod, 0 + numMod, 64, 64, 1, 0, 1.0f - mod, getNumber(number));
      if (number >= 0 && lastnumber != number)
        cross_playSound(saveData.sound > 0, 440, 100);
      if (!doTimeout())
      {
        gameState->mode = 10;
        cross_playSound(saveData.sound > 0, 440, 255);
      }
}

void update() {
#ifdef DEBUG_MODE
  if (cross_input_a() && cross_input_b()) {
    return;
  }
#endif

  tFrameMs = getFrameMs();

  switch (gameState->mode)
  {
  case 0: // Intro screen
    if (gameState->lastmode != gameState->mode)
    {
      gameState->lastmode = gameState->mode;
      setTimeout(1000);
    }
    else
    {
      if (!doTimeout())
      {
        gameState->mode += 1;
      }
    }
    break;
  case 1: // Menu
    if (gameState->lastmode != gameState->mode)
    {
      gameState->lastmode = gameState->mode;
    }
    else
    {
      if (!doTimeout()) {
        processMenu();
        updateMenu();
      }
    }
    break;
  case 2: // Options Menu
    if (gameState->lastmode != gameState->mode)
    {
      gameState->lastmode = gameState->mode;
    }
    else
    {
      if (!doTimeout()) {
        processMenu();
        updateOptionsMenu();
      }
    }
    break;
  case 3: // About
    if (gameState->lastmode != gameState->mode)
    {
      gameState->lastmode = gameState->mode;
      setTimeout(100);
    }
    if (!doTimeout()) {
      if (cross_input_a() || cross_input_b()) {
        setTimeout(100);
        gameState->mode = 2;
      }
    }
    break;
  case 4: // Level Start
    if (gameState->lastmode != gameState->mode)
    {
      gameState->lastmode = gameState->mode;
      setTimeout(1000);
    }
    else
    {
      if (!doTimeout())
        inputLevelInfo();
    }
    break;
  case 5: // Zoom in - Performance is bad - skipping!
    if (gameState->lastmode != gameState->mode)
    {
      gameState->lastmode = gameState->mode;
      setTimeout((int)ZOOM_TIME);
      setLevelDetails();
    }
    else
    {
      if (!doTimeout())
      {
        gameState->mode = 6;
      }
    }
    break;
  case 6: // 3,2,1,GO!
    if (gameState->lastmode != gameState->mode)
    {
      setLevelDetails();
      gameState->lastmode = gameState->mode;
      setTimeout(3000);
    }
   break;
  case 8: // Win Screen / Next Level Select
    if (gameState->lastmode != gameState->mode)
    {
      gameState->lastmode = gameState->mode;
      setTimeout(1000);
      gameState->curlap = 0;
      gameState->newbestLap = false;
    }
    else
    {
      if (!doTimeout())
      {
        inputTrophy();
      }
    }

    break;
  case 10: // Play Game
    if (gameState->lastmode != gameState->mode)
    {
      setLevelDetails();
      gameState->laptimer = true;
      gameState->lastmode = gameState->mode;
    }

    processGameMode();
    updateGameMode();
    if (gameState->curlap == TIMED_LAPS)
    {
      gameState->mode = 8;
    }
    break;
  case 98: // Trophy Screen
    if (gameState->lastmode != gameState->mode)
    {
      gameState->lastmode = gameState->mode;
    }
    if (!doTimeout())
      if (cross_input_b() || cross_input_a()) {
        gameState->mode = 1;
        gameState->enter = false;
        setTimeout(1000);
      }
    break;
  case 99: // Clear Data
    if (gameState->lastmode != gameState->mode)
    {
      gameState->curlap = 0;
      gameState->lastmode = gameState->mode;
      setTimeout(1000);
    }
    else
    {
      switch (gameState->curlap)
      {
      case 0: // Yes A
        if (!doTimeout())
        {
          if (cross_input_a())
          {
            setTimeout(1000);
            gameState->curlap++;
          }
          else if (cross_input_b())
          {
            gameState->enter = false;
            gameState->mode = 0;
            gameState->curlap = 0;
            setTimeout(1000);
          }
        }
        break;
      case 1: // Yes B
        if (!doTimeout())
        {
          if (cross_input_b())
          {
            gameState->curlap++;
            setTimeout(1000);
          }
          else if (cross_input_a())
          {
            gameState->enter = false;
            gameState->mode = 0;
            gameState->curlap = 0;
            setTimeout(1000);
          }
        }
        break;
      case 2: // Pull the trigger
        gameState->enter = false;
        gameState->mode = 0;
        gameState->curlap = 0;
        SaveData data;

        for (int i = 0; i < LEVELS; i++)
        {
          data.BestLapTimes[i] = 0;
        }
        data.maxLevel = 0;

        cross_save(data);
        saveData = data;
        break;
      }
    }
    break;
  }
}

void render() {

  switch (gameState->mode)
  {
  case 0: // Intro screen
    if (gameState->lastmode == gameState->mode) {
      displayIntro();
    }
    break;
  case 1: // Menu
    if (gameState->lastmode == gameState->mode) {
      displayMenu(gameState->menuItem);
    }
    break;
  case 2: // Options Menu
    if (gameState->lastmode == gameState->mode) {
      displayOptionsMenu(gameState->menuItem);
    }
    break;
  case 3: // About
    if (gameState->lastmode == gameState->mode) {
      displayAbout();
    }
    break;
  case 4: // Level Start
    if (gameState->lastmode == gameState->mode) {
      displayMap();
      displayLevelInfo();
    }
    break;
  case 5: // Zoom in - Performance is bad - skipping!
    if (gameState->lastmode == gameState->mode) {
      displayLevelZoom();
    }
    break;
  case 6: // 3,2,1,GO!
    if (gameState->lastmode == gameState->mode) {
      displayZoomer();
    }

   break;
  case 8: // Win Screen / Next Level Select
    if (gameState->lastmode == gameState->mode) {
      drawTrophy();
    }

    break;
  case 10: // Play Game
    if (gameState->lastmode == gameState->mode) {
      displayGameMode();
    }
    break;
  case 98: // Trophy Screen
    if (gameState->lastmode == gameState->mode) {
      drawTrophySheet();
    }
    break;
  case 99: // Clear Data
    if (gameState->lastmode == gameState->mode) {
      switch (gameState->curlap)
      {
      case 0: // Yes A
        buildDeleteString('A');
        cross_print(0, 0, 2, string);
        buildAbortString('B');
        cross_print(0, 30, 2, string);
        break;
      case 1: // Yes B
        buildDeleteString('B');
        cross_print(0, 0, 2, string);
        buildAbortString('A');
        cross_print(0, 30, 2, string);
      break;
      }
    }
    break;
  }

  #ifdef DEBUG_MODE
  sprintf(string,"%0d %3d %3d %3d - %2u", gameState->mode, FIXP_TO_INT(gameState->player1.acceleration.force), FIXP_TO_INT(gameState->player1.X), FIXP_TO_INT(gameState->player1.Y), getFrameMs());
  cross_print(0,58,6,string);
  #endif
}

void racerLoop()
{
#ifdef ARDUBOYG
  if (!cross_loop_start())
    update();
  render();
#else
  if (!cross_loop_start())
    return;
  update();
  render();

#endif
  cross_loop_end();
}
