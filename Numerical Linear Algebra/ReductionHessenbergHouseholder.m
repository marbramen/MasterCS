function [He] = ReductionHessenbergHouseholder(A)
% este código sirve para reducir una matriz A a la forma Hessemberg
% usando matrices de Householder
[n,~] = size(A);
He = A;
for i=1:n-2
    %% hallamos u y sigma
    x = He(i+1:n,i);
    maxX = max(abs(x));
    x = x/maxX;
    s = (-1)^(1+(x(1)>=0));
    sigma = s*norm(x);
    u = x + sigma*eye(n-i,1);
    sigma = maxX*sigma;
    %% multiplicamos Hx = (I-2*u*u'/(u'*u))*x =(-sign(x1)*norm(x),0,...,0)
    He(i+1,i) = -1*sigma;
    He(i+2:n,i) = zeros(n-i-1,1);
    %% multiplicamos H*He (Premultiplication Householder)
    beta = 2/(u'*u);
    for j = i+1:n
        alpha = u'*He(i+1:n,j);
        alpha = alpha * beta;
        He(i+1:n,j) = He(i+1:n,j) - alpha*u;
    end
    %% multiplicamos He*H (Postmultiplication Householder)
    for j=1:n
        alpha = beta*He(j,i+1:n)*u;
        He( j,i+1:n) = He(j,i+1:n) - alpha*u';
    end
end
end