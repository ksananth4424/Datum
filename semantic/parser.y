%code requires{
    #include "ast.h"
}

%{
    #include <iostream>
    #include <fstream>
    #include "ast.h"
    #include "symbol_table.hpp"

    class Start* root;
    std::string SCOPE = "g";
    int yylex();
    extern void yyerror(char*);
    SymbolTable symtab;
%}

%union{
    int ival;
    float fval;
    char cval;
    char* string;

    InbuiltFunctions AstInbuiltFunctions;
    AssignmentOperator AstAssignmentOperator;
    BinaryOperator AstBinaryOperator;

    class Start* AstStart;
    class TypeSpecifier* AstTypeSpecifier;
    //declaration
    class DeclarationStatement *AstDeclarationStatement;
    class InitDeclaration *AstInitDeclaration;
    class Declarator *AstDeclarator;
    class Initializer *AstInitializer;

    //expression
    class Expression *AstExpression;
    class UnaryExpression *AstUnaryExpression;
    class BinaryExpression *AstBinaryExpression;

    class ConstantValue *AstConstantValue;
    class Paramter *AstParameter;

    //statements
    class AssignmentStatement *AstAssignmentStatement;
    class ConditionalStatement *AstConditionalStatement;
    class LoopStatement *AstLoopStatement;
    class ReturnStatement *AstReturnStatement;
    class BreakStatement *AstBreakStatement;
    class ContinueStatement *AstContinueStatement;
    class Statement* AstStatement;

    class FunctionDeclaration *AstFunctionDeclaration;



    vector<InitDeclaration*> *AstInitDeclaratorList;
    vector<Initializer*> *AstInitializerList;
    vector<Paramter*> *AstParameterList;
    vector<Expression*> AstExpressionList;
    vector<FunctionDeclaration*> AstFunctionDeclarationList;
    vector<Statement*> AstStatementList;

}

%token FUNC_LABEL START_LABEL
%token CONSTANT STRING_LITERAL
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

%token<string> IDENTIFIER

%type <AstStart> start
%type <AstStart> program
%type <AstInbuiltFunctions> inbuilt_function
%type <AstAssignmentOperator> assignment_operator
%type <AstTypeSpecifier> type_specifier
%type <AstDeclarationStatement> declaration
%type <AstInitDeclaratorList> init_declarator_list
%type <AstInitDeclaration> init_declarator
%type <AstDeclarator> declarator
%type <AstInitializer> initializer
%type <AstInitializerList> initializer_list
%type <AstParameterList> parameter_list
%type <AstParameter> parameter_declaration
%type <AstExpression> expression
%type <AstExpressionList> expression_list
%type <AstFunctionDeclaration> function_definition
%type <AstFunctionDeclarationList> functions
%type <AstStatement> statement
%type <AstStatementList> statement_list
%type <AstStatementList> compound_statement



%start start
%%

start : program {
    $$ = $1; root = $$;
    root->scope = "g"
    puts("success!!");
} 
//parsing starts from here this triggers 'program'.

program
    : FUNC_LABEL functions START_LABEL statement_list       {$$ = new Start($2, $4,SCOPE); }
    | FUNC_LABEL functions START_LABEL                      {$$ = new Start($2,NULL,SCOPE); }
    | FUNC_LABEL START_LABEL statement_list                 {$$ = new Start(NULL, $3,SCOPE); }
    | START_LABEL statement_list                            {$$ = new Start(NULL, $2,SCOPE); }
    ;

