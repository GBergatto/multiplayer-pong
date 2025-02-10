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
