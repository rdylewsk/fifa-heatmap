CC = g++
CFLAGS = -std=c++11
OBJECTS	= matchdata.o main.o
    
heatmap: $(OBJECTS)
		$(CC) $(OBJECTS) -o heatmap
    
main.o: src/main.cpp
		$(CC) $(CXXFLAGS) -c src/main.cpp -o main.o
    
matchdata.o: src/matchdata.cpp src/matchdata.h
		$(CC) $(CFLAGS) -c src/matchdata.cpp -o matchdata.o

.PHONY: run
run: heatmap
		./heatmap Portugal Spain y < run_input/1.txt > temp.txt
		./heatmap Uruguay Portugal y < run_input/2.txt > temp.txt
		./heatmap Argentina Iceland y < run_input/3.txt > temp.txt
		./heatmap Croatia England y < run_input/4.txt > temp.txt
		./heatmap France Croatia n
