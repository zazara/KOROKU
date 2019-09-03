koroku: src/main.cpp
	g++ -std=c++11 -o bin/koroku src/main.cpp `pkg-config gtkmm-3.0 --cflags --libs`