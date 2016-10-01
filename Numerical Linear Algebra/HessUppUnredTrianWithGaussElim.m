function [Th] = HessUppUnredTrianWithGaussElim(He)
% triangulización de una matriz de Hessenberg superior no reducida
% usando eliminación de Gauss 

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