/* %code requires{
    #include "../semantic/ast.hpp"
} */

%{
    #include <iostream>
    #include <fstream>
    #include "../semantic/ast.hpp"
    #include "../semantic/symbol_table.hpp"
    #include "../semantic/traversal.hpp"
    #include "../semantic/ast.cpp"
    #include "../semantic/symbol_table.cpp"
    #include "../semantic/traversal.cpp"

    class Start* root;
    extern int yylex();
    extern void yyerror(const char*);
    // SymbolTable symtab;
    int lineno;
    int yycolumn;
%}

%union{
    int ival;
    float fval;
    char cval;
    bool bval;
    char* sval;
    char* strval;

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
    vector<pair<Expression*,vector<Statement*>*>> *AstConditionalStatementList;
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
%token<strval> STRING_LITERAL
%token<sval> IDENTIFIER
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
    puts("<-----------Program is syntactically correct--------->");
} 
//parsing starts from here this triggers 'program'.

program
    : 
    FUNC_LABEL functions START_LABEL statement_list       {$$ = new Start($2, $4, lineno, yycolumn); }
    | FUNC_LABEL functions START_LABEL                      {$$ = new Start($2, lineno, yycolumn); }
    | FUNC_LABEL START_LABEL statement_list                 {$$ = new Start($3, lineno, yycolumn); }
    | START_LABEL statement_list                            {$$ = new Start($2, lineno, yycolumn); }
    ;

// these are all the funtions which will be supported by the DSL i.e. they will be in-build functions
inbuilt_function 
    : SHOW_BAR          { $$ = InbuiltFunctions::func_show_bar; }
    | SHOW_LINE         { $$ = InbuiltFunctions::func_show_line; }
    | SHOW_SCATTER      { $$ = InbuiltFunctions::func_show_scatter; }
    | SHOW_BOX          { $$ = InbuiltFunctions::func_show_box; }
    | ROW               { $$ = InbuiltFunctions::func_row; }
    | COL               { $$ = InbuiltFunctions::func_col; }
    | FILTER            { $$ = InbuiltFunctions::func_filter; }
    | SUM               { $$ = InbuiltFunctions::func_sum; }
    | MAX               { $$ = InbuiltFunctions::func_max; }
    | MIN               { $$ = InbuiltFunctions::func_min; }
    | MEAN              { $$ = InbuiltFunctions::func_mean; }
    | JOIN              { $$ = InbuiltFunctions::func_join; }
    | READ              { $$ = InbuiltFunctions::func_read; }
    | WRITE             { $$ = InbuiltFunctions::func_write; }
    | UNIQUE            { $$ = InbuiltFunctions::func_unique; }
    | SHOW              { $$ = InbuiltFunctions::func_show; }
    | SPLIT             { $$ = InbuiltFunctions::func_split; }
    | SORT              { $$ = InbuiltFunctions::func_sort; }
    | SHUFFLE           { $$ = InbuiltFunctions::func_shuffle; }
    | ADD               { $$ = InbuiltFunctions::func_add; }
    | SHAPE             { $$ = InbuiltFunctions::func_shape; }
    | DROP              { $$ = InbuiltFunctions::func_drop; }
    ;

// grammer for make variable declarations, all declarations must end with ';' 
declaration 
    : type_specifier ';'                                    {$$ = new DeclarationStatement($1, lineno, yycolumn);}
    | type_specifier init_declarator_list ';'               {$$ = new DeclarationStatement($1, $2, lineno, yycolumn);}
    /* | ARRAY '(' type_specifier ')' ';'                      {TypeSpecifier* type = new TypeSpecifier(new vector<int>{6}, lineno, yycolumn); type->type->push_back($3->type->at(0)); $$ = new DeclarationStatement(type,lineno, yycolumn);} */
    /* | ARRAY '(' type_specifier ')' init_declarator_list ';' {TypeSpecifier* type = new TypeSpecifier(new vector<int>{6}, lineno, yycolumn); type->type->push_back($3->type->at(0)); $$ = new DeclarationStatement(type, $5, lineno, yycolumn);} */
    ;

