LEXERDIR = lexer
PARSERDIR = parser
SEMANTICDIR = semantic

.PHONY: all lexer parser semantic test clean

all: lexer parser semantic

lexer:
	$(MAKE) --no-print-directory -C $(LEXERDIR)

parser:
	$(MAKE) --no-print-directory -C $(PARSERDIR)

semantic:
	$(MAKE) --no-print-directory -C $(SEMANTICDIR)

test: lexer parser semantic
	@echo "\e[33m\nRunning lexer tests\e[0m"
	$(MAKE) --no-print-directory -C $(LEXERDIR) test
	@echo "\e[33m\nRunning parser tests\e[0m"
	$(MAKE) --no-print-directory -C $(PARSERDIR) test
	@echo "\e[33m\nRunning semantic tests\e[0m"
	$(MAKE) --no-print-directory -C $(SEMANTICDIR) test

clean:
	$(MAKE) --no-print-directory -C $(LEXERDIR) clean
	$(MAKE) --no-print-directory -C $(PARSERDIR) clean
	$(MAKE) --no-print-directory -C $(SEMANTICDIR) clean
	rm -f **/*.o **/*.out **/*.exe **/.*.cpp