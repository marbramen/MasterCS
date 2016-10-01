function [x,car] = JacobiMatrixMethod(A, xk, b, e, nor)
% Codigo para el metodo de Jacobi
% este codigo funciona por las multiplicaciones de las matrices 
% Bgs y bgs
% para resolver sistemas de ecuaciones lineales de nxn
% puede trabajar con la norma 1, 2 e infinito
[~,n] = size(A);
x = zeros(n,1);
car = [1 0]';
D = diag(diag(A));
L = tril(A-D);
U = triu(A-D);
Bja = -1*inv(D)*(L+U);
bja = inv(D)*b; 
fprintf('x :');
for i = 1 :n  fprintf('%5.4f ', x(i)); end
fprintf('\n');
while 1
   if car(1) == 10000
       fprintf('Se excedio el numero de iteraciones, probablemente la matriz no converge\n');
       break;
   end    
   x = Bja*xk + bja;     
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

