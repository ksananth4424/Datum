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

start : loop_statement       {puts("success!!");}

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
    : declaration_specifiers ';'
    | declaration_specifiers init_declarator_list ';'
    ;

declaration_specifiers
    : type_specifier
    | type_specifier declaration_specifiers
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

init_declarator
	: declarator
	| declarator '=' initializer
	;

declarator
    : dir_declarator
    ;

dir_declarator
    : IDENTIFIER
    | '(' declarator ')'
       

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

single_chain_expression
	: IDENTIFIER
    | single_chain_expression '[' expression ']'
    | single_chain_expression FLOW IDENTIFIER '(' ')'
    | single_chain_expression FLOW IDENTIFIER '(' expression_list ')'
    | single_chain_expression FLOW inbuilt_function '(' ')'
    | single_chain_expression FLOW inbuilt_function '(' expression_list ')'
    ;

multi_chain_expression
	: '(' expression_list ')'
    | multi_chain_expression '[' expression ']'
    | multi_chain_expression FLOW IDENTIFIER '(' ')'
    | multi_chain_expression FLOW IDENTIFIER '(' expression_list ')'
    | multi_chain_expression FLOW inbuilt_function '(' ')'
    | multi_chain_expression FLOW inbuilt_function '(' expression_list ')'
    | IDENTIFIER '('  ')' 
    | IDENTIFIER '(' expression_list ')' 
    | inbuilt_function '('  ')' 
    | inbuilt_function '(' expression_list ')' 
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

from_to_step_expression
    : FROM expression TO expression optional_step
    | from_to_step_expression ALSO FROM expression TO expression optional_step
    ;

optional_step
    : 
    | STEP expression
    ;

conditional_statement
	: IF '(' expression ')' '{' statement_list '}'
	| IF '(' expression ')' '{' statement_list '}' ELSE '{' statement_list '}'
	;

statement
    : assignment_expression ';'
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
    : LOOP IDENTIFIER from_to_also_expression '{' statement_list '}'
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