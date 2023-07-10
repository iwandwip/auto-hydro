#include <EEPROM.h>
#include "GravityTDS.h"

#define TdsSensorPin A1
GravityTDS gravityTds;

float temperature = 23.4, tdsValue = 0;

void setup() {
        Serial.begin(9600);
        gravityTds.setPin(TdsSensorPin);
        gravityTds.setAref(5.0);       
        gravityTds.setAdcRange(1024);  
        gravityTds.begin();            
}

void loop() {        
        gravityTds.setTemperature(temperature);  
        gravityTds.update();                     
        tdsValue = gravityTds.getTdsValue();     
        Serial.print(tdsValue, 0);
        Serial.println("ppm");
        delay(1000);
}