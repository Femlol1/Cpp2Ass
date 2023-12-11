all: main

main: main.cpp Player.cpp Utilities.cpp
	g++ main.cpp Player.cpp Utilities.cpp -o main

# Some cleanup functions, invoked by typing "make clean" or "make deepclean"
deepclean:
	rm -f *~ *.o Main main main.exe *.stackdump

clean:
	rm -f *~ *.o *.stackdump
