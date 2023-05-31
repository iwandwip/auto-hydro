#define BUZZER_PIN              12
#define LED_RED                 A7
#define LED_GREEN               A6

void setup() {
        pinMode(LED_RED, OUTPUT);
        pinMode(LED_GREEN, OUTPUT);
        pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(BUZZER_PIN, LOW);
        delay(50);
}
