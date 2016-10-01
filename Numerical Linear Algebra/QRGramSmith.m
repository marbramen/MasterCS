function [Q,R] = QRGramSmith(A);
% Metodo para hallar QR
% donde Q es una base ortogonal y R es una matriz trianguluar superior no
% singular, con los elementos R_{i,i} = 1
% A = [u1, u2, ..., un]
% Q = w1 * w2 * ... * P{n-1} * Pn es una base ortonormal de A
% R_{i,j} = dot(Ui, Wj) 
% este codigo funciona para matriz cuadradas y/o rectangulares

[~,n] = size(A);
Q = [];
R = eye(n);

%Aplicamos el proceso de GranSmith para hallar una base ortogonal
for k = 1:n
    if k == 1
        Q = [Q A(:,1)];
    else
        wk = A(:,k);
        for j = 1:k-1
            vj = Q(:,j);
            cj = dot(wk, vj)/dot(vj, vj);
            proyXSobY = cj*vj;
            wk = wk - proyXSobY;
        end
        %if(dot(wk', wk') == 0)
        %    wk = A(:,k);
        %end
        Q = [Q wk];
    end
end

% dividimos cada componente de Q entre su norma
% obteniendo una base ortonormal
for k = 1:n
    normWk = dot(Q(:,k),Q(:,k));
    Q(:,k) = Q(:,k) / sqrt(normWk);
end

% Hallamos R
for j = 1:n
    for i = j:n
        R(j,i) = dot(A(:,i), Q(:,j));        
    end
end