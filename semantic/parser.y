/* %code requires{
    #include "ast.hpp"
} */

%{
    #include <iostream>
    #include <fstream>
    #include "ast.hpp"
    #include "symbol_table.hpp"

    class Start* root;
    std::string SCOPE = "g";
    extern int yylex();
    extern void yyerror(char*);
    // extern int yylineno;
    // extern int yycolumn;
    // extern char yytext[];
    // extern char linebuf[];
    // // extern const char* filename;

    // // extern void yyerror(char *);
    // struct YYLTYPE;
    // extern void lyyerror(struct YYLTYPE t,char *s,...);
    // extern void warning(const char*);
    SymbolTable symtab;
%}

%union{
    int ival;
    float fval;
    char cval;
    bool bval;
    char* str;

    InbuiltFunctions AstInbuiltFunctions;
    AssignmentOperator AstAssignmentOperator;
    BinaryOperator AstBinaryOperator;
    UnaryOperator AstUnaryOperator;

    Start* AstStart;
    TypeSpecifier* AstTypeSpecifier;
    //declaration
    DeclarationStatement *AstDeclarationStatement;
    InitDeclaration *AstInitDeclaration;
    Declarator *AstDeclarator;
    Initializer *AstInitializer;

    //expression
    Expression *AstExpression;
    UnaryExpression *AstUnaryExpression;
    BinaryExpression *AstBinaryExpression;

    ConstantValue *AstConstantValue;
    Parameter *AstParameter;

    //statements
    AssignmentStatement *AstAssignmentStatement;
    ConditionalStatement *AstConditionalStatement;
    LoopStatement *AstLoopStatement;
    ReturnStatement *AstReturnStatement;
    BreakStatement *AstBreakStatement;
    ContinueStatement *AstContinueStatement;
    Statement* AstStatement;

    FunctionDeclaration *AstFunctionDeclaration;
    Argument *AstArgument;
    SingleChainExpression *AstSingleChainExpression;
    FunctionCall *AstFunctionCall;
    MultiChainExpression *AstMultiChainExpression;



    vector<InitDeclaration*> *AstInitDeclaratorList;
    vector<Initializer*> *AstInitializerList;
    vector<Parameter*> *AstParameterList;
    vector<Expression*> *AstExpressionList;
    vector<FunctionDeclaration*> *AstFunctionDeclarationList;
    vector<Statement*> *AstStatementList;
    vector<tuple<Expression*,Expression*,Expression*>> *AstFromToAlsoExpression;
    vector<pair<Expression*,vector<Statement*>>> *AstConditionalStatementList;
    vector<Argument*> *AstArgumentList;

}

%token FUNC_LABEL START_LABEL
%token INTEGER FLOAT CHAR  
%token ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
%token FLOW EQ_OP
%token GE_OP LE_OP NE_OP
%token SHOW_BAR SHOW_LINE SHOW_SCATTER SHOW_BOX
%token ROW COL FILTER SUM MAX MIN MEAN JOIN READ
%token WRITE UNIQUE SHOW SPLIT SORT SHUFFLE ADD SHAPE DROP
%token FROM TO STEP ALSO
%token AND OR NOT
%token IF ELSE LOOP BREAK CONTINUE RETURN FUNCTION
%token  STRING  BOOL DATASET ARRAY
%token <str> STRING_LITERAL
%token<str> IDENTIFIER
%token<ival> CONST_INTEGER
%token<fval> CONST_FLOAT
%token<cval> CONST_CHAR
%token<bval> BOOL_TRUE BOOL_FALSE



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
%type <AstConstantValue> primary_expression
%type <AstUnaryExpression> unary_expression
%type <AstFromToAlsoExpression> from_to_also_expression
%type <AstExpression> optional_step
%type <AstLoopStatement> loop_statement
%type <AstConditionalStatement> conditional_statement
%type <AstConditionalStatementList> else_if_statement
%type <AstArgument> argument
%type <AstArgumentList> argument_list
%type <AstSingleChainExpression> single_chain_expression
%type <AstAssignmentStatement> assignment_expression
%type <AstUnaryOperator> unary_operator
%type <AstExpressionList> access_list
%type <AstMultiChainExpression> multi_chain_expression
%type <AstExpression> postfix_expression



%start start
%%

start : program {
    $$ = $1; root = $$;
    root->scope = "g";
    puts("success!!");
} 
//parsing starts from here this triggers 'program'.

