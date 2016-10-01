function [lambda, eigv, car] = MethodInversePower_SAE(A,yi,e)
% metodo de la potencia inversa, para hallar el autovalor domninante y su
% correspondiente autovector, trabaja con el autovalor mas pequeño, cuya
% inversa es mayor que las inversas de los otros autovalores
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
% factorización LU con pivoteo parcial
% si usamos la funcion lu de matlab puede ser que L sea una matriz
% trapezoidal, ejemplo A = [0.2 -1 0; -1 -0.8 -1; 0 -1 0.2]
[P,~,L,U] = LUWithPartialPivotSquareMatrix(A);
zi = SolveLU(L,U,P*yi);
alphai = norm(zi,inf);
yi = zi / alphai;
zi = SolveLU(L,U,P*yi);
lambda1 = zi./yi;
while E > e
    alphai = norm(zi,inf);
    yi = zi / alphai;
    zi = SolveLU(L,U,P*yi);    
    lambda2 = zi./yi;
    E = norm(abs(lambda2 -lambda1)./abs(lambda2),inf);
    lambda1 = lambda2;
    car(1) = car(1) + 1;
end
umax = norm(lambda2,inf);
lambda = 1/umax;
eigv = yi;
end