// all the types which can be used for the above "declarations"
type_specifier 
    : INTEGER                       {$$ = new TypeSpecifier(new vector<int>{0}, lineno, yycolumn);}   
    | FLOAT                         {$$ = new TypeSpecifier(new vector<int>{1}, lineno, yycolumn);}
    | STRING                        {$$ = new TypeSpecifier(new vector<int>{2}, lineno, yycolumn);}
    | CHAR                          {$$ = new TypeSpecifier(new vector<int>{3}, lineno, yycolumn);}
    | BOOL                          {$$ = new TypeSpecifier(new vector<int>{4}, lineno, yycolumn);}
    | DATASET                       {$$ = new TypeSpecifier(new vector<int>{5}, lineno, yycolumn);}
    | ARRAY '(' INTEGER ')'         {$$ = new TypeSpecifier(new vector<int>{7}, lineno, yycolumn);}
    | ARRAY '(' FLOAT ')'           {$$ = new TypeSpecifier(new vector<int>{8}, lineno, yycolumn);}
    | ARRAY '(' STRING ')'          {$$ = new TypeSpecifier(new vector<int>{9}, lineno, yycolumn);}
    | ARRAY '(' CHAR ')'            {$$ = new TypeSpecifier(new vector<int>{10}, lineno, yycolumn);}
    | ARRAY '(' BOOL ')'            {$$ = new TypeSpecifier(new vector<int>{11}, lineno, yycolumn);}
    | ARRAY '(' DATASET ')'         {$$ = new TypeSpecifier(new vector<int>{12}, lineno, yycolumn);}
    /* | ARRAY '(' type_specifier ')'  { $$ = $3; $$->type->insert($$->type->begin(),6); } */
    ;

// for declaring multiple variables of same type in one declaration.
init_declarator_list 
    : init_declarator                           {$$ = new vector<InitDeclaration*>(); $$->push_back($1);}
    | init_declarator_list ',' init_declarator  {$$ = $1; $1->push_back($3);}
    ;

// for initializing while declaring variables
init_declarator                     
	: declarator                    {$$ = new InitDeclaration($1, lineno, yycolumn);}
	| declarator '=' initializer    {$$ = new InitDeclaration($1, $3, lineno, yycolumn);}
	;

// TERMINAL => indentifier
declarator 
    : IDENTIFIER                {char* concat = (char*)malloc(strlen($1)+strlen("Hello")); strcpy(concat, $1); strcat(concat, "Hello"); $$ = new Declarator($1, lineno, yycolumn);}  
    ;

// for initializing variables
initializer     
    : assignment_expression     {$$ = new Initializer($1, lineno, yycolumn);}
	| '{' initializer_list '}'  {$$ = new Initializer($2, lineno, yycolumn);}
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
	: type_specifier declarator     {$$ = new Parameter($1, $2, lineno, yycolumn);}
	| type_specifier                {$$ = new Parameter($1, nullptr, lineno, yycolumn);}
	;


// intuitive
primary_expression 
    : CONST_INTEGER           { $$ = new ConstantValue(new TypeSpecifier(new vector<int>{0}, lineno, yycolumn),$1, lineno, yycolumn); }
    | CONST_FLOAT             { $$ = new ConstantValue(new TypeSpecifier(new vector<int>{1}, lineno, yycolumn),$1, lineno, yycolumn); }
    | CONST_CHAR              { $$ = new ConstantValue(new TypeSpecifier(new vector<int>{3}, lineno, yycolumn),$1, lineno, yycolumn); }
    | STRING_LITERAL          { $$ = new ConstantValue(new TypeSpecifier(new vector<int>{2}, lineno, yycolumn),$1, lineno, yycolumn); }
    | BOOL_TRUE               { $$ = new ConstantValue(new TypeSpecifier(new vector<int>{4}, lineno, yycolumn),$1, lineno, yycolumn); }
    | BOOL_FALSE              { $$ = new ConstantValue(new TypeSpecifier(new vector<int>{5}, lineno, yycolumn),$1, lineno, yycolumn); }
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
    : expression                { $$ = new Argument($1, lineno, yycolumn); }
    | from_to_also_expression   { $$ = new Argument($1, lineno, yycolumn); }
    | compound_statement        { $$ = new Argument($1, lineno, yycolumn); }
    ;

