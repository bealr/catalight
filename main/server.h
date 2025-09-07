#ifndef __SERVER_H
#define __SERVER_H

#include <stdint.h>
#include "defines.h"

struct server_message_req_t {
    uint8_t device_id;
    int8_t rssi;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t w;
    uint8_t y;
};

struct server_t {
    int sock;
    struct server_message_req_t clients[20];
};

void server_send(struct server_message_req_t *server_message_req, int device_id);
struct server_t *server_init(int port);
int server_get(struct server_t *server, struct lights_t *light);

#endif