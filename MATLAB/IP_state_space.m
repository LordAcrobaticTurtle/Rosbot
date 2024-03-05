%% Inverted pendulum state space model

% Physical constants
M = (295-250) / 1000; % g -> kg
m = 250 / 1000; % g -> kg
l = 110/100; % mm -> m
r = 30/1000; % mm -> m
g = 9.81; %m/s/s
K_t = 0.3475;
R_a = 12.3; % Ohms

% x-dot = Ax 
C_1 = 2*K_t/(M*l*R_a*r);
C_2 = l*C_1;
% A = [ 0    1            0          0;
%       0 -C_2*K_t/r     m*g/M        0;
%       0    0            0          1;
%       0 -C_1*K_t/r  (M+m)*g/(M*l)  0;];

A = [0 1 0 0;
    0 0 m*g/M 0;
    0 0 0 1;
    0 0 (M+m)*g/(M*l) 0;];


% B = [ 0;
%       C_2;
%       0;
%       C_1;];
B = [ 0; 1/M; 0; 1/(M*l)];


C = [1 0 0 0;
     0 0 1 0;];

D = [0; 0];

sys = ss(A,B,C,D);
rank(obsv(sys))
Q = [1,0,0,0;
     0,1,0,0;
     0,0,1,0;
     0,0,0,1;];
R = 1;

[K,S,P] = lqr(sys,Q,R);


Ac = (A-B*K);
Bc = B;
Cc = C;
Dc = D;

states = {"x", "dx", "theta", "dtheta"};
inputs = {"voltage"};
outputs = {"x", "theta"};

sys_cl = ss(Ac,Bc,Cc,Dc, 'statename', states, 'inputname', inputs, 'outputname', outputs);
t = 0:0.01:10;
r = 0.2*ones(size(t));
[y, t, x] = lsim(sys_cl, r, t);
[AX,H1,H2] = plotyy(t, y(:,1), t, y(:,2), 'plot');
set(get(AX(1), 'Ylabel'), 'String', 'Cart position (m)');
set(get(AX(2), 'Ylabel'), 'String', 'Pend angle (rads)');
title("Step Response with LQR control)");


