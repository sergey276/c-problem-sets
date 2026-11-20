#include <stdio.h>           // Библиотека стандартного ввода/вывода
#include <stdlib.h>          // Для malloc, free и других функций работы с памятью
#include <string.h>          // Для работы со строками (например, strdup)
#include <time.h>            // Для работы со временем (не используется здесь)
#include <ctype.h>           // Для проверки типа символов (не используется здесь)
#include <Arduino.h>         // Библиотека Arduino для работы с пинами и функциями микроконтроллера
#include "mastermind.h"      // Заголовочный файл с функциями mastermind
#include "lcd_wrapper.h"     // Заголовок для работы с LCD-дисплеем

// Генерация случайного кода
char* generate_code(bool repeat, int length) {
    if (length < 1) {            // Проверка на минимальную длину
        return NULL;
    }
    if (!repeat && length > 10) { // Если повторы запрещены, максимум 10 разных цифр
        return NULL;
    }

    char* code = (char*)malloc((length + 1) * sizeof(char)); // Выделение памяти под код + нуль-терминатор

    uint32_t seed = 0;              // Инициализация зерна генератора случайных чисел
    for (int i = 0; i < 16; i++) {  // Считывание шума с аналогового пина A0
        seed *= 4;
        seed += analogRead(A0) & 3;
        randomSeed(seed);          // Установка зерна генератора случайных чисел
    }

    int usedDigits[10] = {0};      // Массив для отслеживания использованных цифр
    for (int i = 0; i < length; i++) {
        int digit;
        do {
            digit = random(0, 10); // Генерация случайной цифры от 0 до 9
        } while (!repeat && usedDigits[digit]); // Если повторы запрещены, ищем новую цифру

        code[i] = '0' + digit;     // Преобразуем цифру в символ
        if (!repeat) {
            usedDigits[digit] = 1; // Помечаем цифру как использованную
        }
    }
    code[length] = '\0';           // Завершаем строку
    return code;
}

// Подсчет очков: peg_a — правильная цифра и место, peg_b — правильная цифра, но не то место
void get_score(const char* secret, const char* guess, int* peg_a, int* peg_b) {
    *peg_a = 0;
    *peg_b = 0;

    int secret_used[4] = {0};
    int guess_used[4] = {0};

    for (int i = 0; i < 4; i++) { // Сначала ищем точные совпадения
        if (secret[i] == guess[i]) {
            (*peg_a)++;
            secret_used[i] = 1;
            guess_used[i] = 1;
        }
    }

    for (int i = 0; i < 4; i++) { // Потом ищем совпадения по значению, но не по позиции
        if (!guess_used[i]) {
            for (int j = 0; j < 4; j++) {
                if (!secret_used[j] && guess[i] == secret[j]) {
                    (*peg_b)++;
                    secret_used[j] = 1;
                    break;
                }
            }
        }
    }
}

// Выключение всех светодиодов
void turn_off_leds() {
    int leds[] = {LED_BLUE_1, LED_RED_1, LED_BLUE_2, LED_RED_2, LED_BLUE_3, LED_RED_3, LED_BLUE_4, LED_RED_4};
    for(int i = 0; i < 8; i++) {
        digitalWrite(leds[i], LOW); // Устанавливаем уровень сигнала LOW — выключаем LED
    }
}

