#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <input/input.h>
#include <wifi/wifi.h> // Wi-Fi module integration

// Global variables
static char ssid[32] = "";  // Stores manually entered SSID
static bool manual_mode = true;  // Tracks if manual mode is enabled

// Callback function for input handling
static void input_callback(InputEvent* input_event, void* context) {
    if(input_event->type == InputTypeShort) {
        switch(input_event->key) {
            case InputKeyUp:
                manual_mode = true;  // Switch to Manual Track
                break;
            case InputKeyDown:
                manual_mode = false; // Switch to Auto Track
                break;
            case InputKeyOk:
                if(manual_mode) {
                    furi_log_print("Manual mode active. Tracking SSID.");
                    track_wifi_source(ssid);  // Start tracking manually entered SSID
                } else {
                    furi_log_print("Auto mode selected.");
                    // Placeholder for auto-mode tracking functionality
                }
                break;
            default:
                break;
        }
    }
}

// Function to render the main menu
static void render_menu(Canvas* canvas) {
    canvas_clear(canvas);
    canvas_draw_str(canvas, 10, 10, "WiFi Tracker Menu:");
    canvas_draw_str(canvas, 10, 30, manual_mode ? "> Manual Track" : "  Manual Track");
    canvas_draw_str(canvas, 10, 50, !manual_mode ? "> Auto Track" : "  Auto Track");
    canvas_draw_str(canvas, 10, 70, "Press OK to start tracking!");
}

// Function to manually enter an SSID
static void enter_ssid() {
    snprintf(ssid, sizeof(ssid), "TestNetwork"); // Example SSID (replace with user input logic)
    furi_log_print("Manual SSID entered: TestNetwork");
}

// Function to scan for a specific SSID and track its signal
static void track_wifi_source(const char* ssid) {
    if(!wifi_is_available()) {
        // Display message if the Wi-Fi module is not connected
        furi_log_error("Wi-Fi module not connected.");
        furi_hal_vibro_on(1000);  // Vibrate for 1 second as an error indication
        furi_delay_ms(1000);
        furi_hal_vibro_off();
        return;
    }

    furi_log_print("Wi-Fi module detected. Starting tracking.");
    furi_hal_led_set(FuriHalLedRed, false);
    furi_hal_vibro_off();

    // Scan loop to track signal strength
    while(true) {
        int signal_strength = wifi_scan_for_ssid(ssid); // Example function to fetch RSSI
        if(signal_strength == -100) {
            // SSID not found
            furi_log_print("SSID not found.");
            furi_hal_vibro_on(500);
            furi_delay_ms(500);
            furi_hal_vibro_off();
            break;
        } else if(signal_strength > -30) {
            // Strong signal (close to the source)
            furi_hal_vibro_on(500);  // Strong vibration
            furi_hal_led_set(FuriHalLedRed, true);
            furi_log_print("Signal strong! Close to the source.");
            furi_delay_ms(500);
            break;
        } else if(signal_strength > -60) {
            // Medium signal (approaching source)
            furi_hal_vibro_on(200);  // Medium vibration
            furi_hal_led_set(FuriHalLedRed, true);
            furi_log_print("Medium signal. Approaching the source.");
            furi_delay_ms(200);
        } else {
            // Weak signal (far from the source)
            furi_hal_vibro_off();
            furi_hal_led_set(FuriHalLedRed, false);
            furi_log_print("Weak signal. Far from the source.");
            furi_delay_ms(1000);
        }
    }
}

// Main application function
int32_t wifi_tracker_app(void* p) {
    // Initialize GUI and input
    Gui* gui = furi_record_open(RECORD_GUI);
    Input* input = furi_record_open(RECORD_INPUT);

    input_set_callback(input, input_callback, NULL);

    // Set up viewport for rendering
    ViewPort* viewport = view_port_alloc();
    view_port_draw_callback_set(viewport, render_menu, NULL);

    gui_add_view_port(gui, viewport, GuiLayerFullscreen);

    // Manually enter SSID (placeholder for actual input)
    enter_ssid();

    while(true) {
        render_menu(gui->canvas);
        furi_delay_ms(100);
    }

    // Cleanup
    view_port_free(viewport);
    furi_record_close(RECORD_GUI);
    furi_record_close(RECORD_INPUT);

    return 0;
}
