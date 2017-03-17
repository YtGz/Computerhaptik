const float twoPi = 6.28;
//sample rate -> samples per cycle/period
const byte sampleRate = 50;
byte current = 0;
double result = 0.0;

void setup() {
  // put your setup code here, to run once:

}

void loop() {

  if(current >=50){
    current = 0;
  }else{
    current++;
  }
  result = sin(current*twoPi/sampleRate);
}


