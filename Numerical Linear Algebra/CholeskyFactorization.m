function [H] = CholeskyFactorization(A)
% Esta factorización funciona si A es simetrica y definida positiva
% A = H*H', donde H es una matriz triangular inferior.
eigV = eig(A);
[m,n] = size(A);
if  m~=n && ~(all(eigV) > 0 && isreal(A)) 
    fprintf('La matriz no es positiva definida');
    return ;
end
H = zeros(n);
H(1,1) = sqrt(A(1,1));
for i = 2:n
    for j = 1:i-1
        sumij = 0;
        if j > 1
            sumij = dot(H(i,1:j-1),H(j,1:j-1));            
        end            
        H(i,j) = (A(i,j) - sumij)/H(j,j);        
    end
    H(i,i) = sqrt(A(i,i) - dot(H(i,1:i-1),H(i,1:i-1)));                
end
end
