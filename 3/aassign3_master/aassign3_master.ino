#include <math.h>
#include <Wire.h>
#include <PID_v1.h>

#define SIZE 5
/*
    DECLARATION
*/
typedef enum {
  POS_FOR = 0,
  POS_POS
} task;
task currentTask = POS_FOR
;

// we use an union to transmit the float value as an array of 4 bytes
union float_b {
    float f;
    unsigned char bytes[4];
};

float_b master_pos;
float_b input_buffer;
float slave_xh = 0;

//PID from library
// Setup vars
float kp = 0.15;
float ki = .00004;
float kd = .0;
double inputPID = .0;
double outputPID = .0;
double setPointPID = .0;
double minLimit = -1.9;
double maxLimit = 1.9;
//Specify the links and initial tuning parameters
PID myPID(&inputPID, &outputPID, &setPointPID, kp, ki, kd, DIRECT);

// Pin
int pwmPin = 5; // PWM output pin for motor
int dirPin = 8; // direction output pin for motor
int sensorPosPin = A2; // input pin for MR sensor

// position tracking

int updatedPos = 0;     // keeps track of the latest updated value of the MR sensor reading
int rawPos = 0;         // current raw reading from MR sensor
int lastRawPos = 0;     // last raw reading from MR sensor
int lastLastRawPos = 0; // last last raw reading from MR sensor
int flipNumber = 0;     // keeps track of the number of flips over the 180deg mark
int tempOffset = 0;
int rawDiff = 0;
int lastRawDiff = 0;
int rawOffset = 0;
int lastRawOffset = 0;
const int flipThresh = 700;  // threshold to determine whether or not a flip over the 180 degree mark occurred
boolean flipped = false;

// Kinematics
double xh = 0;         // position of the handle [cm]
double xh_prev = 0;
double vh = 0;         //velocity of the handle
double v[SIZE];
double lastVh = 0;     //last velocity of the handle
double lastLastVh = 0; //last last velocity of the handle
double rp = 0.004191;   //[m]
double rs = 0.073152;   //[m]
double rh = 0.065659;   //[m]
// Force output variables
double force = 0;           // force at the handle
double sinForce = 0;        // for exercise 4
double springForce = 0;     // for exercise 4
double Tp = 0;              // torque of the motor pulley
double duty = 0;            // duty cylce (between 0 and 255)
unsigned int output = 0;    // output command to the motor

double v_raw = 0;
double omega = 0.85; // for IIR filtering, usually between 0.8 and 0.9
int sampleTime = 100; // 100ms.. dt.. -> 10Hz

int noCollision = 1; // used to mark if paddle is inside colliding object or not (used in ex. 4 [hard surface])
double timeOfImpact; // used in exercise 4


/*
    readPosCount() function
*/
void readPosCount() {
  // Get voltage output by MR sensor
  rawPos = analogRead(sensorPosPin);  //current raw position from MR sensor
  // Calculate differences between subsequent MR sensor readings
  rawDiff = rawPos - lastRawPos;          //difference btwn current raw position and last raw position
  lastRawDiff = rawPos - lastLastRawPos;  //difference btwn current raw position and last last raw position
  rawOffset = abs(rawDiff);
  lastRawOffset = abs(lastRawDiff);

  // Update position record-keeping vairables
  lastLastRawPos = lastRawPos;
  lastRawPos = rawPos;

  // Keep track of flips over 180 degrees
  if ((lastRawOffset > flipThresh) && (!flipped)) { // enter this anytime the last offset is greater than the flip threshold AND it has not just flipped
    if (lastRawDiff > 0) {       // check to see which direction the drive wheel was turning
      flipNumber--;              // cw rotation
    } else {                     // if(rawDiff < 0)
      flipNumber++;              // ccw rotation
    }
    if (rawOffset > flipThresh) { // check to see if the data was good and the most current offset is above the threshold
      updatedPos = rawPos + flipNumber * rawOffset; // update the pos value to account for flips over 180deg using the most current offset
      tempOffset = rawOffset;
    } else {                     // in this case there was a blip in the data and we want to use lastactualOffset instead
      updatedPos = rawPos + flipNumber * lastRawOffset; // update the pos value to account for any flips over 180deg using the LAST offset
      tempOffset = lastRawOffset;
    }
    flipped = true;            // set boolean so that the next time through the loop won't trigger a flip
  } else {                        // anytime no flip has occurred
    updatedPos = rawPos + flipNumber * tempOffset; // need to update pos based on what most recent offset is
    flipped = false;
  }
}

