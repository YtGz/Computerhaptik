#include <math.h>

#define SIZE 5
/*
    DECLARATION
*/
typedef enum {
  SPRING = 0,
  COULOMB_FRICTION,
  VISCOUS_FRICTION,
  VIRTUAL_WALL ,
  HARD_SURFACE,
  TEXTURES
} task;
task currentTask = VIRTUAL_WALL;


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
int sampleTime = 50; // 50ms.. dt.. -> 20Hz

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
switch (currentTask) {

    case SPRING: {
      double kSpring = 0.03;
      force = -kSpring * xh;
      break;
    }

    case COULOMB_FRICTION: {
      double bCoulomb = .7; // damping factor
      force = -bCoulomb * ((vh > 0) - (vh < 0));
      break;
    }

    case VISCOUS_FRICTION: {
      double bViscous = 0.03; // damping factor
      force = -bViscous * vh;
      break;
    }

    case VIRTUAL_WALL: {
      static double x_wall = 1;
      double kSpring = 0.3;
      if (xh < x_wall) {
        force = 0;
      } else {
        force = -kSpring * abs(x_wall - xh);
      }
      break;
    }

    case HARD_SURFACE: {
      static double x_wall = 1;
      double kSpring = 0.8;

      // parameters of decaying sinusoid
      double amplitude = 2.4;
      double decayingRate = 2;
      double frequency = 242;

      if (xh < x_wall) {
        force = 0;
        noCollision = 1;
      } else {
        if(noCollision) {
          timeOfImpact = millis();
          noCollision = 0;
        }
        double t = (millis() - timeOfImpact)/1000.0;
        sinForce = amplitude * exp(-decayingRate*t) * sin(2 * M_PI * frequency * t);
        springForce = -kSpring * abs(x_wall - xh);
        force = sinForce + springForce;
      }
      break;
    }

    case TEXTURES: {
      double bViscous = 0.7; // damping factor
      // max damping range width: 2.5mm (ie. continous damping)
      double bWidth = 1.2; //width of damped element
      double uWidth = (15 - bWidth*6) / 5.0; //width of undamped spacing
      if((xh > uWidth/2 && xh <= uWidth/2 + bWidth) ||
         (xh > uWidth/2 + uWidth + bWidth && xh <= uWidth/2 + uWidth + 2*bWidth) ||
         (xh > uWidth/2 + 2*uWidth + 2*bWidth && xh <= uWidth/2 + 2*uWidth + 3*bWidth) ||
         (xh < -uWidth/2 && xh >= -(uWidth/2 + bWidth)) ||
         (xh < -(uWidth/2 + uWidth + bWidth) && xh >= -(uWidth/2 + uWidth + 2*bWidth)) ||
         (xh < -(uWidth/2 + 2*uWidth + 2*bWidth) && xh >= -(uWidth/2 + 2*uWidth + 3*bWidth))
      ) {
        force = -bViscous * vh; // if we are on a damped texture part, we apply viscous friction
      } else {
        force = 0;
      }
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
  // calculate rendering force
  forceRendering();
  // log data
  if(currentTask == COULOMB_FRICTION || currentTask == VISCOUS_FRICTION) {
    Serial.print(xh);
    Serial.print(" ");
    Serial.print(vh, 4);
    Serial.print(" ");
    Serial.println(force, 4);
  }
  else if(currentTask == HARD_SURFACE) {
    Serial.print(xh);
    Serial.print(" ");
    Serial.print(springForce, 4);
    Serial.print(" ");
    Serial.print(sinForce, 4);
    Serial.print(" ");
    Serial.println(force, 4);
  }
  else {
    Serial.print(xh);
    Serial.print(" ");
    Serial.println(force, 4);
  }
  // output to motor
  motorControl();
  // delay before next reading:
  while (millis() - start < sampleTime) {
  }
}
