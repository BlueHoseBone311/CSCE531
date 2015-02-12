CC = gcc
CCFLAGS = -Wall -g -c
LEX = flex
TARGET = -o babycpp

babycpp: driver_main.o lex.yy.o hashtbl.o
	$(CC) driver_main.o lex.yy.o hashtbl.o $(TARGET) -lfl

lex.yy.o: lex.yy.c
	$(CC) $(CCFLAGS) lex.yy.c

driver_main.o: driver_main.c defs.h 
	$(CC) $(CCFLAGS) main.c

hashtbl.o: hashtbl.c defs.h
	$(CC) $(CCFLAGS) hashtbl.c	

lex.yy.c: analyzer.lex defs.h
	$(LEX) analyzer.lex  

clean: 
	rm *.o lex.yy.c babycpp 