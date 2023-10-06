# README

A collection of src files for the rosbot code. 

# Git procedures
- Features belong under a feature/ branch name
- Bug fixes under a bugfix/ branch name
- Development code goes on development branch 
- Code doesn't go into main until it has been unit tested

# TODO
## SOFTWARE
### LOCALISATION
- Kalman filter: Use accel measurements as orientation measurements and rates as partials / predicted 
  - Hopefully will eliminate gyro drift

- Introduce control software to rosbot for actual balancing


## Comms
- How do I notify the command GUI that a particular packet is incoming. 
1. Poll the serial port.
2. When the rx buffer has X number of bytes in it, read a *packet header* number of bytes
3. Analyze packet header, to get timestamp, *datasize*, and *datatype*
4. Read the next *datasize* bytes into a *datatype* struct 
5. Make the *datatype* struct/data available for the rest of the program

6. ~~Use packet writing logic and write a byte string to a file. ~~
   1. It worked!!! I can implement a comms layer to send bytes off through the UART interface
   2. Abstracting it away.
      1. Create a module that receives a byte stream and the maximum size length of the stream. 
      2. Starting at index 0, look for a packet header, then decode the packet appropriately. There's a huge number of edge/failure cases that I need to take into account AND CAN TEST FOR
         1. Assume header is at index 0 for now.
      3. Preferrably should return an array of found packets
7. Read file in Python treat as a byte string and decode appropriately. 


## Comms Questions 
1. When does the OS get notified about the bytes received by a com port?

## Command GUI
- Getting live plotting working per graph. 
- Select between types of things to graph to make control tuning easier. 


### Encoder software
- Need a way to zero encoders when not moving
- There is a lot of noise, might be trying to read encoder while it is being written to.
- Make count more accurate
- Find a way to measure RPMs -> Watch it aand measure a minunte. Record throttle + estimated RPM


### HARDWARE
  - ~~PCB design~~
  - Purchase crimping tool
  - Bluetooth connection to computer. 
    - TEST RX+TX pins then switch if necessary
  - Add battery measurement system to PCB -> 
    - Voltage divider is good solution. Just select right voltage for 3.3V 
  - JST connectors for motors
  - straight XT30 connection for power, no dangling cable
  - POWER SWITCH
  - Pull up resistors on I2C - a maybe
  - Input button. 

- 3D PRINT  
  - Print stepper base board
  - Print new wheels 


  
