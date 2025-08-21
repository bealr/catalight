#ifndef __WIFI_H
#define __WIFI_H

#include "esp_wifi.h"
#include "lwip/inet.h"
#include "nvs_flash.h"

#define WIFI_SSID      "BEALIGHT"
#define WIFI_PASS      "12345678"
#define MAX_STA_CONN   20

void wifi_init();

#endif