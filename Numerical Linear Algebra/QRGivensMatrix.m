 function [Q,R] = QRGivensMatrix(A)
% donde Q es una base ortogonal, R una matrix triangular superior
% este codigo funciona para matrices cuadradas o rectangulares
[m,n] = size(A);
R = A;
Q = eye(m);

for i = 1 : min(n, m-1);
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
        % A * (J(i,j,theta)'
        q1 = Q(:,i);
        q2 = Q(:,j);
        Q(:,i) = c * q1 + s * q2;
        Q(:,j) = -s * q1 + c * q2;               
    end
end
end

 