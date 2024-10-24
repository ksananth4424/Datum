#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <optional>
using namespace std;

class Start;

class Node;

enum InbuiltFunctions
{
    func_show_bar,
    func_show_line,
    func_show_scatter,
    func_show_box,
    func_row,
    func_col,
    func_filter,
    func_sum,
    func_max,
    func_min,
    func_mean,
    func_join,
    func_read,
    func_write,
    func_unique,
    func_show,
    func_split,
    func_sort,
    func_shuffle,
    func_add,
    func_shape,
    func_drop
};

enum AssignmentOperator
{
    assign,
    add_assign,
    sub_assign,
    mul_assign,
    div_assign,
    mod_assign
};

enum BinaryOperator
{
    add_op,
    sub_op,
    mul_op,
    div_op,
    mod_op,
    and_op,
    or_op,
    eq_op,
    neq_op,
    lt_op,
    gt_op,
    lte_op,
    gte_op,
    ne_op
};

enum UnaryOperator
{
    not_op,
    plus_op,
    minus_op
};


class TypeSpecifier;

class ConstantValue;

class Expression;

class BinaryExpression;

class UnaryExpression;

class Parameter;

class FunctionDeclaration;

class AssignmentStatement;

class ConditionalStatement;

class LoopStatement;

class ReturnStatement;

class BreakStatement;

class ContinueStatement;

class Statement;
// RHS in initialization statements.
class Initializer;
// LHS in initialization statements.
class Declarator;
// in int a=2,b=3; a=2 is an InitDeclaration node. In declaration statement there would be InitDeclaration nodes ( vector<InitDeclaration*>).
class InitDeclaration;

class DeclarationStatement;

class FunctionCall;

class SingleChainExpression;

class Argument;

class Node
{
public:
    char* scope;
    Node();

    virtual ~Node() = default;
};

class TypeSpecifier : public Node
{
public:
    vector<int> *type;
    TypeSpecifier(vector<int> *type);

    virtual ~TypeSpecifier() = default;
};

class Start : public Node
{
public:
    vector<class FunctionDeclaration *> *FunctionList;
    vector<class Statement *> *StatementList;

    Start(vector<class Statement *> *StatementList);
    Start(vector<class FunctionDeclaration *> *FunctionList);
    Start(vector<class FunctionDeclaration *> *FunctionList, vector<class Statement *> *StatementList);
    virtual ~Start() = default;
};

class ConstantValue : public Node
{
public:
    TypeSpecifier *type;
    int ival;
    float fval;
    bool bval;
    char cval;
    char* sval;
    ConstantValue(TypeSpecifier *type, int ival);
    ConstantValue(TypeSpecifier *type, float fval);
    ConstantValue(TypeSpecifier *type, bool bval);
    ConstantValue(TypeSpecifier *type, char cval);
    ConstantValue(TypeSpecifier *type, char* sval);

    virtual ~ConstantValue() = default;
};

class DeclarationStatement : public Node
{
public:
    TypeSpecifier *type;
    vector<class InitDeclaration *> *initDeclarations;
    DeclarationStatement(TypeSpecifier *type);
    DeclarationStatement(TypeSpecifier *type, vector<class InitDeclaration *> *initDeclarations);

    virtual ~DeclarationStatement() = default;
};

class Expression : public Node
{
public:
    // TypeSpecifier type;
    int castType;
    Expression();
    virtual ~Expression() = default;
};

class BinaryExpression : public Expression
{
public:
    Expression *lhs;
    Expression *rhs;
    BinaryOperator op;
    BinaryExpression(Expression *lhs, Expression *rhs, BinaryOperator op);

    virtual ~BinaryExpression() = default;
};

class UnaryExpression : public Expression
{
public:
    Expression *expr;
    char* identifier;
    vector<UnaryOperator> *op ;
    ConstantValue *constantValue;
    InbuiltFunctions inbuiltFunction;
    UnaryExpression(Expression *expr, vector<UnaryOperator> *op, ConstantValue *constantValue, InbuiltFunctions inbuiltFunction);
    UnaryExpression(Expression *expr);
    UnaryExpression(ConstantValue *constantValue);

    virtual ~UnaryExpression() = default;
};

class Initializer : public Node
{
public:
    AssignmentStatement *assignmentExpression;
    vector<class Initializer *> *initializerList;
    Initializer(AssignmentStatement *assignmentExpression);
    Initializer(vector<class Initializer *> *initializerList);

    virtual ~Initializer() = default;
};

class Declarator : public Node
{
public:
    char* identifier;
    Declarator(char* identifier);

    virtual ~Declarator() = default;
};

class InitDeclaration : public Node
{
public:
    Declarator *declarator;
    Initializer *initializer;

    InitDeclaration(Declarator *declarator);
    InitDeclaration(Declarator *declarator, Initializer *initializer);

