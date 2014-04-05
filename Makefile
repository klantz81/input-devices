all: test.cc lib/keyboard.o lib/mouse.o lib/joystick.o
	g++ test.cc -o bin/test lib/keyboard.o lib/mouse.o lib/joystick.o -lpthread

lib/keyboard.o: src/keyboard.h src/keyboard.cc
	g++ src/keyboard.cc -c -o lib/keyboard.o

lib/mouse.o: src/mouse.h src/mouse.cc
	g++ src/mouse.cc -c -o lib/mouse.o

lib/joystick.o: src/joystick.h src/joystick.cc
	g++ src/joystick.cc -c -o lib/joystick.o

clean:
	@rm -f *~ src/*~ lib/* bin/*