program
    : 
    FUNC_LABEL functions START_LABEL statement_list       {$$ = new Start($2, $4); }
    | FUNC_LABEL functions START_LABEL                      {$$ = new Start($2,nullptr); }
    | FUNC_LABEL START_LABEL statement_list                 {$$ = new Start(nullptr, $3); }
    | START_LABEL statement_list                            {$$ = new Start(nullptr, $2); }
    ;

// these are all the funtions which will be supported by the DSL i.e. they will be in-build functions
inbuilt_function 
    : SHOW_BAR          { $$ = InbuiltFunctions::show_bar; }
    | SHOW_LINE         { $$ = InbuiltFunctions::show_line; }
    | SHOW_SCATTER      { $$ = InbuiltFunctions::show_scatter; }
    | SHOW_BOX          { $$ = InbuiltFunctions::show_box; }
    | ROW               { $$ = InbuiltFunctions::row; }
    | COL               { $$ = InbuiltFunctions::col; }
    | FILTER            { $$ = InbuiltFunctions::filter; }
    | SUM               { $$ = InbuiltFunctions::sum; }
    | MAX               { $$ = InbuiltFunctions::max; }
    | MIN               { $$ = InbuiltFunctions::min; }
    | MEAN              { $$ = InbuiltFunctions::mean; }
    | JOIN              { $$ = InbuiltFunctions::join; }
    | READ              { $$ = InbuiltFunctions::read; }
    | WRITE             { $$ = InbuiltFunctions::write; }
    | UNIQUE            { $$ = InbuiltFunctions::unique; }
    | SHOW              { $$ = InbuiltFunctions::show; }
    | SPLIT             { $$ = InbuiltFunctions::split; }
    | SORT              { $$ = InbuiltFunctions::sort; }
    | SHUFFLE           { $$ = InbuiltFunctions::shuffle; }
    | ADD               { $$ = InbuiltFunctions::add; }
    | SHAPE             { $$ = InbuiltFunctions::shape; }
    | DROP              { $$ = InbuiltFunctions::drop; }
    ;

// grammer for make variable declarations, all declarations must end with ';' 
declaration 
    : type_specifier ';'                        {$$ = new DeclarationStatement($1, nullptr);}
    | type_specifier init_declarator_list ';'   {$$ = new DeclarationStatement($1, $2);}
    ;

// all the types which can be used for the above "declarations"
type_specifier 
    : INTEGER                       {$$ = new TypeSpecifier("int",nullptr);}   
    | FLOAT                         {$$ = new TypeSpecifier("float",nullptr);}
    | STRING                        {$$ = new TypeSpecifier("string",nullptr);}
    | CHAR                          {$$ = new TypeSpecifier("char",nullptr);}
    | BOOL                          {$$ = new TypeSpecifier("bool",nullptr);}
    | DATASET                       {$$ = new TypeSpecifier("dataset",nullptr);}
    | ARRAY '(' type_specifier ')'  {$$ = new TypeSpecifier("array",$3);}
    ;

// for declaring multiple variables of same type in one declaration.
init_declarator_list 
    : init_declarator                           {$$ = new vector<InitDeclaration*>(); $$->push_back($1);}
    | init_declarator_list ',' init_declarator  {$$ = $1; $1->push_back($3);}
    ;

// for initializing while declaring variables
init_declarator                     
	: declarator                    {$$ = new InitDeclaration($1, nullptr);}
	| declarator '=' initializer    {$$ = new InitDeclaration($1, $3);}
	;

// TERMINAL => indentifier
declarator 
    : IDENTIFIER                {$$ = new Declarator($1);}  
    ;


// for initializing variables
initializer     
    : assignment_expression     {$$ = new Initializer($1,nullptr);}
	| '{' initializer_list '}'  {$$ = new Initializer(nullptr,$2);}
	;

// for initializing multiple variables in one line
initializer_list 
	: initializer                       {$$ = new vector<Initializer*>(); $$->push_back($1);}
	| initializer_list ',' initializer  {$$ = $1; $1->push_back($3);}
	;

// similar to declarator list, to give parameters to a function
parameter_list  
	: parameter_declaration                      {$$ = new vector<Parameter*>(); $$->push_back($1);}
	| parameter_list ',' parameter_declaration   {$$ = $1; $1->push_back($3);}
    ;

// for the above parameter_list way how each parameter is given to a funtion
parameter_declaration 
	: type_specifier declarator     {$$ = new Parameter($1, $2);}
	| type_specifier                {$$ = new Parameter($1, nullptr);}
	;


