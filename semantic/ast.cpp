#include "ast.hpp"

Node::Node() {

}

Start::Start(vector<FunctionDeclaration *> *FunctionList, vector<Statement *> *StatementList) {
    this->FunctionList = FunctionList;
    this->StatementList = StatementList;
}
//Expression

//BinaryExpression

//UnaryExpression

//ConstantValue

//TypeSpecifier

//Initializer

//Declarator

//InitDeclaration

//Parameter

//Argument

//FunctionCall

//FunctionDeclaration

//SingleChainExpression

//MultiChainExpression

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

//Assignment Statement