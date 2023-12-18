# Rule for creating the main executable
all: main

# Compile main executable
main: main.o Player.o Utilities.o
	g++ main.o Player.o Utilities.o -o main

# Compile main.cpp into main.o
main.o: main.cpp
	g++ -c main.cpp

# Compile Player.cpp into Player.o
Player.o: Player.cpp
	g++ -c Player.cpp

# Compile Utilities.cpp into Utilities.o
Utilities.o: Utilities.cpp
	g++ -c Utilities.cpp

# Clean up object files and executables
clean:
	rm -f *.o main

# Extended clean up
deepclean: clean
	rm -f *~ main.exe *.stackdump
