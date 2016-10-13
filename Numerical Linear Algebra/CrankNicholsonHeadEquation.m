function CrankNicholsonHeadEquation(m,n,c)
% Este código resuelve la ecuación de calor (d^2/dx)u(x,t) = c(d/dt)u(x,t)
% utilizando el metodo de Crack-Nicholson, que es un metodo basado en
% diferencias finitas, con los valores limites:
% u(0,t) = u(1,t) = 0
% u(x,0) = sin(pi*x)
% Explicación:
% sabemos que f'(x)=(f(x+h)-f(x))/h {primera derivada} y
% f''(x)=(f(x+h)-2f(x)+f(x-h))/h^2 {segunda derivada}
% podemos expresar la ecuación de calor como 
% (u(x+h,t)-2u(x,t)+u(x-h,t))/h^2 = c(u(x,t)-u(x,t-k))/k
% haciendo que s=h^2/(c*k) y r=2+s
% s*(u(x+h,t)-2u(x,t)+u(x-h,t))= u(x,t)-u(x,t-k)
% u(x,t-k) = -s*u(x+h,t)+2s*u(x,t)+u(x,t)-s*u(x-h,t)
% u(x,t-k) = -s*u(x+h,t)+r*u(x,t)-s*u(x-h,t)
% u_i=u(i*h,t); b_i=s*u(i*h,t-k)
% al final el sistema se reduce a un sistema tridiagonal Ax = b
% considerar que i=1/n ==> for i=1 to n
% Considerar para que el sistema sea estable k<=h^2/2
% referencia:
%  Numerical Mathematics and Computing,Cheney-Kincaid,6edi,sec 15.1
%  differential equations with boundary value problems 8th Dennis Zill

h  = 1/n;
k = 1/m;
s = h^2/((c)*k);
r = 2 + s;
u = zeros(n-1,m);
ureal = zeros(n-1,1);
ADiaP = r*ones(n-1,1);
ADiaSub = -1*ones(n-2,1);
for i=1:n-1
    u(i,1)=sin(pi*i*h);    
    ureal(i,1)=u(i,1);    
end

  fprintf('en t=%d valor real de u: ', 0);
  for i=1:n-1 fprintf('%5.4f ', u(i,1)); end
  fprintf('\n\n');      

for t = 2:m
   [u(:,t)] = ThomasForTriadiagonalMatrix(ADiaP,ADiaSub,ADiaSub,s*u(:,t-1));             
   t2 = k*(t-1);
   for i=1:n-1
       ureal(i,t) = exp(-t2*c*(pi^2))*sin(i*h*pi);
   end  
     fprintf('en t=%d valor apro de u: ', t-1);
     for i=1:n-1 fprintf('%5.4f ', u(i,t)); end
     fprintf('\n');
     fprintf('en t=%d valor real de u: ', t-1);
     for i=1:n-1 fprintf('%5.4f ', ureal(i,t)); end
     fprintf('\n\n');        
end

%Graficando la ecuación de calor
[X,Y] = meshgrid(1:m,1:n+1);
%Agregando los valores limites
u = [zeros(m,1) u' zeros(m,1)]';
figure; hold on;
mesh(X,Y,u);
surf([1 m],[1 n],repmat(-0.2, [2,2]),abs(u),'facecolor','texture');
colormap(jet);

end
