#pragma once
#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <unordered_map>
#include <optional>
using namespace std;

class Start;

class Node;

enum class InbuiltFunctions
{
    show_bar,
    show_line,
    show_scatter,
    show_box,
    row,
    col,
    filter,
    sum,
    max,
    min,
    mean,
    join,
    read,
    write,
    unique,
    show,
    split,
    sort,
    shuffle,
    add,
    shape,
    drop
};

enum class AssignmentOperator
{
    assign,
    add_assign,
    sub_assign,
    mul_assign,
    div_assign,
    mod_assign
};

enum class BinaryOperator
{
    add,
    sub,
    mul,
    div,
    mod,
    and_op,
    or_op,
    eq,
    neq,
    lt,
    gt,
    lte,
    gte
};

enum class UnaryOperator
{
    not_op,
    plus_op,
    minus_op
};


// class TypeSpecifier;

// class ConstantValue;

// class Expression;

// class BinaryExpression;

// class UnaryExpression;

// class Parameter;

// class FunctionDeclaration;

// class AssignmentStatement;

// class ConditionalStatement;

// class LoopStatement;

// class ReturnStatement;

// class BreakStatement;

// class ContinueStatement;

// class Statement;
// // RHS in initialization statements.
// class Initializer;
// // LHS in initialization statements.
// class Declarator;
// // in int a=2,b=3; a=2 is an InitDeclaration node. In declaration statement there would be InitDeclaration nodes ( vector<InitDeclaration*>).
// class InitDeclaration;

// class DeclarationStatement;

// class FunctionCall;

// class SingleChainExpression;

// class Argument;

class Node
{
public:
    string scope;
    Node();

    virtual ~Node() = default;
};

class TypeSpecifier : public Node
{
public:
    string type;
    TypeSpecifier(string type, TypeSpecifier *TypeSpecifier, string scope);

    virtual ~TypeSpecifier() = default;
};

class Start : public Node
{
public:
    vector<class FunctionDeclaration *> *FunctionList;
    vector<class Statement *> *StatementList;

    Start(vector<class FunctionDeclaration *> *FunctionList, vector<class Statement *> *StatementList, string scope);
    virtual ~Start() = default;
};

class ConstantValue : public Node
{
public:
    TypeSpecifier type;
    string value;
    ConstantValue(TypeSpecifier type, string value, string scope);

    virtual ~ConstantValue() = default;
};

class DeclarationStatement : public Node
{
public:
    TypeSpecifier type;
    vector<class InitDeclaration *> *initDeclarations;
    DeclarationStatement(TypeSpecifier type, vector<class InitDeclaration *> *initDeclarations, string scope);

    virtual ~DeclarationStatement() = default;
};

class Expression : public Node
{
public:
    TypeSpecifier type;
    int castType;
    vector<int> *Dimensions;
    Expression();
    virtual ~Expression() = default;
};

class BinaryExpression : public Expression
{
public:
    Expression *lhs;
    Expression *rhs;
    BinaryOperator op;
    BinaryExpression(Expression *lhs, Expression *rhs, string op, string scope);

    virtual ~BinaryExpression() = default;
};

class UnaryExpression : public Expression
{
public:
    Expression *expr;
    string identifier;
    vector<UnaryOperator> op ;
    ConstantValue *constantValue;
    InbuiltFunctions inbuiltFunction;
    UnaryExpression(Expression *expr, vector<UnaryOperator> op, ConstantValue *constantValue, InbuiltFunctions inbuiltFunction, string scope);

    virtual ~UnaryExpression() = default;
};

class Initializer : public Node
{
public:
    Expression *expression;
    vector<class Initializer *> *initializerList;
    Initializer(Expression *expression, vector<Initializer *> *initializerList, string scope);

    virtual ~Initializer() = default;
};

class Declarator : public Node
{
public:
    string identifier;
    Declarator(string identifier, string scope);

    virtual ~Declarator() = default;
};

class InitDeclaration : public Node
{
public:
    Declarator *declarator;
    Initializer *initializer;

    InitDeclaration(Declarator *declarator, Initializer *initializer, string scope);

    virtual ~InitDeclaration() = default;
};




class AssignmentStatement : public Node
{
public:
    Declarator *declarator;
    Expression *expression;
    AssignmentOperator op;

    AssignmentStatement(Declarator *declarator, Expression *expression, AssignmentOperator op, string scope);

    virtual ~AssignmentStatement() = default;
};

