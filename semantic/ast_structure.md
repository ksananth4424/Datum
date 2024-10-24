This file is explain the node structure of the AST

START:
Just contains the list ( vector ) of Function declarations and a statementList ( vector of statements)

FunctionCall:
1)char* identifier;,InbuiltFunctions inbuiltFunc; as the name of the function could be custom one or inbuilt.
2)vector<Argument *> *argumentList; List of arguments.

NOTE: Argument is different from Parameter(just a declaration of variable) as argument could contain various expressions and statements.

SingleChainExpression:
1)Since every single chain expression starts with a variable, a[0]-> it contains     
char* identifier; used to store the variable
vector<Expression*> *access, used to store the list of accesses, a[0][2][1], it would store a vector of each one of them inside it.
2)vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList;
Rest all in the chain can be interpreted as function calls followed by access, ->show()[0], so a pair is stored containing the function call followed by the access list.

MultiChainExpression:
Same as SingleChainExpression, only base case is different.
    FunctionCall* functionCall; ->as it could start with a function call
    pair<char*,vector<Expression*>*> functionCallStart;
    InbuiltFunctions inbuiltFunc; -> to cover inbuilt functions
    vector<Expression*> *access; ->access list


LOOP Statements:
We have multiple ranges to iterate over. Each range is of the form FROM a TO b STEP C, where FROM,TO and STEP are optional.
So the attributes stores  
1)string identifier; - The iterator variable. eg: i in LOOP i FROM 0 TO 10.
2)vector<tuple<class Expression *, class Expression *,class Expression *>> *fromToPairs;
The first expression would be the START VALUE (FROM), second expression - END VALUE (TO), third expression - STEP VALUE.
3)Statement List - List of Statements in the body.

CONDITIONAL Statements:
Each condition can be mapped with body (statement list) to be executed if valid. It stores vector<pair<class Expression *, vector<class Statement *>>> *ConditionStatements;
In each pair the Expression would be the condition and the statement list would be the body to execute if the condition is valid.
->The first entry in vector would be the IF.
->The last entry is an ELSE only if has NULL in place of the condition
->The rest of the pairs in between are ELSE IF.

BREAK STATEMENT, CONTINUE STATEMENT don't store any attribute. The node itself a statement.

RETURN STATEMENT - It can store a return value which is an expression.




NOTE: Every attribute is a pointer except ENUMS.