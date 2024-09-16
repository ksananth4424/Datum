TEST-DIR := Test
EXEC = test.o
LEX = lex
GCC = gcc

.PHONY : test clean build
.SILENT : test

$(EXEC) : test.l
	$(LEX) test.l
	$(GCC) -o $(EXEC) lex.yy.c

test : $(EXEC)
	cp $(EXEC) $(TEST-DIR)
	@echo "\e[1mTesting test\e[0m"
	@echo "----------------"
	${MAKE} --no-print-directory -C $(TEST-DIR) test

clean:
	rm -f *.yy.c
	rm -f *.o
	rm -f $(TEST-DIR)/*.o

