#include "gui_view_dispatcher.h"
#include <furi.h>
#include <stdlib.h>
#include <string.h>

// Реализация GUI
Gui* gui_alloc() {
    Gui* gui = malloc(sizeof(Gui));
    return gui;
}

void gui_free(Gui* gui) {
    free(gui);
}

void gui_clear(Gui* gui) {
    furi_hal_display_clear();
}

void gui_draw_str(Gui* gui, int x, int y, const char* text) {
    furi_hal_display_set_cursor(x, y);
    furi_hal_display_write(text);
}

void gui_draw_button(Gui* gui, int x, int y, const char* text) {
    furi_hal_display_set_cursor(x, y);
    furi_hal_display_write(text);
}

// Реализация ViewDispatcher
ViewDispatcher* view_dispatcher_alloc() {
    ViewDispatcher* dispatcher = malloc(sizeof(ViewDispatcher));
    dispatcher->gui = NULL;
    dispatcher->render = NULL;
    dispatcher->input_callback = NULL;
    dispatcher->context = NULL;
    return dispatcher;
}

void view_dispatcher_free(ViewDispatcher* dispatcher) {
    free(dispatcher);
}

void view_dispatcher_attach_to_gui(ViewDispatcher* dispatcher, Gui* gui) {
    dispatcher->gui = gui;
}

void view_dispatcher_add_view(ViewDispatcher* dispatcher, void (*render)(Gui* gui)) {
    dispatcher->render = render;
}

void view_dispatcher_set_event_callback(ViewDispatcher* dispatcher, void (*callback)(InputEvent* event, void* context), void* context) {
    dispatcher->input_callback = callback;
    dispatcher->context = context;
}

void view_dispatcher_run(ViewDispatcher* dispatcher) {
    if (dispatcher->render && dispatcher->gui) {
        dispatcher->render(dispatcher->gui);  // Рендеринг текущего вида
    }

    // Обработка ввода с клавиатуры или других источников событий
    InputEvent input_event = {0};
    if (furi_input_read(&input_event)) {
        if (dispatcher->input_callback) {
            dispatcher->input_callback(&input_event, dispatcher->context);
        }
    }
}