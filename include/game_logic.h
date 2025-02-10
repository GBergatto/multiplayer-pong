#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <ncurses.h>

typedef struct {
  int x, y;
  int vx, vy;
} ball_t;

typedef struct {
  const int x;
  int y;
  const int len;
} paddle_t;

typedef struct {
  int left_score;
  int right_score;
} score_t;

void draw_ball(const ball_t* b);
void draw_paddle(const paddle_t *paddle);
void draw_score(int col, const score_t *score);
void move_ball(int row, int col, ball_t *ball, const paddle_t *lp, const paddle_t *rp, score_t *score);

#endif