// Анимация победы
void leds_win(){
    int leds[] = {LED_BLUE_4, LED_RED_4, LED_BLUE_3, LED_RED_3, LED_BLUE_2, LED_RED_2, LED_BLUE_1, LED_RED_1};
    for(int i = 0; i < 8; i++) {
        digitalWrite(leds[i], HIGH);
        delay(50);
    }
    for(int i = 0; i < 8; i++) {
        digitalWrite(leds[i], LOW);
        delay(50);
    }
    // Зажигаем все красные
    digitalWrite(LED_RED_1, HIGH);
    digitalWrite(LED_RED_2, HIGH);
    digitalWrite(LED_RED_3, HIGH);
    digitalWrite(LED_RED_4, HIGH);
    delay(1000);
    // Зажигаем все синие
    digitalWrite(LED_BLUE_1, HIGH);
    digitalWrite(LED_BLUE_2, HIGH);
    digitalWrite(LED_BLUE_3, HIGH);
    digitalWrite(LED_BLUE_4, HIGH);
    delay(1000);
    turn_off_leds();
    delay(500);
    // Зажигаем все снова
    digitalWrite(LED_RED_1, HIGH);
    digitalWrite(LED_RED_2, HIGH);
    digitalWrite(LED_RED_3, HIGH);
    digitalWrite(LED_RED_4, HIGH);
    digitalWrite(LED_BLUE_1, HIGH);
    digitalWrite(LED_BLUE_2, HIGH);
    digitalWrite(LED_BLUE_3, HIGH);
    digitalWrite(LED_BLUE_4, HIGH);
    delay(1000);
    turn_off_leds();
}

// Анимация поражения
void leds_loose(){
    int led_reds[] = {LED_RED_4, LED_RED_3, LED_RED_2, LED_RED_1};

    digitalWrite(LED_RED_4, HIGH); delay(50);
    digitalWrite(LED_RED_4, LOW);
    digitalWrite(LED_RED_3, HIGH); delay(50);
    digitalWrite(LED_RED_3, LOW);
    digitalWrite(LED_RED_2, HIGH); delay(50);
    digitalWrite(LED_RED_2, LOW);
    digitalWrite(LED_RED_1, HIGH); delay(50);
    digitalWrite(LED_RED_1, LOW);

    // Все по очереди снова
    digitalWrite(LED_RED_4, HIGH); delay(50);
    digitalWrite(LED_RED_3, HIGH); delay(50);
    digitalWrite(LED_RED_2, HIGH); delay(50);
    digitalWrite(LED_RED_1, HIGH); delay(50);

    digitalWrite(LED_RED_4, LOW); delay(50);
    digitalWrite(LED_RED_3, LOW); delay(50);
    digitalWrite(LED_RED_2, LOW); delay(50);
    digitalWrite(LED_RED_1, LOW); delay(50);
}

// Визуализация подсказки через светодиоды
void render_leds(const int peg_a, const int peg_b) {
    turn_off_leds();

    int led_reds[] = {LED_RED_4, LED_RED_3, LED_RED_2, LED_RED_1};
    int led_blues[] = {LED_BLUE_4, LED_BLUE_3, LED_BLUE_2, LED_BLUE_1};

    int i;
    for (i = 0; i < peg_a; i++) {
        digitalWrite(led_reds[i], HIGH); // Красный = точное совпадение
    }

    for (int j = 0; j < peg_b; j++, i++) {
        digitalWrite(led_blues[i], HIGH); // Синий = совпадение по значению
    }
}

// Отображение попытки и подсказки на экране
void render_history(char* secret, char** history, const int entry_nr) {
    if (entry_nr < 0 || history[entry_nr] == NULL) return;

    char* guess = history[entry_nr];
    int peg_a = 0, peg_b = 0;
    get_score(secret, guess, &peg_a, &peg_b); // Считаем очки

    char rendered[17];
    snprintf(rendered, 17, "%d: %s %dA%dB", entry_nr+1, guess, peg_a, peg_b); // Формат строки

    lcd_clear();                 // Очистка экрана
    lcd_print_at(0, 0, rendered); // Вывод строки
}