// e.g {variable_name}.{function_name}(expression) 
single_chain_expression 
	: IDENTIFIER                                                                                { $$ = new SingleChainExpression($1,new vector<Expression*>(),new vector<pair<FunctionCall*,vector<Expression*>*>>(), lineno, yycolumn); }
	| IDENTIFIER access_list                                                                    { $$ = new SingleChainExpression($1,$2,new vector<pair<FunctionCall*,vector<Expression*>*>>(), lineno, yycolumn); }
    | single_chain_expression FLOW IDENTIFIER '(' ')'                                           { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,new vector<Argument*>(), lineno, yycolumn),new vector<Expression*>())); }
    | single_chain_expression FLOW IDENTIFIER '(' ')' access_list                               { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,new vector<Argument*>(), lineno, yycolumn),$6)); }
    | single_chain_expression FLOW IDENTIFIER '(' argument_list ')'                             { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,$5, lineno, yycolumn),new vector<Expression*>())); }
    | single_chain_expression FLOW IDENTIFIER '(' argument_list ')' access_list                 { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,$5, lineno, yycolumn),$7)); }
    | single_chain_expression FLOW inbuilt_function '(' ')'                                     { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,new vector<Argument*>(), lineno, yycolumn),new vector<Expression*>())); }
    | single_chain_expression FLOW inbuilt_function '(' ')' access_list                         { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,new vector<Argument*>(), lineno, yycolumn),$6)); }
    | single_chain_expression FLOW inbuilt_function '(' argument_list ')'                       { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,$5, lineno, yycolumn),new vector<Expression*>())); }
    | single_chain_expression FLOW inbuilt_function '(' argument_list ')' access_list           { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,$5, lineno, yycolumn),$7)); }
    ;

access_list
    : '[' expression ']'                { $$ = new vector<Expression*>(); $$->push_back($2); }
    | access_list '[' expression ']'    { $$ = $1; $1->push_back($3); }
    ;

// e.g {variable_name}.{function_name}(expression).{function_name}(expression)
multi_chain_expression 
	: '(' expression_list ')'                                                               { $$ = new MultiChainExpression($2,new vector<Expression*>(), new vector<pair<FunctionCall*, vector<Expression*>*>>(), lineno, yycolumn); }
	| '(' expression_list ')' access_list                                                   { $$ = new MultiChainExpression($2,$4, new vector<pair<FunctionCall*, vector<Expression*>*>>(), lineno, yycolumn); }
    | multi_chain_expression FLOW IDENTIFIER '(' ')'                                        { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,new vector<Argument*>(), lineno, yycolumn),nullptr)); }
    | multi_chain_expression FLOW IDENTIFIER '(' ')' access_list                            { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,new vector<Argument*>(), lineno, yycolumn),$6)); }
    | multi_chain_expression FLOW IDENTIFIER '(' argument_list ')'                          { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,$5, lineno, yycolumn),nullptr)); }
    | multi_chain_expression FLOW IDENTIFIER '(' argument_list ')' access_list              { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,$5, lineno, yycolumn),$7)); }
    | multi_chain_expression FLOW inbuilt_function '(' ')'                                  { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,new vector<Argument*>(), lineno, yycolumn),nullptr)); }
    | multi_chain_expression FLOW inbuilt_function '(' ')' access_list                      { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,new vector<Argument*>(), lineno, yycolumn),$6)); }
    | multi_chain_expression FLOW inbuilt_function '(' argument_list ')'                    { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,$5, lineno, yycolumn),nullptr)); }
    | multi_chain_expression FLOW inbuilt_function '(' argument_list ')' access_list        { $$ = $1; $$->functionCallList->push_back(make_pair(new FunctionCall($3,$5, lineno, yycolumn),$7)); }
    | IDENTIFIER '('  ')'                                                                   { $$ = new MultiChainExpression(new FunctionCall($1,new vector<Argument*>(), lineno, yycolumn),new vector<Expression*>(),new vector<pair<FunctionCall *, vector<Expression*>*>>(), lineno, yycolumn); }                
    | IDENTIFIER '('  ')' access_list                                                       { $$ = new MultiChainExpression(new FunctionCall($1,new vector<Argument*>(), lineno, yycolumn),$4,new vector<pair<FunctionCall *, vector<Expression*>*>>(), lineno, yycolumn); }
    | IDENTIFIER '(' argument_list ')'                                                      { $$ = new MultiChainExpression(new FunctionCall($1,$3, lineno, yycolumn),new vector<Expression*>(), new vector<pair<FunctionCall *, vector<Expression*>*>>(), lineno, yycolumn); }
    | IDENTIFIER '(' argument_list ')' access_list                                          { $$ = new MultiChainExpression(new FunctionCall($1,$3, lineno, yycolumn),$5,new vector<pair<FunctionCall *, vector<Expression*>*>>(), lineno, yycolumn); }
    | inbuilt_function '('  ')'                                                             { $$ = new MultiChainExpression(new FunctionCall($1,new vector<Argument*>(), lineno, yycolumn),new vector<Expression*>(),new vector<pair<FunctionCall *, vector<Expression*>*>>(), lineno, yycolumn); }              
    | inbuilt_function '('  ')' access_list                                                 { $$ = new MultiChainExpression(new FunctionCall($1,new vector<Argument*>(), lineno, yycolumn),$4,new vector<pair<FunctionCall *, vector<Expression*>*>>(), lineno, yycolumn); }
    | inbuilt_function '(' argument_list ')'                                                { $$ = new MultiChainExpression(new FunctionCall($1,$3, lineno, yycolumn),new vector<Expression*>(),new vector<pair<FunctionCall *, vector<Expression*>*>>(), lineno, yycolumn); }
    | inbuilt_function '(' argument_list ')' access_list                                    { $$ = new MultiChainExpression(new FunctionCall($1,$3, lineno, yycolumn),$5,new vector<pair<FunctionCall *, vector<Expression*>*>>(), lineno, yycolumn); }
    ;

 // this includes both single chain and multi chain expressions
