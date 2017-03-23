#include <Wire.h>

// we use an union to transmit the float value as an array of 4 bytes
union {
    float f;
    unsigned char bytes[4];
} float_and_bytes;

const float twoPi = 6.28;
//sample rate -> samples per cycle/period
const byte sampleRate = 50;
byte current = 0;

void setup() {
  // put your setup code here, to run once:
  Wire.begin(); // join i2c bus (address optional for master)
}

void loop() {
  // put your main code here, to run repeatedly:
    if(current >= sampleRate){
    current = 0;
  }else{
    current++;
  }
  float_and_bytes.f = (float) sin(current*twoPi/sampleRate);
  
  Serial.print("Original sample: ")
  Serial.println(float_and_bytes.f);
  Wire.beginTransmission(8); // transmit to device #8
  int i;
  for(i = 0; i < 4; i++) { 
    Wire.write(float_and_bytes.bytes[i]);              // sends one byte
  }
  Wire.endTransmission();    // stop transmitting
  delay(500);

  Wire.requestFrom(8, 4);    // request 4 bytes from slave device #8
  i = 0;
  while (Wire.available()) { // slave may send less than requested
    float_and_bytes.bytes[i] = Wire.read(); // receive a byte
    i++;
  }
  Serial.print("Reversed sample: ");
  Serial.println(float_and_bytes.f);
  Serial.println();
  delay(500);
}
