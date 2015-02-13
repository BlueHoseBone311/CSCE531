CC = gcc
CCFLAGS = -Wall -g -c
LEX = flex
TARGET = -o babycpp

babycpp: lex.yy.o dict.o 
	$(CC) lex.yy.o dict.o $(TARGET) -lfl

lex.yy.o: lex.yy.c
	$(CC) $(CCFLAGS) lex.yy.c

dict.o: dict.c defs.h
	$(CC) $(CCFLAGS) dict.c	

lex.yy.c: driver.lex defs.h
	$(LEX) driver.lex  

clean: 
	rm *.o lex.yy.c babycpp test*.c *.txt *.i