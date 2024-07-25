# pre-requisits: SDL library
# sudo apt-get install libsdl2-dev
all:
	g++ -o run_spaceship.exe code_spaceship.c -I/usr/local/include/SDL2/ -L/usr/local/lib/ -lSDL2

loop:
	g++ -o run_spaceship.exe new_code_loop.c -I/usr/local/include/SDL2/ -L/usr/local/lib/ -lSDL2


old:
	g++ -o run_spaceship.exe new_code.c -I/usr/local/include/SDL2/ -L/usr/local/lib/ -lSDL2
