main: main.cpp
	g++ main.cpp -o main


# Some cleanup functions, invoked by typing "make clean" or "make deepclean"
deepclean:
	rm -f *~ *.o Main main main.exe *.stackdump

clean:
	rm -f *~ *.o *.stackdump