// intuitive
primary_expression 
    : CONST_INTEGER                 { $$ = new ConstantValue(TypeSpecifier("int"),$1); }
    | CONST_FLOAT                   { $$ = new ConstantValue(TypeSpecifier("float"),$1); }
    | CONST_CHAR                   { $$ = new ConstantValue(TypeSpecifier("char"),$1); }
    | STRING_LITERAL          { $$ = new ConstantValue(TypeSpecifier("string"),$1); }
    | BOOL_TRUE               { $$ = new ConstantValue(TypeSpecifier("bool"),$1); }
    | BOOL_FALSE              { $$ = new ConstantValue(TypeSpecifier("bool"),$1); }
    ;

 // similar to parameter list, multiple expressions are dealt.
expression_list
    : expression                        { $$ = new vector<Expression*>(); $$->push_back($1); }
    | expression_list ',' expression    { $$ = $1; $1->push_back($3); }
    ;

// similar to expression list, multiple arguments are dealt.
argument_list 
    : argument                      { $$ = new vector<Argument*>(); $$->push_back($1); }
    | argument_list ',' argument    { $$ = $1; $1->push_back($3); }
    ;

 // what functions take as input when called,can be a expression or statement
argument
    : expression                { $$ = new Argument($1); }
    | from_to_also_expression   { $$ = new Argument($1); }
    | compound_statement        { $$ = new Argument($1); }
    ;

// e.g {variable_name}.{function_name}(expression) 
single_chain_expression 
	: IDENTIFIER                                                                                { $$ = new SingleChainExpression($1,nullptr,new vector<pair<FunctionCall*,vector<Expression*>*>>()); }
	| IDENTIFIER access_list                                                                    { $$ = new SingleChainExpression($1,$2,new vector<pair<FunctionCall*,vector<Expression*>*>>()); }
    | single_chain_expression FLOW IDENTIFIER '(' ')'                                           { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,nullptr),nullptr)); }
    | single_chain_expression FLOW IDENTIFIER '(' ')' access_list                               { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,nullptr),$6)); }
    | single_chain_expression FLOW IDENTIFIER '(' argument_list ')'                             { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,$5),nullptr)); }
    | single_chain_expression FLOW IDENTIFIER '(' argument_list ')' access_list                 { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,$5),$7)); }
    | single_chain_expression FLOW inbuilt_function '(' ')'                                     { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,nullptr),nullptr)); }
    | single_chain_expression FLOW inbuilt_function '(' ')' access_list                         { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,nullptr),$6)); }
    | single_chain_expression FLOW inbuilt_function '(' argument_list ')'                       { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,$5),nullptr)); }
    | single_chain_expression FLOW inbuilt_function '(' argument_list ')' access_list           { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,$5),$7)); }
    ;

access_list
    : '[' expression ']'                { $$ = new vector<Expression*>(); $$->push_back($2); }
    | access_list '[' expression ']'    { $$ = $1; $1->push_back($3); }
    ;

// e.g {variable_name}.{function_name}(expression).{function_name}(expression)
multi_chain_expression 
	: '(' expression_list ')'                                                               { $$ = new MultiChainExpression(make_pair(nullptr,$2),nullptr,nullptr); }
	| '(' expression_list ')' access_list                                                   { $$ = new MultiChainExpression(make_pair(nullptr,$2),$4,nullptr); }
    | multi_chain_expression FLOW IDENTIFIER '(' ')'                                        { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,nullptr),nullptr)); }
    | multi_chain_expression FLOW IDENTIFIER '(' ')' access_list                            { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,nullptr),$6)); }
    | multi_chain_expression FLOW IDENTIFIER '(' argument_list ')'                          { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,$5),nullptr)); }
    | multi_chain_expression FLOW IDENTIFIER '(' argument_list ')' access_list              { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,$5),$7)); }
    | multi_chain_expression FLOW inbuilt_function '(' ')'                                  { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,nullptr),nullptr)); }
    | multi_chain_expression FLOW inbuilt_function '(' ')' access_list                      { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,nullptr),$6)); }
    | multi_chain_expression FLOW inbuilt_function '(' argument_list ')'                    { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,$5),nullptr)); }
    | multi_chain_expression FLOW inbuilt_function '(' argument_list ')' access_list        { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,$5),$7)); }
    | IDENTIFIER '('  ')'                                                                   { $$ = new MultiChainExpression(new FunctionCall($1,nullptr),nullptr,nullptr); }                
    | IDENTIFIER '('  ')' access_list                                                       { $$ = new MultiChainExpression(new FunctionCall($1,nullptr),$4,nullptr); }
    | IDENTIFIER '(' argument_list ')'                                                      { $$ = new MultiChainExpression(new FunctionCall($1,$3),nullptr,nullptr); }
    | IDENTIFIER '(' argument_list ')' access_list                                          { $$ = new MultiChainExpression(new FunctionCall($1,$3),$5,nullptr); }
    | inbuilt_function '('  ')'                                                             { $$ = new MultiChainExpression(new FunctionCall($1,nullptr),nullptr,nullptr); }              
    | inbuilt_function '('  ')' access_list                                                 { $$ = new MultiChainExpression(new FunctionCall($1,nullptr),$4,nullptr); }
    | inbuilt_function '(' argument_list ')'                                                { $$ = new MultiChainExpression(new FunctionCall($1,$3),nullptr,nullptr); }
    | inbuilt_function '(' argument_list ')' access_list                                    { $$ = new MultiChainExpression(new FunctionCall($1,$3),$5,nullptr); }
    ;

 // this includes both single chain and multi chain expressions
