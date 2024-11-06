%{
    #include <iostream>
    #include <fstream>
    int yylex();
    extern void yyerror(const char*);

%}

%token FUNC_LABEL START_LABEL
%token IDENTIFIER STRING_LITERAL BOOL_TRUE BOOL_FALSE CHAR INTEGER FLOAT
%token ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
%token FLOW EQ_OP
%token GE_OP LE_OP NE_OP
%token SHOW_BAR SHOW_LINE SHOW_SCATTER SHOW_BOX
%token ROW COL FILTER SUM MAX MIN MEAN JOIN READ
%token WRITE UNIQUE SHOW SPLIT SORT SHUFFLE ADD SHAPE DROP
%token FROM TO STEP ALSO
%token AND OR NOT
%token IF ELSE LOOP BREAK CONTINUE RETURN FUNCTION
%token STRING BOOL DATASET ARRAY CONST_INT CONST_FLOAT CONST_CHAR

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

start : program       {puts("success!!");} //parsing starts from here this triggers 'program'.

program
    : FUNC_LABEL functions START_LABEL statement_list 
    | FUNC_LABEL functions START_LABEL
    | FUNC_LABEL START_LABEL statement_list
    | START_LABEL statement_list
    ;

// these are all the funtions which will be supported by the DSL i.e. they will be in-build functions
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

// grammer for make variable declarations, all declarations must end with ';' 
declaration 
    : type_specifier ';'
    | type_specifier init_declarator_list ';'
    ;

// all the types which can be used for the above "declarations"
type_specifier 
    : INTEGER
    | FLOAT
    | STRING
    | CHAR
    | BOOL
    | DATASET
    | ARRAY '(' type_specifier ')'
    ;

// for declaring multiple variables of same type in one declaration.
init_declarator_list 
    : init_declarator
    | init_declarator_list ',' init_declarator
    ;

// for initializing while declaring variables
init_declarator 
	: declarator
	| declarator '=' initializer
	;

// TERMINAL => indentifier
declarator 
    : IDENTIFIER 
    /* | '(' declarator ')' */
    /* | declarator '(' parameter_list ')' */
    ;    

// similar to declarator list, to give parameters to a function
parameter_list  
	: parameter_declaration
	| parameter_list ',' parameter_declaration
    ;

// for the above parameter_list way how each parameter is given to a funtion
parameter_declaration 
	: type_specifier declarator
	| type_specifier
	;

// for initializing variables
initializer 
    : assignment_expression
	| '{' initializer_list '}'
	/* | '{' initializer_list ',' '}'  */
	;

// for initializing multiple variables in one line
initializer_list 
	: initializer
	| initializer_list ',' initializer
	;

// intuitive
primary_expression 
    : CONST_INT
    | CONST_FLOAT
    | STRING_LITERAL
    | CONST_CHAR
    | BOOL_TRUE
    | BOOL_FALSE
    ;

 // similar to parameter list, multiple expressions are dealt.
expression_list
    : expression
    | expression_list ',' expression
    ;

// similar to expression list, multiple arguments are dealt.
argument_list 
    : argument
    | argument_list ',' argument
    ;

 // what functions take as input when called,can be a expression or statement
argument
    : expression
    | from_to_also_expression
    | compound_statement
    ;

// e.g {variable_name}.{function_name}(expression) 
single_chain_expression 
	: IDENTIFIER
	| IDENTIFIER '[' expression ']'
    | single_chain_expression FLOW IDENTIFIER '(' ')'
    | single_chain_expression FLOW IDENTIFIER '(' ')' '[' expression ']'
    | single_chain_expression FLOW IDENTIFIER '(' argument_list ')'
    | single_chain_expression FLOW IDENTIFIER '(' argument_list ')' '[' expression ']'
    | single_chain_expression FLOW inbuilt_function '(' ')'
    | single_chain_expression FLOW inbuilt_function '(' ')' '[' expression ']'
    | single_chain_expression FLOW inbuilt_function '(' argument_list ')'
    | single_chain_expression FLOW inbuilt_function '(' argument_list ')' '[' expression ']'
    ;

// e.g {variable_name}.{function_name}(expression).{function_name}(expression)
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

 // this includes both single chain and multi chain expressions
postfix_expression
    : single_chain_expression
    | multi_chain_expression
    | primary_expression
    ;

// this is how unary operators are used
unary_expression
	: postfix_expression
	| unary_operator unary_expression
	;

// unary operators for above unary_expressions
unary_operator 
	: '+'
	| '-'
	| NOT
	;

//operators with precedence order
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

// assignemnt operator derives to '=' or can further move forward with other assignments
assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	;

// this is how assignment is done it can be a single chain expression or a normal expression
assignment_expression 
	: single_chain_expression assignment_operator expression 
	| expression
	;

// this is step for loop, it can be empty or can have a step
optional_step
    : 
    | STEP expression
    ;

// this and next grammer declarations are for the use of if-else statements
conditional_statement 
	: IF '(' expression ')' compound_statement else_if_statement// all the conditional expressions must be used in '(' && ')' 
	| IF '(' expression ')' compound_statement else_if_statement ELSE compound_statement
	;

// similar to the above understanding
else_if_statement 
    : 
    | else_if_statement ELSE IF '(' expression ')' compound_statement// all the conditional expressions must be used in '(' && ')' 
    ;

// all statements should adhere to '{' & '}'' at end and start respec. 
compound_statement 
    : '{' '}'
    | '{' statement_list '}'
    ;

// only these can be a statement
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

// to handle multiple statements
statement_list 
    : statement
    | statement_list statement
    ;

// structure of loops should be adhered to this control of loop is handled by 'from_to_also_expression'
loop_statement 
    : LOOP IDENTIFIER from_to_also_expression compound_statement
    ;

 // this is how loop control is handled
from_to_also_expression
    : TO expression optional_step 
    | FROM expression optional_step
    | FROM expression TO expression optional_step
    | from_to_also_expression ALSO TO expression optional_step  
    | from_to_also_expression ALSO FROM expression optional_step  
    | from_to_also_expression ALSO FROM expression TO expression optional_step  
    ;

// grammer for one or multiple function declarations
functions 
    : function_definition
    | functions function_definition
    ;

// this is how parameter should be given to a function
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

void yyerror(const char* s) { 
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
