function [x,car] = GaussSeidelIterMethod(A, xk, b, e, nor)
% Codigo para el metodo iterativo de GaussSeidel
% por el calculo iterativo de compenentes del vector x
% para resolver sistemas de ecuaciones lineales de nxn
% puede trabajar con la norma 1, 2 e infinito
[~,n] = size(A);
x = zeros(n,1);
car = [1 0]';
fprintf('x :');
for i = 1 :n  fprintf('%5.4f ', x(i)); end
fprintf('\n');
while 1
   if car(1) == 10000
       fprintf('Se excedio el numero de iteraciones, probablemente la matriz no converge\n');
       break;
   end    
   for i = 1:n
       if A(i,i) ~= 0
           sum1i_1 = 0;
           sumin = 0;
           if i > 1
               sum1i_1 = dot(A(i,1:i-1),x(1:i-1));
           end
           if i < n
               sumin = dot(A(i,i+1:n),x(i+1:n));
           end
           x(i) = (b(i) - sum1i_1 - sumin) / A(i,i); 
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
   fprintf('x :');
   for i = 1 :n  fprintf('%5.4f ', x(i)); end
   fprintf('\n');
   xk = x;
end
end

