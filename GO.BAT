gxx -c vesa.cpp -o vesa.o
gxx -c pcx.cpp -o pcx.o
gxx -c sprite.cpp -o sprite.o
gxx -c memory.cpp -o memory.o
gxx -m486 -O3 -o main.exe main.cpp vesa.o pcx.o sprite.o memory.o 
