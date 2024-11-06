#include "ast.hpp"

Node::Node() {

}

string Node::get_scope() {
    return Node::scope.substr(0, scope.find_last_of('.')); 
}

Start::Start(vector<Statement *> *StatementList, int row, int column) {
    this->StatementList = StatementList;
    this->FunctionList = new vector<FunctionDeclaration*>();
    this->row = row;
    this->column = column;
}

Start::Start(vector<FunctionDeclaration *> *FunctionList, int row, int column) {
    this->FunctionList = FunctionList;
    this->StatementList = new vector<Statement*>();
    this->row = row;
    this->column = column;
}

Start::Start(vector<FunctionDeclaration *> *FunctionList, vector<Statement *> *StatementList, int row, int column) {
    this->FunctionList = FunctionList;
    this->StatementList = StatementList;
    this->row = row;
    this->column = column;
}

//Expression
Expression::Expression() {

}

//BinaryExpression
BinaryExpression::BinaryExpression(Expression *lhs, Expression *rhs, BinaryOperator op, int row, int column) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->op = op;
    this->row = row;
    this->column = column;
}

//UnaryExpression
UnaryExpression::UnaryExpression(Expression *expr, vector<UnaryOperator> *op, ConstantValue *constantValue, InbuiltFunctions inbuiltFunction, int row, int column) {
    this->expr = expr;
    this->op = op;
    this->constantValue = constantValue;
    this->inbuiltFunction = inbuiltFunction;
    this->row = row;
    this->column = column;
}

UnaryExpression::UnaryExpression(ConstantValue *constantValue, int row, int column) {
    this->expr = nullptr;
    this->op = new vector<UnaryOperator>();
    this->constantValue = constantValue;
    this->inbuiltFunction = InbuiltFunctions::none;
    this->row = row;
    this->column = column;
}

UnaryExpression::UnaryExpression(Expression *expr, int row, int column) {
    this->expr = expr;
    this->op = new vector<UnaryOperator>();
    this->constantValue = nullptr;
    this->row = row;
    this->column = column;
}

//ConstantValue
ConstantValue::ConstantValue(TypeSpecifier *type, int ival, int row, int column){
    this->type = type;
    this->ival = ival;
    this->row = row;
    this->column = column;
}

ConstantValue::ConstantValue(TypeSpecifier *type, float fval, int row, int column){
    this->type = type;
    this->fval = fval;
    this->row = row;
    this->column = column;
}

ConstantValue::ConstantValue(TypeSpecifier *type, char cval, int row, int column){
    this->type = type;
    this->cval = cval;
    this->row = row;
    this->column = column;
}

ConstantValue::ConstantValue(TypeSpecifier *type, bool bval, int row, int column){
    this->type = type;
    this->bval = bval;
    this->row = row;
    this->column = column;
}

ConstantValue::ConstantValue(TypeSpecifier *type, char* sval, int row, int column){
    this->type = type;
    this->sval = sval;
    this->row = row;
    this->column = column;
}

//TypeSpecifier
TypeSpecifier::TypeSpecifier(vector<int>*type, int row, int column){
    this->type = type;
    this->row = row;
    this->column = column;
}

//Initializer
Initializer::Initializer(AssignmentStatement *assignmentExpression, int row, int column) {
    this->assignmentExpression = assignmentExpression;
    this->initializerList = new vector<Initializer *>();
    this->row = row;
    this->column = column;
}

Initializer::Initializer(vector<Initializer *> *initializerList, int row, int column) {
    this->initializerList = initializerList;
    this->assignmentExpression = nullptr;
    this->row = row;
    this->column = column;
}

//Declarator
Declarator::Declarator(char* identifier, int row, int column) {
    this->identifier = identifier;
    this->row = row;
    this->column = column;
}


//InitDeclaration
InitDeclaration::InitDeclaration(Declarator *declarator, int row, int column) {
    this->declarator = declarator;
    this->initializer = nullptr;
    this->row = row;
    this->column = column;
}

InitDeclaration::InitDeclaration(Declarator *declarator, Initializer *initializer, int row, int column) {
    this->declarator = declarator;
    this->initializer = initializer;
    this->row = row;
    this->column = column;
}

//Parameter
Parameter::Parameter(TypeSpecifier *type, Declarator* identifier, int row, int column) {
    this->type = type;
    this->identifier = identifier;
    this->row = row;
    this->column = column;
}

//Argument
Argument::Argument(Expression *expression, int row, int column) {
    this->expression = expression;
    this->row = row;
    this->column = column;
}

Argument::Argument(vector<tuple<Expression *, Expression *, Expression *>> *fromToAlsoExpression, int row, int column) {
    this->fromToAlsoExpression = fromToAlsoExpression;
    this->row = row;
    this->column = column;
}

Argument::Argument(vector<Statement *> *statements, int row, int column) {
    this->statements = statements;
    this->row = row;
    this->column = column;
}

//FunctionCall
FunctionCall::FunctionCall(char* identifier, vector<Argument *> *argumentList, int row, int column) {
    this->identifier = identifier;
    this->argumentList = argumentList;
    this->row = row;
    this->column = column;
}

FunctionCall::FunctionCall(InbuiltFunctions inbuiltFunc, vector<Argument *> *argumentList, int row, int column) {
    this->inbuiltFunc = inbuiltFunc;
    this->argumentList = argumentList;
    this->row = row;
    this->column = column;
}

