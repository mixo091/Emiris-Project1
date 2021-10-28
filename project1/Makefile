Project1: main.o VectorList.o Utilities.o 
	g++ -g3 -Wall main.o VectorList.o Utilities.o -o Project1

main.o: main.cpp 
	g++ -c -g3 -Wall main.cpp

VectorList.o: ./VectorList/VectorList.cpp ./VectorList/VectorList.hpp
	g++ -c ./VectorList/VectorList.cpp

# HashTable.o: ./HashTable/HashTable.cpp ./HashTable/HashTable.hpp
# 	g++ -c ./HashTable/HashTable.cpp

Utilities.o: ./Utilities/Utilities.cpp ./Utilities/Utilities.hpp
	g++ -c -g3 -Wall ./Utilities/Utilities.cpp

clean:
	rm -rf *.o Project1