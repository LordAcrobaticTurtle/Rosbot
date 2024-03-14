%% SISO IP model and control
disp("Wacks")

M = 0.045; % kg
m = 0.25; % kg
b = 0.5; % 
I = 0.005633333333333334;
g = 9.8;
l = 0.13;
q = (M+m)*(I+m*l^2)-(m*l)^2;
s = tf('s');
P_pend = (m*l*s/q)/(s^3 + (b*(I + m*l^2))*s^2/q - ((M + m)*m*g*l)*s/q - b*m*g*l/q);

Kp = 100;
Ki = 1;
Kd = 20;
C = pid(Kp,Ki,Kd);
T = feedback(P_pend,C);

t=0:0.01:10;
impulse(T,t)
title({'Response of angular Position to an Impulse Disturbance';'under PID Control: Kp = 1, Ki = 1, Kd = 1'});