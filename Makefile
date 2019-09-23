
FLAGS = -g -Wall

ALL: array.o buffer.o search.o stream.o heap.o
	gcc $(FLAGS) -o search search.o array.o buffer.o heap.o stream.o

array.o: array.c array.h
	gcc -c $(FLAGS) array.c
buffer.o: buffer.c buffer.h
	gcc -c $(FLAGS) buffer.c
heap.o: heap.c heap.h array.h
	gcc -c $(FLAGS) heap.c
search.o: search.c array.h heap.h stream.h
	gcc -c $(FLAGS) search.c
stream.o: stream.c stream.h
	gcc -c $(FLAGS) stream.c

clean:
	rm *.o
	rm search
