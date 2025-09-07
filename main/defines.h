#ifndef __DEFINES_H
#define __DEFINES_H

#include <stdint.h>

struct ligh_t {

    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t w;
    uint8_t y;
};

struct lights_t {
    struct ligh_t light[20];
    int rssi[20];
    unsigned int clients_last_seen[20];
};

#endif