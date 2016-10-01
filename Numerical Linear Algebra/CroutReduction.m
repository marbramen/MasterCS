function [L,U] = CroutReduction(A)
%la factorizacion funcionar si A=LU sin intercambio de filas
% se basa en que U(i,i)= 1;
% considerando que a(i,j) = sum_{1}^{min(i,j)}by{k} L(i,k)*U(k,j)
% a(i,k) = (sum_{r}^{k-1}by{r} L(i,r)*U(r,k)) + L(i,k) para i=k,...,n
% a(k,j)=(sum_{r}^(k-1)by{r})L(i,r)*U(r,k))+L(k,k)*U(k,j) para
% j=k+1,...,n

[m,n] = size(A);
if m ~= n
    fprintf('la matriz no es cuadrada');
    return;
end
L = zeros(n);
U = eye(n);
for k=1:n
   for i=k:n
       sum1k_1 = 0;
       if k > 1
           sum1k_1 = dot(L(i,1:k-1),U(1:k-1,k)); 
       end
       L(i,k) = A(i,k) - sum1k_1;
   end
   for j=k+1:n
       sum1k_1 = 0;
       if k > 1
           sum1k_1 = dot(L(i,1:k-1),U(1:k-1,k));
       end
       U(k,j) =  (A(k,j) - sum1k_1) / L(k,k);
   end
end
end