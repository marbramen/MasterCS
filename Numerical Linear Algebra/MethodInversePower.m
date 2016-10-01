function [eigv, car] = MethodInversePower(A,xk,e,sigma)
% metodo de la potencia inversa, iteración inversa
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
eigv = xk;
[P,~,L,U] = LUWithPartialPivotSquareMatrix(A - sigma*eye(n));
while 1    
    eigv = inv(U)*inv(L)*P*eigv;
    eigv = eigv/norm(eigv);    
    if norm((A-sigma*eye(n))*eigv,inf) < e        
        break;
    end    
    if car(1) == 10000
        fprintf('Se excedio el numero de iteraciones, probablemente la matriz no converge\n');
        break;
    end
     if car(1) == 5
         break;
     end
    car(1) = car(1) + 1;
end
end

