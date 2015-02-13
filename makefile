CC = gcc
CCFLAGS = -c
LEX = flex
TARGET = -o babycpp

babycpp: lex.yy.o hashtbl.o 
	$(CC) lex.yy.o hashtbl.o $(TARGET) -lfl

lex.yy.o: lex.yy.c
	$(CC) $(CCFLAGS) lex.yy.c

hashtbl.o: hashtbl.c defs.h
	$(CC) $(CCFLAGS) hashtbl.c	

lex.yy.c: analyzer.lex defs.h
	$(LEX) analyzer.lex  

clean: 
	rm *.o lex.yy.c babycpp *.md