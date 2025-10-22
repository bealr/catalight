#include "server.h"

#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include "esp_netif.h"
#include "lwip/sockets.h"
#include "defines.h"

struct server_t *server_init(int port) {

    struct server_t *server = (struct server_t *) malloc(sizeof(struct server_t));
    int i;

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);

    server->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    bind(server->sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

    // Mettre socket en non bloquant
    int flags = fcntl(server->sock, F_GETFL, 0);
    fcntl(server->sock, F_SETFL, flags | O_NONBLOCK);

    return server;
}

void server_send(struct server_message_req_t *server_message_req, int device_id) {

    const char *base_ip = "192.168.1.1";
    struct sockaddr_in dest_addr;

    uint32_t ip_int;
    inet_pton(AF_INET, base_ip, &ip_int);

    ip_int = ntohl(ip_int); // mettre en host byte order

    // Ajouter device_id au dernier octet
    uint8_t octet4 = (ip_int & 0xFF) + device_id;
    ip_int = (ip_int & 0xFFFFFF00) | (octet4 & 0xFF);

    ip_int = htonl(ip_int); // retour en network byte order

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(1234);
    dest_addr.sin_addr.s_addr = ip_int;

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

    sendto(sock, server_message_req, sizeof(struct server_message_req_t), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    
    close(sock);
}

int server_get(struct server_t *server, struct lights_t *lights) {

    char rx_buffer[128];
    struct sockaddr_in source_addr;
    socklen_t socklen = sizeof(source_addr);
    struct server_message_req_t msg_req;

    int len = recvfrom(server->sock, rx_buffer, sizeof(rx_buffer) - 1, 0,
                        (struct sockaddr *)&source_addr, &socklen);

    if (len != sizeof(struct server_message_req_t))
        return 0;

    memcpy(&msg_req, rx_buffer, sizeof(struct server_message_req_t));
    printf("UDP from: %d\n", msg_req.device_id);

    if (msg_req.device_id > 0 && msg_req.device_id < 20) {
        lights->clients_last_seen[msg_req.device_id] = 0;
    }

    lights->rssi[msg_req.device_id] = msg_req.rssi;
    lights->battery_percent[msg_req.device_id] = msg_req.voltage;
        
    int dev_id = msg_req.device_id;
    msg_req.r = lights->light[msg_req.device_id].r;
    msg_req.g = lights->light[msg_req.device_id].g;
    msg_req.b = lights->light[msg_req.device_id].b;
    msg_req.w = lights->light[msg_req.device_id].w;
    msg_req.y = lights->light[msg_req.device_id].y;
    printf("msg to lampion %d : r:%d\n", msg_req.device_id, msg_req.r);
    msg_req.device_id = 0;

    server_send(&msg_req, dev_id);

    return 0;
}
