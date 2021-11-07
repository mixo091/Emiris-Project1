lsh: main.o Utilities.o 
	g++ -g3 -Wall main.o Utilities.o -o lsh

cube: cube_main.o Utilities.o
	g++ -g3 -Wall cube_main.o Utilities.o -o cube

cube_main.o: cube_main.cpp
	g++ -c -g3 -Wall cube_main.cpp

main.o: main.cpp 
	g++ -c -g3 -Wall main.cpp

Utilities.o: ./Utilities/Utilities.cpp ./Utilities/Utilities.hpp
	g++ -c -g3 -Wall ./Utilities/Utilities.cpp

clean:
	rm -rf *.o lsh cube out out_cube