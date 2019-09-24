TARGET = compiler
OBJECT = lex.yy.c y.tab.c y.tab.o lex.yy.o y.output compiler y.tab.h main.o semcheck.o symtab.o gencode.o output.j output.class
CC = gcc -g
LEX = flex
YACC = yacc -v
YACCFLAG = -d
LIBS = -ll

all: $(TARGET)

compiler: y.tab.o lex.yy.o main.o semcheck.o symtab.o gencode.o
	$(CC) -o $(TARGET) y.tab.o lex.yy.o main.o semcheck.o symtab.o gencode.o $(LIBS)

y.tab.o: y.tab.c
	$(CC) -c y.tab.c

y.tab.c: parser.y
	$(YACC) $(YACCFLAG) parser.y

lex.yy.o: lex.yy.c
	$(CC) -c lex.yy.c

lex.yy.c: lex.l
	$(LEX) lex.l

main.o: main.c
	$(CC) -c main.c
test:
	./compiler testcases/expression/expression.c && java -jar jasmin.jar -d testcases/expression testcases/expression/expression.j && java -classpath testcases/expression expression
	./compiler testcases/fibonacci/fibonacci.c && java -jar jasmin.jar -d testcases/fibonacci testcases/fibonacci/fibonacci.j && java -classpath testcases/fibonacci fibonacci
	./compiler testcases/helloworld/helloworld.c && java -jar jasmin.jar -d testcases/helloworld testcases/helloworld/helloworld.j && java -classpath testcases/helloworld helloworld 
	./compiler testcases/while_for/while_for.c && java -jar jasmin.jar -d testcases/while_for testcases/while_for/while_for.j && java -classpath testcases/while_for while_for
	./compiler testcases/if_stmt/if_stmt.c && java -jar jasmin.jar -d testcases/if_stmt testcases/if_stmt/if_stmt.j && java -classpath testcases/if_stmt if_stmt

clean:
	rm -f $(TARGET) $(OBJECT)

