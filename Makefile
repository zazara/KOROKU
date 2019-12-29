koroku: src/main.cpp
	g++ -std=c++11 -o koroku src/main.cpp src/resources.cpp `pkg-config gtkmm-3.0 --cflags --libs`