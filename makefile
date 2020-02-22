checker.exe: main.o umh.o game.o
	g++ -o checker.exe main.o umh.o game.o

main.o: main.cpp umh.hpp game.hpp
	g++ -c main.cpp
board.o:board.cpp umh.hpp game.hpp
	g++ -c umh.cpp
game.o:game.cpp game.hpp umh.hpp
	g++ -c game.cpp

debug:
	g++ -c -g checker.exe main.cpp umh.cpp game.cpp

clean:
	rm *.exe*.o*.stackdump *~
