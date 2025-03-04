#ifndef NETWORKING_H
#define NETWORKING_H

#include <semaphore.h>
#include <stdint.h>

void *server_init(void *arg);
void *client_init(void *arg);

extern sem_t handshake_sem;

typedef enum {
  PACKET_NONE,
  PACKET_HANDSHAKE,
  PACKET_DISCONNECT,
  PACKET_GAME_STATE,
  PACKET_SCORE_UPDATE,
} packet_type_t;

typedef struct {
  uint8_t type;
  uint32_t size;
  uint8_t payload[];
} packet_t ;

typedef struct {
  uint16_t ball_x, ball_y, ball_vx, ball_vy;
  uint16_t paddle1_y, paddle2_y;
} game_state_t ;

typedef struct {
  uint8_t score1, score2;
} score_update_t ;

#endif 
