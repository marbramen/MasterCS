function [L,U] = LUGaussElimination(A)
% Eliminacion de Gauss sin intercambio de filas
% para obtener matriz L y U
% A = E_r*E_{r-1}*...*E_2*E_1*A
[m,n] = size(A);
r = min(m-1,n);
U = A;
L = eye(m);
for i=1:r
    E = eye(m);
    E(i+1:m,i) = -1*U(i+1:m,i)/U(i,i);
    U = E*U;    
    L(i+1:m,i) = -1*E(i+1:m,i);
end
end