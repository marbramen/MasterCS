for contador in `seq 1 3`; do echo ''; done
rm runBMP.exe
rm output/equa*
echo 'Compilando...'
g++ -Wall main.cpp BMP.h BMP.cpp -o runBMP.exe
echo '======Comienza la ejecución del programa===='
./runBMP.exe