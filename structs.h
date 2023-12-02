#ifndef _STRUCTS_H_
#define _STRUCTS_H_
#include "fixpoint.h"
constexpr uint8_t TIMED_LAPS = 5;
#define LEVEL_CACHE 10 // Tiles in cache

struct Vec3 {
  FIXPOINT x,y;
  FIXPOINT direction;
  FIXPOINT force;
  FIXPOINT time;
};

struct Player {
  FIXPOINT X,Y,XVec,YVec;
  double rotation;
  Vec3 acceleration;
  bool offRoad = false;
};

struct CheckPoint {
  uint8_t x;
  uint8_t y;
  bool passed;
  CheckPoint *nextcheckpoint;
};

constexpr uint8_t LEVELS = 20; // 10 original Levels, 10 fx levels

struct SaveData
{
	uint16_t BestLapTimes[LEVELS]; 
  uint8_t maxLevel = 20;
  uint8_t music = 1;
  uint8_t sound = 1;
  uint8_t map = 1;
  uint8_t car_maxspeed = 4;
  uint8_t car_acceleration = 4;
  uint8_t car_turn = 4;
};

struct GameState {

  uint8_t level;
  __uint24 levelMap;
  uint8_t levelSize;

  bool mapDisplay[LEVEL_CACHE*LEVEL_CACHE];
  int levelMapXMod = -1;
  int levelMapYMod = -1;

  uint8_t lasttile;
  uint8_t lastx;
  uint8_t lasty;
  uint8_t car_tune_total = 12;

  uint32_t laptimes[TIMED_LAPS];
  uint32_t bestLap;
  bool newbestLap;
  CheckPoint *levelCheckPoints;
  bool laptimer;
  bool paused;
  bool startCross;
  bool enter;
  uint8_t curlap;
  uint8_t checkpoints;
  uint8_t checkpointpassed;
  Player player1;

  FIXPOINT StageX = FIXP_TO_INT(0), StageY = FIXP_TO_INT(0);

  uint8_t mode = 0;
  uint8_t lastmode = -1;
  uint8_t menuItem = 0;

  int timeout;

  // Car setup
  // Fixed car variables
  static const FIXPOINT offroad_pen = FLOAT_TO_FIXP(0.01f);
  static const FIXPOINT drag = FLOAT_TO_FIXP(0.04f / 30.0f);

  static const double default_max_turn_speed = 0.003;
  static const FIXPOINT default_max_speed = FLOAT_TO_FIXP(2.5f);
  static const FIXPOINT default_acceleration = FLOAT_TO_FIXP(0.003f);

  static const double mod_turn = 0.0003;
  static const FIXPOINT mod_max_speed = FLOAT_TO_FIXP(0.25f);
  static const FIXPOINT mod_acceleration = FLOAT_TO_FIXP(0.0003f);

  // Modified by car tuning
  double max_turn_speed = 0.003;
  FIXPOINT max_speed = FLOAT_TO_FIXP(2.2f);
  FIXPOINT acceleration = FLOAT_TO_FIXP(0.003f);
  FIXPOINT max_dec = acceleration/5;
  FIXPOINT offroad = max_speed/3;
  FIXPOINT offroad_neg = max_speed/3;
};


#endif
