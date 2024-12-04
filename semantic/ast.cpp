#include "ast.hpp"

// codegen spits out cpp code

Node::Node() {

}

void Node::buildScope(string scope) {
    this->scope = scope;
}

string Node::get_scope() {
    return Node::scope.substr(0, scope.find_last_of('.')); 
}

void Start::buildScope (std::string scope) {
    int child_scope = 0;
    this->scope = scope;
    for (auto &func_dec : *(this->FunctionList)) {
        func_dec->buildScope(scope + "." + to_string(child_scope++));
    }
    int stmt_scope = child_scope;
    for (auto &stmt : *(this->StatementList)) {
        stmt->buildScope(scope + "." + to_string(stmt_scope) + "." + to_string(child_scope++));
    }
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

std::string Start::codegen() {
    string code = "";
    for (auto &func_dec : *(this->FunctionList)) {
        code += func_dec->codegen();
    }
    for (auto &stmt : *(this->StatementList)) {
        code += stmt->codegen();
    }
    return code;
}

//Expression
void Expression::buildScope (std::string scope) {
    this->scope = scope;
}

Expression::Expression() {

}

string Expression::codegen() {
    string code = "";
    if(this->castType==1){
        code += dynamic_cast<UnaryExpression *>(this)->codegen();
    }
    else if(this->castType==2){
        code += dynamic_cast<BinaryExpression *>(this)->codegen();
    }
    else if(this->castType==3){
        UnaryExpression *unaryExpression = dynamic_cast<UnaryExpression *>(this);
        Expression *expr = unaryExpression->expr;
        code += dynamic_cast<SingleChainExpression *>(expr)->codegen();
    }
    return code;
}

//BinaryExpression
void BinaryExpression::buildScope (std::string scope) {
    this->scope = scope;
    if (this->lhs != nullptr) {
        this->lhs->buildScope(scope);
    }
    if (this->rhs != nullptr) {
        this->rhs->buildScope(scope);
    }
}

BinaryExpression::BinaryExpression(Expression *lhs, Expression *rhs, BinaryOperator op, int row, int column) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->op = op;
    this->row = row;
    this->column = column;
}

string BinaryExpression::codegen() {
    string code = "";
    code += this->lhs->codegen();
    switch(this->op) {
        case BinaryOperator::add_op:
            code += " + ";
            break;
        case BinaryOperator::sub_op:
            code += " - ";
            break;
        case BinaryOperator::mul_op:
            code += " * ";
            break;
        case BinaryOperator::div_op:
            code += " / ";
            break;
        case BinaryOperator::mod_op:
            code += " % ";
            break;
        case BinaryOperator::lt_op:
            code += " < ";
            break;
        case BinaryOperator::gt_op:
            code += " > ";
            break;
        case BinaryOperator::lte_op:
            code += " <= ";
            break;
        case BinaryOperator::gte_op:
            code += " >= ";
            break;
        case BinaryOperator::eq_op:
            code += " == ";
            break;
        case BinaryOperator::ne_op:
            code += " != ";
            break;
        case BinaryOperator::and_op:
            code += " && ";
            break;
        case BinaryOperator::or_op:
            code += " || ";
            break;
    }
    code += this->rhs->codegen();
    return code;
}

//UnaryExpression
void UnaryExpression::buildScope (std::string scope) {
    this->scope = scope;
    if (this->expr != nullptr) {
        this->expr->buildScope(scope);
    }
    if (this->constantValue != nullptr) {
        this->constantValue->buildScope(scope);
    }
}

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

string UnaryExpression::codegen() {
    string code = "";
    for (auto &o : *(this->op)){
        if (o == minus_op){
            code+="-";
        }else if (o == plus_op){
            code+="+";
        }else if (o == not_op){
            code+="!";
        }
    }
    if(this->constantValue){
        code+=this->constantValue-> codegen();
    }
    // need to handle inbuilt function
    
    return code;
}

//ConstantValue
void ConstantValue::buildScope (std::string scope) {
    this->scope = scope;
}

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

