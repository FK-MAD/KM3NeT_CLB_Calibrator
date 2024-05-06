//----------------------------------------------------------------------------------
// This code is used to control the stepper motors of the CLB calibration mechanism.
//
// Steps to perform calibration:
// 0) Edit the "CONSTANTS" section as needed
// 1) Upload the code to the Arduino UNO
// 2) Turn on the power supply to the CNC shield
// 3) Set up the CLB with the x-axis pointing north and the z-axis pointing downwards (as per "KM3NeT_CALIB_2017_0043PRO_AHRS_Calibration_v5", page 21)
// 4) Press the pushbutton to energise the steppers and hold the CLB in position
// 5) When ready, press the pushbutton again to start the calibration
// 6) Wait for calibration to end
// 7) When ready, press the pushbutton again to start the calibration check
// 8) Wait for calibration check to end
// 9) When ready, press the pushbutton again to bring the CLB to its original position and untwist the wires
//
// Author: Filippos Katsimalis <fkatsimalis@uth.gr>, <fkatsimalis@gmail.com>
//----------------------------------------------------------------------------------


///INCLUDE THE ACCELSTEPPER LIBRARY///
#include <AccelStepper.h>


///DEFINITION OF STEP & DIRECTION PINS AND STEPPER SETUP///
// ([1],[step pin #],[direction pin #]). [1] is a declaration that a stepper driver (with Step and Direction pins) is used
AccelStepper outer(1,2,5);  // outer tube control (stepper connected to "X" on CNC shield), pin 2=step, pin 5=direction
AccelStepper middle(1,3,6); // middle tube control (stepper connected to "Y" on CNC shield), pin 3=step, pin 6=direction
AccelStepper inner(1,4,7);  // inner tube control (stepper connected to "Z" on CNC shield), pin 4=step, pin 7=direction


///CONSTANTS///
int microstep=16; // the microstep size. depends on the configuration of jumper pins used on the CNC shield, under the stepper drivers.
                  // for the stepper motor drivers used (A4988) see: https://www.pololu.com/product/1182 (step and microstep size)
                  // possible values for the A4988 are: full step -> 1. half step -> 2. quarter step -> 4. eighth step -> 8. sixteenth step -> 16

float gear_ratio=3.27778; // the gear ratio = [number of teeth of big pulleys (on the lower part of the tubes)]/[number of teeth of stepper pulleys] = 59/18

float calibration_wait_time=1;        // delay between each rotation during calibration in seconds
float calibration_check_wait_time=15; // delay between each rotation during calibration check in seconds

float yaw_time=20;             // time to perform the yaw rotation in seconds
float yaw_acceleration=100;    // acceleration/deceleration rate of yaw rotation in steps/seconds^2
float roll_time=1;             // time to perform the roll rotation in seconds
float roll_acceleration=1000;  // acceleration/deceleration rate of roll rotation in steps/seconds^2
float pitch_time=1;            // time to perform the pitch rotation in seconds
float pitch_acceleration=1000; // acceleration/deceleration rate of pitch rotation in steps/seconds^2


///CALCULATIONS///
int steps_per_rev=200*microstep;         // the number of steps required for a full revolution of a stepper axle (small pulley)
float full_rev=steps_per_rev*gear_ratio; // the number of steps required for a full revolution of a tube (big pulley) 

float yaw_speed=(1/yaw_time)*gear_ratio*steps_per_rev;     // the constant speed required to perform a yaw rotation in the specified time in steps/seconds
float roll_speed=(1/roll_time)*gear_ratio*steps_per_rev;   // the constant speed required to perform a roll rotation in the specified time in steps/seconds
float pitch_speed=(1/pitch_time)*gear_ratio*steps_per_rev; // the constant speed required to perform a pitch rotation in the specified time in steps/seconds