// these are all the funtions which will be supported by the DSL i.e. they will be in-build functions
inbuilt_function 
    : SHOW_BAR          { $$ = AstInbuiltFunctions::SHOW_BAR; }
    | SHOW_LINE         { $$ = AstInbuiltFunctions::SHOW_LINE; }
    | SHOW_SCATTER      { $$ = AstInbuiltFunctions::SHOW_SCATTER; }
    | SHOW_BOX          { $$ = AstInbuiltFunctions::SHOW_BOX; }
    | ROW               { $$ = AstInbuiltFunctions::ROW; }
    | COL               { $$ = AstInbuiltFunctions::COL; }
    | FILTER            { $$ = AstInbuiltFunctions::FILTER; }
    | SUM               { $$ = AstInbuiltFunctions::SUM; }
    | MAX               { $$ = AstInbuiltFunctions::MAX; }
    | MIN               { $$ = AstInbuiltFunctions::MIN; }
    | MEAN              { $$ = AstInbuiltFunctions::MEAN; }
    | JOIN              { $$ = AstInbuiltFunctions::JOIN; }
    | READ              { $$ = AstInbuiltFunctions::READ; }
    | WRITE             { $$ = AstInbuiltFunctions::WRITE; }
    | UNIQUE            { $$ = AstInbuiltFunctions::UNIQUE; }
    | SHOW              { $$ = AstInbuiltFunctions::SHOW; }
    | SPLIT             { $$ = AstInbuiltFunctions::SPLIT; }
    | SORT              { $$ = AstInbuiltFunctions::SORT; }
    | SHUFFLE           { $$ = AstInbuiltFunctions::SHUFFLE; }
    | ADD               { $$ = AstInbuiltFunctions::ADD; }
    | SHAPE             { $$ = AstInbuiltFunctions::SHAPE; }
    | DROP              { $$ = AstInbuiltFunctions::DROP; }
    ;

// grammer for make variable declarations, all declarations must end with ';' 
declaration 
    : type_specifier ';'                        {$$ = new DeclarationStatement($1, NULL, SCOPE);}
    | type_specifier init_declarator_list ';'   {$$ = new DeclarationStatement($1, $2, SCOPE);}
    ;

// all the types which can be used for the above "declarations"
type_specifier 
    : INTEGER                       {$$ = new TypeSpecifier("int",NULL,SCOPE);}   
    | FLOAT                         {$$ = new TypeSpecifier("float",NULL,SCOPE);}
    | STRING                        {$$ = new TypeSpecifier("string",NULL,SCOPE);}
    | CHAR                          {$$ = new TypeSpecifier("char",NULL,SCOPE);}
    | BOOL                          {$$ = new TypeSpecifier("bool",NULL,SCOPE);}
    | DATASET                       {$$ = new TypeSpecifier("dataset",NULL,SCOPE);}
    | ARRAY '(' type_specifier ')'  {$$ = new TypeSpecifier("array",$3,SCOPE);}
    ;

// for declaring multiple variables of same type in one declaration.
init_declarator_list 
    : init_declarator                           {$$ = new vector<InitDeclaration*>(); $$->push_back($1);}
    | init_declarator_list ',' init_declarator  {$$ = $1; $1->push_back($3);}
    ;

// for initializing while declaring variables
init_declarator                     
	: declarator                    {$$ = new InitDeclaration($1, NULL,SCOPE);}
	| declarator '=' initializer    {$$ = new InitDeclaration($1, $3,SCOPE);}
	;

// TERMINAL => indentifier
declarator 
    : IDENTIFIER                {$$ = new Declarator($1,SCOPE);}  
    /* | '(' declarator ')' */
    /* | declarator '(' parameter_list ')' */
    ;    

// for initializing variables
initializer     
    : assignment_expression     {$$ = new Initializer($1,NULL,SCOPE);}
	| '{' initializer_list '}'  {$$ = new Initializer(NULL,$2,SCOPE);}
	/* | '{' initializer_list ',' '}'  */
	;

// for initializing multiple variables in one line
initializer_list 
	: initializer                       {$$ = new vector<Initializer*>(); $$->push_back($1);}
	| initializer_list ',' initializer  {$$ = $1; $1->push_back($3);}
	;

// similar to declarator list, to give parameters to a function
parameter_list  
	: parameter_declaration                      {$$ = new vector<Paramter*>(); $$->push_back($1);}
	| parameter_list ',' parameter_declaration   {$$ = $1; $1->push_back($3);}
    ;

