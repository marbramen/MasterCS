function [lambda, eigv, car] = MethodPower_SAE(A,yi,e)
% metodo de la potencia, para hallar el autovalor dominante 
% y su correspondiente autovector
% funciona solo con matrices cuadradas utiliza la norma euclidiana
% Referencias: 
% Apuntes Matematica Computacional - MÉTODOS PARA LA DETERMINACIÓN DE
% AUTOVALORES - Sergio Aquice - 2016
[m,n] = size(A);
if m ~= n
    fprintf('No es matriz cuadrada\n');
    return;    
end;    
car = zeros(1,1);
E = 2*e;
zi = A * yi;
alphai = norm(zi,inf);
yi = zi / alphai;
zi = A * yi;
lambda1 = zi./yi;
while E > e
    alphai = norm(zi,inf);
    yi = zi / alphai;
    zi = A * yi;
    lambda2 = zi./yi;
    E = norm(abs(lambda2 - lambda1)./abs(lambda2),inf);
    lambda1 = lambda2;
    car(1) = car(1) + 1;     
end
lambda = norm(lambda2,inf);
eigv = yi;
end

