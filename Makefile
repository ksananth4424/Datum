LEXERDIR = lexer
PARSERDIR = parser

.PHONY: all lexer parser test clean

all: lexer parser

lexer:
	$(MAKE) --no-print-directory -C $(LEXERDIR)

parser:
	$(MAKE) --no-print-directory -C $(PARSERDIR)

test: lexer parser
	@echo "\e[33m\nRunning lexer tests\e[0m"
	$(MAKE) --no-print-directory -C $(LEXERDIR) test
	@echo "\e[33m\nRunning parser tests\e[0m"
	$(MAKE) --no-print-directory -C $(PARSERDIR) test

clean:
	$(MAKE) --no-print-directory -C $(LEXERDIR) clean
	$(MAKE) --no-print-directory -C $(PARSERDIR) clean
	rm -f **/*.o **/*.out **/*.exe **/.*.cpp