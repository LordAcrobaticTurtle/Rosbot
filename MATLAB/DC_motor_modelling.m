%% DC motor modelling 
% syms J b K R L;
J = 0.0000045;
b = 0.1;
K = 0.35;
R = 12.3;
L = 0.004;
s = tf('s');
P_motor = K/((J*s+b)*(L*s+R)+K^2);

motor_ss = ss(P_motor);

rP_motor = 0.1/(0.5*s+1);

linearSystemAnalyzer(P_motor)
Kp = 100;
kd = 10;
ki = 200;
C = pid(Kp, ki, kd);
sys_cl = feedback(C*P_motor,1);

t = 0:0.01:4;
step(sys_cl,t)
grid
title('Step Response with Proportional Control')
         
A = [-b/J   K/J
    -K/L   -R/L];
B = [0
    1/L];
C = [1   0];
D = 0;
motor_ss = ss(A,B,C,D);

poles = eig(A)

Q = C'*C;
R = 1;
K = lqr(A,B,Q,R)


states = {'dtheta' 'current'};
inputs = {'V'};
outputs = {'dtheta'};

sys_cl = ss(Ac,Bc,Cc,Dc,'statename',states,'inputname',inputs,'outputname',outputs);

t = 0:0.01:5;
r =0.2*ones(size(t));
[y,t,x]=lsim(sys_cl,r,t);
[AX,H1,H2] = plotyy(t,y(:,1),t,y(:,2),'plot');
set(get(AX(1),'Ylabel'),'String','dtheta (m)')
% set(get(AX(2),'Ylabel'),'String','pendulum angle (radians)')
title('Step Response with LQR Control')
