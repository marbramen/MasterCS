function [Pa,Pn,Q1,Q2] = FindBaseOrthonormalAndComplement(A)
%este codigo sirve para hallar 
% Q1 base ortonormal de A 
% Q2 base ortormal del complemento de A
% Pa la proyeccioón ortognonal de la imagen de A (Biswa lo llama rango(A))
% Pn la proyección ortogonal del espacio nulo de A

[m,n] = size(A);
if m >= n && rank(A) ~= n
    fprintf('la matriz no es full rank');
    return;
end
[Q,~] = QRHouseholderMatrix(A);
r = rank(A);
Q1 = Q(:,1:r);
Q2 = Q(:,r+1:m);
Pa = Q1*Q1';
Pn = Q2*Q2';
end