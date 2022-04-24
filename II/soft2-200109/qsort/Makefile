CC = gcc

bin/qsort: src/main.o
	$(CC) -o bin/qsort src/main.o -L./lib -lqsort
lib/libqsort.a: lib/qsort_lib.o
	ar rsv lib/libqsort.a lib/qsort_lib.o

src/main.o: src/main.c
	$(CC) -I./include -o src/main.o -c src/main.c

lib/qsort_lib.o: lib/qsort_lib.c 
	$(CC) -I./include -o lib/qsort_lib.o -c lib/qsort_lib.c