function [lambda, eigv, car] = MethodPower_BND(A,xk,e)
% metodo de la potencia inversa, para hallar el autovalor dominante 
% y su correspondiente autovector
% funciona solo con matrices cuadradas utiliza la norma euclidiana
% Referencias: 
% Sec. 9.1 Numerical Analysis- Burden,Faires 9na edición
% Sec. 9.1 Numerical Linear Algebra and Applications,Biswa Data 2da edi
[m,n] = size(A);
if m ~= n
    fprintf('No es matriz cuadrada\n');
    return;    
end;    
car = zeros(2,1);
while 1
    eigv = A*xk;    
    lambda = norm(eigv,inf);
    eigv = eigv/lambda;    
    if norm(A*eigv - lambda*eigv) < e
        eigv = lambda * eigv / norm(lambda * eigv,inf);
        break;
    end    
    if car(1) == 10000
        fprintf('Se excedio el numero de iteraciones, probablemente la matriz no converge\n');
        break;
    end
    car(1) = car(1) + 1;
    xk = eigv;
end
end

