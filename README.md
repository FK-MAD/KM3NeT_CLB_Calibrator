# KM3NeT CLB Calibrator
## About the KM3NeT CLB Calibrator
The KM3NeT CLB Calibrator is a mechanism that automatically calibrates the magnetic compass of the central logic boards (CLBs) used in the digital optical modules (DOMs) of the [KM3NeT](https://www.km3net.org) neutrino telescope. I designed and implemented the system during my internship at [N.C.S.R. “DEMOKRITOS” - Institute of Nuclear & Particle Physics](http://www.inp.demokritos.gr) between July and September 2020.

## Motivation
Every DOM of the KM3NeT telescope is submerged near the bottom of the Mediterranean Sea and continually reports its orientation based on data from the magnetic compass located on its CLB. For the magnetic compass to work properly, a calibration routine must be carried out for every CLB before its DOM integration. Conventionally, this is done manually using a gyroscope-like tool, seen below.

<img src="https://github.com/FK-MAD/KM3NeT_CLB_Calibrator/blob/main/Images/manual%20gyroscope.jpg" width="700">

This process is monotonous, tedious and provides inconsistent calibration accuracy. For these reasons, an automated system needed to be developed.

## Design Constraints
The design constraints that the automated system needed to satisfy were the following.
1) No metal parts should exist near the CLB. This was to ensure that no interference occured with the earth's magnetic field during calibration.
2) The mechanism must perform a series of eight full 360° rotations about the vertical axis each one with the CLB placed in a different orientation.
3) Each rotation must be smooth and last approximately 20s to ensure good data acquisition.

## Calibrator design
The mechanism is composed of two sub-assemblies.
1) Sub-assembly A: a gyroscope-like mechanism that rotates the CLB. It is made entirely out of plastic parts to satisfy design constraint #1. Most parts are 3D printed, some are machined in a lathe, while some are standard plastic bolts, nuts and timing belts.
2) Sub-assembly B: a base for the stepper motors and other electronics located ~1 meter away from the CLB. Actuation between the stepper motors and the gyroscope-like mechanism is achieved using 3 long non-metalic timing belts.

Both sub-assemblies are secured on a wooden base with plastic bolts and nuts. The two sub-assemblies are seen below.

<img src="https://github.com/FK-MAD/KM3NeT_CLB_Calibrator/blob/main/Images/CLB%20calibrator.png" width="700">

## Electronics
Motion is achieved using three stepper motors each one controlling a rotational axis. Each motor is connected to a stepper driver on a CNC shield. Arduino UNO is used as the microcontroller and a small fan keeps the electronics cool.

## Assembly Files
The folder [Assembly Files](https://github.com/FK-MAD/KM3NeT_CLB_Calibrator/tree/main/Assembly%20Files) contains the CAD files (SolidWorks) of the automatic calibration system. Download the whole folder and open file "Calibrator Assembly v5.SLDASM".

## Videos
Videos of the calibration system performing the calibration routine with a CLB mounted are shown below.

## Control of Stepper Motors
Stepper motors are controled via [stepperControl.ino](stepperControl.ino). To run it, the [AccelStepper](https://www.airspayce.com/mikem/arduino/AccelStepper/index.html) library must be included.