//FunctionDeclaration
FunctionDeclaration::FunctionDeclaration(char* identifier, vector<Parameter *> *inpParameter, vector<Parameter *> *otherParameter, vector<Parameter *> *outParameter, vector<Statement *> *statements, int row, int column) {
    this->identifier = identifier;
    this->inpParameter = inpParameter;
    this->otherParameter = otherParameter;
    this->outParameter = outParameter;
    this->statements = statements;
    this->row = row;
    this->column = column;
}

//SingleChainExpression
SingleChainExpression::SingleChainExpression(char* identifier, vector<Expression *> *access, vector<pair<FunctionCall *, vector<Expression *> *>> *functionCallList, int row, int column) {
    this->identifier = identifier;
    this->access = access;
    this->functionCallList = functionCallList;
    this->row = row;
    this->column = column;
}

//MultiChainExpression
MultiChainExpression::MultiChainExpression(FunctionCall* functionCall, vector<Expression*> *access, vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList, int row, int column){
    this->functionCall = functionCall;
    this->access = access;
    this->functionCallList = functionCallList;
    this->row = row;
    this->column = column;
    this->inbuiltFunc = InbuiltFunctions::none;
}

MultiChainExpression::MultiChainExpression(InbuiltFunctions inbuiltFunc, vector<Expression*> *access, vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList, int row, int column){
    this->inbuiltFunc = inbuiltFunc;
    this->access = access;
    this->functionCallList = functionCallList;
    this->row = row;
    this->column = column;
}
MultiChainExpression::MultiChainExpression(vector<Expression*> *inputExprList, vector<Expression*> *access,  vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList, int row, int column){
    this->access = access;
    this->inputExprList = inputExprList;    
    this->functionCallList = functionCallList;
    this->row = row;
    this->column = column;
    this->inbuiltFunc = InbuiltFunctions::none;

}

// Statements
Statement::Statement(DeclarationStatement *expression, int row, int column)  {
    this->declarationStatement = expression;
    this->statementType = 1;
    this->row = row;
    this->column = column;
}

Statement::Statement(AssignmentStatement *assignmentStatement, int row, int column)  {
    this->assignmentStatement = assignmentStatement;
    this->statementType = 2;
    this->row = row;
    this->column = column;
}

Statement::Statement(ConditionalStatement *conditionalStatement, int row, int column)  {
    this->conditionalStatement = conditionalStatement;
    this->statementType = 3;
    this->row = row;
    this->column = column;
}

Statement::Statement(LoopStatement *loopStatement, int row, int column)  {
    this->loopStatement = loopStatement;
    this->statementType = 4;
    this->row = row;
    this->column = column;
}

Statement::Statement(ReturnStatement *returnStatement, int row, int column)  {
    this->returnStatement = returnStatement;
    this->statementType = 5;
    this->row = row;
    this->column = column;
}

Statement::Statement(BreakStatement *breakStatement, int row, int column)  {
    this->breakStatement = breakStatement;
    this->statementType = 6;
    this->row = row;
    this->column = column;
}

Statement::Statement(ContinueStatement *continueStatement, int row, int column)  {
    this->continueStatement = continueStatement;
    this->statementType = 7;
    this->row = row;
    this->column = column;
}

Statement::Statement(vector<Statement*>* compoundStatement, int row, int column){
    this->compoundStatement = compoundStatement;
    this->statementType = 8;
    this->row = row;
    this->column = column;
}

ContinueStatement::ContinueStatement()  {

}

BreakStatement::BreakStatement()  {

}

ReturnStatement::ReturnStatement()  {
    this->expression = nullptr;
}

ReturnStatement::ReturnStatement(Expression *expression, int row, int column)  {
    this->expression = expression;
    this->row = row;
    this->column = column;
}

DeclarationStatement::DeclarationStatement(TypeSpecifier *type, int row, int column){
    this->type = type;
    this->initDeclarations = nullptr;
    this->row = row;
    this->column = column;
}

DeclarationStatement::DeclarationStatement(TypeSpecifier *type, vector<class InitDeclaration *> *initDeclarations, int row, int column) {
    this->type = type;
    this->initDeclarations = initDeclarations;
    this->row = row;
    this->column = column;
}

LoopStatement::LoopStatement(Declarator* variable, vector<tuple<class Expression *, class Expression *, class Expression *>> *fromToPairs, vector<class Statement *> *statements, int row, int column) {
    this->variable = variable;    
    this->fromToPairs = fromToPairs;
    this->statements = statements;
    this->row = row;
    this->column = column;
}

//Conditional Statement
ConditionalStatement::ConditionalStatement(vector<pair<class Expression *, vector<class Statement *>*>> *ConditionStatements, int row, int column){
    this->ConditionStatements = ConditionStatements;
    this->row = row;
    this->column = column;
}

//Assignment Statement
AssignmentStatement::AssignmentStatement(SingleChainExpression *declarator, Expression *expression, AssignmentOperator op, int row, int column){
    this->declarator = declarator;
    this->expression = expression;
    this->op = op;
    this->row = row;
    this->column = column;
}

AssignmentStatement::AssignmentStatement(Expression *expression, int row, int column){
    this->declarator = nullptr;
    this->expression = expression;
    // this->op = op;
    this->row = row;
    this->column = column;
}