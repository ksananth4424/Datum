#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <optional>
using namespace std;

class Start;

class Node;

enum InbuiltFunctions
{
    func_show_bar, // done
    func_show_line,
    func_show_scatter,
    func_show_box,
    func_row,// done
    func_col,// done
    func_filter,
    func_sum, // done
    func_max, // done
    func_min, // done
    func_mean, // done
    func_join, // done
    func_read, // done
    func_write, // done
    func_unique, // done
    func_show, // done
    func_split,
    func_sort, // done
    func_shuffle, // done
    func_add, // done
    func_shape, // done
    func_drop, // done
    none
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
    string scope;
    int row;
    int column;
    Node();
    virtual void buildScope(string scope);
    virtual string codegen() = 0;
    virtual string get_scope();
    virtual ~Node() = default;
};

class TypeSpecifier : public Node
{
public:
    vector<int> *type;
    TypeSpecifier(vector<int> *type, int row, int column);
    void buildScope(string scope);
    string codegen();
    virtual ~TypeSpecifier() = default;
};

class Start : public Node
{
public:
    vector<class FunctionDeclaration *> *FunctionList;
    vector<class Statement *> *StatementList;

    Start(vector<class Statement *> *StatementList, int row, int column);
    Start(vector<class FunctionDeclaration *> *FunctionList, int row, int column);
    Start(vector<class FunctionDeclaration *> *FunctionList, vector<class Statement *> *StatementList, int row, int column);
    string codegen();
    void buildScope(string scope);
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
    ConstantValue(TypeSpecifier *type, int ival, int row, int column);
    ConstantValue(TypeSpecifier *type, float fval, int row, int column);
    ConstantValue(TypeSpecifier *type, bool bval, int row, int column);
    ConstantValue(TypeSpecifier *type, char cval, int row, int column);
    ConstantValue(TypeSpecifier *type, char* sval, int row, int column);
    void buildScope(string scope);
    string codegen();
    virtual ~ConstantValue() = default;
};

class DeclarationStatement : public Node
{
public:
    TypeSpecifier *type;
    vector<class InitDeclaration *> *initDeclarations;
    DeclarationStatement(TypeSpecifier *type, int row, int column);
    DeclarationStatement(TypeSpecifier *type, vector<class InitDeclaration *> *initDeclarations, int row, int column);
    void buildScope(string scope);
    string codegen();
    virtual ~DeclarationStatement() = default;
};

class Expression : public Node
{
public:
    // TypeSpecifier type;
    int castType;
    Expression();
    void buildScope(string scope);
    string codegen();
    virtual ~Expression() = default;
};

class BinaryExpression : public Expression
{
public:
    Expression *lhs;
    Expression *rhs;
    BinaryOperator op;
    BinaryExpression(Expression *lhs, Expression *rhs, BinaryOperator op, int row, int column);
    void buildScope(string scope);
    string codegen();
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
    UnaryExpression(Expression *expr, vector<UnaryOperator> *op, ConstantValue *constantValue, InbuiltFunctions inbuiltFunction, int row, int column);
    UnaryExpression(Expression *expr, int row, int column);
    UnaryExpression(ConstantValue *constantValue, int row, int column);
    string codegen();
    void buildScope(string scope);

    virtual ~UnaryExpression() = default;
};

class Initializer : public Node
{
public:
    AssignmentStatement *assignmentExpression;
    vector<class Initializer *> *initializerList;
    Initializer(AssignmentStatement *assignmentExpression, int row, int column);
    Initializer(vector<class Initializer *> *initializerList, int row, int column);
    void buildScope(string scope);
    string codegen();
    virtual ~Initializer() = default;
};

class Declarator : public Node
{
public:
    char* identifier;
    Declarator(char* identifier, int row, int column);
    void buildScope(string scope);
    string codegen();
    virtual ~Declarator() = default;
};

class InitDeclaration : public Node
{
public:
    Declarator *declarator;
    Initializer *initializer;

    InitDeclaration(Declarator *declarator, int row, int column);
    InitDeclaration(Declarator *declarator, Initializer *initializer, int row, int column);
    void buildScope(string scope);
    string codegen();
    virtual ~InitDeclaration() = default;
};

class AssignmentStatement : public Node
{
public:
    SingleChainExpression *declarator;
    Expression *expression;
    AssignmentOperator op;

    AssignmentStatement(SingleChainExpression *declarator, Expression *expression, AssignmentOperator op, int row, int column);
    AssignmentStatement(Expression *expression, int row, int column);
    void buildScope(string scope);
    string codegen();
    virtual ~AssignmentStatement() = default;
};

