resources.cpp :
	glib-compile-resources --target=src/resources.cpp --generate-source src/gresource.xml

koroku: src/main.cpp
	g++ -std=c++11 -o bin/koroku src/main.cpp src/resources.cpp `pkg-config gtkmm-3.0 --cflags --libs`