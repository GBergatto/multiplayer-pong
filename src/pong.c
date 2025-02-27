#include <ncurses.h>
#include <pthread.h>

#include "../include/game_logic.h"
#include "../include/networking.h"

int show_menu(char** options, int n) {
  int selected = 0;

  mvprintw(1, 5, " _____                  ");
  mvprintw(2, 5, "|  __ \\                 ");
  mvprintw(3, 5, "| |__) |__  _ __   __ _ ");
  mvprintw(4, 5, "|  ___/ _ \\| '_ \\ / _` |");
  mvprintw(5, 5, "| |  | (_) | | | | (_| |");
  mvprintw(6, 5, "|_|   \\___/|_| |_|\\__, |");
  mvprintw(7, 5, "                   __/ |");
  mvprintw(8, 5, "                  |___/ ");

  while (1) {
    for (int i = 0; i < n; i++) {
      if (i == selected) {
        attron(A_REVERSE);
      }
      mvprintw(10 + i, 5, "%s", options[i]);
      attroff(A_REVERSE);
    }
    refresh();

    int key = getch();
    switch (key) {
      case KEY_UP:
      case 'k':
        selected = (selected - 1 + n) % n;
        break;
      case KEY_DOWN:
      case 'j':
        selected = (selected + 1) % n;
        break;
      case '\n':
        return selected;
    }
  }
}

int main(int argc, char *argv[]) {
  int row, col;
  pthread_t network_thread;

  // intialize screen
  initscr();
  curs_set(0);
  keypad(stdscr, TRUE);

  // get size of the screen
  getmaxyx(stdscr,row,col);

  // menu
  char *options[] = { "Host Game", "Join Game", "Exit" };
  int n_options = sizeof(options) / sizeof(options[0]);
  switch (show_menu(options, n_options)) {
    case 0: // spawn server
      pthread_create(&network_thread, NULL, server_init, NULL);
      pthread_detach(network_thread);
      break;
    case 1: // spawn client
      echo();
      char ip[16];
      mvprintw(10, 10, "Enter server IP: ");
      getnstr(ip, 15);
      // TODO: pass IP address to client thread
      pthread_create(&network_thread, NULL, client_init, NULL);
      pthread_detach(network_thread);
      break;
    case 2:
      endwin();
      return 0;
  }

  noecho();
  cbreak();
  timeout(50); // for getch

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
      case KEY_UP: // right paddle up
        if (right_paddle.y > 0) {
          right_paddle.y--;
        }
        break;
      case KEY_DOWN: // right paddle down
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
