#include "mastermind.h"
#include "lcd_wrapper.h"

int play_again(int play){
    lcd_set_cursor(0, 0);
    lcd_print("Play again?");
    lcd_set_cursor(1, 0);
    while(true){
        if(digitalRead(BTN_1_PIN) == HIGH){
            delay(50);
            if(digitalRead(BTN_1_PIN) == HIGH){
                digitalWrite(LED_RED_1, LOW);
                digitalWrite(LED_RED_2, LOW);
                digitalWrite(LED_RED_3, LOW);
                digitalWrite(LED_RED_4, LOW);
                lcd_print_at(1, 0, "   ");
                lcd_print_at(1, 0, "Yes");
                digitalWrite(LED_BLUE_1, HIGH);
                digitalWrite(LED_BLUE_2, HIGH);
                digitalWrite(LED_BLUE_3, HIGH);
                digitalWrite(LED_BLUE_4, HIGH);
                play = 1;
            }
        } else if(digitalRead(BTN_2_PIN) == HIGH){
            delay(50);
            if(digitalRead(BTN_2_PIN) == HIGH){
                digitalWrite(LED_BLUE_1, LOW);
                digitalWrite(LED_BLUE_2, LOW);
                digitalWrite(LED_BLUE_3, LOW);
                digitalWrite(LED_BLUE_4, LOW);
                lcd_print_at(1, 0, "   ");
                lcd_print_at(1, 0, "No");
                digitalWrite(LED_RED_1, HIGH);
                digitalWrite(LED_RED_2, HIGH);
                digitalWrite(LED_RED_3, HIGH);
                digitalWrite(LED_RED_4, HIGH);
                play = 0;
            }
        } else if(digitalRead(BTN_ENTER_PIN) == HIGH){
            delay(50);
            if(digitalRead(BTN_ENTER_PIN) == HIGH){
                turn_off_leds();
                return play;
            }
        }
    }
}

void idle_mode() {
  while (true) {
    digitalWrite(LED_BLUE_4, HIGH);
    delay(1000);
    digitalWrite(LED_BLUE_3, HIGH);
    delay(1000);
    digitalWrite(LED_BLUE_2, HIGH);
    delay(1000);
    digitalWrite(LED_BLUE_4, LOW);
    digitalWrite(LED_BLUE_3, LOW);
    digitalWrite(LED_BLUE_2, LOW);
    delay(1000);
    if (digitalRead(BTN_ENTER_PIN) == HIGH) {
      delay(50);
      if (digitalRead(BTN_ENTER_PIN) == HIGH) {
        lcd_print_at(0, 0, "Exiting idle mode");
        delay(2000);
        return;
      }
    }
  }
}

void setup() {
  lcd_init();

  pinMode(LED_BLUE_1, OUTPUT);
  pinMode(LED_RED_1, OUTPUT);
  pinMode(LED_BLUE_2, OUTPUT);
  pinMode(LED_RED_2, OUTPUT);
  pinMode(LED_BLUE_3, OUTPUT);
  pinMode(LED_RED_3, OUTPUT);
  pinMode(LED_BLUE_4, OUTPUT);
  pinMode(LED_RED_4, OUTPUT);

  pinMode(BTN_1_PIN, INPUT);
  pinMode(BTN_2_PIN, INPUT);
  pinMode(BTN_3_PIN, INPUT);
  pinMode(BTN_4_PIN, INPUT);
  pinMode(BTN_ENTER_PIN, INPUT);
}

void loop() {
  lcd_set_cursor(0,0);
  lcd_print("Welcome to");
  lcd_set_cursor(1,0);
  lcd_print("MasterMind");
  delay(2000);
  lcd_print_at(0, 0, "Your goal is  to");
  lcd_print_at(1, 0, "guess the code");
  delay(2000);
  lcd_clear();
  int play = 1;
  char* code = generate_code(false, 4);
  play_game(code);
  free(code);
  delay(2000);
  lcd_clear();
  play = play_again(play);
  if(play == 0){
    lcd_clear();
    lcd_print_at(0, 0, "Goodbye!");
    delay(2000);
    lcd_clear();
    turn_off_leds();
    idle_mode();
  }
  delay(500);
  turn_off_leds();
  lcd_clear();
}