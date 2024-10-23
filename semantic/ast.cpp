#include "ast.hpp"

Node::Node() {

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
UnaryExpression::UnaryExpression(Expression *expr, vector<UnaryOperator> op, ConstantValue *constantValue, InbuiltFunctions inbuiltFunction) {
    this->expr = expr;
    this->op = op;
    this->constantValue = constantValue;
    this->inbuiltFunction = inbuiltFunction;
}

//ConstantValue
ConstantValue::ConstantValue(TypeSpecifier type, string value) {
    this->type = type;
    this->value = value;
}

//TypeSpecifier
TypeSpecifier::TypeSpecifier() {

}
TypeSpecifier::TypeSpecifier(string type) {
    this->type = type;
}
TypeSpecifier::TypeSpecifier(string type, TypeSpecifier *typeSpecifier) {
    this->type = type;
    this->typeSpecifier = typeSpecifier;
}

//Initializer
Initializer::Initializer(Expression *expression, vector<Initializer *> *initializerList){
    this->expression = expression;
    this->initializerList = initializerList;
}

//Declarator
Declarator::Declarator(string identifier) {
    this->identifier = identifier;
}

//InitDeclaration
InitDeclaration::InitDeclaration(Declarator *declarator, Initializer *initializer) {
    this->declarator = declarator;
    this->initializer = initializer;
}

//Parameter
Parameter::Parameter(TypeSpecifier type, string identifier) {
    this->type = type;
    this->identifier = identifier;
}

//Argument
Argument::Argument(Expression *expression) {
    this->expression = expression;
}
Argument::Argument(vector<tuple<Expression *, Expression *, Expression *>> FromToAlsoExpression) {
    this->FromToAlsoExpression = FromToAlsoExpression;
}
Argument::Argument(vector<Statement *> statements) {
    this->statements = statements;
}

//FunctionCall
FunctionCall::FunctionCall(string identifier, vector<Argument *> *argumentList) {
    this->identifier = identifier;
    this->argumentList = argumentList;
}
FunctionCall::FunctionCall(InbuiltFunctions inbuiltFunc, vector<Argument *> *argumentList) {
    this->inbuiltFunc = inbuiltFunc;
    this->argumentList = argumentList;
}

//FunctionDeclaration
FunctionDeclaration::FunctionDeclaration(string identifier, vector<Parameter *> *inpParameter, vector<Parameter *> *otherParameter, vector<Parameter *> *outParameter, vector<Statement *> *statements) {
    this->identifier = identifier;
    this->inpParameter = inpParameter;
    this->otherParameter = otherParameter;
    this->outParameter = outParameter;
    this->statements = statements;
}

//SingleChainExpression
SingleChainExpression::SingleChainExpression(string identifier, vector<Expression *> *access, vector<pair<FunctionCall *, vector<Expression *> *>> *functionCallList) {
    this->identifier = identifier;
    this->access = access;
    this->functionCallList = functionCallList;
}

//MultiChainExpression
MultiChainExpression::MultiChainExpression(FunctionCall* functionCall, vector<Expression*> *access, vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList) {
    this->functionCall = functionCall;
    this->access = access;
    this->functionCallList = functionCallList;
}
MultiChainExpression::MultiChainExpression(InbuiltFunctions inbuiltFunc, vector<Expression*> *access, vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList){
    this->inbuiltFunc = inbuiltFunc;
    this->access = access;
    this->functionCallList = functionCallList;
}
MultiChainExpression::MultiChainExpression(pair<string,vector<Expression*>*> functionCallStart, vector<Expression*> *access,  vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList){
    this->functionCallStart = functionCallStart;
    this->access = access;
    this->functionCallList = functionCallList;
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

ContinueStatement::ContinueStatement()  {

}

BreakStatement::BreakStatement()  {

}

ReturnStatement::ReturnStatement(Expression *expression)  {
    this->expression = expression;
}

DeclarationStatement::DeclarationStatement(TypeSpecifier type, vector<class InitDeclaration *> *initDeclarations) : type(type) {
    this->initDeclarations = initDeclarations;
}

LoopStatement::LoopStatement(string identifier, vector<tuple<class Expression *, class Expression *, class Expression *>> *fromToPairs, vector<class Statement *> *statements) {
    this->identifier = identifier;
    this->fromToPairs = fromToPairs;
    this->statements = statements;
}

//Conditional Statement
ConditionalStatement::ConditionalStatement(vector<pair<class Expression *, vector<class Statement *>*>> *ConditionStatements){
    this->ConditionStatements = ConditionStatements;
}

//Assignment Statement
AssignmentStatement::AssignmentStatement(Declarator *declarator, Expression *expression, AssignmentOperator op){
    this->declarator = declarator;
    this->expression = expression;
    this->op = op;
}