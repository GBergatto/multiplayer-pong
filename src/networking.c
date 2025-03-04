#include "../include/networking.h"

#include <netinet/in.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT "7500"
#define MAXBUFLEN 1024

packet_t *create_packet(packet_type_t type, void *data, uint32_t size) {
  packet_t *pkt = malloc(sizeof(packet_t) + size);
  if (!pkt) return NULL;

  pkt->type = type;
  pkt->size = size;
  if (size > 0)
    memcpy(pkt->payload, data, size);

  return pkt;
}

void *server_init(void *arg) {
  int sockfd, rv, numbytes;
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage peer_addr;
  socklen_t addr_len;
  char s[INET6_ADDRSTRLEN];
  uint8_t buffer[MAXBUFLEN];

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE; // use my IP

  if ((rv = getaddrinfo(NULL, SERVER_PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return (void *)EXIT_FAILURE;
  }

  // loop through all addinfo and bind to the first available one
  for (p=servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) <= -1) {
      perror("listener: socket");
      continue;
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) <= -1) {
      close(sockfd);
      perror("listener: bind");
      continue;
    }

    break;
  }

  if (p == NULL) {
    perror("Failed to bind socket\n");
    return (void *)EXIT_FAILURE;
  }

  freeaddrinfo(servinfo);

  printf("listener: waiting to recvfrom...\n");

  if ((numbytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&peer_addr, &addr_len)) == -1) {
    perror("recvfrom");
    return (void *)EXIT_FAILURE;
  }

  packet_t *packet = malloc(numbytes);
  memcpy(packet, buffer, numbytes);

  if (packet->type == PACKET_HANDSHAKE) {
    printf("listener: got packet from %s\n", inet_ntop(peer_addr.ss_family, &(((struct sockaddr_in *)&peer_addr)->sin_addr), s, sizeof s));
    sem_post(&handshake_sem);
  }

  return (void *)EXIT_SUCCESS;
}

void *client_init(void *arg) {
  int sockfd, rv, numbytes;
  struct addrinfo hints, *servinfo, *p;

  char *hostname = (char *)arg;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  if ((rv = getaddrinfo(hostname, SERVER_PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return (void *)EXIT_FAILURE;
  }

  // loop through all addrinfo and open a socket on the first available one
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("talker: socket");
      continue;
    }

    break;
  }

  freeaddrinfo(servinfo);

  if (p == NULL) {
    fprintf(stderr, "talker: failed to create socket\n");
    return (void *)EXIT_FAILURE;
  }

  packet_t *packet = create_packet(PACKET_HANDSHAKE, NULL, 0);
  if (packet == NULL) {
    return (void *)EXIT_FAILURE;
  }

  if ((numbytes = sendto(sockfd, packet, sizeof(packet_t) + packet->size, 0, p->ai_addr, p->ai_addrlen)) == -1) {
    perror("talker: sendto");
    return (void *)EXIT_FAILURE;
  }

  printf("talker: sent %d bytes to %s\n", numbytes, hostname);
  close(sockfd);

  sem_post(&handshake_sem);

  return (void *)EXIT_SUCCESS;
}

