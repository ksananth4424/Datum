#include <iostream>
#include "symbol_table.hpp"
#include "traversal.hpp"
using namespace std;

SymbolTable symtab;

DataType mapTypeToDataType(int type)
{
    switch (type)
    {
    case 0:
        return Integer;
    case 1:
        return Float;
    case 2:
        return String;
    case 3:
        return Char;
    case 4:
        return Boolean;
    case 5:
        return Dataset;
    case 6:
        return Array;
    default:
        throw std::invalid_argument("Unknown type");
    }
}

string dataTypeToString(DataType type)
{
    switch (type)
    {
    case Integer:
        return "Integer";
    case Float:
        return "Float";
    case String:
        return "String";
    case Char:
        return "Char";
    case Boolean:
        return "Boolean";
    case Dataset:
        return "Dataset";
    case Array:
        return "Array";
    default:
        return "Unknown";
    }
}

void traverse(Start *start)
{
    if (start->FunctionList != nullptr)
    {
        cout << "FunctionList\n";
    }
    else
        cout << "No FunctionList\n";
    if (start->StatementList != nullptr)
    {
        cout << "StatementList\n";
        for (auto *statement : *(start->StatementList))
        {
            // Dereference the pointer to vector
            if (statement->statementType == 1)
            {
                cout << "DeclarationStatement\nscope: " << statement->get_scope() << "\n";
            }
            else if (statement->statementType == 2)
            {
                cout << "AssignmentStatement\nscope: " << statement->get_scope() << "\n";
                // cast to AssignmentStatement
                AssignmentStatement *assignmentStatement = statement->assignmentStatement;
                if (assignmentStatement->expression != nullptr)
                {
                    cout << "Expression\n";
                    DataType rhs = traverse_operations(assignmentStatement->expression);
                }
            }
            else if (statement->statementType == 3)
            {
                cout << "ConditionalStatement\nscope: " << statement->get_scope() << "\n";
            }
            else if (statement->statementType == 4)
            {
                cout << "LoopStatement\nscope: " << statement->get_scope() << "\n";
            }
            else if (statement->statementType == 5)
            {
                cout << "ReturnStatement\nscope: " << statement->get_scope() << "\n";
            }
            else if (statement->statementType == 6)
            {
                cout << "BreakStatement\nscope: " << statement->get_scope() << "\n";
            }
            else if (statement->statementType == 7)
            {
                cout << "ContinueStatement\nscope: " << statement->get_scope() << "\n";
            }
            else if (statement->statementType == 8)
            {
                cout << "CompoundStatement\nscope: " << statement->get_scope() << "\n";
            }
        }
    }

    else
        cout << "No StatementList\n";
}

