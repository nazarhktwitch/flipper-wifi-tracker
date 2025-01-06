#ifndef WIFI_MODULE_H
#define WIFI_MODULE_H

#include <stdint.h>

typedef struct {
    char ssid[32];
    int signal_strength;
} WifiNetwork;

void wifi_init();
void wifi_connect(const char* ssid, const char* password);
WifiNetwork* wifi_scan_networks(int* count);
int wifi_get_signal_strength();

#endif // WIFI_MODULE_H