#include <ncurses.h>

typedef struct {
  int x;
  int y;
} ball_t;

typedef struct {
  int x;
  int y;
  int len;
} paddle_t;

typedef struct {
  int left_score;
  int right_score;
} score_t;

void draw_ball(const ball_t* b) {
  mvaddch(b->y, b->x, '0');
}

void draw_paddle(const paddle_t *paddle) {
	for (int i = 0; i < paddle->len; i++) {
		mvaddch(paddle->y + i, paddle->x, '|');
  }
}

void draw_score(int row, int col, const score_t *score) {
	mvprintw(0, col / 2, "%d ? %d", score->left_score, score->right_score);
}

int main() {
  // intialize screen
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);

  // get size of the screen
  int row, col;
  getmaxyx(stdscr,row,col);

  // initialize ball, paddles, and score
  ball_t ball = {0};
  ball.x = col / 2;
  ball.y = row / 2;

  paddle_t left_paddle = {0};
  left_paddle.len = 8;
  left_paddle.x = 1;
  left_paddle.y = row / 2 - left_paddle.len / 2;

  paddle_t right_paddle = {0};
  right_paddle.len = 8;
  right_paddle.x = col - 1;
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
    draw_score(row, col, &score);

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
        break;


    }
    printw("%d", keypress);

    refresh();
  }

  endwin();

  return 0;
}
