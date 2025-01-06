#include "gui_view_dispatcher.h"
#include "wifi_module.h"
#include "keyboard.h"
#include "dialog_ex.h"
#include <furi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>  // Для стандартной проверки assert

#define DEFAULT_SSID "TestSSID"

// Структура для хранения информации о приложении
typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    Keyboard* keyboard;
    DialogEx* dialog_ex;
    char ssid[32]; // SSID для подключения
    WifiNetwork networks[5]; // Для хранения найденных сетей
    int network_count; // Количество найденных сетей
    bool manual_mode; // Флаг для ручного режима
} WifiTrackerApp;

// ========================== Рендеринг ==========================

// Рендеринг главного меню
static void render_menu(Gui* gui) {
    gui_clear(gui);
    gui_draw_str(gui, 10, 10, "Wi-Fi Tracker");
    gui_draw_button(gui, 10, 30, "Manual Track");
    gui_draw_button(gui, 10, 50, "Auto Track");
    gui_draw_button(gui, 10, 70, "About");
}

// ========================== Обработчики ввода ==========================

// Обработчик ввода с клавиатуры
static void menu_input_callback(InputEvent* input_event, void* context) {
    WifiTrackerApp* app = context;
    
    if(input_event->type == InputTypeShort) {
        switch(input_event->key) {
        case InputKeyUp:
            app->manual_mode = !app->manual_mode;  // Переключение режима
            furi_log_info("Mode changed to %s\n", app->manual_mode ? "Manual" : "Auto");  // Логирование
            break;
        case InputKeyOk:
            if(app->manual_mode) {
                // Переход на ввод SSID
                furi_log_info("Manual Track selected\n");  // Логирование
                // Логика для ввода SSID
                keyboard_set_text(app->keyboard, app->ssid);
                // Можно добавить обработчик завершения ввода SSID, если необходимо
            } else {
                // Автоматическое подключение и сканирование
                furi_log_info("Auto Track started\n");  // Логирование
                wifi_init();
                wifi_connect(app->ssid, "password");
                for(int i = 0; i < 10; i++) {
                    int signal_strength = wifi_get_signal_strength();
                    furi_log_info("RSSI: %d\n", signal_strength);  // Логирование сигнала
                }
            }
            break;
        case InputKeyDown:
            furi_log_info("About selected\n");  // Логирование
            break;
        default:
            break;
        }
    }
}

// ========================== Инициализация и очистка ==========================

// Инициализация приложения
static WifiTrackerApp* wifi_tracker_app_alloc() {
    WifiTrackerApp* app = malloc(sizeof(WifiTrackerApp));
    app->gui = gui_alloc();  // Инициализация GUI
    app->view_dispatcher = view_dispatcher_alloc();  // Инициализация диспетчера видов
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui);  // Привязка диспетчера к GUI
    
    app->keyboard = keyboard_alloc();  // Инициализация клавиатуры
    keyboard_set_text(app->keyboard, DEFAULT_SSID);  // Установка текста по умолчанию для SSID
    keyboard_set_header_text(app->keyboard, "Enter SSID");  // Заголовок для клавиатуры

    app->dialog_ex = dialog_ex_alloc();  // Инициализация диалога
    snprintf(app->ssid, sizeof(app->ssid), "%s", DEFAULT_SSID);  // Заполнение SSID по умолчанию
    app->manual_mode = false;  // Изначально выключен ручной режим

    // Сканирование сетей и добавление результата
    app->network_count = wifi_scan_networks(app->networks);  // Сканирование сетей
    furi_log_info("Networks found: %d\n", app->network_count);  // Логирование количества найденных сетей
    view_dispatcher_add_view(app->view_dispatcher, render_menu);  // Добавление вида меню
    view_dispatcher_set_event_callback(app->view_dispatcher, menu_input_callback, app);  // Установка обработчика ввода

    return app;
}

// Освобождение ресурсов
static void wifi_tracker_app_free(WifiTrackerApp* app) {
    assert(app != NULL);  // Проверка на NULL

    furi_log_info("Freeing resources\n");  // Логирование
    free(app->keyboard);  // Освобождение ресурсов клавиатуры
    free(app->dialog_ex);  // Освобождение ресурсов диалога
    free(app->view_dispatcher);  // Освобождение ресурсов диспетчера видов
    free(app->gui);  // Освобождение ресурсов GUI
    free(app);  // Освобождение основной структуры
}

// ========================== Основная функция ==========================

int32_t wifi_tracker_app(void* p) {
    furi_log_info("Starting Wifi Tracker App\n");  // Логирование

    WifiTrackerApp* app = wifi_tracker_app_alloc();  // Инициализация приложения

    view_dispatcher_run(app->view_dispatcher);  // Запуск диспетчера видов для обработки событий

    wifi_tracker_app_free(app);  // Освобождение ресурсов приложения после завершения

    return 0;
}