void buildScope(Node *node, string scope)
{
    if (node == nullptr)
        return;
    int child_scope = 0;
    if (Start *start = dynamic_cast<Start *>(node))
    {
        start->scope = scope;
        for (auto &func_dec : *(start->FunctionList))
        {
            buildScope(func_dec, scope + "." + to_string(child_scope++));
        }
        for (auto &stmt : *(start->StatementList))
        {
            buildScope(stmt, scope + "." + to_string(child_scope++));
        }
    }
    else if (FunctionDeclaration *func_dec = dynamic_cast<FunctionDeclaration *>(node))
    {
        func_dec->scope = scope;
        for (auto &param : *(func_dec->inpParameter))
        {
            buildScope(param, scope + "." + to_string(child_scope++));
        }
        for (auto &param : *(func_dec->otherParameter))
        {
            buildScope(param, scope + "." + to_string(child_scope++));
        }
        for (auto &stmt : *(func_dec->statements))
        {
            buildScope(stmt, scope + "." + to_string(child_scope++));
        }
        // add all the function declarations to the symbol table
        // symbol table format: name, input parameters, other parameters, return parameters, scope, row number, column number

        std::cout << func_dec->identifier << std::endl;
        // get the input parameters
        std::vector<DataType> *inputParameters;
        for(auto* param : *(func_dec->inpParameter)){
            // map the type (which is a number)to DataType enum
            DataType dataType = mapTypeToDataType(param->type->type->at(0));
            inputParameters->push_back(dataType);
        }
        // get the other parameters
        std::vector<DataType> *otherParameters;
        for(auto* param : *(func_dec->otherParameter)){
            // map the type (which is a number)to DataType enum
            DataType dataType = mapTypeToDataType(param->type->type->at(0));
            otherParameters->push_back(dataType);
        }
        // get the return parameters
        std::vector<DataType> *returnParameters;
        for(auto* param : *(func_dec->outParameter)){
            // map the type (which is a number)to DataType enum
            DataType dataType = mapTypeToDataType(param->type->type->at(0));
            returnParameters->push_back(dataType);
        }
        if(func_dec->identifier != nullptr){
            std::string name = func_dec->identifier;
            std::string scope = func_dec->get_scope();
            symtab.insert(name, inputParameters, otherParameters, returnParameters, scope, 0, 0);
        }

    } else if (Statement* stmt = dynamic_cast<Statement*>(node)) {
        stmt->scope = scope;
        switch (stmt->statementType)
        {
        case 1:
            // declaration
            stmt->declarationStatement->scope = scope;
            for (auto &decl : *(stmt->declarationStatement->initDeclarations))
            {
                cout << decl->declarator->identifier << endl;
                // map the type (which is a number)to DataType enum
                DataType dataType = mapTypeToDataType(stmt->declarationStatement->type->type->at(0));
                if(decl->declarator->identifier != nullptr){
                    std::string identifier = decl->declarator->identifier;
                    std::string scope = stmt->declarationStatement->get_scope();
                    symtab.insert(identifier, dataType, scope, 0, 0);
                }
            }
            break;
        case 2:
            // assignment
            stmt->assignmentStatement->scope = scope;
            break;
        case 5:
            // return
            stmt->returnStatement->scope = scope;
            break;
        case 6:
            // break
            stmt->breakStatement->scope = scope;
            break;
        case 7:
            // continue
            stmt->continueStatement->scope = scope;
            break;

        case 3:
            // conditional
            {
                ConditionalStatement *cond_stmt = stmt->conditionalStatement;
                cond_stmt->scope;
                for (auto &[expr, stmt_list] : *cond_stmt->ConditionStatements)
                {
                    for (auto &in_stmt : *stmt_list)
                    {
                        buildScope(in_stmt, scope + "." + to_string(child_scope++));
                    }
                }
                break;
            }
        case 4:
            // loop
            {
                LoopStatement *loop_stmt = stmt->loopStatement;
                loop_stmt->scope = scope;
                cout << loop_stmt->identifier << endl;
                if(loop_stmt->identifier != nullptr){
                    std::string identifier = (loop_stmt)->identifier;
                    DataType dataType = Integer;
                    std::string scope = loop_stmt->get_scope();
                    symtab.insert(identifier, dataType, scope, 0, 0);
                }
                for (auto &in_stmt : *loop_stmt->statements) {
                    buildScope(in_stmt, scope + "." + to_string(child_scope++));
                }
                break;
            }
        default:
            // compound
            {
                auto comp_stmt = stmt->compoundStatement;
                for (auto &in_stmt : *comp_stmt)
                {
                    buildScope(in_stmt, scope + "." + to_string(child_scope++));
                }
                break;
            }
        }
    }
    else
    {
        node->scope = scope;
    }
}