    virtual ~InitDeclaration() = default;
};




class AssignmentStatement : public Node
{
public:
    SingleChainExpression *declarator;
    Expression *expression;
    AssignmentOperator op;

    AssignmentStatement(SingleChainExpression *declarator, Expression *expression, AssignmentOperator op);
    AssignmentStatement(Expression *expression);

    virtual ~AssignmentStatement() = default;
};

class ConditionalStatement : public Node
{
public:
    vector<pair<class Expression *, vector<class Statement *>*>> *ConditionStatements;

    ConditionalStatement(vector<pair<class Expression *, vector<class Statement *>*>> *ConditionStatements);

    virtual ~ConditionalStatement() = default;
};

class LoopStatement : public Node
{
public:
    char* identifier;
    vector<tuple<class Expression *, class Expression *, class Expression *>> *fromToPairs;
    vector<class Statement *> *statements;

    LoopStatement(char* identifier, vector<tuple<class Expression *, class Expression *, class Expression *>> *fromToPairs, vector<class Statement *> *statements);

    virtual ~LoopStatement() = default;
};

class ReturnStatement : public Node
{
public:
    Expression *expression;
    ReturnStatement();
    ReturnStatement(Expression *expression);

    virtual ~ReturnStatement() = default;
};

class BreakStatement : public Node
{
public:
    BreakStatement();

    virtual ~BreakStatement() = default;
};

class ContinueStatement : public Node
{
public:
    ContinueStatement();

    virtual ~ContinueStatement() = default;
};

class Statement : public Node
{
public:
    int statementType;
    DeclarationStatement *declarationStatement;
    AssignmentStatement *assignmentStatement;
    ConditionalStatement *conditionalStatement;
    LoopStatement *loopStatement;
    ReturnStatement *returnStatement;
    BreakStatement *breakStatement;
    ContinueStatement *continueStatement;
    vector<Statement*> * compoundStatement;
    Statement(DeclarationStatement *declarationStatement);
    Statement(AssignmentStatement *assignmentStatement);
    Statement(ConditionalStatement *conditionalStatement);
    Statement(LoopStatement *loopStatement);
    Statement(ReturnStatement *returnStatement);
    Statement(BreakStatement *breakStatement);
    Statement(ContinueStatement *continueStatement);
    Statement(vector<Statement*> *compoundStatement);

    virtual ~Statement() = default;
};



class Parameter : public Node
{
public:
    TypeSpecifier* type;
    Declarator* identifier;
    Parameter(TypeSpecifier* type, Declarator* identifier);

    virtual ~Parameter() = default;
};

class FunctionDeclaration : public Node
{
public:
    char* identifier;
    vector<Parameter *> *inpParameter;
    vector<Parameter *> *otherParameter;
    vector<Parameter *> *outParameter;
    vector<class Statement *> *statements;
    FunctionDeclaration(char* identifier, vector<Parameter *> *inpParameter, vector<Parameter *> *otherParameter, vector<Parameter *> *outParameter, vector<class Statement *> *statements);
    virtual ~FunctionDeclaration() = default;
};

class Argument
{
public:
    Expression *expression;
    vector<tuple<Expression *, Expression *, Expression *>> *fromToAlsoExpression;
    vector<Statement *> *statements;
    Argument(Expression *expression);
    Argument(vector<tuple<Expression *, Expression *, Expression *>> *fromToAlsoExpression);
    Argument(vector<Statement *> *statements);

    virtual ~Argument() = default;
};
class FunctionCall : public Node
{
public:
    char* identifier;
    InbuiltFunctions inbuiltFunc;
    vector<Argument *> *argumentList;
    FunctionCall(char* identifier, vector<Argument *> *argumentList);
    FunctionCall(InbuiltFunctions inbuiltFunc, vector<Argument *> *argumentList);

    virtual ~FunctionCall() = default;
};

class SingleChainExpression : public Expression
{
public:
    char* identifier;
    vector<Expression*> *access;
    vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList;
    SingleChainExpression(char* identifier, vector<Expression*> *access, vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList);

    virtual ~SingleChainExpression() = default;
};

class MultiChainExpression : public Expression
{
public:
    FunctionCall* functionCall;
    pair<char*,vector<Expression*>*> functionCallStart;
    InbuiltFunctions inbuiltFunc;
    vector<Expression*> *access;

    vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList;
    MultiChainExpression(FunctionCall* functionCall, vector<Expression*> *access, vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList);
    MultiChainExpression(InbuiltFunctions inbuiltFunc, vector<Expression*> *access, vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList);
    MultiChainExpression(pair<char*,vector<Expression*>*> functionCallStart, vector<Expression*> *access,  vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList);
    virtual ~MultiChainExpression() = default;
};