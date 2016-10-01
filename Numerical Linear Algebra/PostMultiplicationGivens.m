function [AJ] = PostMultiplicationGivens(A,i,j,c,s)
% Esta funciona obtiene el resultado de A*J(i,j,c,s), donde J es una matriz 
% de Givens expresada por y A es m*n
AJ = A;
q1 = A(:,i);
q2 = A(:,j);
AJ(:,i) = c*q1 -s*q2;
AJ(:,j) = s*q1 + c*q2;
end