void traverse_declaration(Start* root){
    if (root->StatementList != nullptr) {
        for (auto* statement : *(root->StatementList)) {
            if (statement->statementType == 1) {
                //declaration
                DeclarationStatement* decl_stmt = statement->declarationStatement;
                if (decl_stmt->initDeclarations != nullptr) {
                    for (auto* init_decl : *(decl_stmt->initDeclarations)) {
                        if (init_decl->declarator != nullptr) {
                            std::string name = init_decl->declarator->identifier;
                            std::string scope = decl_stmt->get_scope();
                            DataType type = mapTypeToDataType(decl_stmt->type->type->at(0));
                            SymbolTableEntry* entry = symtab.search(name, scope);
                            if (entry != nullptr) {
                                std::cout << "Error: Identifier in declaration statement is already declared\n";
                                continue;
                            }
                            //  type checking here. example: int a = 5.1 should give error here
                            else if (init_decl->initializer->assignmentExpression != nullptr) {
                                DataType rhs = traverse_operations(init_decl->initializer->assignmentExpression->expression);
                                if(type == Integer){
                                    if(rhs != Integer && rhs != Boolean){
                                        std::cout << "Error: Cannot assign "<< rhs <<" to integer\n";
                                    }
                                }
                                else if(type == Float){
                                    if(rhs != Float && rhs != Integer && rhs != Boolean){
                                        std::cout << "Error: Cannot assign "<< rhs <<" to float\n";
                                    }
                                }
                                else if(type == Boolean){
                                    if(rhs != Boolean && rhs != Integer){
                                        std::cout << "Error: Cannot assign "<< rhs <<" to boolean\n";
                                    }
                                }
                                else if(type == String && rhs != String){
                                    std::cout << "Error: Cannot assign " << rhs << " to string\n";
                                }
                                else if(type == Char && rhs != Char){
                                    std::cout << "Error: Cannot assign "<< rhs <<" to char\n";
                                }
                                else if(type == Dataset && rhs != Dataset){
                                    std::cout << "Error: Cannot assign "<< rhs <<" to dataset\n";
                                }
                                else if(type == Array && rhs != Array){
                                    std::cout << "Error: Cannot assign "<< rhs <<" to array\n";
                                }
                            }
                            // type checking in case of array

                            else{
                                symtab.insert(name, type, scope, 0, 0);
                            }


                        }
                    }
                }
            }
        }
    }
}

//function to do semantic checks on the loop statement
void traverse_loop_statement(Start* root) {
    if (root->StatementList != nullptr) {
        for (auto* statement : *(root->StatementList)) {
            if (statement->statementType == 4) {
                //fromtopair check
                LoopStatement* loop_stmt = statement->loopStatement;
                if (loop_stmt->fromToPairs != nullptr) {
                    for (auto &pair : *(loop_stmt->fromToPairs)) {
                        // check if the from and to expressions are of type integer
                        if (std::get<0>(pair) != nullptr) {
                            if (std::get<0>(pair)->castType != 0) {
                                std::cout << "Error: From expression in loop statement is not of type integer\n";
                            }
                        }
                        if (std::get<1>(pair) != nullptr) {
                            if (std::get<1>(pair)->castType != 0) {
                                std::cout << "Error: To expression in loop statement is not of type integer\n";
                            }
                        }
                        if (std::get<2>(pair) != nullptr) {
                            if (std::get<2>(pair)->castType != 0) {
                                std::cout << "Error: Step expression in loop statement is not of type integer\n";
                            }
                        }
                    }
                }
                // check if the identifier is declared in the symbol table
                if (loop_stmt->identifier != nullptr) {
                    std::string name = loop_stmt->identifier;
                    std::string scope = loop_stmt->get_scope();
                    SymbolTableEntry* entry = symtab.search(name, scope);
                    if (entry == nullptr) {
                        std::cout << "Error: Identifier in loop statement is not declared\n";
                    }
                }

                // check if the statements in the loop are valid
                if (loop_stmt->statements != nullptr) {
                    for (auto &stmt : *(loop_stmt->statements)) {
                        // does this work??
                        traverse_loop_statement(dynamic_cast<Start*>(stmt));
                    }
                }
            }
        }
    }
}

