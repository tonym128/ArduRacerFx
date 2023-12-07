#ifndef _RACER_H_
#include "racer.h"
#endif

char string[30];
SaveData saveData;
GameState gameState;
unsigned long tFrameMs = 0;
constexpr int ZOOM_TIME = 6000;
uint24_t pointer = FX_STR_STAR;

// #define DEBUG_MODE
// #define PERF_RENDER

void setTimeout(int timeout) {
  gameState.timeout = timeout;
}

void freeUp(CheckPoint *p)
{
  if (p != nullptr && p->nextcheckpoint != nullptr)
    freeUp(p->nextcheckpoint);
  if (p != nullptr)
    free(p);
}

bool doTimeout() {
  if (gameState.timeout > 0) {
    gameState.timeout -= getFrameMs();
    return true;
  }
  return false;
}

void buildMapCache() {
    // Map XY logic 
    uint8_t levelTile;
    int playerMapTileX = FIXP_TO_INT(gameState.player1.X)/64;
    int playerMapTileY = FIXP_TO_INT(gameState.player1.Y)/64;

    if (gameState.levelSize > LEVEL_CACHE) {
      //Fog of war type calc
      playerMapTileX -= LEVEL_CACHE/2;
      playerMapTileY -= LEVEL_CACHE/2;
      if (playerMapTileX<0) playerMapTileX=0;
      if (playerMapTileX>gameState.levelSize-LEVEL_CACHE) playerMapTileX=gameState.levelSize-LEVEL_CACHE;
      if (playerMapTileY<0) playerMapTileY=0;
      if (playerMapTileY>gameState.levelSize-LEVEL_CACHE) playerMapTileY=gameState.levelSize-LEVEL_CACHE;
    } else {
      playerMapTileX = 0;
      playerMapTileY = 0;
    }

    if (gameState.levelMapXMod != playerMapTileX || gameState.levelMapYMod != playerMapTileY) {
      // Map cache invalidated, repopulate
      gameState.levelMapXMod = playerMapTileX;
      gameState.levelMapYMod = playerMapTileY;
      for (uint8_t j = 0; j < LEVEL_CACHE && j < gameState.levelSize; j++) {
        for (uint8_t i = 0; i < LEVEL_CACHE && i < gameState.levelSize; i++) {
            levelTile = getLevelTile(gameState.levelMap, i + playerMapTileX, j + playerMapTileY, gameState.levelSize);
            gameState.mapDisplay[i + j*LEVEL_CACHE] = ((levelTile <= 11) | (levelTile >= 16 && levelTile <= 19) || (levelTile >= 24));
        }
      }
    }
}

void setLevelDetails()
{
  gameState.levelMap = getLevelMap(gameState.level);
  gameState.levelSize = getLevelMapSize(gameState.level);

  // Invalidate cached map
  gameState.levelMapXMod = -1;
  gameState.levelMapYMod = -1;

  gameState.lasttile = 100;
  gameState.lastx = 100;
  gameState.lasty = 100;
  gameState.paused = 0;
  gameState.player1.acceleration.force = FIXP_TO_INT(0);
  gameState.player1.acceleration.direction = FIXP_TO_INT(0);

  gameState.checkpoints = 0;
  gameState.checkpointpassed = 0;

  freeUp(gameState.levelCheckPoints);
  CheckPoint *checkpoint = nullptr;
  CheckPoint *firstcheckpoint = nullptr;

  for (int x = 0; x < gameState.levelSize; x++)
  {
    for (int y = 0; y < gameState.levelSize; y++)
    {
      uint8_t levelTile = getLevelTile(gameState.levelMap, x, y, gameState.levelSize);
      if (levelTile == 24)
      {
        gameState.player1.X = FLOAT_TO_FIXP(x * 64.0f);
        gameState.player1.Y = FLOAT_TO_FIXP(y * 64.0f + 32 - 4);
        gameState.player1.rotation = PI / 2;
      }
      else if (levelTile == 25)
      {
        gameState.player1.X = FLOAT_TO_FIXP(x * 64.0f + 32 - 4);
        gameState.player1.Y = FLOAT_TO_FIXP(y * 64.0f);
        gameState.player1.rotation = PI;
      }
      else if (levelTile == 26 || levelTile == 27)
      {
        if (checkpoint == nullptr)
        {
          checkpoint = new CheckPoint();
          gameState.levelCheckPoints = checkpoint;
        }
        else
        {
          checkpoint->nextcheckpoint = new CheckPoint();
          checkpoint = checkpoint->nextcheckpoint;
        }

        checkpoint->x = x;
        checkpoint->y = y;
        checkpoint->passed = false;
        gameState.checkpoints++;
      }
    }
  }

  gameState.laptimer = false;
  gameState.curlap = 0;
  gameState.newbestLap = false;
  for (int i = 0; i < TIMED_LAPS; i++)
  {
    gameState.laptimes[i] = 0;
  }

  gameState.bestLap = saveData.BestLapTimes[gameState.level - 1]*10;

  gameState.player1.offRoad = false;
}

