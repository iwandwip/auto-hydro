float calibration_value = 21.34 + 4.0;
int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;

void setup() {
        Serial.begin(9600);
        pinMode(A0, INPUT);
}

void loop() {
        for (int i = 0; i < 10; i++) {
                buffer_arr[i] = analogRead(A0);
                delay(30);
        }
        for (int i = 0; i < 9; i++) {
                for (int j = i + 1; j < 10; j++) {
                        if (buffer_arr[i] > buffer_arr[j]) {
                                temp = buffer_arr[i];
                                buffer_arr[i] = buffer_arr[j];
                                buffer_arr[j] = temp;
                        }
                }
        }
        avgval = 0;
        for (int i = 2; i < 8; i++) {
                avgval += buffer_arr[i];
        }
        float volt = (float)avgval * 5.0 / 1024 / 6;
        float ph_act = -5.70 * volt + calibration_value;

        Serial.print("pH Val  :");
        Serial.println(ph_act);
        Serial.print("pH volt :");
        Serial.println(volt);
        Serial.print("Analog  :");
        Serial.println(analogRead(A0));
}

double regress(double x) {
        double terms[] = {
                1.4789234356773051e+000,
                8.4414032409362727e-001,
                -4.3054194758747766e-003
        };

        size_t csz = sizeof terms / sizeof *terms;

        double t = 1;
        double r = 0;
        for (int i = 0; i < csz; i++) {
                r += terms[i] * t;
                t *= x;
        }
        return r;
}
