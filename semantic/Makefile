EXEC = semantic.o
BUILDDIR = Build
TESTDIR = Tests
FLAGS =

.SILENT: test run
.PHONY: clean build test run debug


build: $(EXEC)

$(EXEC): y.tab.c ast.cpp traversal.cpp symbol_table.cpp
	g++ $(FLAGS) -o $(EXEC) y.tab.c ast.cpp traversal.cpp symbol_table.cpp

run: $(EXEC)
	./$(EXEC) input.txt

y.tab.c y.tab.h: parser.y lex.yy.c
	yacc -d parser.y

lex.yy.c: lexer.l
	lex lexer.l

debug: FLAGS += -DDEBUG
debug: clean build run

test: $(EXEC)
	cd $(TESTDIR) && ./run.sh

clean:
	rm -f *.yy.c
	rm -f *.tab.c
	rm -f *.tab.h
	rm -f *.o