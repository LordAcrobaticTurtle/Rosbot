%% DC motor modelling 
J = 0.01;
b = 0.1;
K = 0.01;
R = 1;
L = 0.5;
s = tf('s');
P_motor = K/((J*s+b)*(L*s+R)+K^2);

motor_ss = ss(P_motor);

rP_motor = 0.1/(0.5*s+1);

% linearSystemAnalyzer(P_motor)
Kp = 100;
kd = 10;
ki = 200;
C = pid(Kp, ki, kd);
sys_cl = feedback(C*P_motor,1);

t = 0:0.01:4;
step(sys_cl,t)
grid
title('Step Response with Proportional Control')
         
