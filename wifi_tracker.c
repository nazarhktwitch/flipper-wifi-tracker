#include "wifi_tracker.h"

// ============================== GUI (Интегрированный) ==============================
Gui* gui_alloc() {
    Gui* gui = malloc(sizeof(Gui));
    return gui;
}

void gui_free(Gui* gui) {
    free(gui);
}

void gui_clear(Gui* gui) {
    // Очистка экрана
}

void gui_draw(Gui* gui) {
    // Отображение контента на экране (заглушка)
}

// ============================== VIEW DISPATCHER (Интегрированный) ==============================
ViewDispatcher* view_dispatcher_alloc() {
    ViewDispatcher* dispatcher = malloc(sizeof(ViewDispatcher));
    return dispatcher;
}

void view_dispatcher_free(ViewDispatcher* dispatcher) {
    free(dispatcher);
}

void view_dispatcher_attach_to_gui(ViewDispatcher* dispatcher, Gui* gui, int type) {
    // Привязка диспетчера видов к GUI
}

void view_dispatcher_add_view(ViewDispatcher* dispatcher, WifiTrackerViewId view_id, void (*render)(void*, void*)) {
    // Добавление вида в диспетчер
}

void view_dispatcher_switch_to_view(ViewDispatcher* dispatcher, WifiTrackerViewId view_id) {
    // Переключение между видами
}

void view_dispatcher_run(ViewDispatcher* dispatcher) {
    // Запуск диспетчера видов
}

void view_dispatcher_set_event_callback(ViewDispatcher* dispatcher, void (*callback)(void*, void*), void* context) {
    // Установка обработчика событий для диспетчера
}

// ============================== КЛАВИАТУРА (Интегрированная) ==============================
Keyboard* keyboard_alloc() {
    Keyboard* keyboard = malloc(sizeof(Keyboard));
    return keyboard;
}

void keyboard_free(Keyboard* keyboard) {
    free(keyboard);
}

void keyboard_set_callback(Keyboard* keyboard, void (*callback)(void*, void*), void* context) {
    // Установка обработчика для ввода с клавиатуры
}

void keyboard_set_text(Keyboard* keyboard, const char* text) {
    // Установка текста для поля ввода
}

void keyboard_set_header_text(Keyboard* keyboard, const char* text) {
    // Установка текста заголовка для клавиатуры
}

// ============================== ДИАЛОГ (Интегрированный) ==============================
DialogEx* dialog_ex_alloc() {
    DialogEx* dialog_ex = malloc(sizeof(DialogEx));
    return dialog_ex;
}

void dialog_ex_free(DialogEx* dialog_ex) {
    free(dialog_ex);
}

void dialog_ex_show(DialogEx* dialog_ex, const char* message) {
    // Отображение сообщения в диалоге
}

// ============================== ФУНКЦИИ РЕНДЕРИНГА ==============================
static void render_menu(Canvas* canvas, void* context) {
    WifiTrackerApp* app = context;
    canvas_clear(canvas);
    canvas_draw_str(canvas, 10, 10, "Wi-Fi Tracker");
    canvas_draw_str(canvas, 10, 30, app->manual_mode ? "> Manual Track" : "  Manual Track");
    canvas_draw_str(canvas, 10, 50, "  Auto Track");
    canvas_draw_str(canvas, 10, 70, "  About");
}

static void render_networks(Canvas* canvas, void* context) {
    WifiTrackerApp* app = context;
    canvas_clear(canvas);
    for (int i = 0; i < app->network_count; i++) {
        canvas_draw_str(canvas, 10, 30 + (i * 10), app->networks[i].ssid);
    }
}

// Обработчик событий
static void menu_input_callback(InputEvent* input_event, void* context) {
    WifiTrackerApp* app = context;

    if(input_event->type == InputTypeShort) {
        switch(input_event->key) {
        case InputKeyUp:
            app->manual_mode = !app->manual_mode;
            break;
        case InputKeyOk:
            if(app->manual_mode) {
                view_dispatcher_switch_to_view(app->view_dispatcher, WifiTrackerViewKeyboard);
            } else {
                furi_log_info("Auto Track started");
                wifi_init();
                wifi_connect(app->ssid, "password");
                for(int i = 0; i < 10; i++) {
                    int signal_strength = wifi_get_signal_strength();
                    furi_log_info("RSSI: %d", signal_strength);
                }
            }
            break;
        case InputKeyDown:
            furi_log_info("About selected");
            break;
        default:
            break;
        }
    }
}

// Инициализация приложения
WifiTrackerApp* wifi_tracker_app_alloc() {
    WifiTrackerApp* app = malloc(sizeof(WifiTrackerApp));
    app->gui = gui_alloc();
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, 0); // Пример: Полноэкранный тип

    app->keyboard = keyboard_alloc();
    keyboard_set_callback(app->keyboard, NULL, app); // Заменить NULL на фактическую функцию-обработчик
    keyboard_set_text(app->keyboard, DEFAULT_SSID);
    keyboard_set_header_text(app->keyboard, "Enter SSID");

    app->dialog_ex = dialog_ex_alloc();
    snprintf(app->ssid, sizeof(app->ssid), "%s", DEFAULT_SSID);
    app->manual_mode = false;

    app->networks = wifi_scan_networks(&app->network_count);
    view_dispatcher_add_view(app->view_dispatcher, WifiTrackerViewMenu, render_menu);
    view_dispatcher_add_view(app->view_dispatcher, WifiTrackerViewKeyboard, NULL); // Пример: Вид клавиатуры
    view_dispatcher_add_view(app->view_dispatcher, WifiTrackerViewNetworkList, render_networks);
    view_dispatcher_set_event_callback(app->view_dispatcher, menu_input_callback, app);

    return app;
}

// Освобождение ресурсов
void wifi_tracker_app_free(WifiTrackerApp* app) {
    furi_assert(app);
    keyboard_free(app->keyboard);
    dialog_ex_free(app->dialog_ex);
    view_dispatcher_free(app->view_dispatcher);
    gui_free(app->gui);
    free(app);
}

// Основная функция
int32_t wifi_tracker_app(void* p) {
    UNUSED(p);

    WifiTrackerApp* app = wifi_tracker_app_alloc();
    view_dispatcher_switch_to_view(app->view_dispatcher, WifiTrackerViewMenu);
    view_dispatcher_run(app->view_dispatcher);
    wifi_tracker_app_free(app);

    return 0;
}