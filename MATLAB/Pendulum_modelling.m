% syms I_p dtheta dpsi dx_p dy_p m_p dx_w m_w I_w

% K = 0.5 * I_w * dpsi^2 * 2 + 0.5 * m_p*(dx_p)^2 + 0.5*m_p*(dy_p)^2 + 0.5*m_w*dx_w^2 + 0.5 * I_p * dtheta^2;

% m_w = 0.01; % kilogram
% m_p = 0.2;
% L = 0.093; % mm
% g = 9.81;
% b = 0.1;

syms t
phi = sym('t');
syms x_p y_p x_w y_w dx_w dy_w dx_p dy_p L theta dtheta phi dphi r m_w m_p I_o I_w g



x_w = r*phi;
y_w = r;

dx_w = r*dtheta;
dx_y = 0;

x_p = x_w + L * sin(theta);
y_p = y_w + L * cos(theta);

dx_p = dx_w + dtheta * L * cos(theta);
dy_p = dy_w - dtheta * L * sin(theta);

I_o = m_p * L*L;

K_Tw = m_w * (dx_w)^2;
K_Rw = I_w * (dphi)^2;
K_Tp = 0.5 * m_p * (dx_p ^ 2 + dy_p ^ 2);
K_Rp = 0.5 * I_o * dtheta ^ 2;

P_w = m_w * g * r * 2;
P_p = m_p * g * y_p;

K = K_Tp + K_Rp + K_Rw + K_Tw;
P = P_w + P_p;

L = K - P;
dy_w = 0;









% x_init = [0; 0; pi; 0.1];
% 
% A = [0 1 0 0;
%      g*(m_w+m_p)/(m_w*L) 0 0 b/(m_w * L);
%      0    0    0    0;
%      -m_p*g/m_w 0 0 -b/m_w];
% 
% B = [0; -1/(m_w*L); 0 ; 1/m_w];
% 
% C = [1 0 0 0; 0 0 1 0];
% D = [0; 0];
% 
% states = {'x' 'x_dot' 'phi' 'phi_dot'};
% inputs = {'u'};
% outputs = {'x'; 'phi'};
% 
% sys_ss = ss(A,B,C,D,'statename',states,'inputname',inputs,'outputname',outputs);
% impulse(sys_ss);
