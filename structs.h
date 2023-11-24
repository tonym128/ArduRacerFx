#ifndef _STRUCTS_H_
#define _STRUCTS_H_
#include "fixpoint.h"
constexpr uint8_t TIMED_LAPS = 5;

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

constexpr uint8_t LEVELS = 10; // 10 Levels

struct SaveData
{
	unsigned int BestLapTimes[LEVELS]; 
  uint8_t maxLevel = 1;
  uint8_t music = 1;
  uint8_t sound = 1;
  uint8_t map = 1;
  uint8_t car_maxspeed = 4;
  uint8_t car_acceleration = 4;
  uint8_t car_turn = 4;
  uint8_t car_tune_total = 12;
};

struct GameState {

  uint8_t level;
  __uint24 levelMap;
  uint8_t levelSize;
  uint8_t lasttile;
  uint8_t lastx;
  uint8_t lasty;

  unsigned int laptimes[TIMED_LAPS];
  unsigned int bestLap;
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
  double max_turn_speed = 0.003;
  
  // Max Speed 60fps
  FIXPOINT max_speed = FLOAT_TO_FIXP(2.5f);
  FIXPOINT acceleration = FLOAT_TO_FIXP(0.003f);
  FIXPOINT max_dec = 3*acceleration;
  FIXPOINT offroad = max_speed/4;
  FIXPOINT offroad_neg = max_speed/4;

  FIXPOINT offroad_pen = FLOAT_TO_FIXP(0.01f);
  FIXPOINT drag = FLOAT_TO_FIXP(0.04f / 30.0f);
};

constexpr double mod_turn = 0.0003;
constexpr FIXPOINT mod_max_speed = FLOAT_TO_FIXP(0.25f);
constexpr FIXPOINT mod_accelerate = FLOAT_TO_FIXP(0.0003f);

#endif
