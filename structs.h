#ifndef STRUCTS_H_
#define STRUCTS_H_

typedef struct Player {
  short x;
  short y;
  short height;
  short points;
} Player;

typedef struct Ball {
  short x;
  short y;
  short directionX;
  short directionY;
} Ball;

#endif
