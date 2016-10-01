function [Th] = HessUppUnredTrianWithGaussElim(He)
% triangulizaci�n de una matriz de Hessenberg superior no reducida
% usando eliminaci�n de Gauss 

Th = He;
[m, n] = size(Th);
if m ~= n
    fprintf('la matriz no es cuadrada');
    return 
end
for i = 1:n-1
   esim = -1*Th(i+1,i)/Th(i,i);
   Th(i+1,:) = esim*Th(i,:)+Th(i+1,:);   
end
end