// for the above parameter_list way how each parameter is given to a funtion
parameter_declaration 
	: type_specifier declarator     {$$ = new Paramter($1, $2,SCOPE);}
	| type_specifier                {$$ = new Paramter($1, NULL,SCOPE);}
	;


// intuitive
primary_expression 
    : CONSTANT
    | STRING_LITERAL
    ;

 // similar to parameter list, multiple expressions are dealt.
expression_list
    : expression                        { $$ = new vector<Expression*>(); $$->push_back($1); }
    | expression_list ',' expression    { $$ = $1; $1->push_back($3); }
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
    | single_chain_expression '[' expression ']'
    | single_chain_expression FLOW IDENTIFIER '(' ')'
    | single_chain_expression FLOW IDENTIFIER '(' argument_list ')'
    | single_chain_expression FLOW inbuilt_function '(' ')'
    | single_chain_expression FLOW inbuilt_function '(' argument_list ')'
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
	: unary_expression              { $$ = $1; }
	| expression '*' expression     { $$ = new BinaryExpression($1, $3, AstBinaryOperator::MUL,SCOPE); }
	| expression '/' expression     { $$ = new BinaryExpression($1, $3, AstBinaryOperator::DIV,SCOPE); }
	| expression '%' expression     { $$ = new BinaryExpression($1, $3, AstBinaryOperator::MOD,SCOPE); }
	| expression '+' expression     { $$ = new BinaryExpression($1, $3, AstBinaryOperator::ADD,SCOPE); }
	| expression '-' expression     { $$ = new BinaryExpression($1, $3, AstBinaryOperator::SUB,SCOPE); }
	| expression '>' expression     { $$ = new BinaryExpression($1, $3, AstBinaryOperator::GT,SCOPE); }
	| expression '<' expression     { $$ = new BinaryExpression($1, $3, AstBinaryOperator::LT,SCOPE); }
	| expression LE_OP expression   { $$ = new BinaryExpression($1, $3, AstBinaryOperator::LE,SCOPE); }
	| expression GE_OP expression   { $$ = new BinaryExpression($1, $3, AstBinaryOperator::GE,SCOPE); }
	| expression EQ_OP expression   { $$ = new BinaryExpression($1, $3, AstBinaryOperator::EQ,SCOPE); }
	| expression NE_OP expression   { $$ = new BinaryExpression($1, $3, AstBinaryOperator::NE,SCOPE); }
	| expression AND expression     { $$ = new BinaryExpression($1, $3, AstBinaryOperator::AND,SCOPE); }
	| expression OR expression      { $$ = new BinaryExpression($1, $3, AstBinaryOperator::OR,SCOPE); }
    ;

// assignemnt operator derives to '=' or can further move forward with other assignments
assignment_operator
	: '='           { $$ = AstAssignmentOperator::ASSIGN; }
	| MUL_ASSIGN    { $$ = AstAssignmentOperator::MUL_ASSIGN; }
	| DIV_ASSIGN    { $$ = AstAssignmentOperator::DIV_ASSIGN; }
	| MOD_ASSIGN    { $$ = AstAssignmentOperator::MOD_ASSIGN; }
	| ADD_ASSIGN    { $$ = AstAssignmentOperator::ADD_ASSIGN; }
	| SUB_ASSIGN    { $$ = AstAssignmentOperator::SUB_ASSIGN; }
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
    : '{' '}'                   {$$ = new vector<Statement*>();}
    | '{' statement_list '}'    {$$ = $2;}
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
    : statement                 {$$ = new vector<Statement*>(); $$->push_back($1);}
    | statement_list statement  {$$ = $1; $1->push_back($2);}
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
    : function_definition               {$$ = new vector<FunctionDeclaration*>(); $$->push_back($1);}
    | functions function_definition     {$$ = $1; $1->push_back($2);}
    ;

// this is how parameter should be given to a function
function_definition 
    : FUNCTION '(' parameter_list ')' FLOW IDENTIFIER '(' parameter_list ')' FLOW '(' parameter_list ')' compound_statement     {$$ = new FunctionDeclaration($3, $5, $7, $11,SCOPE);}
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