string ConstantValue::codegen() {
    string code = "";
    switch(this->type->type->at(0)) {
        case 1:
            code += to_string(this->ival);
            break;
        case 2:
            code += to_string(this->fval);
            break;
        case 3:
            code += to_string(this->bval);
            break;
        case 4:
            code += this->cval;
            break;
        case 5:
            code += this->sval;
            break;
    }
    return code;
}

//TypeSpecifier
void TypeSpecifier::buildScope (std::string scope) {
    this->scope = scope;
}

TypeSpecifier::TypeSpecifier(vector<int>*type, int row, int column){
    this->type = type;
    this->row = row;
    this->column = column;
}

string TypeSpecifier::codegen() {
    string code = "";
    switch(this->type->at(0)) {
        case 0:
            code += "int";
            break;
        case 1:
            code += "float";
            break;
        case 4:
            code += "bool";
            break;
        case 3:
            code += "char";
            break;
        case 2:
            code += "string";
            break;
        case 6:
        //NEED TO HANDLE THIS DATASET
            code += "dataset";
            break;
        case 7:
            code += "vector<int>";
            break;
        case 8:
            code += "vector<float>";
            break;
        case 9:
            code += "vector<string>";
            break;
        case 10:
            code += "vector<char>";
            break;
        case 11:
            code += "vector<bool>";
            break;
        case 12:
            code += "vector<dataset>";
            break;

    }
    return code;
}

//Initializer
void Initializer::buildScope (std::string scope) {
    this->scope = scope;
    if (this->assignmentExpression != nullptr) {
        this->assignmentExpression->buildScope(scope);
    }
    if (this->initializerList != nullptr) {
        for (auto &initializer : *(this->initializerList)) {
            initializer->buildScope(scope);
        }
    }
}

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

string Initializer::codegen() {
    string code = "";
    if (this->assignmentExpression != nullptr) {
        code += this->assignmentExpression->codegen();
    }
    if (this->initializerList != nullptr) {
        for (auto &initializer : *(this->initializerList)) {
            code += initializer->codegen();
        }
    }
    return code;
}

//Declarator
void Declarator::buildScope (std::string scope) {
    this->scope = scope;
}

Declarator::Declarator(char* identifier, int row, int column) {
    this->identifier = identifier;
    this->row = row;
    this->column = column;
}

string Declarator::codegen() {
    return std::string(this->identifier);
}

//InitDeclaration
void InitDeclaration::buildScope (std::string scope) {
    this->scope = scope;
    if (this->declarator != nullptr) {
        this->declarator->buildScope(scope);
    }
    if (this->initializer != nullptr) {
        this->initializer->buildScope(scope);
    }
}

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

string InitDeclaration::codegen() {
    string code = "";
    code += this->declarator->codegen();
    if (this->initializer != nullptr) {
        code += " = ";
        code += this->initializer->codegen();
    }
    return code;
}

//Parameter
void Parameter::buildScope (std::string scope) {
    this->scope = scope;
    if (this->type != nullptr) {
        this->type->buildScope(scope);
    }
    if (this->identifier != nullptr) {
        this->identifier->buildScope(scope);
    }
}

Parameter::Parameter(TypeSpecifier *type, Declarator* identifier, int row, int column) {
    this->type = type;
    this->identifier = identifier;
    this->row = row;
    this->column = column;
}

string Parameter::codegen() {
    string code = "";
    code += this->type->codegen();
    code += " ";
    code += this->identifier->codegen();
    return code;
}

//Argument
void Argument::buildScope (std::string scope) {
    this->scope = scope;
    if (this->expression != nullptr) {
        this->expression->buildScope(scope);
    }
    if (this->fromToAlsoExpression != nullptr) {
        for (auto &[from, to, also] : *fromToAlsoExpression) {
            if (from != nullptr) {
                from->buildScope(scope);
            }
            if (to != nullptr) {
                to->buildScope(scope);
            }
            if (also != nullptr) {
                also->buildScope(scope);
            }
        }
    }
    int child_scope = 0;
    if (this->statements != nullptr) {
        for (auto &stmt : *statements) {
            stmt->buildScope(scope + "." + to_string(child_scope++));
        }
    }
}

Argument::Argument(Expression *expression, int row, int column) {
    this->expression = expression;
    this->fromToAlsoExpression = nullptr;
    this->statements = nullptr;
    this->row = row;
    this->column = column;
}

