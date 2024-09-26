%{
    #include <stdio.h>
    void yyerror(char*);
    int yylex();

%}

%token FUNC_LABEL START_LABEL
%token IDENTIFIER CONSTANT STRING_LITERAL
%token ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
%token FLOW EQ_OP
%token GE_OP LE_OP NE_OP
%token SHOW_BAR SHOW_LINE SHOW_SCATTER SHOW_BOX
%token ROW COL FILTER SUM MAX MIN MEAN JOIN READ
%token WRITE UNIQUE SHOW SPLIT SORT SHUFFLE ADD SHAPE DROP
%token FROM TO STEP ALSO
%token AND OR NOT
%token IF ELSE LOOP BREAK CONTINUE RETURN FUNCTION
%token INTEGER FLOAT STRING CHAR BOOL DATASET ARRAY

%left '*'
%left '/'
%left '%'
%left '+'
%left '-'
%left '>'
%left '<'
%left LE_OP
%left GE_OP
%left EQ_OP
%left NE_OP
%left AND
%left OR

%start start
%%

start : program       {puts("success!!");}

program
    : FUNC_LABEL functions START_LABEL statement_list
    | FUNC_LABEL functions START_LABEL
    | FUNC_LABEL START_LABEL statement_list
    | START_LABEL statement_list
    ;

inbuilt_function
    : SHOW_BAR
    | SHOW_LINE
    | SHOW_SCATTER
    | SHOW_BOX
    | ROW
    | COL
    | FILTER
    | SUM
    | MAX
    | MIN
    | MEAN
    | JOIN
    | READ
    | WRITE
    | UNIQUE
    | SHOW
    | SPLIT
    | SORT
    | SHUFFLE
    | ADD
    | SHAPE
    | DROP
    ;

declaration
    : type_specifier ';'
    | type_specifier init_declarator_list ';'
    ;

type_specifier
    : INTEGER
    | FLOAT
    | STRING
    | CHAR
    | BOOL
    | DATASET
    | ARRAY '(' type_specifier ')'
    ;

init_declarator_list
    : init_declarator
    | init_declarator_list ',' init_declarator
    ;

init_declarator
	: declarator
	| declarator '=' initializer
	;

declarator
    : IDENTIFIER
    /* | '(' declarator ')' */
    /* | declarator '(' parameter_list ')' */
    ;    

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
    ;

parameter_declaration
	: type_specifier declarator
	| type_specifier
	;

initializer
    : assignment_expression
	| '{' initializer_list '}'
	/* | '{' initializer_list ',' '}'  */
	;

initializer_list
	: initializer
	| initializer_list ',' initializer
	;

primary_expression 
    : CONSTANT
    | STRING_LITERAL
    ;

expression_list
    : expression
    | expression_list ',' expression
    ;

argument_list
    : argument
    | argument_list ',' argument
    ;

argument
    : expression
    | compound_statement
    ;

single_chain_expression
	: IDENTIFIER
    | single_chain_expression '[' expression ']'
    | single_chain_expression FLOW IDENTIFIER '(' ')'
    | single_chain_expression FLOW IDENTIFIER '(' argument_list ')'
    | single_chain_expression FLOW inbuilt_function '(' ')'
    | single_chain_expression FLOW inbuilt_function '(' argument_list ')'
    ;

multi_chain_expression
	: '(' expression_list ')'
    | multi_chain_expression '[' expression ']'
    | multi_chain_expression FLOW IDENTIFIER '(' ')'
    | multi_chain_expression FLOW IDENTIFIER '(' argument_list ')'
    | multi_chain_expression FLOW inbuilt_function '(' ')'
    | multi_chain_expression FLOW inbuilt_function '(' argument_list ')'
    | IDENTIFIER '('  ')' 
    | IDENTIFIER '(' argument_list ')' 
    | inbuilt_function '('  ')' 
    | inbuilt_function '(' argument_list ')' 
    ;

postfix_expression
    : single_chain_expression
    | multi_chain_expression
    | primary_expression
    ;

unary_expression
	: postfix_expression
	| unary_operator unary_expression
	;

unary_operator
	: '+'
	| '-'
	| NOT
	;

expression
	: unary_expression
	| expression '*' expression
	| expression '/' expression
	| expression '%' expression
	| expression '+' expression
	| expression '-' expression
	| expression '>' expression
	| expression '<' expression
	| expression LE_OP expression
	| expression GE_OP expression
	| expression EQ_OP expression
	| expression NE_OP expression
	| expression AND expression
	| expression OR expression
    ;

assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	;

assignment_expression
	: single_chain_expression assignment_operator expression
	| expression
	;

optional_step
    : 
    | STEP expression
    ;

conditional_statement
	: IF '(' expression ')' compound_statement else_if_statement
	| IF '(' expression ')' compound_statement else_if_statement ELSE compound_statement
	;

else_if_statement
    : 
    | else_if_statement ELSE IF '(' expression ')' compound_statement
    ;

compound_statement
    : '{' '}'
    | '{' statement_list '}'
    ;

statement
    : assignment_expression ';'
    | compound_statement
    | conditional_statement
    | loop_statement
    | declaration
    | RETURN expression ';'
    | RETURN ';'
    | BREAK ';'
    | CONTINUE ';'
    ;

statement_list
    : statement
    | statement_list statement
    ;

loop_statement
    : LOOP IDENTIFIER from_to_also_expression compound_statement
    ;

from_to_also_expression
    : TO expression optional_step 
    | TO expression optional_step ALSO from_to_also_expression_rec
    | FROM expression TO expression optional_step
    | FROM expression TO expression optional_step ALSO from_to_also_expression_rec
    ;

from_to_also_expression_rec
    :
    | from_to_also_expression
    ;

functions
    : function_definition
    | functions function_definition
    ;

function_definition
    : FUNCTION '(' parameter_list ')' FLOW IDENTIFIER '(' parameter_list ')' FLOW '(' parameter_list ')' compound_statement
    | FUNCTION '(' parameter_list ')' FLOW IDENTIFIER '(' ')' FLOW '(' parameter_list ')' compound_statement
    | FUNCTION '(' parameter_list ')' FLOW IDENTIFIER '(' parameter_list ')' FLOW  parameter_declaration  compound_statement
    | FUNCTION '(' parameter_list ')' FLOW IDENTIFIER '(' ')' FLOW  parameter_declaration  compound_statement
    ;
%%

#include "lex.yy.c"

int yywrap() {
	return 1;
}

void yyerror( char* s) { 
	printf("%s\n",s);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        puts("error: usage ./a.out <input_file>");
        return 0;
    }
    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        puts("error: couldn't open the file");
        return 0;
    }
	yyparse(); 
}