LEXERDIR = Lexer
PARSERDIR = Parser

.PHONY: all lexer parser test clean

all:
	lexer parser

lexer:
	$(MAKE) --no-print-directory -C $(LEXERDIR)

parser:
	$(MAKE) --no-print-directory -C $(PARSERDIR)

test: lexer parser
	$(MAKE) --no-print-directory -C $(LEXERDIR) test
	$(MAKE) --no-print-directory -C $(PARSERDIR) test

clean:
	$(MAKE) --no-print-directory -C $(LEXERDIR) clean
	$(MAKE) --no-print-directory -C $(PARSERDIR) clean
	rm -f **/*.o **/*.out **/*.exe **/.*.cpp