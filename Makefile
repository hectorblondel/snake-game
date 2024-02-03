main : main.cpp snake.cpp
	g++ -o $@ $^ `sdl2-config --cflags --libs` ; ./$@

test : test.cpp
	g++ -o $@ $^ `sdl2-config --cflags --libs` ; ./$@