#include "../include/game_logic.h"

void draw_ball(const ball_t* b) {
  mvaddch(b->y, b->x, '0');
}

void draw_paddle(const paddle_t *paddle) {
	for (int i = 0; i < paddle->len; i++) {
		mvaddch(paddle->y + i, paddle->x, '|');
  }
}

void draw_score(int col, const score_t *score) {
	mvprintw(0, col / 2, "%d ? %d", score->left_score, score->right_score);
}

void move_ball(int row, int col, ball_t *ball, const paddle_t *lp, const paddle_t *rp, score_t *score) {
  if (ball->x < 0) {
    // right player scored
    score->right_score++;

    ball->x = col / 2,
    ball->y = row / 2,
    ball->vx = 1;
    ball->vy = 1;
  } else if (ball->x > col) {
    // left player scored
    score->left_score++;

    ball->x = col / 2,
    ball->y = row / 2,
    ball->vx = 1;
    ball->vy = 1;
  }

  if (ball->y + ball->vy <= 0 || ball->y + ball->vy >= row) {
    ball->vy *= -1;
  }

  // collision with paddles
  if ((ball->x + ball->vx == lp->x && ball->y + ball->vy - lp->y >= 0 && ball->y + ball->vy - lp->y < lp->len) ||
      (ball->x + ball->vx == rp->x && ball->y + ball->vy - rp->y >= 0 && ball->y + ball->vy - rp->y < rp->len)) {
    ball->vx *= -1;
  }

  // update ball's position
  ball->x += ball->vx;
  ball->y += ball->vy;
}