void setup() {

  
  ///PIN DEFINITION AS OUTPUT///
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);


  ///WAIT UNTIL CLB IS READY TO BE CALIBRATED///
  // pin 8=enable. HIGH -> steppers are disabled. LOW -> steppers are enabled
  digitalWrite(8,HIGH); // disable steppers
  while (digitalRead(9) == HIGH) {
  // do nothing. wait until the user levels the CLB and points it towards north. when the pushbutton is pressed the code exits the loop and the steppers are energised.
  }
  digitalWrite(8,LOW); // enable steppers
  while (digitalRead(9) == HIGH) {
  // do nothing. wait until calibration is ready to begin. when the pushbutton is pressed the code exits the loop and calibration starts.
  }
  
  ///SEQUENCE OF ROTATIONS///
  // yaw->rotation around the z-axis, pitch->rotation around the x-axis, roll->rotation around the y-axis
  // axes of the CLB are shown on "KM3NeT_CALIB_2017_0043PRO_AHRS_Calibration_v5", page 5
  // yaw rotations are alternating in direction to not twist the cables
  // full_rev=360 deg, full_rev/4=90 deg, full_rev/8=45 deg


  ///CALIBRATION///
  // roll rotations
  yaw(full_rev);
    roll(full_rev/4);
  yaw(-full_rev);
    roll(full_rev/4);  
  yaw(full_rev);
    roll(-3*full_rev/4); 
  yaw(-full_rev);
    roll(full_rev/4);

  // pitch rotations
    pitch(full_rev/4);
  yaw(full_rev);
    pitch(-full_rev/2);
  yaw(-full_rev);
    pitch(full_rev/4);
  
  // inclined (roll & pitch) rotations 
    roll(full_rev/8);
  yaw(full_rev);
    pitch(full_rev/8);
  yaw(-full_rev);
    roll(-full_rev/8);
  pitch(-full_rev/8);

  // end of calibration

  while (digitalRead(9) == HIGH) {
  // do nothing. wait until calibration check is ready to begin. when the pushbutton is pressed the code exits the loop and calibration check starts.
  }

  ///CALIBRATION CHECK///
  // yaw rotations
  yaw(full_rev/4);
    delay(calibration_check_wait_time*1000);
  yaw(full_rev/4);
    delay(calibration_check_wait_time*1000);  
  yaw(full_rev/4);

  // end of calibration check

  while (digitalRead(9) == HIGH) {
  // do nothing. when the pushbutton is pressed the code exits the loop and the CLB comes to its original position in order to keep the wires twist-free. 
  }

  ///CLB TO ITS ORIGINAL POSITION IN ORDER TO KEEP THE WIRES TWIST-FREE///
  // yaw rotations
  yaw(-3*full_rev/4);

  // end of sequence
 
}


void loop() {} // there is no need for the "loop()" function


///YAW FUNCTION///
// all motors are needed (outer, middle and inner)
void yaw(int x){
  outer.setMaxSpeed(yaw_speed);
  outer.setAcceleration(yaw_acceleration);
  outer.moveTo(x);
  middle.setMaxSpeed(yaw_speed);
  middle.setAcceleration(yaw_acceleration);
  middle.moveTo(-x);
  inner.setMaxSpeed(yaw_speed);
  inner.setAcceleration(yaw_acceleration);
  inner.moveTo(-x);

  while (outer.run() == true) {
    outer.run();
    middle.run();
    inner.run();
  }

  outer.setCurrentPosition(0);
  middle.setCurrentPosition(0);
  inner.setCurrentPosition(0);

  delay(calibration_wait_time*1000);
}


///ROLL FUNCTION///
// middle and inner motors are needed
void roll(int y){
  middle.setMaxSpeed(roll_speed);
  middle.setAcceleration(roll_acceleration);
  middle.moveTo(y);
  inner.setMaxSpeed(roll_speed);
  inner.setAcceleration(roll_acceleration);
  inner.moveTo(-y);

  while (middle.run() == true) {
    middle.run();
    inner.run();
  }

  middle.setCurrentPosition(0);
  inner.setCurrentPosition(0);

  delay(calibration_wait_time*1000);
}


///PITCH FUNCTION///
// only inner motor is needed
void pitch(int z){
  inner.setMaxSpeed(pitch_speed);
  inner.setAcceleration(pitch_acceleration);
  inner.moveTo(z);

  while (inner.run() == true) {
    inner.run();
  }

  inner.setCurrentPosition(0);

  delay(calibration_wait_time*1000);
}