// Основной игровой цикл
void play_game(char* secret) {
    const int max_attempts = 10;
    char* history[max_attempts];  // История попыток
    for (int i = 0; i < max_attempts; i++) {
        history[i] = NULL;
    }

    char guess[5] = "0000";       // Начальная угадываемая строка
    int attempt = 0;

    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Guess my number."); // Приветствие

    int hidx = attempt; // Индекс истории

    while (attempt < max_attempts) {
        bool guess_made = false;
        bool history_read = false;

        while (!guess_made) {
            lcd_set_cursor(1, 0);
            lcd_print("Guess: ");
            lcd_print(guess);

            // Проверка кнопок — увеличение значений или просмотр истории
            if (digitalRead(BTN_1_PIN) == HIGH && (digitalRead(BTN_2_PIN) == HIGH || digitalRead(BTN_3_PIN) == HIGH)) {
                delay(50);
                if (digitalRead(BTN_1_PIN) == HIGH) {
                    history_read = true;
                    break;
                }
            } else if (digitalRead(BTN_1_PIN) == HIGH) {
                delay(50);
                guess[0] = (guess[0] - '0' + 1) % 10 + '0'; // Увеличение цифры 1
                lcd_set_cursor(1, 7);
                lcd_print(&guess[0]);
                delay(200);
            } else if (digitalRead(BTN_2_PIN) == HIGH) {
                delay(50);
                guess[1] = (guess[1] - '0' + 1) % 10 + '0'; // Цифра 2
                lcd_set_cursor(1, 8);
                lcd_print(&guess[1]);
                delay(200);
            } else if (digitalRead(BTN_3_PIN) == HIGH) {
                delay(50);
                guess[2] = (guess[2] - '0' + 1) % 10 + '0'; // Цифра 3
                lcd_set_cursor(1, 9);
                lcd_print(&guess[2]);
                delay(200);
            } else if (digitalRead(BTN_4_PIN) == HIGH) {
                delay(50);
                guess[3] = (guess[3] - '0' + 1) % 10 + '0'; // Цифра 4
                lcd_set_cursor(1, 10);
                lcd_print(&guess[3]);
                delay(200);
            } else if (digitalRead(BTN_ENTER_PIN) == HIGH) {
                delay(50);
                guess_made = true;
                delay(200);
            }
        }

        if (history_read) {
            render_history(secret, history, hidx);
            int peg_a = 0, peg_b = 0;
            get_score(secret, history[hidx], &peg_a, &peg_b);
            render_leds(peg_a, peg_b);
            while (true) {
                if (digitalRead(BTN_2_PIN) == HIGH) {
                    delay(50);
                    hidx = (hidx - 1 + max_attempts) % max_attempts;
                    render_history(secret, history, hidx);
                    get_score(secret, history[hidx], &peg_a, &peg_b);
                    render_leds(peg_a, peg_b);
                } else if (digitalRead(BTN_3_PIN) == HIGH) {
                    delay(50);
                    hidx = (hidx + 1) % max_attempts;
                    render_history(secret, history, hidx);
                    get_score(secret, history[hidx], &peg_a, &peg_b);
                    render_leds(peg_a, peg_b);
                } else if (digitalRead(BTN_1_PIN) == LOW) {
                    break;
                }
            }
            continue;
        }

        history[attempt] = strdup(guess); // Сохраняем попытку

        int peg_a = 0, peg_b = 0;
        get_score(secret, guess, &peg_a, &peg_b);
        render_leds(peg_a, peg_b);
        render_history(secret, history, attempt);

        if (peg_a == 4) { // Угадали!
            turn_off_leds();
            char attemps[2];
            itoa(attempt, attemps, 10);
            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_print("You win in");
            lcd_print_at(1, 0, attemps);
            lcd_set_cursor(1, 1);
            lcd_print(" guesses!");
            for(int a = 0; a < 2; a++){
                leds_win();
            }
            turn_off_leds();
            break;
        }

        attempt++;

        if (attempt >= max_attempts) { // Проигрыш
            turn_off_leds();
            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_print("Game over!");
            delay(2000);
            lcd_set_cursor(0, 0);
            lcd_print("Guess: ");
            lcd_print(guess);
            lcd_set_cursor(1, 0);
            lcd_print("Secret: ");
            lcd_print(secret);
            for(int a = 0; a < 4; a++){
                leds_loose();
            }
            turn_off_leds();
            break;
        }
    }

    for (int i = 0; i < max_attempts; i++) {
        if (history[i] != NULL) {
            free(history[i]); // Освобождение памяти
        }
    }
}
