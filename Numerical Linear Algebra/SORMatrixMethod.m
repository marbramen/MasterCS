function [x, car] = SORMatrixMethod(A, xk, b, w, e, nor)
% Codigo para el metodo SOR (Sucessive Over Relaxation)
% usando matrices Bsor y dsor
% para resolver sistemas de ecuaciones lineales de nxn
% puede trabajar con la norma 1, 2 e infinito
[~,n] = size(A);
x = zeros(n,1);
car = [1 0]';
D = diag(diag(A));
L = tril(A-D);
U = triu(A-D);
Bsor = inv(D+w*L)*((1-w)*D-w*U);
dsor = w*inv(D+w*L)*b;
while 1
   if car(1) == 10000
       fprintf('Se excedio el numero de iteraciones, probablemente la matriz no converge\n');
       break;
   end   
   x = Bsor*x + dsor;    
   x
   errAc = 2*e;
   if nor == 1
      % norma 1
      errAc = sum(abs(x-xk))/sum(abs(x));
   elseif nor == 2
      % norma 2
      errAc = norm(x-xk)/norm(x);
   elseif nor == 3
      % norma infinito
      errAc = max(abs(x-xk),[],1) / max(abs(x),[],1);        
   end
   car(1) = car(1) + 1;
   % falta mejorar para saber cuando converge
   car(2) = sum(abs(x-xk)) == 0; 
   if errAc < e
       break;
   end
   xk = x;
end
end

