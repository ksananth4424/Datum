#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <optional>

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "../semantic/traversal.hpp"
#include "../semantic/symbol_table.hpp"

using namespace std;

static unique_ptr<llvm::LLVMContext> TheContext;
static unique_ptr<llvm::Module> TheModule;
static unique_ptr<llvm::IRBuilder<>> Builder;
static map<std::string, llvm::Value *> NamedValues;

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
    func_drop,
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
    virtual string get_scope();
    virtual ~Node() = default;
    virtual llvm::Value *codegen() = 0;
};

class TypeSpecifier : public Node
{
public:
    vector<int> *type;
    TypeSpecifier(vector<int> *type, int row, int column);
    void buildScope(string scope);

    virtual ~TypeSpecifier() = default;
    llvm::Value *codegen() override;
    
};

class Start : public Node
{
public:
    vector<class FunctionDeclaration *> *FunctionList;
    vector<class Statement *> *StatementList;

    Start(vector<class Statement *> *StatementList, int row, int column);
    Start(vector<class FunctionDeclaration *> *FunctionList, int row, int column);
    Start(vector<class FunctionDeclaration *> *FunctionList, vector<class Statement *> *StatementList, int row, int column);
    void buildScope(string scope);
    virtual ~Start() = default;
    llvm::Value *codegen() override;
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

    virtual ~ConstantValue() = default;
    llvm::Value *codegen() override;

};

class DeclarationStatement : public Node
{
public:
    TypeSpecifier *type;
    vector<class InitDeclaration *> *initDeclarations;
    DeclarationStatement(TypeSpecifier *type, int row, int column);
    DeclarationStatement(TypeSpecifier *type, vector<class InitDeclaration *> *initDeclarations, int row, int column);
    void buildScope(string scope);

    virtual ~DeclarationStatement() = default;
    llvm::Value *codegen() override;

};

class Expression : public Node
{
public:
    // TypeSpecifier type;
    int castType;
    Expression();
    void buildScope(string scope);
    virtual ~Expression() = default;
    llvm::Value *codegen() override;

};

class BinaryExpression : public Expression
{
public:
    Expression *lhs;
    Expression *rhs;
    BinaryOperator op;
    BinaryExpression(Expression *lhs, Expression *rhs, BinaryOperator op, int row, int column);
    void buildScope(string scope);

    virtual ~BinaryExpression() = default;
    llvm::Value *codegen() override;

};

class UnaryExpression : public Expression
{
public:
    Expression *expr;
    char* identifier;
    vector<UnaryOperator> *op ;
    ConstantValue *constantValue;
    InbuiltFunctions inbuiltFunction;
    // SingleChainExpression *singleChainExpression;
    // MultiChainExpression *multiChainExpression;
    // UnaryExpression(SingleChainExpression *singleChainExpression);
    // UnaryExpression(MultiChainExpression *multiChainExpression);
    UnaryExpression(Expression *expr, vector<UnaryOperator> *op, ConstantValue *constantValue, InbuiltFunctions inbuiltFunction, int row, int column);
    UnaryExpression(Expression *expr, int row, int column);
    UnaryExpression(ConstantValue *constantValue, int row, int column);
    void buildScope(string scope);

    virtual ~UnaryExpression() = default;
    llvm::Value *codegen() override;

};

class Initializer : public Node
{
public:
    AssignmentStatement *assignmentExpression;
    vector<class Initializer *> *initializerList;
    Initializer(AssignmentStatement *assignmentExpression, int row, int column);
    Initializer(vector<class Initializer *> *initializerList, int row, int column);
    void buildScope(string scope);

    virtual ~Initializer() = default;
    llvm::Value *codegen() override;

};

class Declarator : public Node
{
public:
    char* identifier;
    Declarator(char* identifier, int row, int column);
    void buildScope(string scope);

    virtual ~Declarator() = default;
    llvm::Value *codegen() override;

};

class InitDeclaration : public Node
{
public:
    Declarator *declarator;
    Initializer *initializer;