Argument::Argument(vector<tuple<Expression *, Expression *, Expression *>> *fromToAlsoExpression, int row, int column) {
    this->fromToAlsoExpression = fromToAlsoExpression;
    this->expression = nullptr;
    this->statements = nullptr;
    this->row = row;
    this->column = column;
}

Argument::Argument(vector<Statement *> *statements, int row, int column) {
    this->statements = statements;
    this->expression = nullptr;
    this->fromToAlsoExpression = nullptr;
    this->row = row;
    this->column = column;
}

string Argument::codegen() {
    string code = "";
    if (this->expression != nullptr) {
        code += this->expression->codegen();
    }
    if (this->fromToAlsoExpression != nullptr) {
        code += fromToPairsCodeGen(this->fromToAlsoExpression);
    }
    return code;

}

//FunctionCall
void FunctionCall::buildScope(string scope) {
    this->scope = scope;
    for (auto &arg : *argumentList) {
        arg->buildScope(scope);
    }
}

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

string FunctionCall::codegen() {
    string code = "";
     
    return code;
}

//FunctionDeclaration
void FunctionDeclaration::buildScope(string scope) {
    this->scope = scope;
    int child_scope = 0;
    if (this->inpParameter != nullptr) {    
        this->inpParameter->buildScope(scope + "." + to_string(child_scope++));
    }
    for (auto &param : *(this->otherParameter)) {
        param->buildScope(scope + "." + to_string(child_scope++));
    }
    if (this->outParameter != nullptr) {
        this->outParameter->buildScope(scope + "." + to_string(child_scope++));
    }
    for (auto &stmt : *(this->statements)) {
        stmt->buildScope(scope + "." + to_string(child_scope++));
    }
}

FunctionDeclaration::FunctionDeclaration(char* identifier, Parameter* inpParameter, vector<Parameter *> *otherParameter, Parameter* outParameter, vector<Statement *> *statements, int row, int column) {
    this->identifier = identifier;
    this->inpParameter = inpParameter;
    this->otherParameter = otherParameter;
    this->outParameter = outParameter;
    this->statements = statements;
    this->row = row;
    this->column = column;
}

string FunctionDeclaration::codegen() {
    string code = "";
    code += this->outParameter->codegen(); //doubt here
    code += this->identifier;
    code += "(";
    if (this->inpParameter != nullptr) {
        code += this->inpParameter->codegen();
    }
    for (auto &param : *(this->otherParameter)) {
        code += ", ";
        code += param->codegen();
    }
    code += ") ";
    if (this->outParameter != nullptr) {
        code += this->outParameter->codegen();
    }
    code += " {\n";
    for (auto &stmt : *(this->statements)) {
        code += stmt->codegen();
    }
    code += "}\n";
    return code;
}

//SingleChainExpression
void SingleChainExpression::buildScope(string scope) {
    this->scope = scope;
    if (this->access != nullptr) {
        access->buildScope(scope);
    }
    if (this->functionCallList != nullptr) {
        for (auto &[func, expr] : *functionCallList) {
            func->buildScope(scope);
            if (expr != nullptr) {
                expr->buildScope(scope);
            }
        }
    }
}

SingleChainExpression::SingleChainExpression(char* identifier, Expression* access, vector<pair<FunctionCall *, Expression* >> *functionCallList, int row, int column) {
    this->identifier = identifier;
    this->access = access;
    this->functionCallList = functionCallList;
    this->row = row;
    this->column = column;
}

SingleChainExpression::SingleChainExpression(Expression *inputExpr, Expression *access, vector<pair<FunctionCall *, Expression* >> *functionCallList, int row, int column) {
    this->identifier = identifier;
    this->access = access;
    this->inputExpr = inputExpr;
    this->functionCallList = functionCallList;
    this->row = row;
    this->column = column;
}

SingleChainExpression::SingleChainExpression(FunctionCall *functionCall, Expression* access, int row, int column) {
    this->functionCallList = new vector<pair<FunctionCall *, Expression *>>();
    this->functionCallList->push_back(make_pair(functionCall, access));
    this->identifier = nullptr;
    this->access = nullptr;
    this->row = row;
    this->column = column;
}

