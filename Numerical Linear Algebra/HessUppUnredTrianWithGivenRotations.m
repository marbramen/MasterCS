function [Th] = HessUppUnredTrianWithGivenRotations(He)
% triangulización de una matriz de Hessenberg superior no reducida
% usando matrices de Givens
% cuando se hace se ejectua J(i,j,theta) la posición (j,i) es
% cero, por tanto bastaría volver los elementos de la subdiagonal superior
% iguales a cero
% input: Matrix H
% output: A 

    Th = He;
    [~, n] = size(Th);
    % cada elemento de la subdiagonal
    for p = 2 : n
        % obtener la matriz de Givens para la posición TH(p,p-1)
        x1 = Th(p-1, p-1);
        x2 = Th(p, p-1);
        
        %Algoritmo 5.5.1 Biswa Nath Data: Para hallar c y s
        t = x2 / x1;
        c = 1 / (1 + t * t)^.5;
        s = c * t;
        
        if (abs(x2) >= abs(x1))
            t = x1 / x2;
            s = 1 / (1 + t * t)^.5;
            c = s * t;
        end
        % hacemos J(p-1,p,c,s)*H para volver la posicion (p,p-1) 
        % en H igual a cero
        %Algoritmo 5.5.3 Biswa Nath Data para hallar J(p-1,p,c,s)TH
        for k = 1 : n
            a = Th(p-1, k);
            b = Th(p, k);
            Th(p-1, k) = a * c + b * s;
            Th(p, k) = -a * s + b * c;            
        end
    end
        