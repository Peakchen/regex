CC=g++
FLAGS=-g

all:
	#$(CC) -c $(FLAGS) astnode.cpp -o astnode.o
	$(CC) -c $(FLAGS) regex.cpp -o regex.o
	#$(CC) -c $(FLAGS) state.cpp -o state.o
	$(CC) -c $(FLAGS) stream.cpp -o stream.o
	#$(CC) -c $(FLAGS) edge.cpp -o edge.o
	$(CC) -c $(FLAGS) main.cpp -o main.o
	$(CC) $(FLAGS) regex.o stream.o main.o -o main
clean:
	rm *.o main
