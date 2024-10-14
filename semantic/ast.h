#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <unordered_map>
#include <optional>
using namespace std;

class Start;

class Node;

enum class TypeSpecifier
{
    INTEGER,
    FLOAT,
    STRING,
    CHAR,
    BOOL,
    DATASET,
    ARRAY
};

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

class ConstantValue;

class Expression;

class BinaryExpression;

class UnaryExpression;

class FunctionDeclaration;

class AssignmentStatement;

class ConditionalStatement;

class LoopStatement;

class ReturnStatement;

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
    int scope;
    Node();

    virtual ~Node() = default;
};

class Start : public Node
{
public:
    vector<class DeclarationStatement *> *DeclarationsList;
    vector<class Statement *> *AssignmentList;
    vector<class FunctionDeclaration *> *FunctionList;

    Start(vector<class DeclarationStatement *> *DeclarationsList, vector<class Statement *> *AssignmentList, vector<class FunctionDeclaration *> *FunctionList, int scope);
    virtual ~Start() = default;
};

class ConstantValue : public Node
{
public:
    TypeSpecifier type;
    string value;
    ConstantValue(TypeSpecifier type, string value, int scope);

    virtual ~ConstantValue() = default;
};

class DeclarationStatement : public Node
{
public:
    TypeSpecifier type;
    vector<class InitDeclaration *> *initDeclarations;
    DeclarationStatement(TypeSpecifier type, vector<class InitDeclaration *> *initDeclarations, int scope);

    virtual ~DeclarationStatement() = default;
};

class InitDeclaration : public Node
{
public:
    Declarator *declarator;
    Initializer *initializer;

    InitDeclaration(Declarator *declarator, Initializer *initializer, int scope);

    virtual ~InitDeclaration() = default;
};

class Declarator : public Node
{
public:
    string identifier;
    Declarator(string identifier, int scope);

    virtual ~Declarator() = default;
};

class Initializer : public Node
{
public:
    Expression *expression;
    Initializer(Expression *expression, int scope);

    virtual ~Initializer() = default;
};

class AssignmentStatement : public Node
{
public:
    Declarator *declarator;
    Expression *expression;
    AssignmentOperator op;

    AssignmentStatement(Declarator *declarator, Expression *expression, AssignmentOperator op, int scope);

    virtual ~AssignmentStatement() = default;
};

class ConditionalStatement : public Node
{
public:
    Expression *condition;
    vector<class Statement *> *ifStatements;
    vector<class Statement *> *elseStatements;

    ConditionalStatement(Expression *condition, vector<class Statement *> *ifStatements, vector<class Statement *> *elseStatements, int scope);

    virtual ~ConditionalStatement() = default;
};

class LoopStatement : public Node
{
public:
    Expression *condition;
    vector<class Statement *> *statements;

    LoopStatement(Expression *condition, vector<class Statement *> *statements, int scope);

    virtual ~LoopStatement() = default;
};

class ReturnStatement : public Node
{
public:
    Expression *expression;
    ReturnStatement(Expression *expression, int scope);

    virtual ~ReturnStatement() = default;
};

class Statement : public Node
{
public:
    int statementType;
    class AssignmentStatement *assignmentStatement;
    class ConditionalStatement *conditionalStatement;
    class LoopStatement *loopStatement;
    class ReturnStatement *returnStatement;
    Statement(class AssignmentStatement *assignmentStatement, class ConditionalStatement *conditionalStatement, class LoopStatement *loopStatement, class ReturnStatement *returnStatement, int statementType, int scope);

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
    string op;
    BinaryExpression(Expression *lhs, Expression *rhs, string op,int scope);

    virtual ~BinaryExpression() = default;
};

class UnaryExpression : public Expression
{
    Expression *expr;
    string identifier;
    string op;
    ConstantValue *constantValue;
    InbuiltFunctions inbuiltFunction;
    UnaryExpression(Expression *expr, string op, ConstantValue *constantValue, InbuiltFunctions inbuiltFunction,int scope);

    virtual ~UnaryExpression() = default;
};

class FunctionDeclaration : public Node
{
public:
    TypeSpecifier returnType;
    string identifier;
    vector<class DeclarationStatement *> *parameters;
    vector<DeclarationStatement *> *localDeclarations;
    vector<class Statement *> *statements;
    FunctionDeclaration(TypeSpecifier returnType, string identifier, vector<class DeclarationStatement *> *parameters, vector<DeclarationStatement *> *localDeclarations, vector<class Statement *> *statements, int scope);

    virtual ~FunctionDeclaration() = default;
};