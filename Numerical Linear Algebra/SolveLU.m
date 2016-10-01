function [x] = SolveLU(L,U,b)
[~,n] = size(L);
y = zeros(n,1);
x = zeros(n,1);
% sustitución de adelante hacia atras
y(1) = b(1)/L(1,1);
for i=2:n
    y(i) = (b(i) - dot(L(i,1:i-1),y(1:i-1)))/L(i,i);
end
% sustitución de atras hacia adelante
x(n) = y(n) / U(n,n);
for i=n-1:-1:1
    x(i) = (y(i) - dot(U(i,i+1:n),x(i+1:n)))/U(i,i);
end    
end