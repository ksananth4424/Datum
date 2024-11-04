#include "ast.hpp"

Node::Node() {

}

string Node::get_scope() {
    return Node::scope.substr(0, scope.find_last_of('.')); 
}

Start::Start(vector<Statement *> *StatementList) {
    this->StatementList = StatementList;
    this->FunctionList = new vector<FunctionDeclaration*>();
}

Start::Start(vector<FunctionDeclaration *> *FunctionList) {
    this->FunctionList = FunctionList;
    this->StatementList = new vector<Statement*>();

}

Start::Start(vector<FunctionDeclaration *> *FunctionList, vector<Statement *> *StatementList) {
    this->FunctionList = FunctionList;
    this->StatementList = StatementList;
}

//Expression
Expression::Expression() {

}

//BinaryExpression
BinaryExpression::BinaryExpression(Expression *lhs, Expression *rhs, BinaryOperator op) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->op = op;
}

//UnaryExpression
UnaryExpression::UnaryExpression(Expression *expr, vector<UnaryOperator> *op, ConstantValue *constantValue, InbuiltFunctions inbuiltFunction) {
    this->expr = expr;
    this->op = op;
    this->constantValue = constantValue;
    this->inbuiltFunction = inbuiltFunction;
}
UnaryExpression::UnaryExpression(ConstantValue *constantValue) {
    this->expr = nullptr;
    this->op = new vector<UnaryOperator>();
    this->constantValue = constantValue;
}

UnaryExpression::UnaryExpression(Expression *expr) {
    this->expr = expr;
    this->op = new vector<UnaryOperator>();
    this->constantValue = nullptr;
}
//ConstantValue
ConstantValue::ConstantValue(TypeSpecifier *type, int ival){
    this->type = type;
    this->ival = ival;
}

ConstantValue::ConstantValue(TypeSpecifier *type, float fval){
    this->type = type;
    this->fval = fval;
}

ConstantValue::ConstantValue(TypeSpecifier *type, char cval){
    this->type = type;
    this->cval = cval;
}

ConstantValue::ConstantValue(TypeSpecifier *type, bool bval){
    this->type = type;
    this->bval = bval;
}

ConstantValue::ConstantValue(TypeSpecifier *type, char* sval){
    this->type = type;
    this->sval = sval;
}

//TypeSpecifier
TypeSpecifier::TypeSpecifier(vector<int>*type){
    this->type = type;
}
//Initializer
Initializer::Initializer(AssignmentStatement *assignmentExpression) {
    this->assignmentExpression = assignmentExpression;
    this->initializerList = new vector<Initializer *>();
}

Initializer::Initializer(vector<Initializer *> *initializerList) {
    this->initializerList = initializerList;
    this->assignmentExpression = nullptr;
}

//Declarator
Declarator::Declarator(char* identifier) {
    this->identifier = identifier;
}


//InitDeclaration
InitDeclaration::InitDeclaration(Declarator *declarator) {
    this->declarator = declarator;
    this->initializer = nullptr;
}

InitDeclaration::InitDeclaration(Declarator *declarator, Initializer *initializer) {
    this->declarator = declarator;
    this->initializer = initializer;
}

//Parameter
Parameter::Parameter(TypeSpecifier *type, Declarator* identifier) {
    this->type = type;
    this->identifier = identifier;
}

//Argument
Argument::Argument(Expression *expression) {
    this->expression = expression;
}
Argument::Argument(vector<tuple<Expression *, Expression *, Expression *>> *fromToAlsoExpression) {
    this->fromToAlsoExpression = fromToAlsoExpression;
}
Argument::Argument(vector<Statement *> *statements) {
    this->statements = statements;
}

//FunctionCall
FunctionCall::FunctionCall(char* identifier, vector<Argument *> *argumentList) {
    this->identifier = identifier;
    this->argumentList = argumentList;
}
FunctionCall::FunctionCall(InbuiltFunctions inbuiltFunc, vector<Argument *> *argumentList) {
    this->inbuiltFunc = inbuiltFunc;
    this->argumentList = argumentList;
}

