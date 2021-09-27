CC = g++
CFLAGS = -std=c++11
OBJECTS	= matchdata.o main.o
    
heatmap: $(OBJECTS)
		$(CC) $(OBJECTS) -o heatmap
    
main.o: src/main.cpp
		$(CC) $(CXXFLAGS) -c src/main.cpp -o main.o
    
matchdata.o: src/matchdata.cpp src/matchdata.h
		$(CC) $(CFLAGS) -c src/matchdata.cpp -o matchdata.o