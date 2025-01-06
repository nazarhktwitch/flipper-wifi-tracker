#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stddef.h>

// Типы клавиш
typedef enum {
    InputKeyUp,
    InputKeyDown,
    InputKeyOk,
    InputKeyCancel,
    InputKeyMax
} InputKey;

// Структура события ввода
typedef struct {
    InputKey key;
    int type;  // Тип события: короткое нажатие, длительное и т.д.
} InputEvent;

// Структура клавиатуры
typedef struct {
    char* text;         // Текущий вводимый текст
    char* header_text;  // Заголовок клавиатуры (например, "Enter SSID")
    size_t text_length; // Длина текущего текста
} Keyboard;

// Функции для работы с клавиатурой

// Функция выделения памяти и инициализации клавиатуры
Keyboard* keyboard_alloc(void);

// Установка текста для клавиатуры
void keyboard_set_text(Keyboard* keyboard, const char* text);

// Получение текущего текста клавиатуры
const char* keyboard_get_text(Keyboard* keyboard);

// Установка заголовка для клавиатуры
void keyboard_set_header_text(Keyboard* keyboard, const char* header_text);

// Очистка ресурсов клавиатуры
void keyboard_free(Keyboard* keyboard);

// Обработчик ввода клавиш
void keyboard_handle_input(Keyboard* keyboard, InputEvent* input_event);

// Прочие вспомогательные функции
void keyboard_update_display(Keyboard* keyboard);

#endif  // KEYBOARD_H