void processCarTune() {
  if (cross_input_up()) {gameState.menuItem--;setTimeout(100);}
  if (cross_input_down()) {gameState.menuItem++;setTimeout(100);}

  if (!gameState.desiredActivated && cross_input_left()) {
    switch (gameState.menuItem)
    {
    case 1: // Top Speed
      if (saveData.car_maxspeed > 0) {
        saveData.car_maxspeed--;
        setTimeout(100);
      }
      break;
    case 2: // Acceleration
      if (saveData.car_acceleration > 0) {
        saveData.car_acceleration--;
        setTimeout(100);
      }
      break;   
    case 3: // Turning
      if (saveData.car_turn > 0) {
        saveData.car_turn--;
        setTimeout(100);
      }
      break;   
    }
  }

  if (!gameState.desiredActivated && cross_input_right()) {
    int freePoints = gameState.car_tune_total - saveData.car_maxspeed - saveData.car_acceleration - saveData.car_turn;
    if (freePoints <= 0) {
      return;
    }
    switch (gameState.menuItem)
    {
    case 1: // Top Speed
      if (saveData.car_maxspeed < 7) {
        saveData.car_maxspeed++;
        setTimeout(100);
      }
      break;
    case 2: // Acceleration
      if (saveData.car_acceleration < 7) {
        saveData.car_acceleration++;
        setTimeout(100);
      }
      break;
    case 3: // Turning
      if (saveData.car_turn < 7) {
        saveData.car_turn++;
        setTimeout(100);
      }
      break;
    }
  }

  if (gameState.menuItem < 1) gameState.menuItem = 3;
  if (gameState.menuItem > 3) gameState.menuItem = 1;

  if (cross_input_a()) {
    //Save
    cross_save(saveData);
    gameState.mode = 4;
    setTimeout(100);
  }

  if (!gameState.desiredActivated && cross_input_b()) {
    saveData.car_maxspeed = 4;
    saveData.car_acceleration = 4;
    saveData.car_turn = 4;
    }
}

void processGameMode() {
  // Paused game processing
  if (gameState.paused)
  {
    if (cross_input_down())
    {
      gameState.mode = 4;
    }

    if (cross_input_a() || cross_input_b())
    {
      if (gameState.paused && !doTimeout())
        gameState.paused = false;
    }
    // Shortcut exit, we don't do anything else
    return;
  }

  // Game play input
  if (cross_input_up())
  {
    gameState.paused = true;
    cross_stop_audio(saveData.sound);
    setTimeout(400);
    return;
  }

  if (cross_input_left())
  {
    gameState.player1.rotation -= gameState.max_turn_speed * tFrameMs;
  }

  if (cross_input_right())
  {
    gameState.player1.rotation += gameState.max_turn_speed * tFrameMs;
  }

  // Process Player Rotation
  if (gameState.player1.rotation < 0)
    gameState.player1.rotation = PI * 2;
  if (gameState.player1.rotation > PI * 2)
    gameState.player1.rotation = 0;

  if (cross_input_a() || cross_input_b())
  {
    gameState.player1.acceleration.force += gameState.acceleration * tFrameMs;
  }

  if (gameState.laptimer)
  {
    gameState.laptimes[(gameState.curlap)] += tFrameMs;
    if (gameState.laptimes[(gameState.curlap)] > 99990)
    {
      gameState.laptimes[(gameState.curlap)] = 99990;
    }
  }
}


void updateGameMode()
{
  if (gameState.paused)
    return;
  int xyMax = 64 * gameState.levelSize;

  gameState.player1.XVec = xVec2(gameState.player1.acceleration.force, FLOAT_TO_FIXP(gameState.player1.rotation));
  gameState.player1.YVec = yVec2(gameState.player1.acceleration.force, FLOAT_TO_FIXP(gameState.player1.rotation));
  gameState.player1.X += gameState.player1.XVec;
  gameState.player1.Y += gameState.player1.YVec;

  int x = FIXP_TO_INT(gameState.player1.X);
  int y = FIXP_TO_INT(gameState.player1.Y);

  bool collision = false;
  if (x > xyMax)
  {
    gameState.player1.X = FLOAT_TO_FIXP(xyMax);
    collision = true;
  }
  else if (x < 0)
  {
    gameState.player1.X = INT_TO_FIXP(0);
    collision = true;
  }

  if (y > xyMax)
  {
    gameState.player1.Y = FLOAT_TO_FIXP(xyMax);
    collision = true;
  }
  else if (y < 0)
  {
    gameState.player1.Y = INT_TO_FIXP(0);
    collision = true;
  }

  // Side Hard wall collisions
  if (collision)
  {
    if (gameState.player1.acceleration.force > 0)
      gameState.player1.acceleration.force -= gameState.acceleration * tFrameMs;
    if (gameState.player1.acceleration.force < 0)
      gameState.player1.acceleration.force += gameState.acceleration * tFrameMs;
  }
 
  if (gameState.player1.acceleration.force > gameState.max_speed)
    gameState.player1.acceleration.force = gameState.max_speed;

  // Drag
  if (gameState.player1.acceleration.force > 0)
  {
    gameState.player1.acceleration.force -= gameState.drag * tFrameMs;
    if (gameState.player1.acceleration.force < 0)
      gameState.player1.acceleration.force = 0;
  }
  else if (gameState.player1.acceleration.force < 0)
  {
    gameState.player1.acceleration.force += gameState.drag * tFrameMs;
    if (gameState.player1.acceleration.force > 0)
      gameState.player1.acceleration.force = 0;
  }

  // Off Road
  if (gameState.player1.offRoad)
  {
    if (gameState.player1.acceleration.force > gameState.offroad)
    {
      gameState.player1.acceleration.force -= gameState.offroad_pen * tFrameMs;
    }
    else if (gameState.player1.acceleration.force < gameState.offroad_neg)
    {
      gameState.player1.acceleration.force += gameState.offroad_pen * tFrameMs;
    }
  }

  // Tile Logic
  int tilex = (x + 5) / 64;
  int tiley = (y + 5) / 64;
  uint8_t tile = getLevelTile(gameState.levelMap, tilex, tiley, gameState.levelSize);

  if (gameState.lastx != tilex || gameState.lasty != tiley)
  {
    // Went through a check point
    if (gameState.lasttile == 26 || gameState.lasttile == 27)
    {
      bool done = false;
      CheckPoint *a = gameState.levelCheckPoints;
      while (a != nullptr && !done)
      {
        if (a->x == gameState.lastx && a->y == gameState.lasty)
        {
          if (!a->passed)
          {
            a->passed = true;
            gameState.checkpointpassed++;
          }
          done = true;
        }
        a = a->nextcheckpoint;
      }
    }

    // Was in start block and no longer in start block
    if (gameState.lasttile == 24 || gameState.lasttile == 25)
    {
      if (gameState.laptimer)
      {
        if (gameState.checkpoints == gameState.checkpointpassed)
        {
          // Finished a lap
          if (gameState.laptimes[(gameState.curlap)] < gameState.bestLap || gameState.bestLap == 0)
          {
            gameState.bestLap = gameState.laptimes[(gameState.curlap)];
            gameState.newbestLap = true;

            // Update Best Lap Time to Rom
            if (saveData.BestLapTimes[gameState.level - 1] == 0 || saveData.BestLapTimes[gameState.level - 1] > gameState.laptimes[(gameState.curlap)]/10)
            {
              saveData.BestLapTimes[gameState.level - 1] = gameState.laptimes[(gameState.curlap)]/10;
              uint16_t levelTime = FX::readIndexedUInt16(FX_LEVEL_TIMES, (gameState.level - 1) * 3 + 2);
              if (saveData.BestLapTimes[gameState.level - 1] < levelTime)
              { // If the time is better than 3rd place, we can go forwards
                if (saveData.maxLevel < gameState.level + 1 && gameState.level < LEVELS)
                  saveData.maxLevel = gameState.level + 1;
              }

              if (gameState.level > saveData.maxLevel)
                saveData.maxLevel = gameState.level;

              cross_save(saveData);
            }
          }

          gameState.curlap++;
          gameState.checkpointpassed = 0;

          CheckPoint *point = gameState.levelCheckPoints;
          while (point != nullptr)
          {
            point->passed = false;
            point = point->nextcheckpoint;
          }
        }
      }
      else
      {
        gameState.laptimer = true;
      }
    }
  }

  gameState.lasttile = tile;
  gameState.lastx = tilex;
  gameState.lasty = tiley;
}

