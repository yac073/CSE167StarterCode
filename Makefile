#NOTE: This makefile doesn't actually work. Use/Modify it at your own risk!
CC = g++
#Makefile flags for OSX devices
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
CFLAGS = -g -DGL_GLEXT_PROTOTYPES -DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED -DOSX -Wno-deprecated-register -Wno-deprecated-declarations -Wno-shift-op-parentheses -Wno-parentheses-equality
INCFLAGS = -I./glm-0.9.7.1 -I/usr/X11/include -I./include/
LDFLAGS = -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -L./lib/mac/ \
		-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
		-lGL -lGLU -lm -lglfw3 -lstdc++
#Makefile flags for Unix devices
else
#TODO
endif
RM = /bin/rm -f
all: glfwStarterProject
glfwStarterProject: main.o Window.o Cube.o
	$(CC) $(CFLAGS) -o glfwStarterProject main.o Window.o Cube.o $(INCFLAGS) $(LDFLAGS)
main.o: Window.o main.cpp main.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
Window.o: Cube.o Window.cpp Window.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c Window.cpp
Cube.o: Cube.cpp Cube.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c Cube.cpp
clean:
	$(RM) *.o glfwStarterProject
