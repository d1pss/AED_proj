CC = gcc
CFLAGS = -Wall -O3
MODE = -std=c99

navigate: navigate.o file_management.o tarefas.o array_management.o
	$(CC) $(CFLAGS) $(MODE) -o $@ $^

navigate.o: navigate.c
	$(CC) $(CFLAGS) $(MODE) -c $<

file_management.o: file_management.c file_management.h
	$(CC) $(CFLAGS) $(MODE) -c $<

tarefas.o: tarefas.c tarefas.h
	$(CC) $(CFLAGS) $(MODE) -c $<

array_management.o: array_management.c array_management.h
	$(CC) $(CFLAGS) $(MODE) -c $<


clean:
	rm -f *.o navigate
