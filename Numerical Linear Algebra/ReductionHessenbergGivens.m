function [He] = ReductionHessenbergGivens(A)
% este código sirve para reducir una matriz A a la forma Hessemberg
% usando matrices de Givens
[n,~] = size(A);
He = A;
for i=1:n-2
    for j=i+2:n
       x = [He(i+1,i) He(j,i)]';
       if x(2) ~= 0             
           %% calculamos la matriz de Givens J(c,s,i+1,j)
           t = x(1) / x(2);
           s = 1 / sqrt(1 + t*t);
           c = s*t;
           if abs(x(1)) >= abs(x(2))
               t = x(2) / x(1);
               c = 1 / sqrt(1 + t * t);
               s = c*t;
           end
           %% premultiplication Givens: J(i+1,j,c,s) * A
           ri = He(i+1,:);
           rj = He(j,:);
           He(i+1,:) = c*ri + s*rj;
           He(j,:) = -s*ri + c*rj;
           %% postmultiplicaton Givens: A * J(i+1,j,c,s)'
           ci = He(:,i+1);
           cj = He(:,j);
           He(:,i+1) = c*ci + s*cj;
           He(:,j) = -s*ci + c*cj;
       end
    end
end
end