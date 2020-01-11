koroku: src/main.cpp src/Brush.h src/Canvas.h src/TransparentWindow.h src/resources.cpp
	g++ -std=c++11 -o koroku src/main.cpp src/resources.cpp `pkg-config gtkmm-3.0 --cflags --libs`