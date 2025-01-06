#ifndef GUI_VIEW_DISPATCHER_H
#define GUI_VIEW_DISPATCHER_H

#include <furi.h>

// Структуры для GUI и View Dispatcher
typedef struct {
    Gui* gui;
    void (*render)(Gui* gui);  // Функция для рендеринга текущего вида
    void (*input_callback)(InputEvent* event, void* context); // Обработчик ввода
    void* context;
} ViewDispatcher;

typedef struct {
    char ssid[32]; // SSID сети
    WifiNetwork networks[5]; // Для хранения найденных сетей
    int network_count; // Количество найденных сетей
} WifiTrackerApp;

Gui* gui_alloc();
void gui_free(Gui* gui);
void gui_clear(Gui* gui);
void gui_draw_str(Gui* gui, int x, int y, const char* text);
void gui_draw_button(Gui* gui, int x, int y, const char* text);

ViewDispatcher* view_dispatcher_alloc();
void view_dispatcher_free(ViewDispatcher* dispatcher);
void view_dispatcher_attach_to_gui(ViewDispatcher* dispatcher, Gui* gui);
void view_dispatcher_add_view(ViewDispatcher* dispatcher, void (*render)(Gui* gui));
void view_dispatcher_set_event_callback(ViewDispatcher* dispatcher, void (*callback)(InputEvent* event, void* context), void* context);
void view_dispatcher_run(ViewDispatcher* dispatcher);

#endif