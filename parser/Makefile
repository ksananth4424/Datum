EXEC = parser.o
OBJECTS = object_folder/y.tab.o object_folder/lex.yy.o

.SILENT: test run
.PHONY: clean build test run

$(EXEC): $(OBJECTS)
	gcc -o $(EXEC) ${OBJECTS}

y.tab.c y.tab.h: parser.y
	yacc -d parser.y
	
lex.yy.c: lexer.l y.tab.h
	lex lexer.l

object_folder/%.o: %.c
	mkdir -p object_folder
	gcc -c -o $@ $<

test: ${EXEC}
	cd Tests && ./run.sh

clean:
	rm -f *.yy.c
	rm -f *.tab.c
	rm -f *.tab.h
	rm -f *.o
	rm -f object_folder/*.o
	rm -f object_folder/*.o
	rm -r object_folder