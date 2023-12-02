# README

A collection of src files for the rosbot code. 

# Git procedures
- Features belong under a feature/ branch name
- Bug fixes under a bugfix/ branch name
- Development code goes on development branch 
- Code doesn't go into main until it has been unit tested

# TODO
## SOFTWARE

23-10-23
Can now setup testing for each module so I can be certain that the code is working before putting on
the robot

Can start writing localisation, control or comms modules. SHOULD ABSOLUTELY BE USING TDD

ANYTIME a serial connection is made while data is already being streamed, any zero bit in that stream can be 
interpreted as the start bit 
DON'T STREAM DATA UNLESS A CONNECTION IS MADE


### LOCALISATION
- Kalman filter: Use accel measurements as orientation measurements and rates as partials / predicted 
  - Hopefully will eliminate gyro drift

- Introduce control software to rosbot for actual balancing


### Control
- Torque constant -> According to website https://www.aliexpress.com/item/1005004999529855.html?spm=a2g0o.order_list.order_list_main.5.2d5218023v7Bh6
- 12V motor reductino ratio of 100 with 300rpm 
  - Max torque = 1.6kgcm 
  - Stall current = 0.9A
  - Therefore motor constant is approximately 1.78 kgcm/A

### Encoder software
- Need a way to zero encoders when not moving
- There is a lot of noise, might be trying to read encoder while it is being written to.
- Make count more accurate
- Find a way to measure RPMs -> Watch it aand measure a minunte. Record throttle + estimated RPM


### HARDWARE
  - ~~PCB design~~
  - Purchase crimping tool
  - Bluetooth connection to computer/phone 
    - ~~TEST RX+TX pins then switch if necessary~~
  - Add battery measurement system to PCB -> 
    - Voltage divider is good solution. Just select right voltage for 3.3V 
  - ~~JST connectors for motors~~
  - ~~straight XT30 connection for power, no dangling cable~~
  - POWER SWITCH
  - Pull up resistors on I2C - a maybe
  - ~~Input button. ~~

- HC-05 password is 1234 on phone

- 3D PRINT  
  - Print stepper base board
  - Print new wheels 


  
