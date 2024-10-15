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
    SHOW_BAR,
    SHOW_LINE,
    SHOW_SCATTER,
    SHOW_BOX,
    ROW,
    COL,
    FILTER,
    SUM,
    MAX,
    MIN,
    MEAN,
    JOIN,
    READ,
    WRITE,
    UNIQUE,
    SHOW,
    SPLIT,
    SORT,
    SHUFFLE,
    ADD,
    SHAPE,
    DROP
};

enum AssignmentOperator
{
    ASSIGN,
    ADD_ASSIGN,
    SUB_ASSIGN,
    MUL_ASSIGN,
    DIV_ASSIGN,
    MOD_ASSIGN
};

enum class BinaryOperator
{
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    AND,
    OR,
    EQ,
    NEQ,
    LT,
    GT,
    LTE,
    GTE
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
    TypeSpecifier(string type,TypeSpecifier* TypeSpecifier, string scope);

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

class InitDeclaration : public Node
{
public:
    Declarator *declarator;
    Initializer *initializer;

    InitDeclaration(Declarator *declarator, Initializer *initializer, string scope);

    virtual ~InitDeclaration() = default;
};

class Declarator : public Node
{
public:
    string identifier;
    Declarator(string identifier, string scope);

    virtual ~Declarator() = default;
};

class Initializer : public Node
{
public:
    Expression *expression;
    vector<class Initializer *> *initializerList;
    Initializer(Expression *expression,vector<Initializer*> *initializerList ,string scope);

    virtual ~Initializer() = default;
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
    Expression *condition;
    vector<class Statement *> *ifStatements;
    vector<class Statement *> *elseStatements;

    ConditionalStatement(Expression *condition, vector<class Statement *> *ifStatements, vector<class Statement *> *elseStatements, string scope);

    virtual ~ConditionalStatement() = default;
};

class LoopStatement : public Node
{
public:
    Expression *condition;
    vector<class Statement *> *statements;

    LoopStatement(Expression *condition, vector<class Statement *> *statements, string scope);

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
    class AssignmentStatement *assignmentStatement;
    class ConditionalStatement *conditionalStatement;
    class LoopStatement *loopStatement;
    class ReturnStatement *returnStatement;
    Statement(class AssignmentStatement *assignmentStatement, class ConditionalStatement *conditionalStatement, class LoopStatement *loopStatement, class ReturnStatement *returnStatement, int statementType, string scope);

    virtual ~Statement() = default;
};

class Expression : public Node
{
public:
    TypeSpecifier type;
    vector<int> Dimensions;
    Expression();
    virtual ~Expression() = default;
};

class BinaryExpression : public Expression
{
    Expression *lhs;
    Expression *rhs;
    BinaryOperator op;
    BinaryExpression(Expression *lhs, Expression *rhs, string op,string scope);

    virtual ~BinaryExpression() = default;
};

class UnaryExpression : public Expression
{
    Expression *expr;
    string identifier;
    string op;
    ConstantValue *constantValue;
    InbuiltFunctions inbuiltFunction;
    UnaryExpression(Expression *expr, string op, ConstantValue *constantValue, InbuiltFunctions inbuiltFunction,string scope);

    virtual ~UnaryExpression() = default;
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
    TypeSpecifier returnType;
    string identifier;
    vector<class DeclarationStatement *> *parameters;
    vector<DeclarationStatement *> *localDeclarations;
    vector<class Statement *> *statements;
    FunctionDeclaration(TypeSpecifier returnType, string identifier, vector<class DeclarationStatement *> *parameters, vector<DeclarationStatement *> *localDeclarations, vector<class Statement *> *statements, string scope);

    virtual ~FunctionDeclaration() = default;
};