void traverse_function_declaration(Node* node) {
    if (node == nullptr) return;
    if (Start* start = dynamic_cast<Start*>(node)) {
        for (auto &func_dec : *(start->FunctionList)) {
            traverse_function_declaration(func_dec);
        }
    } else if (FunctionDeclaration* func_dec = dynamic_cast<FunctionDeclaration*>(node)) {
        for (auto &param : *(func_dec->inpParameter)) {
            if (param->identifier == nullptr) {
                cout << "error: identifier name missing in function declaration input parameter" << endl;
            }
        }
        for (auto &param : *(func_dec->otherParameter)) {
            if (param->identifier == nullptr) {
                cout << "error: identifier name missing in function declaration input parameter" << endl;
            }
        }

        std::cout << func_dec->identifier << std::endl;

        for(auto* param : *(func_dec->outParameter)){
            if (param->identifier == nullptr) {
                cout << "error: unecessary identifier name in function declaration output parameter" << endl;
            }
        }
    } else {
    }
}

DataType traverse_single_chain_expression(SingleChainExpression *singleChainExpression)
{
    char* identifier = singleChainExpression->identifier;
    std::string scope = singleChainExpression->get_scope();
    std::string identifier_str = std::string(identifier);
    SymbolTableEntry* entry = symtab.search(identifier_str, scope);
    if(entry == nullptr)
    {
        cout << "Error: Identifier " << identifier << " not declared\n";
        return Unknown;
    }
    DataType currentDataType = entry->dataType;
    DataType tempDataType = entry->dataType;
    //loop over the function calls
    for(auto &functionCall : *(singleChainExpression->functionCallList))
    {
        //checking if the function is declared
        if(functionCall.first->identifier != nullptr)
        {
            std::string function_name = std::string(functionCall.first->identifier);
            std::string temp_scope = functionCall.first->get_scope();
            SymbolTableEntry* function_entry = symtab.search(function_name, temp_scope);
            if(function_entry == nullptr)
            {
                cout << "Error: Function " << function_name << " not declared\n";
                return Unknown;
            }
            //checking if the function is called with the correct number of arguments
            if(functionCall.second->size() != function_entry->inputParameters->size())
            {
                cout << "Error: Function " << function_name << " called with incorrect number of arguments\n";
                return Unknown;
            }
            //checking if the arguments are of the correct type
            for(int i = 0; i < functionCall.second->size(); i++)
            {
                Expression* argument = functionCall.second->at(i);
                DataType argumentDataType = traverse_operations(argument);
                if(argumentDataType != function_entry->inputParameters->at(i))
                {
                    cout << "Error: Argument " << i << " of function " << function_name << " is of incorrect type\n";
                    return Unknown;
                }
            }
            //checking if the return type of the function is the same as the current data type
            if(function_entry->returnParameters->size() > 0)
            {
                if(function_entry->returnParameters->at(0) != tempDataType)
                {
                    cout << "Error: Function " << function_name << " returns a value of incorrect type\n";
                    return Unknown;
                }
            }
            tempDataType = function_entry->dataType;
        }
        else
        {
            //checking if the inbuilt function is called with the correct number of arguments
            // incorrect condition in the loop statement fix this
            if(functionCall.second->size() != functionCall.first->argumentList->size())
            {
                cout << "Error: Inbuilt function called with incorrect number of arguments\n";
                return Unknown;
            }
            //checking if the argument is of the correct type
            Expression* argument = functionCall.second->at(0);
            DataType argumentDataType = traverse_operations(argument);
            if(argumentDataType != tempDataType)
            {
                cout << "Error: Argument of inbuilt function is of incorrect type\n";
                return Unknown;
            }
            // check this once!!
            tempDataType = Dataset;
        }
    }
    


}

DataType traverse_multi_chain_expression(MultiChainExpression *multiChainExpression)
{
}

