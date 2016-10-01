function [Hx, u] = HouseholderPreMultVector(x,r)
% Este codigo calcula la multiplicacion de Hx, donde H es un matriz de
% Householder y x es un vector, tal que Hx tengas ceros 
% de la posición r+1 to n
% Hx =(-sigma,0,0,...,0)

[n,m] = size(x);
if m > 1 || r >= n
    fprintf('x no es vector o r es mayor al tamaño del vector');
    return;
end
Hx = x;
%% Obtenemos u y sigma en base a x(r:n);
x1 = x(r:n);
maxX1 = max(abs(x1));
x1 = x1/maxX1;
sigx1 = (-1)^(1+(x1(1)>=0));
sigma = maxX1*sigx1*norm(x1);
%% sobreescribimos x en base a sigma
Hx(r) = -1*sigma;
Hx(r+1:n) = zeros(n-r,1);
end