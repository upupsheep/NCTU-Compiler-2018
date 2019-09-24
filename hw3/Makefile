TARGET = parser
OBJECT = lex.yy.c y.tab.c y.output parser y.tab.h
CC = gcc -g
LEX = flex
YACC = yacc -v
YACCFLAG = -d
LIBS = -ly -lfl

all: $(TARGET)

parser: lex.yy.c y.tab.c symbolTable.c
	$(CC) -o parser lex.yy.c y.tab.c symbolTable.c $(LIBS)

y.tab.c: parser.y
	$(YACC) $(YACCFLAG) parser.y

lex.yy.c: lex.l
	$(LEX) lex.l

clean:
	rm -f $(TARGET) $(OBJECT)

