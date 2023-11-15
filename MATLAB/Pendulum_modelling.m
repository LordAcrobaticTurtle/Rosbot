% syms I_p dtheta dpsi dx_p dy_p m_p dx_w m_w I_w

% K = 0.5 * I_w * dpsi^2 * 2 + 0.5 * m_p*(dx_p)^2 + 0.5*m_p*(dy_p)^2 + 0.5*m_w*dx_w^2 + 0.5 * I_p * dtheta^2;

m_w = 0.01; % kilogram
m_p = 0.2;
L = 0.093; % mm
g = 9.81;
b = 0.1;

x_init = [0; 0; pi; 0.1];

A = [0 1 0 0;
     g*(m_w+m_p)/(m_w*L) 0 0 b/(m_w * L);
     0    0    0    0;
     -m_p*g/m_w 0 0 -b/m_w];

B = [0; -1/(m_w*L); 0 ; 1/m_w];

C = [1 0 0 0; 0 0 1 0];
D = [0; 0];

states = {'x' 'x_dot' 'phi' 'phi_dot'};
inputs = {'u'};
outputs = {'x'; 'phi'};

sys_ss = ss(A,B,C,D,'statename',states,'inputname',inputs,'outputname',outputs);
impulse(sys_ss);
