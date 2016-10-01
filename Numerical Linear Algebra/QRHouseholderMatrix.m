function [Q,R] = QRHouseholderMatrix(A)
% Metodo para hallar QR, donde Q es una matriz ortogonal y R es una matriz
% triangular superior.
[m,n] = size(A);
R = A;
Q = eye(m);
 for i = 1:min(m-1,n)
     %% hallamos u y sigma
     x = R(i:m,i);
     maxX = max(abs(x));
     x = x/maxX;
     s = (-1)^(1+(x(1)>=0));
     sigma = s*norm(x);
     u = x + sigma*eye(m-i+1,1);
     sigma = maxX*sigma;
     %% multiplicamos Hx = (I-2*u*u'/(u'*u))*x = (-sign(x1)*norm(x),0,...,0)        
     R(i,i) = -1*sigma;
     R(i+1:m,i) = zeros(m-i,1);    
     %% multiplicamos Haj = (I-2*u*u'/(u'*u))*aj
     beta = 2/(u'*u);
     for j = i+1:n
         alpha = u'*R(i:m,j);
         alpha = alpha * beta;
         R(i:m,j) = R(i:m,j) - alpha*u;
     end
     %% Calculamos Q
     for j = 1:m
         alpha = u'*Q(i:m,j);
         alpha = alpha*beta;
         Q(i:m,j) = Q(i:m,j) - alpha*u;
     end     
 end
Q = Q';
end