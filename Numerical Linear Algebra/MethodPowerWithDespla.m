function [lambda,eigv,car] = MethodPowerWithDespla(A,sigma,yi,e)
% metodo de la potencia con desplazamiento, para hallar el autovalor 
% domninante y su correspondiente autovector, trabaja de igual manera que 
% el metodo de la potencia, solo que se utiliza un sigma que es un valor 
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
B = A - sigma*eye(n);
zi = B*yi;
fprintf('z1: ');
for i = 1:m fprintf('%d ', zi(i)); end 
fprintf('\n');    
alpha1 = norm(zi,inf);
fprintf('alpha1: %5.4f\n', alpha1); 
yi = zi/alpha1;
fprintf('y1: ');
for i = 1:m fprintf('%5.4f ', yi(i)); end 
fprintf('\n');    
zi = B*yi;
fprintf('z2: ');
for i = 1:m fprintf('%d ', zi(i)); end 
fprintf('\n');
lambda1 = zi./yi;
fprintf('lambda1: ');
for i = 1:m fprintf('%d ', lambda1(i)); end 
fprintf('\n\n');
car(1) = 1;
while E > e
    alpha1 = norm(zi,inf);
    fprintf('alpha%d: %5.4f\n', car(1)+ 1, alpha1); 
    yi = zi/alpha1;
    fprintf('y%d: ',car(1)+1);
    for i = 1:m fprintf('%5.4f ', yi(i)); end 
    fprintf('\n');
    zi = B*yi;
    fprintf('z%d: ',car(1)+2);
    for i = 1:m fprintf('%d ', zi(i)); end 
    fprintf('\n');
    lambda2 = zi./yi;
    fprintf('lambda%d: ',car(1)+1);
    for i = 1:m fprintf('%d ', lambda2(i)); end 
    fprintf('\n');
    E = norm(abs(lambda2-lambda1)./abs(lambda2),inf);
    fprintf('E = norm(abs(lambda%d-lambda%d)r/(lambda%d)r,inf) = %5.4f ',car(1)+1,car(1),car(1)+1, E);
    fprintf('\n\n');
    lambda1 = lambda2;
    car(1) = car(1) + 1;
end
umax = norm(lambda2,inf);
lambda = umax - sigma;
eigv = yi;
end