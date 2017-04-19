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
task currentTask = SPRING;

// Kinematics
double xh = 0;         // position of the handle [m]
double xh_prev = 0;
double vh = 0;         //velocity of the handle

// Force output variables
double force = 0;           // force at the handle

double v_raw = 0;
double omega = 0.85; // for IIR filtering, usually between 0.8 and 0.9

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
      static double x_wall = 10.0;
      double kSpring = 0.03;
      if (xh < x_wall) {
        force = 0;
      } else {
        force = -kSpring * abs(x_wall - xh);
      }
      break;
    }

    case HARD_SURFACE:
      //TODO
      break;

    case TEXTURES:
      //TODO
      break;
  }

}

int main() {
  double i;
  for(i = 0; i > -8; i -= 0.1) {
    updatePos(i);
    forceRendering();
    printf("xh: %.2f    f: %.3f\n", xh, force);
  }
  for(i += 0.2; i < 8; i += 0.1) {
    updatePos(i);
    forceRendering();
    printf("xh: %.2f    f: %.3f\n", xh, force);
  }
  for(i -= 0.2; i > 0; i -= 0.1) {
    updatePos(i);
    forceRendering();
    printf("xh: %.2f    f: %.3f\n", xh, force);
  }
}