postfix_expression
    : single_chain_expression   { $$ = $1;$$->castType = 3; }
    | multi_chain_expression    { $$ = $1;$$->castType = 4; }
    ;

// this is how unary operators are used
unary_expression        
	: unary_operator unary_expression   { $$ = $2; $$->op->push_back($1);}
    | primary_expression        { $$ = new UnaryExpression($1, lineno, yycolumn); $$->castType = 1; }
    | postfix_expression            { $$ = new UnaryExpression($1, lineno, yycolumn); $$->castType = $1->castType; }   
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
	| expression '*' expression     { $$ = new BinaryExpression($1, $3, BinaryOperator::mul_op, lineno, yycolumn); $$->castType = 2; }
	| expression '/' expression     { $$ = new BinaryExpression($1, $3, BinaryOperator::div_op, lineno, yycolumn); $$->castType = 2; }
	| expression '%' expression     { $$ = new BinaryExpression($1, $3, BinaryOperator::mod_op, lineno, yycolumn); $$->castType = 2; }
	| expression '+' expression     { $$ = new BinaryExpression($1, $3, BinaryOperator::add_op, lineno, yycolumn); $$->castType = 2; }
	| expression '-' expression     { $$ = new BinaryExpression($1, $3, BinaryOperator::sub_op, lineno, yycolumn); $$->castType = 2; }
	| expression '>' expression     { $$ = new BinaryExpression($1, $3, BinaryOperator::gt_op, lineno, yycolumn); $$->castType = 2; }
	| expression '<' expression     { $$ = new BinaryExpression($1, $3, BinaryOperator::lt_op, lineno, yycolumn); $$->castType = 2; }
	| expression LE_OP expression   { $$ = new BinaryExpression($1, $3, BinaryOperator::lte_op, lineno, yycolumn); $$->castType = 2; }
	| expression GE_OP expression   { $$ = new BinaryExpression($1, $3, BinaryOperator::gte_op, lineno, yycolumn); $$->castType = 2; }
	| expression EQ_OP expression   { $$ = new BinaryExpression($1, $3, BinaryOperator::eq_op, lineno, yycolumn); $$->castType = 2; }
	| expression NE_OP expression   { $$ = new BinaryExpression($1, $3, BinaryOperator::ne_op, lineno, yycolumn); $$->castType = 2; }
	| expression AND expression     { $$ = new BinaryExpression($1, $3, BinaryOperator::and_op, lineno, yycolumn); $$->castType = 2; }
	| expression OR expression      { $$ = new BinaryExpression($1, $3, BinaryOperator::or_op, lineno, yycolumn); $$->castType = 2; }
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
	: single_chain_expression assignment_operator expression  { $$ = new AssignmentStatement($1, $3, $2, lineno, yycolumn); }
	| expression                                              { $$ = new AssignmentStatement($1, lineno, yycolumn); }
	;

// this is step for loop, it can be empty or can have a step
optional_step
    :                   {$$ = nullptr;}
    | STEP expression   {$$ = $2;}
    ;

// this and next grammer declarations are for the use of if-else statements
conditional_statement 
	: IF '(' expression ')' compound_statement else_if_statement                            {reverse($6->begin(), $6->end()); $6->push_back(make_pair($3,$5)); reverse($6->begin(), $6->end());  $$ = new ConditionalStatement($6, lineno, yycolumn);}
	| IF '(' expression ')' compound_statement else_if_statement ELSE compound_statement    {$6->push_back({nullptr, $8}); reverse($6->begin(), $6->end()); $6->push_back(make_pair($3,$5)); reverse($6->begin(), $6->end()); $$ = new ConditionalStatement($6, lineno, yycolumn);}
	;
    