string SingleChainExpression::codegen() {
    string code = "";
    if (this->identifier != nullptr) {
        code += this->identifier;
        if (this->access != nullptr) {
            code += "[";
            code += this->access->codegen();
            code += "]";
        }
    } else if (this->inputExpr != nullptr) {
        code += this->inputExpr->codegen();
        if (this->access != nullptr) {
            code += "[";
            code += this->access->codegen();
            code += "]";
        }
    } 
    if (this->functionCallList!= nullptr && this->functionCallList->size()>0){
        for (auto &[func, expr] : *functionCallList) {
            code += ")";
            code = func->codegen() + "(" + code;
            if (expr != nullptr) {
                code += "[";
                code += expr->codegen();
                code += "]";
            }
        }
    }
    code += ";\n";
    return code;

    
}

// Statements
void Statement::buildScope(string scope) {
    this->scope = scope;
    switch (this->statementType) {
        case 1:
            this->declarationStatement->buildScope(scope);
            break;
        case 2:
            this->assignmentStatement->buildScope(scope);
            break;
        case 3:
            this->conditionalStatement->buildScope(scope);
            break;
        case 4:
            this->loopStatement->buildScope(scope);
            break;
        case 5:
            this->returnStatement->buildScope(scope);
            break;
        case 6:
            this->breakStatement->buildScope(scope);
            break;
        case 7:
            this->continueStatement->buildScope(scope);
            break;
        case 8:
            int child_scope = 0;
            for (auto &stmt : *compoundStatement) {
                stmt->buildScope(scope + "." + to_string(child_scope++));
            }
            break;
    }
}

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

string Statement::codegen() {
    string code = "";
    switch (this->statementType) {
        case 1:
            code += this->declarationStatement->codegen();
            break;
        case 2:
            code += this->assignmentStatement->codegen();
            break;
        case 3:
            code += this->conditionalStatement->codegen();
            break;
        case 4:
            code += this->loopStatement->codegen();
            break;
        case 5:
            code += this->returnStatement->codegen();
            break;
        case 6:
            code += this->breakStatement->codegen();
            break;
        case 7:
            code += this->continueStatement->codegen();
            break;
        case 8:
            for (auto &stmt : *compoundStatement) {
                code += stmt->codegen();
            }
            break;
    }
    return code;
}

ContinueStatement::ContinueStatement()  {

}

string ContinueStatement::codegen() {
    return "continue;\n";
}

BreakStatement::BreakStatement()  {

}

string BreakStatement::codegen() {
    return "break;\n";
}

void ReturnStatement::buildScope(string scope) {
    this->scope = scope;
    if (this->expression != nullptr) {
        this->expression->buildScope(scope);
    }
}

ReturnStatement::ReturnStatement()  {
    this->expression = nullptr;
}

ReturnStatement::ReturnStatement(Expression *expression, int row, int column)  {
    this->expression = expression;
    this->row = row;
    this->column = column;
}

string ReturnStatement::codegen() {
    string code = "return ";
    if (this->expression != nullptr) {
        code += this->expression->codegen();
    }
    code += ";\n";
    return code;
}

