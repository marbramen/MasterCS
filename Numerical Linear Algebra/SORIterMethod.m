function [x, car] = SORIterMethod(A, xk, b, w, e, nor)
% Codigo para el metodo iterativo de SOR
% Sucessive Over Relaxation
% para resolver sistemas de ecuaciones lineales de nxn
% puede trabajar con la norma 1, 2 e infinito
[~,n] = size(A);
x = zeros(n,1);
car = [1 0]';
while 1
   if car(1) == 10000
       fprintf('Se excedio el numero de iteraciones, probablemente la matriz no converge\n');
       break;
   end   
   for i = 1:n
       if A(i,i) ~= 0
           sum1i_1 = 0;
           sumi1n= 0;
           if i > 1
               sum1i_1 = dot(A(i,1:i-1),x(1:i-1));
           end
           if i < n
               sumi1n = dot(A(i,i+1:n),xk(i+1:n));
           end
           x(i) = (1-w)*xk(i) + (b(i) - sum1i_1 - sumi1n)*w/A(i,i);
       end        
   end
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