// similar to the above understanding
else_if_statement 
    :                                                                        {$$ = new vector<pair<Expression*,vector<Statement*>*>>() ;}
    | else_if_statement ELSE IF '(' expression ')' compound_statement        {$$ = $1; $1->push_back(make_pair($5,$7));}
    ;

// all statements should adhere to '{' & '}'' at end and start respec. 
compound_statement 
    : '{' '}'                   {$$ = new vector<Statement*>();}
    | '{' statement_list '}'    {$$ = $2;}
    ;

// only these can be a statement
statement
    : assignment_expression ';' {$$ = new Statement($1, lineno, yycolumn);}
    | compound_statement        {$$ = new Statement($1, lineno, yycolumn);}
    | conditional_statement     {$$ = new Statement($1, lineno, yycolumn);}
    | loop_statement            {$$ = new Statement($1, lineno, yycolumn);}
    | declaration               {$$ = new Statement($1, lineno, yycolumn);}
    | RETURN expression ';'     {$$ = new Statement(new ReturnStatement($2, lineno, yycolumn), lineno, yycolumn);}
    | RETURN ';'                {$$ = new Statement(new ReturnStatement(), lineno, yycolumn);}
    | BREAK ';'                 {$$ = new Statement(new BreakStatement(), lineno, yycolumn);}
    | CONTINUE ';'              {$$ = new Statement(new ContinueStatement(), lineno, yycolumn);}
    ;

// to handle multiple statements
statement_list 
    : statement                 {$$ = new vector<Statement*>(); $$->push_back($1);}
    | statement_list statement  {$$ = $1; $1->push_back($2);}
    ;

// structure of loops should be adhered to this control of loop is handled by 'from_to_also_expression'
loop_statement 
    : LOOP declarator from_to_also_expression compound_statement    {$$ = new LoopStatement($2, $3, $4, lineno, yycolumn);}
    ;

 // this is how loop control is handled
from_to_also_expression
    : TO expression optional_step                                                   {$$ = new vector<tuple<Expression*,Expression*,Expression*>> (); $$->push_back(make_tuple(nullptr,$2,$3));}
    | FROM expression optional_step                                                 {$$ = new vector<tuple<Expression*,Expression*,Expression*>> (); $$->push_back(make_tuple($2,nullptr,$3));}           
    | FROM expression TO expression optional_step                                   {$$ = new vector<tuple<Expression*,Expression*,Expression*>> (); $$->push_back(make_tuple($2,$4,$5)); }
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
    : FUNCTION '(' parameter_list ')' FLOW IDENTIFIER '(' parameter_list ')' FLOW '(' parameter_list ')' compound_statement     {$$ = new FunctionDeclaration($6,$3,$8,$12,$14, lineno, yycolumn);}
    | FUNCTION '(' parameter_list ')' FLOW IDENTIFIER '(' ')' FLOW '(' parameter_list ')' compound_statement                    {$$ = new FunctionDeclaration($6,$3, new vector<Parameter*>(),$11,$13, lineno, yycolumn);}
    | FUNCTION '(' parameter_list ')' FLOW IDENTIFIER '(' parameter_list ')' FLOW  parameter_declaration  compound_statement    {vector<Parameter *> *p = new vector<Parameter*>(); p->push_back($11); $$ = new FunctionDeclaration($6,$3,$8,p,$12, lineno, yycolumn);}
    | FUNCTION '(' parameter_list ')' FLOW IDENTIFIER '(' ')' FLOW  parameter_declaration  compound_statement                   {vector<Parameter *> *p = new vector<Parameter*>(); p->push_back($10); $$ = new FunctionDeclaration($6,$3,new vector<Parameter*>(),p,$11, lineno, yycolumn);}
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
    if (yyin == nullptr) {
        puts("error: couldn't open the file");
        return 0;
    }
	yyparse(); 
    /* return 0; */
    traverse(root);
    symtab.print();

    if(error_count == 0) {
        puts("<-----------Program is semantically correct--------->\n");
    }
    else if (error_count == 1) {
        printf("Total %d error found\n", error_count);
    }else{
        printf("Total %d errors found\n", error_count);
    }
}
