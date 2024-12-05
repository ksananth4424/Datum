LEXERDIR = lexer
PARSERDIR = parser
SEMANTICDIR = semantic
TRANSPILER = Transpiler

.PHONY: all lexer parser semantic test clean

all: lexer parser semantic transpiler

lexer:
	@echo "\e[33m\nBuilding lexer\e[0m"
	$(MAKE) --no-print-directory -C $(LEXERDIR)

parser:
	@echo "\e[33m\nBuilding parser\e[0m"
	$(MAKE) --no-print-directory -C $(PARSERDIR)

semantic:
	@echo "\e[33m\nBuilding semantic\e[0m"
	$(MAKE) --no-print-directory -C $(SEMANTICDIR)

transpiler:
	@echo "\e[33m\nBuilding transpiler\e[0m"
	$(MAKE) --no-print-directory -C $(TRANSPILER)

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
	$(MAKE) --no-print-directory -C $(TRANSPILER) clean
	rm -f **/*.o **/*.out **/*.exe **/.*.cpp