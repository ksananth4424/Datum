%token FUNC_LABEL START_LABEL
%token IDENTIFIER CONSTANT STRING_LITERAL
%token ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN 
%token FLOW EQ_OP INC_OP DEC_OP
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

%start expression
%%

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

primary_expression 
    : IDENTIFIER
    | CONSTANT
    | STRING_LITERAL
    | '(' expression ')'
    ;

expression_list
    : expression
    | expression_list ',' expression

chain_input 
    : postfix_expression
    | '(' expression_list ')'
    ;

postfix_expression
    : primary_expression
    | postfix_expression '[' expression ']'
    | chain_input FLOW IDENTIFIER '(' ')'
    | chain_input FLOW IDENTIFIER '(' argument_expression_list ')'
    /* | postfix_expression INC_OP
    | postfix_expression DEC_OP */
    ;

argument_expression_list
	: assignment_expression
	| argument_expression_list ',' assignment_expression
	;

unary_expression
	: postfix_expression
	/* | INC_OP unary_expression
	| DEC_OP unary_expression */
	| unary_operator unary_expression
	;

unary_operator
	: '+'
	| '-'
	| NOT
	;

/* multiplicative_expression
	: unary_expression
	| multiplicative_expression '*' unary_expression
	| multiplicative_expression '/' unary_expression
	| multiplicative_expression '%' unary_expression
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression
	| additive_expression '-' multiplicative_expression
	;

relational_expression
	: additive_expression
	| relational_expression '<' additive_expression
	| relational_expression '>' additive_expression
	| relational_expression LE_OP additive_expression
	| relational_expression GE_OP additive_expression
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression
	| equality_expression NE_OP relational_expression
	;

logical_and_expression
	: equality_expression
	| logical_and_expression AND equality_expression
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR logical_and_expression
	; */

lhs_assignment_expression
	: IDENTIFIER
    | lhs_assignment_expression '[' expression ']'
    | lhs_assignment_expression FLOW IDENTIFIER '(' ')'
    | lhs_assignment_expression FLOW IDENTIFIER '(' argument_expression_list ')'

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

assignment_expression
	: expression
	| lhs_assignment_expression assignment_operator expression
	;

assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	;



%%


#include <stdio.h>
#include "lex.yy.c"

int yywrap() {
	return 1;
}

void yyerror( char* s) { 
	printf("%s\n",s);
}

int main() {
	yyparse(); 
}