    InitDeclaration(Declarator *declarator, int row, int column);
    InitDeclaration(Declarator *declarator, Initializer *initializer, int row, int column);
    void buildScope(string scope);

    virtual ~InitDeclaration() = default;
    llvm::Value *codegen() override;

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

    virtual ~AssignmentStatement() = default;
    llvm::Value *codegen() override;

};

class ConditionalStatement : public Node
{
public:
    vector<pair<class Expression *, vector<class Statement *>*>> *ConditionStatements;

    ConditionalStatement(vector<pair<class Expression *, vector<class Statement *>*>> *ConditionStatements, int row, int column);
    void buildScope(string scope);

    virtual ~ConditionalStatement() = default;
    llvm::Value *codegen() override;

};

class LoopStatement : public Node
{
public:
    Declarator* variable;
    vector<tuple<class Expression *, class Expression *, class Expression *>> *fromToPairs;
    vector<class Statement *> *statements;

    LoopStatement(Declarator* variable, vector<tuple<class Expression *, class Expression *, class Expression *>> *fromToPairs, vector<class Statement *> *statements, int row, int column);
    void buildScope(string scope);

    virtual ~LoopStatement() = default;
    llvm::Value *codegen() override;

};

class ReturnStatement : public Node
{
public:
    Expression *expression;
    ReturnStatement();
    ReturnStatement(Expression *expression, int row, int column);
    void buildScope(string scope);

    virtual ~ReturnStatement() = default;
    llvm::Value *codegen() override;

};

class BreakStatement : public Node
{
public:
    BreakStatement();

    virtual ~BreakStatement() = default;
    llvm::Value *codegen() override;

};

class ContinueStatement : public Node
{
public:
    ContinueStatement();

    virtual ~ContinueStatement() = default;
    llvm::Value *codegen() override;

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

    virtual ~Statement() = default;
    llvm::Value *codegen() override;

};

class Parameter : public Node
{
public:
    TypeSpecifier* type;
    Declarator* identifier;
    Parameter(TypeSpecifier* type, Declarator* identifier, int row, int column);
    void buildScope(string scope);

    virtual ~Parameter() = default;
    llvm::Value *codegen() override;

};

class FunctionDeclaration : public Node
{
public:
    char* identifier;
    vector<Parameter *> *inpParameter;
    vector<Parameter *> *otherParameter;
    vector<Parameter *> *outParameter;
    vector<class Statement *> *statements;
    FunctionDeclaration(char* identifier, vector<Parameter *> *inpParameter, vector<Parameter *> *otherParameter, vector<Parameter *> *outParameter, vector<class Statement *> *statements, int row, int column);
    void buildScope(string scope);

    virtual ~FunctionDeclaration() = default;
    llvm::Function *codegen() override;
    
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

    virtual ~Argument() = default;
    llvm::Value *codegen() override;

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

    virtual ~FunctionCall() = default;
    llvm::Value *codegen() override;

};

class SingleChainExpression : public Expression
{
public:
    char* identifier;
    vector<Expression*> *access;
    vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList;
    SingleChainExpression(char* identifier, vector<Expression*> *access, vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList, int row, int column);
    void buildScope(string scope);

    virtual ~SingleChainExpression() = default;
    llvm::Value *codegen() override;

};

class MultiChainExpression : public Expression
{
public:
    FunctionCall* functionCall;
    InbuiltFunctions inbuiltFunc;
    vector<Expression*> *access;
    vector<Expression*> *inputExprList;


    vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList;
    MultiChainExpression(FunctionCall* functionCall, vector<Expression*> *access, vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList, int row, int column);
    MultiChainExpression(InbuiltFunctions inbuiltFunc, vector<Expression*> *access, vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList, int row, int column);;
    MultiChainExpression(vector<Expression*> *inputExprList, vector<Expression*> *access,  vector<pair<FunctionCall *, vector<Expression*>*>> *functionCallList, int row, int column);
    void buildScope(string scope);
    virtual ~MultiChainExpression() = default;
    llvm::Value *codegen() override;

};

#endif