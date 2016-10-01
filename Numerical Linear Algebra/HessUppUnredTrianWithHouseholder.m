function [Th] = HessUppUnredTrianWithHouseholder(He)
% triangulización de una matriz de Hessenberg superior no reducida
% usando matrices de matriz de Householder
[m, n] = size(He);
if m ~= n
    fprintf('la matriz no es cuadrada');
    return 
end
Th = He;
for i = 1:n-1
    %% calculamos u y sigma en base a x(i:n,i)
    x = Th(i:n,i);
    maxX = max(abs(x));
    x = x/maxX;
    s = (-1)^(1+(x(1)>=0));
    sigma = s*norm(x);
    u = x + sigma*eye(m-i+1,1);    
    sigma = maxX*sigma;   
    %% multiplicamos Hx = (I-2*u*u'/(u'*u))*x = (-sign(x1)*norm(x),0,...,0)        
    Th(i,i) = -1*sigma;
    Th(i+1:n,i) = zeros(n-i,1);
    %% multiplicamos Haj = (I-2*u*u'/(u'*u))*aj
    beta = 2/(u'*u);
    for j = i+1:n        
        alpha = u'*Th(i:n,j);
        alpha = alpha * beta;
        Th(i:n,j) = Th(i:n,j) - alpha*u;
    end        
    Th
end 
end