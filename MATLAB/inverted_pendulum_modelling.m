%% Inverted Pendulum modelling
% Taken from Medium article 
% https://medium.com/geekculture/dynamics-modelling-and-simulation-of-self-balancing-robot-in-c-d32a3b835bbf
% 
% M = (295-250) / 1000; % Wegiht of wheels + motors
% m = 250 / 1000;
% l = 0.11; % 
% a = 0.;
% r = 30/1000;
options = odeset('Reltol', 1e-5, 'Abstol', [1e-5 1e-5 1e-5 1e-5]);
[t,x] = ode45(@lqrCloseLoop, [0:0.1:20],[0 0 0 0.1], options);

figure(1) 
plot(t, x(:,3)*180.0/pi);
xlabel('time in s');
ylabel('pendulum angle in deg');
title('pen angle plot')

figure(2) 
plot(t,x(:,1));
title('Cart position')
    


function dx = invertedPendulumLinear(t,x)
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
A = [ 0    1            0          0;
      0 -C_2*K_t/r     m*g/M        0;
      0    0            0          1;
      0 -C_1*K_t/r  (M+m)*g/(M*l)  0;];

b = [ 0;
      C_2;
      0;
      C_1;];

dx = A*x;

end


function dx = openLoopIP(t,x)
% Physical constants
M = (295-250) / 1000; % g -> kg
m = 250 / 1000; % g -> kg
l = 110/100; % mm -> m
r = 30/1000; % mm -> m
g = 9.81; %m/s/s
K_t = 0.3475;
R_a = 12.3; % Ohms

A = [0 1 0 0;
    0 0 m*g/M 0;
    0 0 0 1;
    0 0 (M+m)*g/(M*l) 0;];

dx = A*x;

end

function dx = lqrCloseLoop(t,x)
% Physical constants
M = (295-250) / 1000; % g -> kg
m = 250 / 1000; % g -> kg
l = 110/100; % mm -> m
r = 30/1000; % mm -> m
g = 9.81; %m/s/s
K_t = 0.3475;
R_a = 12.3; % Ohms

A = [0  1   0             0;
    0   0   m*g/M         0;
    0   0   0             1;
    0   0   (M+m)*g/(M*l) 0;];

% B = [ 0;
%       C_2;
%       0;
%       C_1;];
B = [ 0; 1/M; 0; 1/(M*l)];

C = [1 0 0 0;
     0 1 0 0;
     0 0 1 0;
     0 0 0 1;];

D = [0; 0; 0;0];

sys = ss(A,B,C,D);
% x = [x, dx, o, do]

Q = [5000,0,0,0;
     0,0,0,0;
     0,0,100,0;
     0,0,0,0;];
R = 0.1;

[K,S,P] = lqr(sys,Q,R);


Ac = (A-B*K);
Bc = B;
Cc = C;
Dc = D;

dx = Ac*x;


end
