function [x] = ThomasForTriadiagonalMatrix(ADiaP,ADiaSub,ADiaSup,b)
% Algoritmo de Thomas para resolver Ax=b, donde A es una matriz
% tridiagonal cuadrada, primero se factoriza A = LU
% los elementos de L(matriz triangular inferior unitaria) son ceros a excepción
% diagonal principal y su subDiagonal(LDiaSub), U(matriz triangular
% superior) cuyos elementos son ceros a excepción de la diagonal principal
% y la diagonal superior( ADiaSup), ya que queremos resolver el sistema
% Ax=b, no es necesario almacenar los elementos de L y U en matrices n*n
% Referencia: Numerical Mathematics - Affio Quarteroni 2000, pag 91

[n,~] = size(ADiaP);
LDiaSub = zeros(n-1,1);
UDiaP = ones(n,1);
y = zeros(n,1);
x = zeros(n,1);
UDiaP(1) = ADiaP(1);
for i=2:n
    LDiaSub(i-1) = ADiaSub(i-1) / UDiaP(i-1);
    UDiaP(i) = ADiaP(i) - LDiaSub(i-1)*ADiaSup(i-1);
end

y(1) = b(1);
for i=2:n
    y(i) = b(i) - LDiaSub(i-1)*y(i-1);
end
x(n) = y(n)/UDiaP(n);
for i=n-1:-1:1
    x(i) = (y(i)-ADiaSup(i)*x(i+1))/UDiaP(i);
end

end

