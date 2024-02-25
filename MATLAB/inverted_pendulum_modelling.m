%% Inverted Pendulum modelling
% Taken from Medium article 
% https://medium.com/geekculture/dynamics-modelling-and-simulation-of-self-balancing-robot-in-c-d32a3b835bbf
syms theta u

M = (295-250) / 1000; % Wegiht of wheels
m = 250 / 1000;
l = 0.11; % 
a = 0.63;
r = 50/1000;
    


% Ax = B
A = zeros(4,4);
A(1,1) = m*(a^2 + l^2/12);
A(1,2) = m*a*r*cos(theta);
A(2,1) = A(1,2);
A(2,2) = (m + 3/2 * M) * r^2;

B = zeroes(2,1);
B(1) = m*a*g*sin(theta)-u;
B(2) = m*a*r*


