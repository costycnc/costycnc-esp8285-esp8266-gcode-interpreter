
// define the parameters of our machine.
#define X_STEPS_PER_INCH 416.772354
//int X_STEPS_PER_MM=340; //16.4083604
#define X_MOTOR_STEPS    400

#define Y_STEPS_PER_INCH 416.772354
//int Y_STEPS_PER_MM=340;
#define Y_MOTOR_STEPS    400

#define Z_STEPS_PER_INCH 16256.0
#define Z_STEPS_PER_MM   640.0
#define Z_MOTOR_STEPS    400

//our maximum feedrates
//int FAST_XY_FEEDRATE=150;
#define FAST_Z_FEEDRATE  50.0

// Units in curve section
#define CURVE_SECTION_INCHES 0.019685
#define CURVE_SECTION_MM 0.5

// Set to one if sensor outputs inverting (ie: 1 means open, 0 means closed)
// RepRap opto endstops are *not* inverting.
#define SENSORS_INVERTING 0

// How many temperature samples to take.  each sample takes about 100 usecs.
#define TEMPERATURE_SAMPLES 5

/****************************************************************************************
* digital i/o pin assignment
*
* this uses the undocumented feature of Arduino - pins 14-19 correspond to analog 0-5
****************************************************************************************/


// 0 out
// 1 input
//cartesian bot pins
//#define X_STEP_PIN 2 liber
//#define X_DIR_PIN 3 liber
//#define X_MIN_PIN 4 liber
//#define X_MAX_PIN 9 liber
//#define X_ENABLE_PIN 15 liber

//#define Y_STEP_PIN 10 liber
//#define Y_DIR_PIN 7
//#define Y_MIN_PIN 8
//#define Y_MAX_PIN 13
//#define Y_ENABLE_PIN 15

//#define Z_STEP_PIN 19
//#define Z_DIR_PIN 18
//#define Z_MIN_PIN 17
//#define Z_MAX_PIN 16
//#define Z_ENABLE_PIN 15


