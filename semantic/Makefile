EXEC = parser.o

.SILENT: test run
.PHONY: clean build test run

$(EXEC): y.tab.c
	g++ -o $(EXEC) y.tab.c

y.tab.c y.tab.h: parser.y lex.yy.c
	yacc -d parser.y

lex.yy.c: lexer.l
	lex lexer.l

test: ${EXEC}
	cd Tests && ./run.sh

clean:
	rm -f *.yy.c
	rm -f *.tab.c
	rm -f *.tab.h
	rm -f *.o