postfix_expression
    : single_chain_expression   { $$ = $1; }
    | multi_chain_expression    { $$ = $1; }
    ;

// this is how unary operators are used
unary_expression        
	: postfix_expression                { $$ = static_cast<UnaryExpression>$1;}   
	| unary_operator unary_expression   { $$ = $2; $$->op.push_back($1);}
    | primary_expression        { $$ = new AstUnaryExpression(nullptr,nullptr,$1,nullptr); }
	;

// unary operators for above unary_expressions
unary_operator 
	: '+'       { $$ = UnaryOperator::plus_op; }
	| '-'       { $$ = UnaryOperator::minus_op; }
	| NOT       { $$ = UnaryOperator::not_op; }
	;

//operators with precedence order
expression 
	: unary_expression              { $$ = $1; }
	| expression '*' expression     { $$ = new BinaryExpression($1, $3, BinaryOperator::mul); }
	| expression '/' expression     { $$ = new BinaryExpression($1, $3, BinaryOperator::div); }
	| expression '%' expression     { $$ = new BinaryExpression($1, $3, BinaryOperator::mod); }
	| expression '+' expression     { $$ = new BinaryExpression($1, $3, BinaryOperator::add); }
	| expression '-' expression     { $$ = new BinaryExpression($1, $3, BinaryOperator::sub); }
	| expression '>' expression     { $$ = new BinaryExpression($1, $3, BinaryOperator::gt); }
	| expression '<' expression     { $$ = new BinaryExpression($1, $3, BinaryOperator::lt); }
	| expression LE_OP expression   { $$ = new BinaryExpression($1, $3, BinaryOperator::lte); }
	| expression GE_OP expression   { $$ = new BinaryExpression($1, $3, BinaryOperator::gte); }
	| expression EQ_OP expression   { $$ = new BinaryExpression($1, $3, BinaryOperator::eq); }
	| expression NE_OP expression   { $$ = new BinaryExpression($1, $3, BinaryOperator::ne); }
	| expression AND expression     { $$ = new BinaryExpression($1, $3, BinaryOperator::and_op); }
	| expression OR expression      { $$ = new BinaryExpression($1, $3, BinaryOperator::or_op); }
    ;

// assignemnt operator derives to '=' or can further move forward with other assignments
assignment_operator
	: '='           { $$ = AssignmentOperator::assign; }
	| MUL_ASSIGN    { $$ = AssignmentOperator::mul_assign; }
	| DIV_ASSIGN    { $$ = AssignmentOperator::div_assign; }
	| MOD_ASSIGN    { $$ = AssignmentOperator::mod_assign; }
	| ADD_ASSIGN    { $$ = AssignmentOperator::add_assign; }
	| SUB_ASSIGN    { $$ = AssignmentOperator::sub_assign; }
	;

// this is how assignment is done it can be a single chain expression or a normal expression
assignment_expression 
	: single_chain_expression assignment_operator expression  { $$ = new AssignmentStatement($1, $3, $2); }
	| expression                                              { $$ = new AssignmentStatement(nullptr,$1,nullptr); }
	;

// this is step for loop, it can be empty or can have a step
optional_step
    :                   {$$ = nullptr;}
    | STEP expression   {$$ = $2;}
    ;

