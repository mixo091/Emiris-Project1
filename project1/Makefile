FLAGS = -c -g3 -Wall -O3
OBJS = *.o 
TARGETS = cube lsh cluster
CC = g++
SRC_DIR = ./src

lsh: main.o Utilities.o 
	g++ -Wall -o lsh main.o Utilities.o -O3

cube: cube_main.o Utilities.o
	g++ -Wall -o cube cube_main.o Utilities.o -O3

cluster: clustering_main.o  ClusteringUtilities.o
	g++ -Wall -o cluster clustering_main.o  ClusteringUtilities.o -O3

cube_main.o: ./src/cube_main.cpp
	$(CC) $(FLAGS) ./src/cube_main.cpp

main.o: ./src/main.cpp 
	$(CC) $(FLAGS) ./src/main.cpp

clustering_main.o: ./src/clustering_main.cpp
	$(CC) $(FLAGS) ./src/clustering_main.cpp	

Utilities.o: ./src/Utilities/Utilities.cpp ./src/Utilities/Utilities.hpp
	$(CC) $(FLAGS) ./src/Utilities/Utilities.cpp

ClusteringUtilities.o: ./src/Clustering/ClusteringUtilities.cpp ./src/Clustering/ClusteringUtilities.hpp
	$(CC) $(FLAGS)  ./src/Clustering/ClusteringUtilities.cpp
clean:
	rm -rf $(TARGETS) $(OBJS)
