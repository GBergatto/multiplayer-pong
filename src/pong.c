#include <ncurses.h>
#include <pthread.h>

#include "../include/game_logic.h"

int main() {
  // intialize screen
  initscr();
  cbreak();
  noecho();
  timeout(50); // for getch
  curs_set(0);
  keypad(stdscr, TRUE);

  // get size of the screen
  int row, col;
  getmaxyx(stdscr,row,col);

  // initialize ball, paddles, and score
  ball_t ball = {
    .x = col / 2,
    .y = row / 2,
    .vx = 1,
    .vy = 1,
  };

  paddle_t left_paddle = {
    .len = 7,
    .x = 1,
  };
  left_paddle.y = row / 2 - left_paddle.len / 2;

  paddle_t right_paddle = {
    .len = 7,
    .x = col - 1,
  };
  right_paddle.y = row / 2 - right_paddle.len / 2;

  score_t score = {0};

  // game loop
  int run = 1;
  int keypress = 0;
  while (run) {
    clear();

    draw_ball(&ball);
    draw_paddle(&left_paddle);
    draw_paddle(&right_paddle);
    draw_score(col, &score);
    move_ball(row, col, &ball, &left_paddle, &right_paddle, &score);

    // get input
    keypress = getch();
    switch (keypress) {
      case 'q':
        run = 0;
        break;
      case 'w': // left paddle up
        if (left_paddle.y > 0) {
          left_paddle.y--;
        }
        break;
      case 's': // left paddle down
        if (left_paddle.y + left_paddle.len < row) {
          left_paddle.y++;
        }
        break;
      case 259: // right paddle up
        if (right_paddle.y > 0) {
          right_paddle.y--;
        }
        break;
      case 258: // right paddle down
        if (right_paddle.y + right_paddle.len < row) {
          right_paddle.y++;
        }
        break;
    }

    refresh();
  }

  endwin();

  return 0;
}
