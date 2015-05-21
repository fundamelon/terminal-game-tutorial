bin/main: bin/main.o bin/quiz.o
	g++ -g -o bin/main bin/main.o bin/quiz.o -std=c++11 -Wall -pedantic -lncurses

bin/main.o: src/main.cpp
	mkdir -p bin	
	g++ -g -c -o bin/main.o src/main.cpp 

bin/quiz.o: src/quiz.cpp
	mkdir -p bin	
	g++ -g -c -o bin/quiz.o src/quiz.cpp 