void setLevelTimeString(char number, uint16_t time)
{
  sprintf(string, ("%c-%2u.%02u"), number, time / 100, time % 100);
}

void setLevelString()
{
  sprintf(string, ("Level %d"), gameState.level);
}

#ifdef PERF_RENDER
double split;
#endif

void displayGameMode()
{
#ifdef PERF_RENDER
  split = getMs() ;
#endif

  int x = FIXP_TO_INT(gameState.player1.X) - 60;
  int y = FIXP_TO_INT(gameState.player1.Y) - 30;
  int xyMax = 64 * gameState.levelSize;

  if (x < 0)
    x = 0;
  else if (x > xyMax - 128)
    x = xyMax - 128;

  if (y < 0)
    y = 0;
  else if (y > xyMax - 64)
    y = xyMax - 64;
  int inlinex = x % 64 * -1;
  int inliney = y % 64 * -1;

#ifdef PERF_RENDER
  split = getMs() - split;
  sprintf(string, "Before Draw - %d\n",(int)split);
  Serial.write(string);
  split = getMs();
#endif
  // Draw Road
  for (uint8_t j = y / 64; j < (y / 64) + 2 && j < gameState.levelSize; j++)
  {
    for (uint8_t i = x / 64; i < (x / 64) + 3 && i < gameState.levelSize; i++)
    {
      if (inlinex < -64 | inlinex > xyMax | inliney < -64 | inlinex > xyMax){
        inlinex += 64;
        continue;
      }
      uint8_t levelTile = getLevelTile(gameState.levelMap, i, j, gameState.levelSize);
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

#ifdef PERF_RENDER
  split = getMs() - split;
  sprintf(string, "Draw Road - %d\n",(int)split);
  Serial.write(string);
  split = getMs();
#endif

  // Off Road Check (Do it here because we rely on the populated screen
  int carx = FIXP_TO_INT(gameState.player1.X);
  int cary = FIXP_TO_INT(gameState.player1.Y);

  if (carx < 60)
  {
    // use x as is in direct to screen;
  }
  else if (carx > (xyMax+10) - 68)
  {
    carx = 128 - ((xyMax+10) - carx);
  }
  else
  {
    carx = 60;
  }

  if (cary < 30)
  {
    // use y as is in direct to screen;
  }
  else if (cary > (xyMax+10) - 36)
  {
    cary = 64 - ((xyMax+10) - cary);
  }
  else
  {
    cary = 30;
  }

  gameState.player1.offRoad = false;
  if (cross_getPixel(carx + 8, cary + 8) == 1)
  {
    gameState.player1.offRoad = true;
  }

  // Draw Car
  uint8_t carDirection = getCarDirection(((FIXPOINT)(gameState.player1.rotation * 10)));

  FX::drawBitmap(carx, cary, FX_DATA_CAR2, carDirection, dbmMasked);

  sprintf(string, ("%c-%2lu.%02lu %d/%d\n%c-%2u.%02u"), gameState.curlap + 1 + 48, gameState.laptimes[(gameState.curlap)] / 1000, gameState.laptimes[(gameState.curlap)] / 10 % 100, gameState.checkpointpassed, gameState.checkpoints, gameState.newbestLap ? '*' : 'B', saveData.BestLapTimes[gameState.level - 1]  / 100, saveData.BestLapTimes[gameState.level - 1]  % 100);
  cross_print(0, 0, 1, string);

#ifdef PERF_RENDER
  split = getMs() - split;
  sprintf(string, "Car and Strings- %d\n",(int)split);
  Serial.write(string);
  split = getMs();
#endif
  uint8_t mapSize = gameState.levelSize*2;
  if (mapSize > LEVEL_CACHE*2) mapSize = LEVEL_CACHE*2;

  for (int i = 0; i < mapSize+6; i++)
  {
    cross_drawVLine(128 - mapSize + i, 0, 6, 0);
  }

  if (!gameState.player1.offRoad && gameState.player1.acceleration.force != 0) {
      cross_playSound(!gameState.paused && (saveData.sound > 0), FIXP_TO_FLOAT(gameState.player1.acceleration.force)/FIXP_TO_FLOAT(gameState.max_speed)*500+rand()%30, 30);
  }
  else if (gameState.player1.offRoad && gameState.player1.acceleration.force != 0)
  {
    if (((gameState.laptimes[(gameState.curlap)] / 100) % 3) == 0)
      cross_playSound(!gameState.paused && (saveData.sound > 0), 100, 30);
  }
 
  // Draw Map
  uint8_t mapX = 128-mapSize;
  if (saveData.map) {
    buildMapCache();
    uint8_t levelTile = 0;
    uint8_t mapY = 7;

    for (uint8_t j = 0; j < LEVEL_CACHE && j < gameState.levelSize; j++) {
      for (uint8_t i = 0; i < LEVEL_CACHE && i < gameState.levelSize; i++) {
        cross_drawPixel(mapX+i*2,mapY+j*2,gameState.mapDisplay[i+j*LEVEL_CACHE]);
        cross_drawPixel(mapX+i*2+1,mapY+j*2,gameState.mapDisplay[i+j*LEVEL_CACHE]);
        cross_drawPixel(mapX+i*2,mapY+j*2+1,gameState.mapDisplay[i+j*LEVEL_CACHE]);
        cross_drawPixel(mapX+i*2+1,mapY+j*2+1,gameState.mapDisplay[i+j*LEVEL_CACHE]);
      }
    }

    // Overdraw player pixel ticking
    int carMapX = FIXP_TO_INT(gameState.player1.X)/64 * 2;
    int carMapY = FIXP_TO_INT(gameState.player1.Y)/64 * 2;

    bool display = (getCurrentMs()/200)%2==0;
    cross_drawPixel(mapX+carMapX-gameState.levelMapXMod*2,mapY+carMapY-gameState.levelMapYMod*2,display);
    cross_drawPixel(mapX+carMapX-gameState.levelMapXMod*2+1,mapY+carMapY-gameState.levelMapYMod*2,display);
    cross_drawPixel(mapX+carMapX-gameState.levelMapXMod*2,mapY+carMapY-gameState.levelMapYMod*2+1,display);
    cross_drawPixel(mapX+carMapX-gameState.levelMapXMod*2+1,mapY+carMapY-gameState.levelMapYMod*2+1,display);
  }

#ifdef PERF_RENDER
  split = getMs() - split;
  sprintf(string, "Map - %d\n",(int)split);
  Serial.write(string);
  split = getMs();
#endif

  // Display Speed
  int speed = FIXP_TO_FLOAT(gameState.player1.acceleration.force)/FIXP_TO_FLOAT(gameState.max_speed)*mapSize;
  int lines = 0;
  for (int i = 1; i <= speed; i++)
  {
    cross_drawVLine(mapX + i, 0, 7, 1);

  }
  cross_drawHLine(mapX, 6, mapSize, 0);
  
  // Set LEDs
  // cross_set_leds(gameState.offroad ? 256 : 0, speed == mapSize ? 256 : 0, speed*25);

  if (gameState.paused)
  {
    cross_print(0, 20, 3, FX_STR_PAUSED);
    for (int i = 0; i < 16; i++)
      cross_drawHLine(0, i + 48, 64, 0);

    cross_print(0, 48, 1, FX_STR_AB_CONT);
    cross_print(0, 56, 1, FX_STR_LEVEL_SELECT);
  }

#ifdef PERF_RENDER
  split = getMs() - split;
  sprintf(string, "Speedo - %d\n",(int)split);
  Serial.write(string);
  split = getMs();
#endif
}

void racerSetup()
{
  cross_setup();

  saveData = cross_load();
  if (saveData.maxLevel==0) {
    saveData.maxLevel = 1;
    cross_save(saveData);
  }
  gameState.level = 1;
  gameState.levelMap = getLevelMap(gameState.level);
  gameState.levelSize = getLevelMapSize(gameState.level);
  gameState.laptimer = false;
  setTimeout(100);
  gameState.lastmode = -1;
  gameState.mode = 0;

  cross_loop_end();
}

void processMenu()
{
  // Debounce input
  if (doTimeout()) return;

  // Check for up button
  if (cross_input_up())
  {
    if (gameState.menuItem == 0)
      gameState.menuItem = 3;
    else
      gameState.menuItem -= 1;
    setTimeout(100);
  }
  // Check for down button
  else if (cross_input_down())
  {
    if (gameState.menuItem == 3)
      gameState.menuItem = 0;
    else
      gameState.menuItem += 1;
    setTimeout(100);
  }
  // Check for button press
  else if (cross_input_a())
  {
    gameState.enter = true;
    setTimeout(100);
  }
  else if (cross_input_b()) {
    gameState.mode = 1;
    gameState.menuItem = 0;
    gameState.enter = false;
  }
}

void displayOptionsMenu(int menuItem)
{
  FX::drawBitmap(0 ,0 , FX_DATA_LOGO   , 0, dbmNormal);
  FX::drawBitmap(50,15, FX_DATA_LOGO_FX, 0, dbmMasked);

  cross_print(90, 30 + 0, 1, FX_STR_ABOUT);
  cross_print(90, 30 + 8, 1, FX_STR_MUSIC);
  cross_print(90, 30 + 16, 1,FX_STR_SOUND);
  cross_print(90, 30 + 24, 1,FX_STR_MAP);
  
  if (saveData.music > 0)
    cross_print(90 + 5 * 6, 30 + 8, 1, FX_STR_STAR);
  if (saveData.sound > 0)
    cross_print(90 + 5 * 6, 30 + 16, 1, FX_STR_STAR);
  if (saveData.map > 0)
    cross_print(90 + 5 * 6, 30 + 24, 1, FX_STR_STAR);

  cross_print(84, 30 + menuItem * 8, 1, pointer);
}

void displayAbout() {
  FX::drawBitmap(0, 0, FX_DATA_ABOUT, 0, dbmNormal);

  cross_print(60, 0, 1,  FX_STR_MADE_BY);
  cross_print(60, 20, 1, FX_STR_GITHUB);
  cross_print(60, 40, 1, FX_STR_TWITTER);
  for (int i = 0; i < 3; i++) {
    cross_print(70, 10 + i * 20, 1, FX_STR_TONYM128);
  }
}

void updateOptionsMenu() {
  if (gameState.enter)
    switch (gameState.menuItem) {
      case 0:
        if (doTimeout()) return;
        gameState.mode = 3;
        gameState.enter = false;
        setTimeout(100);
      case 1:
        // Toggle Music
        if (doTimeout()) return;
        cross_stop_audio(true);
        if (saveData.music == 0) { saveData.music = 1;}
        else saveData.music = 0;
        gameState.enter = false;
        setTimeout(100);
        cross_save(saveData);
        break;
      case 2:
        // Toggle Sound
        if (doTimeout()) return;
        if (saveData.sound == 0) { saveData.sound = 1;}
        else saveData.sound = 0;

        gameState.enter = false;
        setTimeout(100);
        cross_save(saveData);
        break;
      case 3:
        // Toggle Map
        if (doTimeout()) return;
        if (saveData.map == 0) { saveData.map = 1;}
        else saveData.map = 0;

        gameState.enter = false;
        setTimeout(100);
        cross_save(saveData);
        break;
      case 99:
        //Back
        if (doTimeout()) return;
        gameState.mode = 1;
        gameState.enter = false;
        gameState.menuItem = 3;
        setTimeout(100);
        break;
    }
}

void updateMenu()
{
  if (gameState.enter)
  {
    switch (gameState.menuItem)
    {
    case 0:
      // Go To Last Level Finished
      gameState.level = saveData.maxLevel;
      gameState.levelMap = getLevelMap(gameState.level);
      gameState.levelSize = getLevelMapSize(gameState.level);
      gameState.mode = 4; // Level Start
      break;
    case 1:
      // Go To First Level
      gameState.level = 1;
      gameState.levelMap = getLevelMap(gameState.level);
      gameState.levelSize = getLevelMapSize(gameState.level);
      gameState.mode = 4; // Level Start
      break;
    case 2:
      // Times
      gameState.mode = 98;
      setTimeout(1000);
      break;
    case 3:
      // Options Menu
      gameState.mode = 2;
      setTimeout(100);
      gameState.menuItem = 0;
      gameState.enter = false;
      break;
    }
  }
}

void displayMenu(int menuItem)
{
  FX::drawBitmap(0 ,0 , FX_DATA_LOGO   , 0, dbmNormal);
  FX::drawBitmap(50,15, FX_DATA_LOGO_FX, 0, dbmMasked);

  cross_print(90, 30 + 0, 1,  FX_STR_CONTINUE);
  cross_print(90, 30 + 8, 1,  FX_STR_START);
  cross_print(90, 30 + 16, 1, FX_STR_TROPHIES);
  cross_print(90, 30 + 24, 1, FX_STR_OPTIONS);

  cross_print(84, 30 + menuItem * 8, 1, pointer);
}

void displayMap()
{
  for (int y = 0; y < gameState.levelSize; y++)
  {
    for (int x = 0; x < gameState.levelSize; x++)
    {
      uint8_t levelTile = getLevelTile(gameState.levelMap, x, y, gameState.levelSize);
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
      
      switch (gameState.levelSize)
      {
      case 10:
        FX::drawBitmap(x * 6, y * 6, FX_DATA_TILES_6, tilemap, dbmNormal);
        break;
      case 16:
        FX::drawBitmap(x * 4, y * 4, FX_DATA_TILES_4, tilemap, dbmNormal);
        break;
      case 30:
        FX::drawBitmap(x * 2, y * 2, FX_DATA_TILES_2, tilemap, dbmNormal);
        break;
      }
    }
  }
}

void drawBestTimes()
{
  for (int i = 0; i < 3; i++)
  {
    setLevelTimeString(i + 1 + 48, FX::readIndexedUInt16(FX_LEVEL_TIMES,(gameState.level - 1)*3 + i));

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
  setLevelTimeString('B', saveData.BestLapTimes[gameState.level - 1]);

  cross_print(74, 10, 1, string);
  cross_drawHLine(64, 19, 64, 1);
  drawBestTimes();
  cross_drawHLine(64, 46, 64, 1);
  
  if (doTimeout()) {}
  else if ((getCurrentMs() / 1000) % 2 == 0)
    {
      cross_print(64, 64 - 16, 1, FX_STR_PRESSA);
      cross_print(64, 64 - 8, 1,  FX_STR_TOSTART);
    }
  else {
      cross_print(64, 64 - 16, 1, FX_STR_LEFT);
      cross_print(64, 64 - 8, 1, FX_STR_TOTUNE);
  }
};

void inputLevelInfo()
{
  if (cross_input_a())
  {
    gameState.mode++;
    setTimeout(100);
  }

  if (cross_input_b())
  {
    gameState.mode = 1;
    gameState.enter = false;
    setTimeout(100);
  }

  if (cross_input_up())
  {
    setTimeout(500);
    if (gameState.level < saveData.maxLevel)
       gameState.level++;
    if (gameState.level > LEVELS)
      gameState.level = LEVELS;
    gameState.levelMap = getLevelMap(gameState.level);
    gameState.levelSize = getLevelMapSize(gameState.level);
  }

  if (cross_input_down())
  {
    setTimeout(500);
    gameState.level--;
    if (gameState.level < 1)
      gameState.level = 1;
    gameState.levelMap = getLevelMap(gameState.level);
    gameState.levelSize = getLevelMapSize(gameState.level);
  }
  
  // Tune car
  if (cross_input_left())
  {
    gameState.mode = 99;
  }
}


bool displayLevelZoom()
{
  float time = ((ZOOM_TIME - gameState.timeout) / (float)ZOOM_TIME);
  float zoom = 0.1f + 0.9f * time;
  int pixelSize = (int)floor(6.0f + 58.0f * time);

  float headtox = ((FIXP_TO_FLOAT(gameState.player1.X) - 64) * time * 2);
  float headtoy = ((FIXP_TO_FLOAT(gameState.player1.Y) - 32) * time * 2);

  if (headtox < 0)
    headtox = 0;
  if (headtoy < 0)
    headtoy = 0;

  if (headtox > FIXP_TO_FLOAT(gameState.player1.X) - 64)
    headtox = FIXP_TO_FLOAT(gameState.player1.X) - 64;

  if (headtoy > FIXP_TO_FLOAT(gameState.player1.Y) - 32)
    headtoy = FIXP_TO_FLOAT(gameState.player1.Y) - 32;

  headtox *= (pixelSize / 64.0f);
  headtoy *= (pixelSize / 64.0f);

  int endx = (int)(64 + 64 * (time * 7));
  if (endx > 128)
    endx = 128;

  // Choose map
  __uint24 tiles = getTileSet(pixelSize);
  uint8_t tileOffset = getTileSetOffset(pixelSize)/2;

  for (int y = 0; y < gameState.levelSize; y++)
  {
    for (int x = 0; x < gameState.levelSize; x++)
    {
      // Basic Screen Check
      if ((x * pixelSize + pixelSize - headtox - tileOffset) > 0 && (y * pixelSize + pixelSize - headtoy - tileOffset) > 0 && (x * pixelSize - headtox - tileOffset) < endx && (y * pixelSize - headtoy - tileOffset < 64))
      {
        uint8_t levelTile = getLevelTile(gameState.levelMap, x, y, gameState.levelSize);
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

        FX::drawBitmap((int)(x * pixelSize - headtox - tileOffset),  (int)(y * pixelSize - headtoy - tileOffset), tiles, tilemap, dbmNormal);
      }
    }
  }

  return false;
}

void drawContinueMenu()
{
  if ((getCurrentMs() / 1000) % 2 == 0)
  {
    if (saveData.maxLevel > gameState.level)
      cross_print(64, 64 - 16, 1, FX_STR_NEXTRETRY);
  }
}

void displayCarTune()
{
  cross_print(3, 0, 1, "Car Tuning");
  cross_drawHLine(3, 6, 70, 1);
  cross_print(6, 2+15, 1, "Speed");
  cross_print(6, 2+25, 1, "Accel");
  cross_print(6, 2+35, 1, "Turning");
  cross_print(6, 2+45, 1, "Points");
  cross_print(6, 59, 1, "A - Save, B - Reset");

  cross_print(0,2+5+10*gameState.menuItem,1,pointer);

  int i = 0;
  for (i = 15; i < 60; i += 10) 
    cross_drawHLine(45, i, 70, 1);
    
  for (i = 45; i < 116; i += 10) 
    cross_drawVLine(i, 15, 40, 1);
  int speed = saveData.car_maxspeed;
  int accel = saveData.car_acceleration;
  int turn = saveData.car_turn;
  int freePoints = gameState.car_tune_total - saveData.car_maxspeed - saveData.car_acceleration - saveData.car_turn;

  if (gameState.desiredActivated) {
    speed = accel = turn = freePoints = 7;
  }

  int y = 18;
  for (i = 0; i < speed; i++) {
    cross_print(3+45+i*10,y,1,"X");
  }
  y+=10;
  for (i = 0; i < accel; i++) {
    cross_print(3+45+i*10,y,1,"X");
  }

  y+=10;
  for (i = 0; i < turn; i++) {
    cross_print(3+45+i*10,y,1,"X");
  }

  y+=10;
  for (i = 0; i < freePoints; i++) {
    cross_print(3+45+i*10,y,1,"X");
  }

}

void drawTrophySheet()
{
  for (int i = 13; i < 53; i += 13) 
    cross_drawHLine(0, i, 128, 1);

  cross_print(5, 17, 1, "G");
  cross_print(5, 31, 1, "S");
  cross_print(5, 43, 1, "B");

  for (int level = 0; level < 10; level++)
  {
    int modLevel = level + 10*gameState.paused;
    sprintf(string, ("%d"), modLevel + 1);
    cross_print(18 + 3*!gameState.paused + level * 11, 3, 1, string);
    cross_drawVLine(16 + level * 11, 0, 53, 1);
    if (saveData.BestLapTimes[modLevel] > 0)
      for (int i = 0; i < 3; i++)
      {
        if (saveData.BestLapTimes[modLevel] < FX::readIndexedUInt16(FX_LEVEL_TIMES,modLevel*3 + i))
        {
          cross_print(20 + level * 11, 4 + (1 + i) * 13, 1, ("X"));
          break;
        }
      }
  }

  uint8_t totalScore = 0;

  for (int level = 0; level < 20; level++) {
    if (saveData.BestLapTimes[level] > 0)
      for (int i = 0; i < 3; i++)
      {
        if (saveData.BestLapTimes[level] < FX::readIndexedUInt16(FX_LEVEL_TIMES,level*3 + i))
        {
          switch (i) {
          case 0:
            //First Place
            totalScore += 5;
          break;
          case 1:
            //Second Place
            totalScore += 3;
          break;
          case 2:
            //Third Place
            totalScore += 1;
            break;
          }

          break;
        }
      }
  }

  sprintf(string, "Total Score : %3d/100", totalScore);
  cross_print(24, 58, 1, string);
}

void drawGoalTimes()
{
  setLevelString();
  cross_print(74, 0, 1, string);
  cross_drawHLine(64, 8, 64, 1);

  setLevelTimeString('B', saveData.BestLapTimes[gameState.level - 1]);
  cross_print(74, 10, 1, string);
  cross_drawHLine(64, 19, 64, 1);

  drawBestTimes();

  cross_drawHLine(64, 46, 64, 1);

  drawContinueMenu();
  cross_print(0, 64 - 7, 1, FX_STR_LTIMES);
}

void drawAllTimes()
{
  setLevelString();
  cross_print(74, 0, 1, string);
  cross_drawHLine(64, 8, 64, 1);

  for (int i = 0; i < TIMED_LAPS; i++)
  {
    setLevelTimeString(i + 1 + 48, gameState.laptimes[i]/10);
    cross_print(74, 9 + 8 * i, 1, string);
  }

  drawContinueMenu();
  cross_print(0, 64 - 7, 1, FX_STR_RGOAL);
  cross_drawHLine(64, 64 - 17, 128 - 64, 1);
}

void drawTrophy()
{
  FX::drawBitmap(0, 0, FX_DATA_TROPHY, 0, dbmNormal);

  if (gameState.curlap == 0)
  {
    drawGoalTimes();
  }
  else if (gameState.curlap == 1)
  {
    drawAllTimes();
  };
};

void inputTrophy()
{
  if (cross_input_left())
  {
    gameState.curlap = 1;
  }

  if (cross_input_right())
  {
    gameState.curlap = 0;
  }

  if (cross_input_a())
  {
    if (gameState.level < saveData.maxLevel)
    {
      gameState.level += 1;
      if (gameState.level > LEVELS)
        gameState.level = LEVELS;
      gameState.mode = 4;
      gameState.levelMap = getLevelMap(gameState.level);
      gameState.levelSize = getLevelMapSize(gameState.level);
    }
  }

  if (cross_input_b())
  {
    gameState.mode = 4;
  }
};

void displayIntro() {
  int x = 50-gameState.timeout/10;
  FX::drawBitmap(0 ,0 , FX_DATA_LOGO   , 0, dbmNormal);
  FX::drawBitmap(x, 15, FX_DATA_LOGO_FX, 0, dbmMasked);
}

void displayZoomer() {
      displayGameMode();
      int lastnumber = gameState.timeout == 3000 ? 4 : gameState.timeout / 1000 + 1;

      int distance = gameState.timeout % 1000;
      int number = gameState.timeout / 1000;

      if (number > 2) number = 2;
      else if (number < 0) number = 0;

      float mod = (1000.0f - distance) / 1000.0f;
      int numMod = (int)(32 * mod);
      cross_drawBitmapTile(32 + numMod, 0 + numMod, 64, 64, 1, 0, 1.0f - mod, FX::readIndexedUInt24(FX_DATA_NUMBERS,number));
      if (gameState.timeout > 2900 and gameState.timeout < 3000) cross_playSound(saveData.sound > 0, 1000, 900);
      else if (gameState.timeout > 1900 and gameState.timeout < 2000) cross_playSound(saveData.sound > 0, 1000, 900);
      else if (gameState.timeout >  900 and gameState.timeout < 1000) cross_playSound(saveData.sound > 0, 1000, 900);
      
      if (!doTimeout())
      {
        gameState.mode = 10;
        cross_playSound(saveData.sound > 0, 1500, 1000);
      }
}

void update() {
#ifdef DEBUG_MODE
  if (cross_input_a() && cross_input_b()) {
    return;
  }
#endif

  tFrameMs = getFrameMs();

  switch (gameState.mode)
  {
  case 0: // Intro screen
    if (gameState.lastmode != gameState.mode)
    {
      gameState.lastmode = gameState.mode;
      setTimeout(1000);
    }
    else
    {
      if (!doTimeout())
      {
        gameState.mode += 1;
      }
    }
    break;
  case 1: // Menu
    if (gameState.lastmode != gameState.mode)
    {
      cross_stop_audio(saveData.music);
      cross_play_audio(saveData.music, FX_SOUND_INTRO);
      gameState.lastmode = gameState.mode;
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
    if (gameState.lastmode != gameState.mode)
    {
      cross_stop_audio(saveData.music);
      cross_play_audio(saveData.music, FX_SOUND_LEVEL_TUNE);
      gameState.lastmode = gameState.mode;
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
    if (gameState.lastmode != gameState.mode)
    {
      cross_stop_audio(saveData.music);
      cross_play_audio(saveData.music, FX_SOUND_ABOUT);
      gameState.lastmode = gameState.mode;
      setTimeout(100);
    }
    if (!doTimeout()) {
      if (cross_input_a() || cross_input_b()) {
        setTimeout(100);
        cross_stop_audio(saveData.music);
        cross_play_audio(saveData.music, FX_SOUND_LEVEL_TUNE);
        gameState.mode = 2;
      }
    }
    break;
  case 4: // Level Start
    if (gameState.lastmode != gameState.mode)
    {
      cross_stop_audio(saveData.music);
      cross_play_audio(saveData.music, FX_SOUND_LEVEL_SELECT);
      gameState.lastmode = gameState.mode;
      setTimeout(1000);
    }
    else
    {
      if (!doTimeout())
        inputLevelInfo();
    }
    break;
  case 5: // Zoom in
    if (gameState.lastmode != gameState.mode)
    {
      cross_stop_audio(saveData.music);
      cross_play_audio(saveData.music, FX_SOUND_LEVEL_ZOOM);
      gameState.lastmode = gameState.mode;
      setTimeout((int)ZOOM_TIME);
      setLevelDetails();
    }
    else
    {
      if (!doTimeout())
      {
        gameState.mode = 6;
      }
    }
    break;
  case 6: // 3,2,1,GO!
    if (gameState.lastmode != gameState.mode)
    {
      cross_stop_audio(saveData.sound || saveData.music);
      setLevelDetails();
      gameState.lastmode = gameState.mode;
      setTimeout(3000);
    }
   break;
  case 8: // Win Screen / Next Level Select
    if (gameState.lastmode != gameState.mode)
    {
      cross_stop_audio(saveData.sound || saveData.music);
      gameState.lastmode = gameState.mode;
      setTimeout(1000);
      gameState.curlap = 0;
      gameState.newbestLap = false;
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
    if (gameState.lastmode != gameState.mode)
    {
      setLevelDetails();
      if (gameState.desiredActivated) {
        gameState.max_turn_speed = gameState.default_max_turn_speed + (4) * gameState.mod_turn;
        gameState.max_speed = gameState.default_max_speed + (4)* gameState.mod_max_speed;
        gameState.acceleration = gameState.default_acceleration + (4) * gameState.mod_acceleration;
        gameState.offroad = gameState.max_speed/4;
        gameState.offroad_neg = gameState.max_speed/4;
      } else {
        gameState.max_turn_speed = gameState.default_max_turn_speed + (saveData.car_turn - 4) * gameState.mod_turn;
        gameState.max_speed = gameState.default_max_speed + (saveData.car_maxspeed -4)* gameState.mod_max_speed;
        gameState.acceleration = gameState.default_acceleration + (saveData.car_acceleration -4) * gameState.mod_acceleration;
        gameState.offroad = gameState.max_speed/4;
        gameState.offroad_neg = gameState.max_speed/4;
      }
      gameState.laptimer = true;
      gameState.lastmode = gameState.mode;
    }

    processGameMode();
    updateGameMode();
    if (gameState.curlap == TIMED_LAPS)
    {
      gameState.mode = 8;
    }
    break;
  case 98: // Trophy Screen
    if (gameState.lastmode != gameState.mode)
    {
      cross_stop_audio(saveData.music);
      cross_play_audio(saveData.music, FX_SOUND_WIN);
      gameState.lastmode = gameState.mode;
    }
    if (!doTimeout()) {
        if (cross_input_b() || cross_input_a()) {
          gameState.mode = 1;
          gameState.enter = false;
          setTimeout(1000);
        }
        if (cross_input_left() || cross_input_right()) {
          setTimeout(200);
          gameState.paused = !gameState.paused;
        }
      }
    break;
  case 99: // Tune Car
    if (gameState.lastmode != gameState.mode)
    {
      cross_stop_audio(saveData.music);
      cross_play_audio(saveData.music, FX_SOUND_LEVEL_TUNE);
      gameState.lastmode = gameState.mode;
      gameState.menuItem = 1;
      gameState.paused = 0;
      setTimeout(1000);
    }
    else
    {
      if (!doTimeout()) processCarTune();
    }
    break;
  }
}

void render() {

  switch (gameState.mode)
  {
  case 0: // Intro screen
    if (gameState.lastmode == gameState.mode) {
      displayIntro();
    }
    break;
  case 1: // Menu
    if (gameState.lastmode == gameState.mode) {
      displayMenu(gameState.menuItem);
    }
    break;
  case 2: // Options Menu
    if (gameState.lastmode == gameState.mode) {
      displayOptionsMenu(gameState.menuItem);
    }
    break;
  case 3: // About
    if (gameState.lastmode == gameState.mode) {
      displayAbout();
    }
    break;
  case 4: // Level Start
    if (gameState.lastmode == gameState.mode) {
      displayMap();
      displayLevelInfo();
    }
    break;
  case 5: // Zoom in
    if (gameState.lastmode == gameState.mode) {
      displayLevelZoom();
    }
    break;
  case 6: // 3,2,1,GO!
    if (gameState.lastmode == gameState.mode) {
      displayZoomer();
    }

   break;
  case 8: // Win Screen / Next Level Select
    if (gameState.lastmode == gameState.mode) {
      drawTrophy();
      cross_play_audio(saveData.music, FX_SOUND_TROPHY);
    }

    break;
  case 10: // Play Game
    if (gameState.lastmode == gameState.mode) {
      displayGameMode();
    }
    break;
  case 98: // Trophy Screen
    if (gameState.lastmode == gameState.mode) {
      drawTrophySheet();
    }
    break;
  case 99: // Tune Car
    if (gameState.lastmode == gameState.mode) {
      displayCarTune();
    }
    break;
  }

  #ifdef DEBUG_MODE
  sprintf(string,"%0d %3d %3d %3d - %2u", gameState.mode, FIXP_TO_INT(gameState.player1.acceleration.force), FIXP_TO_INT(gameState.player1.X), FIXP_TO_INT(gameState.player1.Y), getFrameMs());
  cross_print(0,58,6,string);
  #endif
}

#ifdef PERF_RENDER
double renderTime;
#endif

void racerLoop()
{
  if (!cross_loop_start()) {
    return;
  }

  if (!gameState.desiredActivated) {
    uint8_t keyCheck = checkDesired(gameState.currentKeyPress);
    switch (keyCheck) {
      case 1: 
        gameState.currentKeyPress = 0;
        break;
      case 2:
        gameState.currentKeyPress += 1;
        break;
    }

    if (gameState.currentKeyPress == 11) {
        gameState.desiredActivated = true;
        pointer = FX_STR_MORE;
    }
  }
#ifdef PERF_RENDER
  renderTime = getMs();
#endif

  update();
  render();

#ifdef PERF_RENDER
  renderTime = getMs()- renderTime;
  sprintf(string,"RenderTime - %i\n",(int)renderTime);
  Serial.write(string);
#endif
  cross_loop_end();
}
