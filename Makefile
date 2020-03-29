build:
	g++ main.cpp gamelib.cpp `pkg-config --libs --cflags sdl2 SDL2_ttf` -o run
