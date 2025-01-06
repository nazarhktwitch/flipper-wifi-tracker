#ifndef WIFI_TRACKER_H
#define WIFI_TRACKER_H

#include <furi.h>
#include <furi_hal.h>
#include <stdlib.h>
#include <string.h>
#include "wifi_module.h"  // Подключение Wi-Fi модуля

#define DEFAULT_SSID "TestSSID"

typedef enum {
    WifiTrackerViewMenu,
    WifiTrackerViewKeyboard,
    WifiTrackerViewNetworkList
} WifiTrackerViewId;

typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    Keyboard* keyboard;
    DialogEx* dialog_ex;
    char ssid[32]; // Буфер для SSID
    WifiNetwork networks[5]; // Для хранения найденных сетей
    int network_count; // Количество найденных сетей
    bool manual_mode; // Флаг для ручного режима
} WifiTrackerApp;

WifiTrackerApp* wifi_tracker_app_alloc();
void wifi_tracker_app_free(WifiTrackerApp* app);
int32_t wifi_tracker_app(void* p);

#endif // WIFI_TRACKER_H