function [L,U] = DoolittleReduction(A)
%la factorizacion funcionar si A=LU sin intercambio de filas
%se basa en que L(i,i)= 1;
% considerando que a(i,j) = sum_{1}^{min(i,j)}by{k} L(i,k)*U(k,j)
% a(k,j) = (sum_{r}^{k-1}by{r} L(k,r)*U(r,j)) + U(k,j) para j=k,...,n
% a(i,k)=(sum_{r}^(k-1)by{r})L(i,r)*U(r,k))+L(i,k)*U(k,k) para
% i=k+1,...,n

[m,n] = size(A);
if m ~= n
    fprintf('la matriz no es cuadrada');
    return;
end
L = eye(n);
U = zeros(n);
for k=1:n
    for j=k:n
        sum1k_1 = 0;
        if k > 1
            sum1k_1 = dot(L(k,1:k-1),U(1:k-1,j));
        end
        U(k,j)  = A(k,j) - sum1k_1;
    end
    for i=k+1:n
        sum1k_1 = 0;
        if k > 1
            sum1k_1 = dot(L(i,1:k-1),U(1:k-1,k));
        end
        L(i,k) = (A(i,k) - sum1k_1) / U(k,k);
    end
end
end