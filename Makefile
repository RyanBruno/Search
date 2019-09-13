
FLAGS = -g -Wall

ALL: array.o search.o stack.o stream.o queue.o
	gcc $(FLAGS) -o search search.o array.o stack.o stream.o queue.o
	
array.o: array.c array.h
	gcc -c $(FLAGS) array.c
search.o: search.c
	gcc -c $(FLAGS) search.c
stack.o: stack.c stack.h array.h
	gcc -c $(FLAGS) stack.c
stream.o: stream.c stream.h
	gcc -c $(FLAGS) stream.c
queue.o: queue.c queue.h
	gcc -c $(FLAGS) queue.c

clean:
	rm *.o
	rm search