/*
    calPosMeter()
*/
void calPosMeter()
{
  double rh = 60;   //[mm]
  double ts = .0146422 * updatedPos - 1.5587313 - 5.1; // Compute the angle of the sector pulley (ts) in degrees based on updatedPos, 3° Offset
  ts *= -1.;
  xh_prev = xh;
  xh = rh * (ts * 3.14159 / 180); // Compute the position of the handle based on ts
  v_raw = (xh - xh_prev) / .05;

  /************************************************************
  * smoothing (optional)
  *************************************************************/
  // use infinite impulse response filter for smoothing
  //vh = (1 - omega) * v_raw + omega * vh;
  vh = v_raw;
}
/*
    forceRendering()
*/
void forceRendering()
{
  //compute PID - in: measured xh; setPoint: masterPos; out: force
  inputPID = xh;
  setPointPID = (double)slave_xh;
  myPID.Compute();
  force = outputPID;
}


void communicateWithSlave()
{
  // send current pos to slave
  master_pos.f = xh;
  Wire.beginTransmission(8); // transmit to device #8
  int i;
  for(i = 0; i < 4; i++) { 
    Wire.write(master_pos.bytes[i]);              // sends one byte
  }
  Wire.endTransmission(); // stop transmitting

  // request feedback from slave for force feedback
  i = 0;
  Wire.requestFrom(8, 4);    // request 4 bytes from slave device #8
  while( Wire.available() ) { // slave may send less than requested
    input_buffer.bytes[i] = Wire.read(); // receive a byte
    i++;
  }
  
  switch (currentTask) {

    case POS_FOR: {
      force = (double) input_buffer.f;    // output the reversed force directly
      break;
    }

    case POS_POS: {   // render/calculate the force from the delta xh: error = slave_xh - master_xh
      slave_xh = (double) input_buffer.f;
      forceRendering();
      break;
    }
  }
}


/*
      Output to motor
*/
void motorControl()
{

  Tp = rp / rs * rh * force;  // Compute the require motor pulley torque (Tp) to generate that force
  // Determine correct direction for motor torque
  if (force < 0) {
    digitalWrite(dirPin, HIGH);
  } else {
    digitalWrite(dirPin, LOW);
  }

  // Compute the duty cycle required to generate Tp (torque at the motor pulley)
  duty = sqrt(abs(Tp) / 0.03);

  // Make sure the duty cycle is between 0 and 100%
  if (duty > 1) {
    duty = 1;
  } else if (duty < 0) {
    duty = 0;
  }
  output = (int)(duty * 255);  // convert duty cycle to output signal
  analogWrite(pwmPin, output); // output the signal
}
/*
   setPwmFrequency
*/
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if (pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if (pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if (pin == 3 || pin == 11) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

/*
      Setup function - this function run once when reset button is pressed.
*/

void setup() {
  // Set up serial communication
  Serial.begin(115200);

  // Set PWM frequency
  setPwmFrequency(pwmPin, 1);
  // Input pins
  pinMode(sensorPosPin, INPUT); // set MR sensor pin to be an input

  // Output pins
  pinMode(pwmPin, OUTPUT);  // PWM pin for motor
  pinMode(dirPin, OUTPUT);  // dir pin for motor

  // Initialize motor
  analogWrite(pwmPin, 0);     // set to not be spinning (0/255)
  digitalWrite(dirPin, LOW);  // set direction

  // Initialize position valiables
  lastLastRawPos = analogRead(sensorPosPin);
  lastRawPos = analogRead(sensorPosPin);

  Wire.begin(); // join i2c bus (address optional for master)

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(100); //10Hz
  myPID.SetOutputLimits(minLimit, maxLimit);
}

/*
    Loop function
*/
void loop() {
  unsigned long start = millis();
  // put your main code here, to run repeatedly
  // read the position in count
  readPosCount();
  // convert position to centimeters
  calPosMeter();
  // establish communication, calculate rendering force
  communicateWithSlave();

  Serial.println(force);

  // output to motor
  motorControl();
  // wait till sample period over, 10Hz
  while (millis() - start < sampleTime) {
  }
}
