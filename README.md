# README

A collection of src files for the rosbot code. 

# Git procedures
- Features belong under a feature/ branch name
- Bug fixes under a bugfix/ branch name
- Development code goes on development branch 
- Code doesn't go into main until it has been unit tested

## TODO
- SOFTWARE
  - A stepper driver code that incorporates feedback from AS5600 for disturbance rejection
    - Set speeds
  - IMU class -> Mostly done with Electronic Cats library
  - Velocity estimation from AS5600
  - Accuracy confirmation with AS5600
  - Localisation estimate
  - Raw angle to degrees


### Encoder software
- Need a way to zero encoders when not moving
- There is a lot of noise, might be trying to read encoder while it is being written to.
- Make count more accurate
- Find a way to measure RPMs -> Watch it aand measure a minunte. Record throttle + estimated RPM


### HARDWARE
  - ~~PCB design~~
  - Purchase crimping tool
  - Bluetooth connection to computer. 
  - Add battery measurement system to PCB -> Voltage divider is good solution. Just select right voltage for 3.3V 
  - JST connectors for motors
  - straight XT30 connection for power, no dangling cable
  - 
  - 

- 3D PRINT  
  - Print stepper base board
  - Print new wheels 


  
