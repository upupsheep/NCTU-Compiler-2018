#makefile for parser
# yacc -d -v parser.y
# lex scanner.l
# gcc lex.yy.c y.tab.c -ly -ll
# ./a.out < example.c
TARGET = parser
OBJECT = lex.yy.c y.tab.h y.tab.c
cc = gcc -g
LEX = flex
LIBS = -ly -lfl -lbsd
YACC = yacc -d -v

all: parser

parser: lex.yy.c y.tab.c
	$(cc) lex.yy.c y.tab.c -o $(TARGET) $(LIBS)

y.tab.c: yacctemplate.y
	$(YACC) yacctemplate.y

lex.yy.c: lex.l
	$(LEX) lex.l

clean:
	rm -f $(TARGET) $(OBJECT)



