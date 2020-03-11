SRCS = src/main.cpp src/Brush.h src/Canvas.h src/TransparentWindow.h

RSRCS = resources/gresource.xml resources/glade_project.glade resources/style.css

koroku:$(SRCS) $(RSRCS)
	g++ -std=c++11 -o koroku src/main.cpp resources/resources.cpp `pkg-config gtkmm-3.0 --cflags --libs`

resources:$(RSRCS)
	glib-compile-resources --target=resources/resources.cpp --generate-source resources/gresource.xml 

clean:
	rm ./koroku