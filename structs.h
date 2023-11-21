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
  uint8_t sound = 1;
  uint8_t map = 1;
};

struct GameState {

  uint8_t level;
  __uint24 levelMap;
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
  // Max Speed 20fps
  // FIXPOINT max_speed = FLOAT_TO_FIXP(5.0f);
  // FIXPOINT max_dec = FLOAT_TO_FIXP(-2.0f);
  // FIXPOINT offroad = FLOAT_TO_FIXP(1.5f);
  // FIXPOINT offroad_neg = FLOAT_TO_FIXP(-1.5f);
  // FIXPOINT offroad_pen = FLOAT_TO_FIXP(0.015f * tFrameMs);
  // FIXPOINT drag = FLOAT_TO_FIXP(0.06f / 30.0f * tFrameMs);

  // Max Speed 60fps
  FIXPOINT max_speed = FLOAT_TO_FIXP(2.0f);
  FIXPOINT acceleration = FLOAT_TO_FIXP(0.1);
  FIXPOINT max_dec = FLOAT_TO_FIXP(-0.8f);
  FIXPOINT offroad = FLOAT_TO_FIXP(0.6f);
  FIXPOINT offroad_neg = FLOAT_TO_FIXP(-0.6f);

  FIXPOINT offroad_pen = FLOAT_TO_FIXP(0.015);
  FIXPOINT drag = FLOAT_TO_FIXP(0.06f / 30.0f);
};

#endif