void DeclarationStatement::buildScope(string scope) {
    this->scope = scope;
    if (this->initDeclarations != nullptr) {
        for (auto &initDeclaration : *(this->initDeclarations)) {
            initDeclaration->buildScope(scope);
        }
    }
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

string DeclarationStatement::codegen() {
    string code = "";
    code += this->type->codegen();
    code += " ";
    for (auto &initDeclaration : *(this->initDeclarations)) {
        code += initDeclaration->codegen();
        code += ", ";
    }
    code.pop_back();
    code.pop_back();
    code += ";\n";
    return code;
}

void LoopStatement::buildScope(string scope) {
    this->scope = scope;
    int child_scope = 0;
    if (this->variable != nullptr) {
        this->variable->buildScope(scope + "." + to_string(child_scope++));
    }
    for (auto &stmt : *statements) {
        stmt->buildScope(scope + "." + to_string(child_scope++));
    }
    for (auto &[from, to, also] : *fromToPairs) {
        if (from != nullptr) {
            from->buildScope(scope + "." + to_string(child_scope++));
        }
        if (to != nullptr) {
            to->buildScope(scope + "." + to_string(child_scope++));
        }
        if (also != nullptr) {
            also->buildScope(scope + "." + to_string(child_scope++));
        }
    }
}

LoopStatement::LoopStatement(Declarator* variable, vector<tuple<class Expression *, class Expression *, class Expression *>> *fromToPairs, vector<class Statement *> *statements, int row, int column) {
    this->variable = variable;    
    this->fromToPairs = fromToPairs;
    this->statements = statements;
    this->row = row;
    this->column = column;
}
string fromToPairsCodeGen(vector<tuple<Expression *, Expression *, Expression *>> *fromToPairs) {
    string code = "";
    vector<string> fromTostep;
    code+= "vector<int> temp;\n";
    for (auto &[from, to, step] : *fromToPairs) {
        code += "for (int i = ";
        code += from->codegen();
        code += "; i < ";
        code += to->codegen();
        code += "; i += ";
        code += step->codegen();
        code += ") {\n";
        code += "temp.push_back(i);\n";
        code += "}\n";
    }
    code += "vector<int>(temp);\n";
    return code;
}

string LoopStatement::codegen() {
    string code = "";
    if (this->variable != nullptr) {
        code += "for ( int ";
        code += this->variable->codegen();
        code += " : ";
        code+= fromToPairsCodeGen(this->fromToPairs);
        code += "++) {\n";
    }
    for (auto &stmt : *statements) {
        code += stmt->codegen();
    }
    code += "}\n";
    return code;
}

//Conditional Statement
void ConditionalStatement::buildScope(string scope) {
    int child_scope = 0;
    this->scope = scope;
    for (auto &[expr, stmt_list] : *ConditionStatements) {
        for (auto &in_stmt : *stmt_list) {
            in_stmt->buildScope(scope + "." + to_string(child_scope++));
        }
        if (expr != nullptr) {
            expr->buildScope(scope + "." + to_string(child_scope++));
        }
    }
}

ConditionalStatement::ConditionalStatement(vector<pair<class Expression *, vector<class Statement *>*>> *ConditionStatements, int row, int column){
    this->ConditionStatements = ConditionStatements;
    this->row = row;
    this->column = column;
}

string ConditionalStatement::codegen() {
    string code = "";
    // get the first pair in the condition statement
    auto &[expr, stmt_list] = ConditionStatements->at(0);
    code += "if (";
    if (expr != nullptr) {
        code += expr->codegen();
    }
    code += ") {\n";
    for (auto &stmt : *stmt_list) {
        code += stmt->codegen();
    }
    code += "}\n";
    // get the rest of the pairs in the conditinal statement
    for (int i = 1; i < ConditionStatements->size(); i++) {
        auto &[expr, stmt_list] = ConditionStatements->at(i);
        if(expr != nullptr) {
            code += "else if (";
            code += expr->codegen();
            code += ") {\n";
        } else {
            code += "else {\n";
        }
        for (auto &stmt : *stmt_list) {
            code += stmt->codegen();
        }
        code += "}\n";
    }
    return code;
}

//Assignment Statement
void AssignmentStatement::buildScope(string scope) {
    this->scope = scope;
    if (this->expression != nullptr) {
        this->expression->buildScope(scope);
    }
    if (this->declarator != nullptr) {
        this->declarator->buildScope(scope);
    }
}

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

string AssignmentStatement::codegen() {
    string code = "";
    if (this->declarator != nullptr) {
        code += this->declarator->codegen();
        switch(this->op) {
            case AssignmentOperator::assign:
                code += " = ";
                break;
            case AssignmentOperator::add_assign:
                code += " += ";
                break;
            case AssignmentOperator::sub_assign:
                code += " -= ";
                break;
            case AssignmentOperator::mul_assign:
                code += " *= ";
                break;
            case AssignmentOperator::div_assign:
                code += " /= ";
                break;
            case AssignmentOperator::mod_assign:
                code += " %= ";
                break;
        }
    }
    code += this->expression->codegen();
    code += ";\n";
    return code;
}