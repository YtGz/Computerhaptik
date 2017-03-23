#include <math.h>

const float twoPi = 6.28;
//sample rate -> samples per cycle/period
const byte sampleRate = 50;
byte current = 0;
double result = 0.0;

void setup() {
  
}

void loop() {

  if(current >= sampleRate){
    current = 0;
  }else{
    current++;
  }
  result = sin(current*twoPi/sampleRate);
}