class ConditionalStatement : public Node
{
public:
    vector<pair<class Expression *, vector<class Statement *>*>> *ConditionStatements;

    ConditionalStatement(vector<pair<class Expression *, vector<class Statement *>*>> *ConditionStatements, string scope);

    virtual ~ConditionalStatement() = default;
};

class LoopStatement : public Node
{
public:
    string identifier;
    vector<tuple<class Expression *, class Expression *, class Expression *>> *fromToPairs;
    vector<class Statement *> *statements;

    LoopStatement(string identifier, vector<pair<class Expression *, class Expression *>> *fromToPairs, vector<class Statement *> *statements, string scope);

    virtual ~LoopStatement() = default;
};

class ReturnStatement : public Node
{
public:
    Expression *expression;
    ReturnStatement(Expression *expression, string scope);

    virtual ~ReturnStatement() = default;
};

class BreakStatement : public Node
{
public:
    BreakStatement(string scope);

    virtual ~BreakStatement() = default;
};

class ContinueStatement : public Node
{
public:
    ContinueStatement(string scope);

    virtual ~ContinueStatement() = default;
};

class Statement : public Node
{
public:
    int statementType;
    class DeclarationStatement *declarationStatement;
    class AssignmentStatement *assignmentStatement;
    class ConditionalStatement *conditionalStatement;
    class LoopStatement *loopStatement;
    class ReturnStatement *returnStatement;
    class BreakStatement *breakStatement;
    class ContinueStatement *continueStatement;
    Statement(class DeclarationStatement *declarationStatement, string scope);
    Statement(class AssignmentStatement *assignmentStatement, string scope);
    Statement(class ConditionalStatement *conditionalStatement, string scope);
    Statement(class LoopStatement *loopStatement, string scope);
    Statement(class ReturnStatement *returnStatement, string scope);
    Statement(class BreakStatement *breakStatement, string scope);
    Statement(class ContinueStatement *continueStatement, string scope);

    virtual ~Statement() = default;
};



class Parameter : public Node
{
public:
    TypeSpecifier type;
    string identifier;
    Parameter(TypeSpecifier type, string identifier, string scope);

    virtual ~Parameter() = default;
};

class FunctionDeclaration : public Node
{
public:
    string identifier;
    vector<Parameter *> *inpParameter;
    vector<Parameter *> *otherParameter;
    vector<Parameter *> *outParameter;
    vector<class Statement *> *statements;
    FunctionDeclaration(string identifier, vector<Parameter *> *inpParameter, vector<Parameter *> *otherParameter, vector<Parameter *> *outParameter, vector<class Statement *> *statements, string scope);
    virtual ~FunctionDeclaration() = default;
};

class Argument
{
public:
    Expression *expression;
    vector<tuple<Expression *, Expression *, Expression *>> FromToAlsoExpression;
    vector<Statement *> statements;
    Argument(Expression *expression, string SCOPE);
    Argument(vector<tuple<Expression *, Expression *, Expression *>> FromToAlsoExpression, string SCOPE);
    Argument(vector<Statement *> statements, string SCOPE);

    virtual ~Argument() = default;
};
class FunctionCall : public Node
{
public:
    string identifier;
    InbuiltFunctions inbuiltFunc;
    vector<Argument *> argumentList;
    FunctionCall(string identifier, vector<Argument *> *argumentList, string scope);
    FunctionCall(InbuiltFunctions inbuiltFunc, vector<Argument *> *argumentList, string scope);

    virtual ~FunctionCall() = default;
};

class SingleChainExpression : public Expression
{
public:
    string identifier;
    vector<Expression*> *access;
    vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList;
    SingleChainExpression(string identifier, vector<Expression*> *access, vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList, string scope);

    virtual ~SingleChainExpression() = default;
};

class MultiChainExpression : public Expression
{
public:
    FunctionCall* functionCall;
    pair<string,vector<Expression*>*> functionCallStart;
    InbuiltFunctions inbuiltFunc;
    vector<Expression*> *access;

    vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList;
    MultiChainExpression(FunctionCall* functionCall, vector<Expression*> *access, vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList, string scope);
    MultiChainExpression(InbuiltFunctions inbuiltFunc, vector<Expression*> *access, vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList, string scope);
    MultiChainExpression(pair<string,vector<Expression*>*> functionCallStart, vector<Expression*> *access,  vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList, string scope);
    virtual ~MultiChainExpression() = default;
};