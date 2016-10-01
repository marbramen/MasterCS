function [lambda,eigv,car] = MethodInversePowerWithDespla(A,sigma,yi,e)
% metodo de la potencia inversa con desplazamiento, para hallar el autovalor 
% domninante y su correspondiente autovector, trabaja de igual manera que 
% el metodo de la potencia inversa, solo que se utiliza un sigma que es un valor 
% aproximado a un autovalor real, buscando en (A-sigma*I)
% funciona solo con matrices cuadradas utiliza la norma euclidiana
% Referencias: 
% 1 Métodos Numéricos: Resumen y ejemplos, Tema 7, Francisco Palacios 2009
% 2 Apuntes Matematica Computacional - MÉTODOS PARA LA DETERMINACIÓN DE
% AUTOVALORES - Sergio Aquice - 2016
[n,m] = size(A);
if m ~= n
    fprintf('No es matriz cuadrada\n');
    return;    
end;  
car = zeros(1,1);
E = 2*e;
% factorización LU con pivoteo parcial
% si usamos la funcion lu de matlab puede ser que L sea una matriz
% trapezoidal, ejemplo A = [0.2 -1 0; -1 -0.8 -1; 0 -1 0.2]
[P,~,L,U] = LUWithPartialPivotSquareMatrix(A-sigma*eye(n));
zi = SolveLU(L,U,P*yi);
alphai = norm(zi,inf);
yi = zi / alphai;
zi = SolveLU(L,U,P*yi);
lambda1 = zi./yi;
car(1) = car(1) + 1;
while E > e
     alphai = norm(zi,inf);     
     yi = zi / alphai;
     zi = SolveLU(L,U,P*yi);
     lambda2 = zi./yi;    
     E = norm(abs(lambda2 - lambda1)./abs(lambda2),inf);
     lambda1 = lambda2;
     car(1) = car(1) + 1;
end
umax = norm(lambda2,inf);
lambda = (1/umax + sigma);
eigv = yi;
end