class ConditionalStatement : public Node
{
public:
    vector<pair<class Expression *, vector<class Statement *>*>> *ConditionStatements;

    ConditionalStatement(vector<pair<class Expression *, vector<class Statement *>*>> *ConditionStatements, int row, int column);
    void buildScope(string scope);
    string codegen();
    virtual ~ConditionalStatement() = default;
};

class LoopStatement : public Node
{
public:
    Declarator* variable;
    vector<tuple<class Expression *, class Expression *, class Expression *>> *fromToPairs;
    vector<class Statement *> *statements;

    LoopStatement(Declarator* variable, vector<tuple<class Expression *, class Expression *, class Expression *>> *fromToPairs, vector<class Statement *> *statements, int row, int column);
    void buildScope(string scope);
    string codegen();
    virtual ~LoopStatement() = default;
};

class ReturnStatement : public Node
{
public:
    Expression *expression;
    ReturnStatement();
    ReturnStatement(Expression *expression, int row, int column);
    void buildScope(string scope);
    string codegen();
    virtual ~ReturnStatement() = default;
};

class BreakStatement : public Node
{
public:
    BreakStatement();
    string codegen();
    virtual ~BreakStatement() = default;
};

class ContinueStatement : public Node
{
public:
    ContinueStatement();
    string codegen();
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
    Statement(DeclarationStatement *declarationStatement, int row, int column);
    Statement(AssignmentStatement *assignmentStatement, int row, int column);
    Statement(ConditionalStatement *conditionalStatement, int row, int column);
    Statement(LoopStatement *loopStatement, int row, int column);
    Statement(ReturnStatement *returnStatement, int row, int column);
    Statement(BreakStatement *breakStatement, int row, int column);
    Statement(ContinueStatement *continueStatement, int row, int column);
    Statement(vector<Statement*> *compoundStatement, int row, int column);
    void buildScope(string scope);
    string codegen();
    virtual ~Statement() = default;
};

class Parameter : public Node
{
public:
    TypeSpecifier* type;
    Declarator* identifier;
    Parameter(TypeSpecifier* type, Declarator* identifier, int row, int column);
    void buildScope(string scope);
    string codegen();
    virtual ~Parameter() = default;
};

class FunctionDeclaration : public Node
{
public:
    char* identifier;
    Parameter* inpParameter;
    vector<Parameter *> *otherParameter;
    Parameter* outParameter;
    vector<class Statement *> *statements;
    FunctionDeclaration(char* identifier, Parameter* inpParameter, vector<Parameter *> *otherParameter, Parameter* outParameter, vector<class Statement *> *statements, int row, int column);
    void buildScope(string scope);
    string codegen();
    virtual ~FunctionDeclaration() = default;
};

class Argument : public Node
{
public:
    Expression *expression;
    vector<tuple<Expression *, Expression *, Expression *>> *fromToAlsoExpression;
    vector<Statement *> *statements;
    Argument(Expression *expression, int row, int column);
    Argument(vector<tuple<Expression *, Expression *, Expression *>> *fromToAlsoExpression, int row, int column);
    Argument(vector<Statement *> *statements, int row, int column);
    void buildScope(string scope);
    string codegen();
    virtual ~Argument() = default;
};
class FunctionCall : public Node
{
public:
    char* identifier;
    InbuiltFunctions inbuiltFunc;
    vector<Argument *> *argumentList;
    FunctionCall(char* identifier, vector<Argument *> *argumentList, int row, int column);
    FunctionCall(InbuiltFunctions inbuiltFunc, vector<Argument *> *argumentList, int row, int column);
    void buildScope(string scope);
    string codegen();
    virtual ~FunctionCall() = default;
};

class SingleChainExpression : public Expression
{
public:
    char* identifier;
    Expression* inputExpr;
    Expression* access;
    vector<pair<FunctionCall *, Expression*>> *functionCallList;
    SingleChainExpression(char* identifier, Expression* access, vector<pair<FunctionCall *, Expression*>> *functionCallList, int row, int column);
    SingleChainExpression(Expression* inputExpr, Expression* access, vector<pair<FunctionCall *, Expression*>> *functionCallList, int row, int column);
    SingleChainExpression(FunctionCall* functionCall, Expression* access, int row, int column);
    void buildScope(string scope);
    string codegen();
    virtual ~SingleChainExpression() = default;
};

#endif