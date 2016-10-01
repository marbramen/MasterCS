function [AH] = PostMultiplicationHouseholder(u,A)
% Esta funciona obtiene el resultado de A*H, donde H es una matriz de
% Householder expresada por (I -2*u*u'/(u'*u)) y A es m*n
[m,n] = size(A);
AH = zeros(m,n);
beta = 2/(u'*u);
for j = 1:m
    alpha = beta*A(j,:)*u;
    AH(j,:) = A(j,:) - alpha*u';
end
end