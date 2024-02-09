syms t A z w_n w_d
x = A*exp(-z*w_n*t)*cos(w_d*t);
simplify(diff(x,t))

