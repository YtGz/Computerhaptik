#include <Wire.h>


// we use an union to transmit the float value as an array of 4 bytes
union {
    float f;
    unsigned char bytes[4];
} float_and_bytes;

void setup() {
  // put your setup code here, to run once:
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  Serial.begin(115200);           // start serial for output
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int arg) {
  int i;
  for(i = 0; i < 4; i++) {
    float_and_bytes.bytes[i] = Wire.read(); // receive a byte
  }
  //Serial.println(float_and_bytes.f);
  //float_and_bytes.f *= -1; //reverse the received data
  for(i = 0; i < 4; i++) { 
    //Serial.println(float_and_bytes.bytes[i]);
    Wire.write(float_and_bytes.bytes[i]);              // sends one byte
  }
}

void requestEvent(int arg) {
  int i;
  Serial.println(float_and_bytes.f);
  //float_and_bytes.f *= -1; //reverse the received data
  Wire.write(float_and_bytes.bytes, 4);              // sends one byte
}

