 function [P,Q,R] = QRPermutationGivensMatrix(A)
% donde Q es una base ortogonal, R una matrix triangular superior
% este codigo funciona para matriz nxm no singulares
% se halla A*P=Q*R
[m,n] = size(A);
R = A;
Q = eye(m);
P = eye(n);
for i = 1 : min(n, m-1);
    %% Construye Pi en base a la columna de A que tenga la mayor norma
    Pi = eye(n-i+1);
    pos = 1;
    normMax = -1;
    for j = i:n
        normTemp = norm(R(i:m,j) ,inf);
        if normTemp > normMax
            pos = j-(i-1);
            normMax = normTemp;
        end
    end    
    Pi(:,[pos  1]) = Pi(:,[1 pos]);
    Pi = blkdiag(eye(i-1),Pi);   
    P = P * Pi;
    %% Mismo proceso de QR con Matriz de Givens   
    R = R * Pi;
    for j = i+1 : m
        x = [R(i,i) R(j,i)]';               
        t = x(1) / x(2);
        s = 1 / sqrt(1 + t*t);
        c = s*t;
        if abs(x(1)) >= abs(x(2))
            t = x(2) / x(1);
            c = 1 / sqrt(1 + t * t);
            s = c*t;        
        end  
        % premultiplication by Givens matrix: sum of rows A
        % J(i,j,theta) * A               
        r1 = R(i,:);
        r2 = R(j,:);
        R(i,:) = c * r1 + s * r2;
        R(j,:) = -s * r1 + c * r2;
        
        % postmultiplication by Givens matrix: sum of columns A 
        % A * J(i,j,theta)
        q1 = Q(:,i);
        q2 = Q(:,j);
        Q(:,i) = c * q1 + s * q2;
        Q(:,j) = -s * q1 + c * q2;               
    end
end
end

 