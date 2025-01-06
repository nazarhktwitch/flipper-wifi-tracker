#include "keyboard.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_TEXT_LENGTH 32  // Максимальная длина текста

// Реализация функции выделения памяти и инициализации клавиатуры
Keyboard* keyboard_alloc(void) {
    Keyboard* keyboard = malloc(sizeof(Keyboard));
    if (keyboard == NULL) {
        return NULL;  // Ошибка выделения памяти
    }
    
    keyboard->text = malloc(MAX_TEXT_LENGTH * sizeof(char));
    if (keyboard->text == NULL) {
        free(keyboard);
        return NULL;  // Ошибка выделения памяти
    }
    
    keyboard->header_text = NULL;  // Заголовок по умолчанию
    keyboard->text[0] = '\0';  // Изначально текст пустой
    keyboard->text_length = 0;
    
    return keyboard;
}

// Установка текста для клавиатуры
void keyboard_set_text(Keyboard* keyboard, const char* text) {
    if (keyboard == NULL || text == NULL) return;
    strncpy(keyboard->text, text, MAX_TEXT_LENGTH - 1);
    keyboard->text[MAX_TEXT_LENGTH - 1] = '\0';  // Обеспечиваем нулевой терминатор
    keyboard->text_length = strlen(keyboard->text);
}

// Получение текущего текста клавиатуры
const char* keyboard_get_text(Keyboard* keyboard) {
    if (keyboard == NULL) return NULL;
    return keyboard->text;
}

// Установка заголовка для клавиатуры
void keyboard_set_header_text(Keyboard* keyboard, const char* header_text) {
    if (keyboard == NULL) return;
    if (header_text != NULL) {
        keyboard->header_text = strdup(header_text);
    } else {
        keyboard->header_text = NULL;
    }
}

// Очистка ресурсов клавиатуры
void keyboard_free(Keyboard* keyboard) {
    if (keyboard == NULL) return;
    free(keyboard->text);  // Освобождение памяти для текста
    if (keyboard->header_text != NULL) {
        free(keyboard->header_text);  // Освобождение памяти для заголовка
    }
    free(keyboard);  // Освобождение памяти для самой структуры
}

// Обработчик ввода клавиш
void keyboard_handle_input(Keyboard* keyboard, InputEvent* input_event) {
    if (keyboard == NULL || input_event == NULL) return;

    switch (input_event->key) {
    case InputKeyUp:
        // Логика для нажатия клавиши "Вверх" (например, переход к предыдущему символу)
        if (keyboard->text_length > 0) {
            keyboard->text[keyboard->text_length - 1] = '\0';
            keyboard->text_length--;
        }
        break;

    case InputKeyDown:
        // Логика для нажатия клавиши "Вниз" (например, добавление символа)
        if (keyboard->text_length < MAX_TEXT_LENGTH - 1) {
            // Предположим, что добавляется символ 'A' для примера
            keyboard->text[keyboard->text_length] = 'A';
            keyboard->text[keyboard->text_length + 1] = '\0';
            keyboard->text_length++;
        }
        break;

    case InputKeyOk:
        // Логика для нажатия "Ок" (например, завершение ввода)
        break;

    case InputKeyCancel:
        // Логика для нажатия "Отмена" (например, отмена ввода)
        break;

    default:
        break;
    }
}

// Прочие вспомогательные функции
void keyboard_update_display(Keyboard* keyboard) {
    if (keyboard == NULL) return;
    
    // Логика обновления дисплея (например, вывод на экран)
    if (keyboard->header_text != NULL) {
        printf("Header: %s\n", keyboard->header_text);  // Заголовок
    }
    printf("Input Text: %s\n", keyboard->text);  // Отображаем текущий текст
}