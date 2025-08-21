#include "wifi.h"
#include <string.h>

void wifi_init() {
    
nvs_flash_init();
esp_netif_init();
esp_event_loop_create_default();

// 1. Crée interface AP
esp_netif_t *ap_netif = esp_netif_create_default_wifi_ap();

// 2. Configure Wi-Fi
wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
esp_wifi_init(&cfg);

wifi_config_t wifi_config = {
    .ap = {
        .ssid = WIFI_SSID,
        .ssid_len = strlen(WIFI_SSID),
        .channel = 1,
        .password = WIFI_PASS,
        .max_connection = MAX_STA_CONN,
        .authmode = WIFI_AUTH_WPA_WPA2_PSK
    },
};
if (strlen(WIFI_PASS) == 0) {
    wifi_config.ap.authmode = WIFI_AUTH_OPEN;
}

esp_wifi_set_mode(WIFI_MODE_AP);
esp_wifi_set_config(WIFI_IF_AP, &wifi_config);

// 3. Démarre Wi-Fi (par défaut en DHCP)
esp_wifi_start();

// 4. Stop DHCP server
esp_netif_dhcps_stop(ap_netif);

// 5. Mets IP fixe
esp_netif_ip_info_t ip_info;
IP4_ADDR(&ip_info.ip, 192,168,1,1);
IP4_ADDR(&ip_info.gw, 192,168,1,1);
IP4_ADDR(&ip_info.netmask, 255,255,255,0);
esp_netif_set_ip_info(ap_netif, &ip_info);
}
