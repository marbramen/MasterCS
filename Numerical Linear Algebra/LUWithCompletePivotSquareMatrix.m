function [P,A,Q,L,U] = LUWithCompletePivotSquareMatrix(A);
% Metodo para hallar LU usando pivote parcial donde
% PAQ = LU
% P = P_m * P_{m-1} * ... * P_2 * P_1
% Q = Q_m * P_{m-1} * ... * Q_2 * Q_1
% L = P * inv(M_m * P_m * M_{m-1} * P_{m-1} * ... * M_1 * P_1)
% donde P_i es una matriz de permutación para 2 filas
% donde Q_i es una matriz de permutación para 2 columnas
% donde M_i es una matriz de operaciones elementales
% en cada iteración se busca en el mirrork el elemento mayor
% este codigo funciona para matriz cuadradas

[~,n] = size(A);
PBuf = [];
MBuf = [];
QBuf = [];
%Generamos el U iterativamente
AIter = A;
for k = 1 : n-1
    % buscamos la rk fila para el intercambio,
    % generamos la Pk matriz y la matriz Qk
    [filMax rowId] = max(abs(AIter(k:n,k:n)),[],1);
    [maxValue, colId] = max(filMax,[],2);
    rowId = rowId(1,colId);
    if maxValue ~= 0
        rowId = rowId + k - 1;
        colId = colId + k - 1;
        Pk = eye(n);    
        temp = Pk(rowId,:);
        Pk(rowId,:) = Pk(k,:);
        Pk(k,:) = temp;
        Qk = eye(n);
        temp = Qk(:,colId);
        Qk(:,colId) = Qk(:,k);
        Qk(:,k) = temp;
        
        %Realizamos el intercambio
        AIter = Pk * AIter * Qk;    
        Pk 
        Qk
        AIter
        % hallamos la MK matriz conformado por los mij
        Mk = eye(n);
        for i = k + 1 : n
            Mk(i,k) = -1 * AIter(i,k) / AIter(k,k);
        end
        
        AIter = Mk * AIter;
        Mk
        AIter
        %guardamos los Mk y Pk
        PBuf = [Pk PBuf];
        QBuf = [QBuf Qk];
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
 
%Hallamos Q
Q = eye(n);
for i = 1 : colN/n
    QTemp = QBuf(:,((i-1)*n)+1:i*n);
    Q = Q * QTemp;
end 

%Hallamos L
MPInv = eye(n);
for i = 1 : colN/n
    PTemp = PBuf(:,((i-1)*n)+1:i*n);
    MTemp = MBuf(:,((i-1)*n)+1:i*n);
    MPInv = MPInv * MTemp * PTemp;
end
L = P * inv(MPInv);