#define BUTTON_PIN 4
#define BTN_DEB 50

bool is_open = false;

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    static uint32_t tmr = 0;
    static bool pState = false;
    static bool stableState = false;

    bool current_state = !digitalRead(BUTTON_PIN);

    if (millis() - tmr >= BTN_DEB) {
        tmr = millis();

        if (current_state != stableState) {  // состояние устоялось после дребезга
            stableState = current_state;

            if (stableState && !pState) {  // только передний фронт (нажатие)
                is_open = !is_open;
                Serial.print("is_open: ");
                Serial.println(is_open);
            }
            pState = stableState;
        }
    }

    digitalWrite(LED_BUILTIN, is_open);
}