// this and next grammer declarations are for the use of if-else statements
conditional_statement 
	: IF '(' expression ')' compound_statement else_if_statement                            {$6->push_front(make_pair($3,$5)); $$ = new ConditionalStatement($6);}
	| IF '(' expression ')' compound_statement else_if_statement ELSE compound_statement    {$6->push_front(make_pair($3,$5)); $6->push_back(make_pair(nullptr,$8)); $$ = new ConditionalStatement($6);}
	;

// similar to the above understanding
else_if_statement 
    :                                                                        {$$ = new vector<pair<Expression*,vector<Statement*>>>() ;}
    | else_if_statement ELSE IF '(' expression ')' compound_statement        {$$ = $1; $1->push_back(make_pair($5,$7));}
    ;

// all statements should adhere to '{' & '}'' at end and start respec. 
compound_statement 
    : '{' '}'                   {$$ = new vector<Statement*>();}
    | '{' statement_list '}'    {$$ = $2;}
    ;

// only these can be a statement
statement :
    /* : assignment_expression ';' {$$ = new Statement($1);} */
    /* | compound_statement        {$$ = new Statement($1);} */
    /* | conditional_statement     {$$ = new Statement($1);} */
    /* | loop_statement            {$$ = new Statement($1);} */
      declaration               {$$ = new Statement($1);}
    /* | RETURN expression ';'     {$$ = new Statement(new ReturnStatement($2));} */
    /* | RETURN ';'                {$$ = new Statement(new ReturnStatement(nullptr));} */
    /* | BREAK ';'                 {$$ = new Statement(new BreakStatement(SCOPE));} */
    | CONTINUE ';'              {$$ = new Statement(new ContinueStatement(SCOPE));}
    /* ; */

// to handle multiple statements
statement_list 
    : statement                 {$$ = new vector<Statement*>(); $$->push_back($1);}
    | statement_list statement  {$$ = $1; $1->push_back($2);}
    ;

// structure of loops should be adhered to this control of loop is handled by 'from_to_also_expression'
/* loop_statement 
    : LOOP IDENTIFIER from_to_also_expression compound_statement    {$$ = new LoopStatement($2, $3, $4);}
    ; */

 // this is how loop control is handled
from_to_also_expression
    : TO expression optional_step                                                   {$$ = new vector<tuple<Expression*,Expression*,Expression*>> (); $$->push_back(make_tuple(nullptr,$2,$3));}
    | FROM expression optional_step                                                 {$$ = new vector<tuple<Expression*,Expression*,Expression*>> (); $$->push_back(make_tuple($2,nullptr,$3));}           
    | FROM expression TO expression optional_step                                   {$$ = new vector<tuple<Expression*,Expression*,Expression*>> (); $$->push_back(make_tuple($2,$4,$5));}
    | from_to_also_expression ALSO TO expression optional_step                      {$$ = $1; $1->push_back(make_tuple(nullptr,$4,$5));}
    | from_to_also_expression ALSO FROM expression optional_step                    {$$ = $1; $1->push_back(make_tuple($4,nullptr,$5));}
    | from_to_also_expression ALSO FROM expression TO expression optional_step      {$$ = $1; $1->push_back(make_tuple($4,$6,$7));}
    ;

// grammer for one or multiple function declarations
functions 
    : function_definition               {$$ = new vector<FunctionDeclaration*>(); $$->push_back($1);}
    | functions function_definition     {$$ = $1; $1->push_back($2);}
    ;

// this is how parameter should be given to a function
function_definition 
    : FUNCTION '(' parameter_list ')' FLOW IDENTIFIER '(' parameter_list ')' FLOW '(' parameter_list ')' compound_statement     {$$ = new FunctionDeclaration($6,$3,$8,$12,$14);}
    | FUNCTION '(' parameter_list ')' FLOW IDENTIFIER '(' ')' FLOW '(' parameter_list ')' compound_statement                    {$$ = new FunctionDeclaration($6,$3,nullptr,$11,$13);}
    | FUNCTION '(' parameter_list ')' FLOW IDENTIFIER '(' parameter_list ')' FLOW  parameter_declaration  compound_statement    {vector<Parameter *> *p = new vector<Parameter*>(); p->push_back($11); $$ = new FunctionDeclaration($6,$3,$8,p,$12);}
    | FUNCTION '(' parameter_list ')' FLOW IDENTIFIER '(' ')' FLOW  parameter_declaration  compound_statement                   {vector<Parameter *> *p = new vector<Parameter*>(); p->push_back($10); $$ = new FunctionDeclaration($6,$3,nullptr,p,$11);}
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
    if (yyin == nullptr) {
        puts("error: couldn't open the file");
        return 0;
    }
	yyparse(); 
}
