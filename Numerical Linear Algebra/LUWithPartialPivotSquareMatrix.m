function [P,A,L,U] = LUWithPartialPivotSquareMatrix(A);
% Metodo para hallar LU usando pivote parcial donde
% PA = LU
% P = P_m * P_m * ... * P_2 * P_1
% L = P * inv(M_m * P_m * M_{m-1} * P_{m-1} * ... * M_1 * P_1)
% donde P_i es una matriz de permutación para 2 filas
% donde M_i es una matriz de operaciones elementales
% en cada iteración se busca la fila que tenga el mayor elemento en la
% columna, y se hace un intercambio de esta. 
% este codigo funciona para matriz cuadradas

[~,n] = size(A);
PBuf = [];
MBuf = [];
%Generamos el U iterativamente
AIter = A;
for k = 1 : n-1
    % buscamos la rk fila para el intercambio y generamos la Pk matriz  
    Pk = eye(n);    
    [maxValue, rowId] = max(abs(AIter(k:n,k)),[],1);
    if maxValue ~= 0
        rowId = rowId + k - 1;     
        temp = Pk(rowId,:);
        Pk(rowId,:) = Pk(k,:);
        Pk(k,:) = temp;

        %Realizamos el intercambio
        AIter = Pk * AIter;    
        % hallamos la MK matriz conformado por los mij
        Mk = eye(n);
        for i = k + 1 : n
            Mk(i,k) = -1 * AIter(i,k) / AIter(k,k);
        end            
        AIter = Mk * AIter;
        %guardamos los Mk y Pk
        PBuf = [Pk PBuf];
        MBuf = [Mk MBuf];
    end
end
U = AIter;

[~,colN] = size(PBuf);
%Hallamos P 
P = eye(n);
for i = 1 : colN/n
    PTemp = PBuf(:,((i-1)*n)+1:i*n);
    P = P * PTemp;
end
 
%Hallamos L
MPInv = eye(n);
for i = 1 : colN/n
    PTemp = PBuf(:,((i-1)*n)+1:i*n);
    MTemp = MBuf(:,((i-1)*n)+1:i*n);
    MPInv = MPInv * MTemp * PTemp;
end
L = P * inv(MPInv);