DataType traverse_operations(Expression *root)
{
    if (root == nullptr)
        return DataType::Unknown;
    if (root->castType == 1)
    {
        cout << "UnaryExpression\n";
        UnaryExpression *unaryExpression = dynamic_cast<UnaryExpression *>(root);
        DataType datatype = mapTypeToDataType(unaryExpression->constantValue->type->type->at(0));
        if (datatype == Integer)
        {
            return Integer;
        }
        else if (datatype == Float)
        {
            return Float;
        }
        else if (datatype == String)
        {
            for (auto &o : *(unaryExpression->op))
            {
                if (o == minus_op)
                {
                    cout << "Error: Cannot \"-\" perform operation on string\n";
                }
                else if (o == plus_op)
                {
                    cout << "Error: Cannot \"+\" perform operation on string\n";
                }
            }
            return String;
        }
        else if (datatype == Char)
        {
            for (auto &o : *(unaryExpression->op))
            {
                if (o == minus_op)
                {
                    cout << "Error: Cannot \"-\" perform operation on string\n";
                }
                else if (o == plus_op)
                {
                    cout << "Error: Cannot \"+\" perform operation on string\n";
                }
            }
            return Char;
        }
        else if (datatype == Boolean)
        {
            for (auto &o : *(unaryExpression->op))
            {
                if (o == minus_op)
                {
                    cout << "Error: Cannot \"-\" perform operation on string\n";
                }
                else if (o == plus_op)
                {
                    cout << "Error: Cannot \"+\" perform operation on string\n";
                }
            }
            return Boolean;
        }
        else if (datatype == Dataset)
        {
            if (unaryExpression->op->size() > 0)
            {
                cout << "Error: Cannot perform Unary operations on dataset\n";
            }
            return Dataset;
        }
        else if (datatype == Array)
        {
            if (unaryExpression->op->size() > 0)
            {
                cout << "Error: Cannot perform Unary operations on array\n";
            }
            return Array;
        }
    }
    else if (root->castType == 2)
    {
        BinaryExpression *binaryExpression = dynamic_cast<BinaryExpression *>(root);
        DataType lhs = traverse_operations(binaryExpression->lhs);
        DataType rhs = traverse_operations(binaryExpression->rhs);
        if (binaryExpression->op == add_op)
        {
            if (lhs == Integer && rhs == Integer)
            {
                return Integer;
            }
            else if (lhs == Float && rhs == Float)
            {
                return Float;
            }
            else if (lhs == String && rhs == String)
            {
                return String;
            }
            else if (lhs == String && rhs == Char)
            {
                return String;
            }
            else if (lhs == Boolean && rhs == Boolean)
            {
                return Boolean;
            }
            else if (lhs == Integer && rhs == Float)
            {
                return Float;
            }
            else if (lhs == Float && rhs == Integer)
            {
                return Float;
            }
            else if (lhs == Boolean && rhs == Integer)
            {
                return Integer;
            }
            else if (lhs == Integer && rhs == Boolean)
            {
                return Integer;
            }
            else if (lhs == Boolean && rhs == Float)
            {
                return Float;
            }
            else if (lhs == Float && rhs == Boolean)
            {
                return Float;
            }
            else if (lhs == Dataset && rhs == Dataset)
            {
                return Dataset;
            }
            else if (lhs == Array && rhs == Array)
            {
                return Array;
            }
            else
            {
                cout << "Error: Cannot perform \"+\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        }
        else if (binaryExpression->op == sub_op)
        {
            if (lhs == Integer && rhs == Integer)
            {
                return Integer;
            }
            else if (lhs == Float && rhs == Float)
            {
                return Float;
            }
            else
            {
                cout << "Error: Cannot perform \"-\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        }
        else if (binaryExpression->op == mul_op)
        {
            if (lhs == Integer && rhs == Integer)
            {
                return Integer;
            }
            else if (lhs == Float && rhs == Float)
            {
                return Float;
            }
            else
            {
                cout << "Error: Cannot perform \"*\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        }
        else if (binaryExpression->op == div_op)
        {
            if (lhs == Integer && rhs == Integer)
            {
                return Integer;
            }
            else if (lhs == Float && rhs == Float)
            {
                return Float;
            }
            else
            {
                cout << "Error: Cannot perform \"/\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        }
        else if (binaryExpression->op == mod_op)
        {
            if (lhs == Integer && rhs == Integer)
            {
                return Integer;
            }
            else
            {
                cout << "Error: Cannot perform \"%\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        }
        else if (binaryExpression->op == and_op)
        {
            if (lhs == Boolean && rhs == Boolean)
            {
                return Boolean;
            }
            else if (lhs == Integer && rhs == Integer)
            {
                return Integer;
            }
            else
            {
                cout << "Error: Cannot perform \"&&\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        }
        else if (binaryExpression->op == or_op)
        {
            if (lhs == Boolean && rhs == Boolean)
            {
                return Boolean;
            }
            else if (lhs == Integer && rhs == Integer)
            {
                return Integer;
            }
            else
            {
                cout << "Error: Cannot perform \"||\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        }
        else if (binaryExpression->op == eq_op)
        {
            if (lhs == Integer && rhs == Integer)
            {
                return Boolean;
            }
            else if (lhs == Float && rhs == Float)
            {
                return Boolean;
            }
            else if (lhs == String && rhs == String)
            {
                return Boolean;
            }
            else if (lhs == Char && rhs == Char)
            {
                return Boolean;
            }
            else if (lhs == Boolean && rhs == Boolean)
            {
                return Boolean;
            }
            else if (lhs == Dataset && rhs == Dataset)
            {
                return Boolean;
            }
            else if (lhs == Array && rhs == Array)
            {
                return Boolean;
            }
            else
            {
                cout << "Error: Cannot perform \"==\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        }
        else if (binaryExpression->op == neq_op)
        {
            if (lhs == Integer && rhs == Integer)
            {
                return Boolean;
            }
            else if (lhs == Float && rhs == Float)
            {
                return Boolean;
            }
            else if (lhs == String && rhs == String)
            {
                return Boolean;
            }
            else if (lhs == Char && rhs == Char)
            {
                return Boolean;
            }
            else if (lhs == Boolean && rhs == Boolean)
            {
                return Boolean;
            }
            else if (lhs == Dataset && rhs == Dataset)
            {
                return Boolean;
            }
            else if (lhs == Array && rhs == Array)
            {
                return Boolean;
            }
            else
            {
                cout << "Error: Cannot perform \"!=\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        }
    }
    else if (root->castType == 3)
    {
        return traverse_single_chain_expression(dynamic_cast<SingleChainExpression *>(root));
    }
    else if (root->castType == 4)
    {
        return traverse_multi_chain_expression(dynamic_cast<MultiChainExpression *>(root));
    }
    else
    {
        return DataType::Unknown;
    }
}

void traverse_assignment(Start* root)
{
    if(root == nullptr)
    {
        return;
    }
    if(root->StatementList == nullptr)
    {
        return;
    }
    for(auto* each_statement : *(root->StatementList))
    {
        if(each_statement->statementType == 2)
        {
            AssignmentStatement* assignmentStatement = each_statement->assignmentStatement;
            if(assignmentStatement->declarator == nullptr)
            {
                cout << "Error: Declarator is missing in assignment statement\n";
                return;
            }
            if(assignmentStatement->expression == nullptr)
            {
                cout << "Error: Expression is missing in assignment statement\n";
                return;
            }
            DataType lhs = traverse_single_chain_expression(assignmentStatement->declarator);
            DataType rhs = traverse_operations(assignmentStatement->expression);
            if(lhs != rhs)
            {
                cout << "Error: Assignment is invalid\n";
            }
        }
    }
}