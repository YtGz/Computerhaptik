// program to test implementation of assignment 2 without a haptic paddle


#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
task currentTask = HARD_SURFACE;

// Kinematics
double xh = 0;         // position of the handle [cm]
double xh_prev = 0;
double vh = 0;         //velocity of the handle

// Force output variables
double force = 0;           // force at the handle
double sinForce = 0;        // for exercise 4
double springForce = 0;     // for exercise 4

double v_raw = 0;
double omega = 0.85; // for IIR filtering, usually between 0.8 and 0.9

int noCollision = 1; // used to mark if paddle is inside colliding object or not (used in ex. 4 [hard surface])
double timeOfImpact; // used in exercise 4

void updatePos(double newPos)
{
  xh_prev = xh;
  xh = newPos;
  /*v_raw*/vh = (xh - xh_prev) / .0125; //0.0125 => 80 xh-steps (full simulated handle movement from center to either side) in one second

  /************************************************************
  * smoothing (optional)
  *************************************************************/
  // use infinite impulse response filter for smoothing
  //vh = (1 - omega) * v_raw + omega * vh;
}
/*
    forceRendering()
*/
void forceRendering()
{
switch (currentTask) {

    case SPRING: {
      double kSpring = 1.3;
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
      static double x_wall = 1.0;
      double kSpring = 0.03;
      if (xh < x_wall) {
        force = 0;
      } else {
        force = -kSpring * abs(x_wall - xh);
      }
      break;
    }

    case HARD_SURFACE: {
      static double x_wall = 1.0;
      double kSpring = 0.03;

      // parameters of decaying sinusoid
      double amplitude = 0.2;
      double decayingRate = 2;
      double frequency = 24;

      if (xh < x_wall) {
        force = 0;
        noCollision = 1;
      } else {
        if(noCollision) {
          timeOfImpact = xh < 0? -xh : xh/*millis() //abs(xh) here is just for testing*/;
          noCollision = 0;
        }
        double t = (xh < 0? -xh : xh)/*millis() //abs(xh) here is just for testing*/ - timeOfImpact;
        sinForce = amplitude * exp(-decayingRate*t) * sin(2 * M_PI * frequency * t);
        springForce = -kSpring * abs(x_wall - xh);
        force = sinForce + springForce;
      }
      break;
    }

    case TEXTURES: {
      double bViscous = 0.03; // damping factor
      // max damping range width: 2.5mm (ie. continous damping)
      double bWidth = 0.12; //width of damped element
      double uWidth = (1.5 - bWidth*6) / 5.0; //width of undamped spacing
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

// loop simulates handle movement (assumption: xh is in cm)
int main() {
  double i;
  for(i = 0; i > -8; i -= 0.1) {
    updatePos(i);
    forceRendering();
    //printf("xh: %.2f    f: %.3f\n", xh, force);
    if(currentTask == COULOMB_FRICTION || currentTask == VISCOUS_FRICTION)
      printf("%.2f %.3f %.3f\n", xh, vh, force);
    else if(currentTask == HARD_SURFACE)
      printf("%.2f %.3f %.3f %.3f\n", xh, springForce, sinForce, force);
    else
      printf("%.2f %.3f\n", xh, force);
  }
  for(i += 0.2; i < 8; i += 0.1) {
    updatePos(i);
    forceRendering();
    //printf("xh: %.2f    f: %.3f\n", xh, force);
    if(currentTask == COULOMB_FRICTION || currentTask == VISCOUS_FRICTION)
      printf("%.2f %.3f %.3f\n", xh, vh, force);
    else if(currentTask == HARD_SURFACE)
      printf("%.2f %.3f %.3f %.3f\n", xh, springForce, sinForce, force);
    else
      printf("%.2f %.3f\n", xh, force);
  }
  for(i -= 0.2; i > 0; i -= 0.1) {
    updatePos(i);
    forceRendering();
    //printf("xh: %.2f    f: %.3f\n", xh, force);
    if(currentTask == COULOMB_FRICTION || currentTask == VISCOUS_FRICTION)
      printf("%.2f %.3f %.3f\n", xh, vh, force);
    else if(currentTask == HARD_SURFACE)
      printf("%.2f %.3f %.3f %.3f\n", xh, springForce, sinForce, force);
    else
      printf("%.2f %.3f\n", xh, force);
  }
}