//FunctionDeclaration
FunctionDeclaration::FunctionDeclaration(char* identifier, vector<Parameter *> *inpParameter, vector<Parameter *> *otherParameter, vector<Parameter *> *outParameter, vector<Statement *> *statements) {
    this->identifier = identifier;
    this->inpParameter = inpParameter;
    this->otherParameter = otherParameter;
    this->outParameter = outParameter;
    this->statements = statements;
}

//SingleChainExpression
SingleChainExpression::SingleChainExpression(char* identifier, vector<Expression *> *access, vector<pair<FunctionCall *, vector<Expression *> *>> *functionCallList) {
    this->identifier = identifier;
    this->access = access;
    this->functionCallList = functionCallList;
}

//MultiChainExpression
MultiChainExpression::MultiChainExpression(FunctionCall* functionCall, vector<Expression*> *access, vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList) {
    this->functionCall = functionCall;
    this->access = access;
    this->functionCallList = functionCallList;
    this->inbuiltFunc = InbuiltFunctions::none;
}
MultiChainExpression::MultiChainExpression(InbuiltFunctions inbuiltFunc, vector<Expression*> *access, vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList){
    this->inbuiltFunc = inbuiltFunc;
    this->access = access;
    this->functionCallList = functionCallList;
}
MultiChainExpression::MultiChainExpression(pair<char*,vector<Expression*>*> functionCallStart, vector<Expression*> *access,  vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList){
    this->functionCallStart = functionCallStart;
    this->access = access;
    this->functionCallList = functionCallList;
    this->inbuiltFunc = InbuiltFunctions::none;

}

// Statements
Statement::Statement(DeclarationStatement *declarationStatement)  {
    this->declarationStatement = declarationStatement;
    this->statementType = 1;
}

Statement::Statement(AssignmentStatement *assignmentStatement)  {
    this->assignmentStatement = assignmentStatement;
    this->statementType = 2;
}

Statement::Statement(ConditionalStatement *conditionalStatement)  {
    this->conditionalStatement = conditionalStatement;
    this->statementType = 3;
}

Statement::Statement(LoopStatement *loopStatement)  {
    this->loopStatement = loopStatement;
    this->statementType = 4;
}

Statement::Statement(ReturnStatement *returnStatement)  {
    this->returnStatement = returnStatement;
    this->statementType = 5;
}

Statement::Statement(BreakStatement *breakStatement)  {
    this->breakStatement = breakStatement;
    this->statementType = 6;
}

Statement::Statement(ContinueStatement *continueStatement)  {
    this->continueStatement = continueStatement;
    this->statementType = 7;
}

Statement::Statement(vector<Statement*>* compoundStatement){
    this->compoundStatement = compoundStatement;
    this->statementType = 8;
}

ContinueStatement::ContinueStatement()  {

}

BreakStatement::BreakStatement()  {

}

ReturnStatement::ReturnStatement()  {
    this->expression = nullptr;
}

ReturnStatement::ReturnStatement(Expression *expression)  {
    this->expression = expression;
}

DeclarationStatement::DeclarationStatement(TypeSpecifier *type){
    this->type = type;
    this->initDeclarations = nullptr;
}

DeclarationStatement::DeclarationStatement(TypeSpecifier *type, vector<class InitDeclaration *> *initDeclarations) {
    this->type = type;
    this->initDeclarations = initDeclarations;
}

LoopStatement::LoopStatement(char* identifier, vector<tuple<class Expression *, class Expression *, class Expression *>> *fromToPairs, vector<class Statement *> *statements) {
    this->identifier = identifier;
    this->fromToPairs = fromToPairs;
    this->statements = statements;
}

//Conditional Statement
ConditionalStatement::ConditionalStatement(vector<pair<class Expression *, vector<class Statement *>*>> *ConditionStatements){
    this->ConditionStatements = ConditionStatements;
}

//Assignment Statement
AssignmentStatement::AssignmentStatement(SingleChainExpression *declarator, Expression *expression, AssignmentOperator op){
    this->declarator = declarator;
    this->expression = expression;
    this->op = op;
}

AssignmentStatement::AssignmentStatement(Expression *expression){
    this->declarator = nullptr;
    this->expression = expression;
    // this->op = op;
}