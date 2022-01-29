#include "HX711.h"
#include <Ewma.h>
#include <Wire.h>

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
long filtered_data;
Ewma adcFilter1(0.5);   // Less smoothing - faster to detect changes, but more prone to noise
HX711 scale; //declare HX711


void setup() {
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  // Start the I2C Bus as Slave on address 9
  Wire.begin(9); 
  Wire.setClock(400000);
   // Attach a function to trigger when something is received.
  Wire.onRequest(requestEvent);
}





    
void requestEvent() {
    byte buf[4] = {};

    buf[0] = filtered_data >> 24; //shift the data bitwise into their correct spot in the byte array
    buf[1] = filtered_data >> 16;
    buf[2] = filtered_data >> 8;
    buf[3] = filtered_data; 
    
    Wire.write(buf, sizeof(buf)); //return a long (4 bytes to robo rio)   
}
void loop() {
  
  if (scale.is_ready()) {
    long raw = scale.read();
    filtered_data = adcFilter1.filter(raw); 
    //Wire.beginTransmission(112);
    //Wire.write(test);
    //Wire.endTransmission();   
    Serial.println(filtered_data);
    //Serial.println(filtered2);

    
}
    delay(500);
}
