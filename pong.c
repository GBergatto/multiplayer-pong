#include <ncurses.h>

typedef struct {
  int x;
  int y;
} ball_t;

void draw_ball(ball_t* b) {
  mvprintw(b->x, b->y, "0");
}

int main() {
  ball_t ball = {0};
  ball.x = 30;
  ball.y = 30;

  initscr();
  noecho();
  curs_set(0);

  int run = 1;
  int keypress = 0;
  while (run) {
    clear();
    draw_ball(&ball);
    // draw paddles
    // draw score

    // get input
    keypress = getch();
    switch (keypress) {
      case 'q':
        run = 0;
        break;

    }
    printw("%d", keypress);

    refresh();
  }

  endwin();

  return 0;
}
