#include <iostream>
#include "symbol_table.hpp"
#include "traversal.hpp"
using namespace std;

SymbolTable symtab;
int error_count = 0;

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

string mapInbuiltFunctionToString(InbuiltFunctions func){
    switch (func){
    case func_show_bar: return "show_bar";
    case func_show_line: return "show_line";
    case func_show_scatter: return "show_scatter";
    case func_show_box: return "show_box";
    case func_row: return "row";
    case func_col: return "col";
    case func_filter: return "filter";
    case func_sum: return "sum";
    case func_max: return "max";
    case func_min: return "min";
    case func_mean: return "mean";
    case func_join: return "join";
    case func_read: return "read";
    case func_write: return "write";
    case func_unique: return "unique";
    case func_show: return "show";
    case func_split: return "split";
    case func_sort: return "sort";
    case func_shuffle: return "shuffle";
    case func_add: return "add";
    case func_shape: return "shape";
    case func_drop: return "drop";
    case none: return "none";
    default: return "none";
    }
}

void insert_inbuiltfunction_symtab(std::string func){
    std::string scope = "g";
    if(func == "show_bar"){
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope,0,0);
    } else if(func == "show_line"){
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope,0,0);
    } else if(func == "show_scatter"){
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope,0,0);
    } else if(func == "show_box"){
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope,0,0);
    } else if(func == "row"){
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope,0,0);
    } else if(func == "col"){
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope,0,0);
    } else if(func == "filter"){
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope,0,0);
    } else if(func == "sum"){
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope,0,0);
    } else if(func == "max"){
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope,0,0);
    } else if(func == "min"){
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope,0,0);
    } else if(func == "read"){
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope,0,0);
    } else if(func == "write"){
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope,0,0);
    } else if(func == "unique"){
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope,0,0);
    } else if(func == "show"){
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope,0,0);
    } else if(func == "split"){
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope,0,0);
    } else if(func == "sort"){
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope,0,0);
    } else if(func == "shuffle"){
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope,0,0);
    } else if(func == "add"){
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope,0,0);
    } else if(func == "shape"){
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope,0,0);
    } else if(func == "drop"){
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope,0,0);
    }else{
        cout << "\e[31m Error: \e[0m Inbuilt function " << func << " not supported.\n";
        error_count++;
    }
}

void traverse_statement(Statement *stmt,bool isFunction=false,bool isLoop=false)
{
    cout << "entering statement" << endl;
    if (stmt == nullptr) {
        return;
    }
    cout << stmt->statementType << endl;
    switch (stmt->statementType)
    {
    case 1:
        // declaration
        cout << "declaration" << endl;
        traverse_declaration(dynamic_cast<DeclarationStatement *>(stmt->declarationStatement));
        break;
    case 2:
        // assignment
        cout << "assignment" << endl;
        traverse_assignment(dynamic_cast<AssignmentStatement *>(stmt->assignmentStatement));
        break;
    case 5:
        // return
        if(!isFunction){
            cout<<"Error: Return statement is not allowed outside of function!\n";
        }
        break;
    case 6:
        // break
        if(!isLoop){
            cout<<"Error: Break statement is not allowed outside of loop!\n";
        }
        break;
    case 7:
        // continue
        if(!isLoop){
            cout<<"Error: Continue statement is not allowed outside of loop!\n";
        }
        break;

    case 3:
        // conditional
        cout << "conditional" << endl;
        traverse_if_statement(stmt->conditionalStatement,isFunction,isLoop);
        break;
    case 4:
        // loop
        // cout << "loop" << endl;
        traverse_loop_statement(stmt->loopStatement,isFunction);
        break;
    default:
        // compound
        {
            cout << "compound" << endl;
            auto comp_stmt = stmt->compoundStatement;
            for (auto &in_stmt : *comp_stmt)
            {
                traverse_statement(dynamic_cast<Statement *>(in_stmt),isFunction,isLoop);
            }
            break;
        }
    }
}

void buildScope(Node *node, string scope)
{
    cout << "entering buildscope" << endl;
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
    }
    else if (Statement *stmt = dynamic_cast<Statement *>(node))
    {
        stmt->scope = scope;
        switch (stmt->statementType)
        {
        case 1:
            // declaration
            stmt->declarationStatement->scope = scope;
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
                for (auto &in_stmt : *loop_stmt->statements)
                {
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
    else {
        node->scope = scope;
    }
}

void traverse_declaration(DeclarationStatement *decl_stmt)
{
    cout << "entering declaration" << endl;
    if (decl_stmt == nullptr)
        return; 
    if (decl_stmt->initDeclarations != nullptr)
    {
        for (auto *init_decl : *(decl_stmt->initDeclarations))
        {
            if (init_decl->declarator != nullptr)
            {
                std::string name = init_decl->declarator->identifier;
                // cout << name << endl;
                std::string scope = decl_stmt->get_scope();
                // cout << scope << endl;
                DataType type = mapTypeToDataType(decl_stmt->type->type->at(0));
                SymbolTableEntry *entry = symtab.search(name, scope);
                if (entry != nullptr)
                {
                    std::cout << "\e[31m Error: \e[0m Identifier " << name << " in declaration statement is already declared\n";
                    error_count++;
                    continue;
                }else
                {
                    cout << "inserting into symbol table" << endl;
                    symtab.insert(name, type, scope, 0, 0);
                }
                //  type checking here. example: int a = 5.1 should give error here
                if (init_decl->initializer != nullptr && init_decl->initializer->assignmentExpression!=nullptr && init_decl->initializer->assignmentExpression->expression!=nullptr){
                    Expression* expr = init_decl->initializer->assignmentExpression->expression;
                    DataType rhs = traverse_operations(expr,scope);
                    if (type == Integer)
                    {
                        if (rhs != Integer && rhs != Boolean)
                        {
                            std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to integer\n";
                            error_count++;
                        }
                    }
                    else if (type == Float)
                    {
                        if (rhs != Float && rhs != Integer && rhs != Boolean)
                        {
                            std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to float\n";
                            error_count++;
                        }
                    }
                    else if (type == Boolean)
                    {
                        if (rhs != Boolean && rhs != Integer)
                        {
                            std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to boolean\n";
                            error_count++;
                        }
                    }
                    else if (type == String && rhs != String)
                    {
                        std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to string\n";
                        error_count++;
                    }
                    else if (type == Char && rhs != Char)
                    {
                        std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to char\n";
                        error_count++;
                    }
                    else if (type == Dataset && rhs != Dataset)
                    {
                        std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to dataset\n";
                        error_count++;
                    }
                    else if (type == Array && rhs != Array)
                    {
                        std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to array\n";
                        error_count++;
                    }
                }
                // type checking in case of array

                } else cout<<"initilizer is null"<<endl;
            }
        }
    cout << "exiting declaration" << endl;
}

// function to do semantic checks on the loop statement
void traverse_loop_statement(LoopStatement *loop_stmt,bool isFunction)
{
    cout << "entering loop" << endl;
    std::string name = loop_stmt->identifier;
    std::string scope = loop_stmt->scope;
    SymbolTableEntry *entry = symtab.search(name, scope);
    if (entry != nullptr)
    {
        std::cout << "\e[31m Error: \e[0m Identifier in loop statement is already declared\n";
        error_count++;
    }else{
        DataType dataType = Integer;
        symtab.insert(name, dataType, scope, 0, 0);
    }                
    
    // fromtopair check
    if (loop_stmt->fromToPairs != nullptr)
    {
        for (auto &pair : *(loop_stmt->fromToPairs))
        {
            // check if the from and to expressions are of type integer
            if (std::get<0>(pair) != nullptr)
            {
                if (traverse_operations(std::get<0>(pair),scope) != 0)
                {
                    std::cout << "\e[31m Error: \e[0m From expression in loop statement is not of type integer\n";
                    error_count++;
                }
            }
            if (std::get<1>(pair) != nullptr)
            {
                if (traverse_operations(std::get<1>(pair),scope) != 0)
                {
                    std::cout << "\e[31m Error: \e[0m To expression in loop statement is not of type integer\n";
                    error_count++;
                }
            }
            if (std::get<2>(pair) != nullptr)
            {
                if (traverse_operations(std::get<2>(pair),scope) != 0)
                {
                    std::cout << "\e[31m Error: \e[0m Step expression in loop statement is not of type integer\n";
                    error_count++;
                }
            }
        }
    }

    // check if the statements in the loop are valid
    if (loop_stmt->statements != nullptr)
    {
        for (auto &stmt : *(loop_stmt->statements))
        {
            Statement *statement = dynamic_cast<Statement *>(stmt);
            traverse_statement(statement,isFunction,true);
        }
    }
    cout << "exiting loop" << endl;
}

// need to check the return statement
void traverse_function_declaration(FunctionDeclaration *func_dec) {
    cout << "entering function declaration" << endl;
    if (func_dec == nullptr)
        return;
    
    // get the input parameters
    std::vector<DataType> *inputParameters = new std::vector<DataType>();
    for (auto *param : *(func_dec->inpParameter)) {
        // map the type (which is a number)to DataType enum
        DataType dataType = mapTypeToDataType(param->type->type->at(0));
        inputParameters->push_back(dataType);
        string scope = param->get_scope();
        if (param->identifier == nullptr) {
            cout << "Error: identifier name missing in function declaration input parameter!" << endl;
        } else {
            string name = param->identifier->identifier;
            symtab.insert(name, dataType, scope, 0, 0);
        }
    }
    // get the other parameters
    std::vector<DataType> *otherParameters = new std::vector<DataType>();
    for (auto *param : *(func_dec->otherParameter)) {
        // map the type (which is a number)to DataType enum
        DataType dataType = mapTypeToDataType(param->type->type->at(0));
        otherParameters->push_back(dataType);
        string scope = param->get_scope();
        if (param->identifier == nullptr) {
            cout << "Error: identifier name missing in function declaration other parameter!" << endl;
        } else {
            string name = param->identifier->identifier;
            symtab.insert(name, dataType, scope, 0, 0);
        }
    }
    // get the return parameters
    std::vector<DataType> *returnParameters = new std::vector<DataType>();
    for (auto *param : *(func_dec->outParameter)) {
        // map the type (which is a number)to DataType enum
        DataType dataType = mapTypeToDataType(param->type->type->at(0));
        returnParameters->push_back(dataType);
        if (param->identifier != nullptr) {
            cout << "Error: unecessary identifier missing in function declaration return parameter!" << endl;
        }
    }
    if (func_dec->identifier != nullptr) {
        std::string name = func_dec->identifier;
        std::string scope = func_dec->get_scope();
        symtab.insert(name, inputParameters, otherParameters, returnParameters, scope, 0, 0);
    } else {
        cout << "Error: identifier name missing in function declaration!" << endl;
    }
    
    for (auto *stmt : *(func_dec->statements)) {
        if (stmt->returnStatement != nullptr) {
            // check if the return statement is valid
            auto returnExpression = stmt->returnStatement->expression;
            auto returnDataType = traverse_operations(returnExpression, stmt->get_scope());
            // TODO: match returnDataType with returnParameters
            // traverse_return_statement(stmt->returnStatement);
        } else {
            Statement *statement = dynamic_cast<Statement *>(stmt);
            traverse_statement(statement,true,false);
        }
    }
}

void traverse_function_call(FunctionCall* functionCall,SymbolTableEntry* entry){
    cout << "Entering function call" << endl;
    vector<Argument *> *argumentList = functionCall->argumentList;
    vector<DataType> *otherParameters = entry->otherParameters;
    // vector<DataType> *outParameters = entry->returnParameters;

    if (argumentList->size() != otherParameters->size()) {
        cout << "Error: Function " << entry->name << " expects " << otherParameters->size() << " arguments but " << argumentList->size() << " provided\n";
    } else {
        for (int i = 0; i < argumentList->size(); i++) {
            Argument *argument = argumentList->at(i);
            if (argument->expression != nullptr) {
                Expression *expr = argument->expression;
                DataType exprType = traverse_operations(expr, entry->scope);
                if (exprType == Boolean) {
                    cout << "Expression is boolean\n";
                }
                if (exprType != otherParameters->at(i)) {
                    cout << "Error: Function " << entry->name << " expects " << dataTypeToString(otherParameters->at(i)) << " but " << dataTypeToString(exprType) << " provided\n";
                }
            } else if (argument->fromToAlsoExpression != nullptr) {
                for (auto &tuple : *(argument->fromToAlsoExpression)) {
                    Expression *from = std::get<0>(tuple);
                    Expression *to = std::get<1>(tuple);
                    Expression *also = std::get<2>(tuple);
                    DataType fromType = traverse_operations(from, entry->scope);
                    DataType toType = traverse_operations(to, entry->scope);
                    DataType alsoType = traverse_operations(also, entry->scope);
                    if (fromType != Integer || toType != Integer || alsoType != Integer) {
                        cout << "Error: Function " << entry->name << " expects integer arguments\n";
                    }
                }
            } else if (argument->statements != nullptr) {
                for (auto &stmt : *(argument->statements)) {
                    Statement *statement = dynamic_cast<Statement *>(stmt);
                    traverse_statement(statement,true,false);
                }
            } else {
                cout << "Error: Argument is null\n";
            }
        }
    }
    
}

DataType traverse_single_chain_expression(SingleChainExpression *singleChainExpression,string scope)
{
    cout << "Entering single chain expression" << endl;
    if(singleChainExpression==nullptr){
        cout << "\e[31m Error: \e[0m SingleChainExpression is null\n";
        error_count++;
        return Unknown;
    }
    char *identifier = singleChainExpression->identifier;
    std::string identifier_str = std::string(identifier);
    SymbolTableEntry *entry = symtab.search(identifier_str, scope);
    cout << "Identifier: " << identifier_str << endl;
    if (entry == nullptr){
        cout << "\e[31m Error: \e[0m Identifier " << identifier_str << " not declared\n";
        error_count++;
        return Unknown;
    }
    DataType currentDataType = entry->dataType;
    for(auto &funcPair : *(singleChainExpression->functionCallList)){
        FunctionCall *functionCall = funcPair.first;
        cout << "Function call: " << endl;
    
        std::string functionName;
        if (functionCall->identifier != nullptr){
            functionName = std::string(functionCall->identifier);
        } else {
            functionName = mapInbuiltFunctionToString(functionCall->inbuiltFunc);
        }

        SymbolTableEntry *functionEntry = symtab.search(functionName, scope);
        if(functionEntry==nullptr){
            cout << "\e[31m Error: \e[0m Function " << functionName << " not declared\n";
            error_count++;
            return Unknown;
        }
        vector<DataType> *inputParameters = functionEntry->inputParameters;
        vector<DataType> *returnParameters = functionEntry->returnParameters;
        traverse_function_call(functionCall,functionEntry);
        if (functionEntry->inputParameters->size()!=1){
            cout << "\e[31m Error: \e[0m Function " << functionName << " expects more than one input parameter\n";
            error_count++;
        } else if(returnParameters->size()!=1){
            cout << "\e[31m Error: \e[0m Function " << functionName << " returns more than one return parameter\n";
            error_count++;
        } else {
            DataType functionInputParameter = functionEntry->inputParameters->at(0);
            if (functionInputParameter != currentDataType){
                cout << "\e[31m Error: \e[0m Function " << functionName << " input parameter is not of the same type as the identifier\n"<<functionName<<" expects "<<dataTypeToString(functionInputParameter)<<" but input is of type "<<dataTypeToString(currentDataType)<<"!\n";
                error_count++;
            } 
            currentDataType = returnParameters->at(0);
        }
    }
    cout << "Exiting single chain expression\n";
    return currentDataType;
}

vector<DataType> traverse_function_call_list_multi(vector<pair<FunctionCall *, vector<Expression *>*>> functionCallList,vector<DataType> currentDataType,string scope)
{
    cout << "Entering function call list multi" << endl;
    for(auto &funcPair : (functionCallList)){
            FunctionCall *functionCall = funcPair.first;
            std::string functionName = std::string(functionCall->identifier);
            SymbolTableEntry *functionEntry = symtab.search(functionName, scope);
            if(functionEntry==nullptr){
                cout << "\e[31m Error: \e[0m Function " << functionName << " not declared\n";
                error_count++;
                return currentDataType;
            }
            vector<DataType> *inputParameters = functionEntry->inputParameters;
            vector<DataType> *returnParameters = functionEntry->returnParameters;
            if(inputParameters->size()!=currentDataType.size()){
                cout << "\e[31m Error: \e[0m Function " << functionName << " expects more than the provided input parameter\n";
                error_count++;

            } else {
                for(int i=0;i<inputParameters->size();i++){
                    if(inputParameters->at(i)!=currentDataType.at(i)){
                        cout << "\e[31m Error: \e[0m Function " << functionName << " input parameter is not of the same type as the "<<functionName<<" expects "<<dataTypeToString(inputParameters->at(i))<<" but input is of type "<<dataTypeToString(currentDataType.at(i))<<"!\n";
                        error_count++;
                    }
                }
            }
            currentDataType.clear();
            currentDataType = *returnParameters;
    }
    cout << "Exiting function call list multi\n";
    return currentDataType;
}

//intbuilt functions must be initialized by default in symbol table
// CHECK: init of functionCallName
vector<DataType> traverse_multi_chain_expression(MultiChainExpression *multiChainExpression,string scope)
{
    cout << "Entering multi chain expression" << endl;
    if(multiChainExpression->functionCall!=nullptr || multiChainExpression->inbuiltFunc!=InbuiltFunctions::none){
        std::string functionCallName;
        if(multiChainExpression->functionCall!=nullptr){
            functionCallName = string(multiChainExpression->functionCall->identifier) ;
        } else {
            functionCallName = string(mapInbuiltFunctionToString(multiChainExpression->inbuiltFunc));
        }
        SymbolTableEntry *entry = symtab.search(functionCallName,scope);
        if(entry==nullptr){
            cout << "\e[31m Error: \e[0m Function " << functionCallName << " not declared\n";
            error_count++;
            return vector<DataType>();
        }
        traverse_function_call(multiChainExpression->functionCall,entry);
        return traverse_function_call_list_multi(*multiChainExpression->functionCallList,*entry->returnParameters,scope);
    } else {
        vector<Expression*> *inputList = multiChainExpression->functionCallStart.second;
        vector<DataType> inputTypes;
        for(auto &expr : *inputList){
            DataType exprType = traverse_operations(expr,scope);
            inputTypes.push_back(exprType);
        }
        return traverse_function_call_list_multi(*(multiChainExpression->functionCallList),inputTypes,scope);
    }
}

DataType traverse_operations(Expression *root,string scope){
    cout << "Entering traverse operations" << endl;
    if (root == nullptr)
        return DataType::Unknown;
    if (root->castType == 1)
    {
        UnaryExpression *unaryExpression = dynamic_cast<UnaryExpression *>(root);
        if(unaryExpression->constantValue==nullptr){
            std::string identifier = std::string(unaryExpression->identifier);
            SymbolTableEntry *entry = symtab.search(identifier, scope);
            if (entry == nullptr){
                cout << "\e[31m Error: \e[0m Identifier " << identifier << " not declared\n";
                error_count++;
                return Unknown;
            }
            DataType datatype = entry->dataType;
            if (datatype == Integer) return Integer;
            else if (datatype == Float){
                for(auto &o:*(unaryExpression->op)){
                    if (o==not_op) {
                        cout << "\e[31m Error: \e[0m Cannot \"!\" perform operation on float\n";
                        error_count++;
                    }
                    return Float;
                    }
            } else if (datatype == String){
                for (auto &o : *(unaryExpression->op)){
                    if (o == minus_op){
                        cout << "Error: Cannot \"-\" perform operation on string\n";
                    }else if (o == plus_op){
                        cout << "Error: Cannot \"+\" perform operation on string\n";
                    }
                }
                return String;
            } else if (datatype == Char){
                for (auto &o : *(unaryExpression->op)){
                    if (o == minus_op){
                        cout << "\e[31m Error: \e[0m Cannot \"-\" perform operation on string\n";
                        error_count++;
                    } else if (o == plus_op){
                        cout << "\e[31m Error: \e[0m Cannot \"+\" perform operation on string\n";
                        error_count++;
                    }
                }
                return Char;
            } else if (datatype == Boolean)
            {
                for (auto &o : *(unaryExpression->op)){
                    if (o == minus_op){
                        cout << "\e[31m Error: \e[0m Cannot \"-\" perform operation on string\n";
                        error_count++;
                    } else if (o == plus_op){
                        cout << "\e[31m Error: \e[0m Cannot \"+\" perform operation on string\n";
                        error_count++;
                    }
                }
                return Boolean;
            } else if (datatype == Dataset){
                if (unaryExpression->op->size() > 0){
                    cout << "\e[31m Error: \e[0m Cannot perform Unary operations on dataset\n";
                    error_count++;
                }
                return Dataset;
            } else if (datatype == Array){
                if (unaryExpression->op->size() > 0){
                    cout << "\e[31m Error: \e[0m Cannot perform Unary operations on array\n";
                    error_count++;
                }
                return Array;
            } else {
                return DataType::Unknown;
            }  
        } else {
        DataType datatype = mapTypeToDataType(unaryExpression->constantValue->type->type->at(0));
        if (datatype == Integer) {
            return Integer;
        } else if (datatype == Float) {
            return Float;
        } else if (datatype == String) {
            for (auto &o : *(unaryExpression->op)){
                if (o == minus_op){
                    cout << "\e[31m Error: \e[0m Cannot \"-\" perform operation on string\n";
                    error_count++;
                }else if (o == plus_op){
                    cout << "\e[31m Error: \e[0m Cannot \"+\" perform operation on string\n";
                    error_count++;
                }
            }
            return String;
        } else if (datatype == Char) {
            for (auto &o : *(unaryExpression->op)){
                if (o == minus_op) {
                    cout << "\e[31m Error: \e[0m Cannot \"-\" perform operation on string\n";
                    error_count++;
                } else if (o == plus_op){
                    cout << "\e[31m Error: \e[0m Cannot \"+\" perform operation on string\n";
                    error_count++;
                }
            }
            return Char;
        } else if (datatype == Boolean){
            for (auto &o : *(unaryExpression->op))
            {
                if (o == minus_op){
                    cout << "\e[31m Error: \e[0m Cannot \"-\" perform operation on string\n";
                    error_count++;
                } else if (o == plus_op){
                    cout << "\e[31m Error: \e[0m Cannot \"+\" perform operation on string\n";
                    error_count++;
                }
            }
            return Boolean;
        } else if (datatype == Dataset){
            if (unaryExpression->op->size() > 0){
                cout << "\e[31m Error: \e[0m Cannot perform Unary operations on dataset\n";
                error_count++;
            }
            return Dataset;
        } else if (datatype == Array){
            if (unaryExpression->op->size() > 0){
                cout << "\e[31m Error: \e[0m Cannot perform Unary operations on array\n";
                error_count++;
            }
            return Array;
        }
        }
    } else if (root->castType == 2){
        BinaryExpression *binaryExpression = dynamic_cast<BinaryExpression *>(root);
        DataType lhs = traverse_operations(binaryExpression->lhs,scope);
        DataType rhs = traverse_operations(binaryExpression->rhs,scope);
        if (binaryExpression->op == add_op){
            if (lhs == Integer && rhs == Integer){
                return Integer;
            } else if (lhs == Float && rhs == Float){
                return Float;
            } else if (lhs == String && rhs == String){
                return String;
            } else if (lhs == String && rhs == Char){
                return String;
            } else if (lhs == Boolean && rhs == Boolean){
                return Boolean;
            } else if (lhs == Integer && rhs == Float){
                return Float;
            } else if (lhs == Float && rhs == Integer){
                return Float;
            } else if (lhs == Boolean && rhs == Integer){
                return Integer;
            } else if (lhs == Integer && rhs == Boolean){
                return Integer;
            } else if (lhs == Boolean && rhs == Float){
                return Float;
            } else if (lhs == Float && rhs == Boolean){
                return Float;
            } else if (lhs == Dataset && rhs == Dataset){
                return Dataset;
            } else if (lhs == Array && rhs == Array){
                return Array;
            } else {
                cout << "\e[31m Error: \e[0m Cannot perform \"+\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
                error_count++;
                return Unknown;
            }
        }
        else if (binaryExpression->op == sub_op)
        {
            if (lhs == Integer && rhs == Integer){
                return Integer;
            } else if (lhs == Float && rhs == Float){
                return Float;
            } else {
                cout << "\e[31m Error: \e[0m Cannot perform \"-\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
                error_count++;
                return Unknown;
            }
        } else if (binaryExpression->op == mul_op){
            if (lhs == Integer && rhs == Integer){
                return Integer;
            } else if (lhs == Float && rhs == Float){
                return Float;
            } else{
                cout << "\e[31m Error: \e[0m Cannot perform \"*\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
                error_count++;
                return Unknown;
            }
        } else if (binaryExpression->op == div_op){
            if (lhs == Integer && rhs == Integer){
                return Integer;
            } else if (lhs == Float && rhs == Float){
                return Float;
            } else{
                cout << "\e[31m Error: \e[0m Cannot perform \"/\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
                error_count++;
                return Unknown;
            }
        } else if (binaryExpression->op == mod_op){
            if (lhs == Integer && rhs == Integer){
                return Integer;
            } else {
                cout << "\e[31m Error: \e[0m Cannot perform \"%\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
                error_count++;
                return Unknown;
            }
        } else if (binaryExpression->op == and_op){
            if (lhs == Boolean && rhs == Boolean){
                return Boolean;
            } else if (lhs == Integer && rhs == Integer){
                return Integer;
            } else {
                cout << "\e[31m Error: \e[0m Cannot perform \"&&\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
                error_count++;
                return Unknown;
            }
        } else if (binaryExpression->op == or_op){
            if (lhs == Boolean && rhs == Boolean){
                return Boolean;
            } else if (lhs == Integer && rhs == Integer){
                return Integer;
            } else{
                cout << "\e[31m Error: \e[0m Cannot perform \"||\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
                error_count++;
                return Unknown;
            }
        } else if (binaryExpression->op == eq_op){
            if (lhs == Integer && rhs == Integer){
                return Boolean;
            } else if (lhs == Float && rhs == Float){
                return Boolean;
            } else if (lhs == String && rhs == String){
                return Boolean;
            } else if (lhs == Char && rhs == Char){
                return Boolean;
            } else if (lhs == Boolean && rhs == Boolean){
                return Boolean;
            } else if (lhs == Dataset && rhs == Dataset){
                return Boolean;
            } else if (lhs == Array && rhs == Array){
                return Boolean;
            } else{
                cout << "\e[31m Error: \e[0m Cannot perform \"==\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
                error_count++;
                return Unknown;
            }
        } else if (binaryExpression->op == neq_op){
            if (lhs == Integer && rhs == Integer){
                return Boolean;
            } else if (lhs == Float && rhs == Float){
                return Boolean;
            } else if (lhs == String && rhs == String){
                return Boolean;
            } else if (lhs == Char && rhs == Char){
                return Boolean;
            } else if (lhs == Boolean && rhs == Boolean){
                return Boolean;
            } else if (lhs == Dataset && rhs == Dataset){
                return Boolean;
            } else if (lhs == Array && rhs == Array){
                return Boolean;
            } else{
                cout << "\e[31m Error: \e[0m Cannot perform \"!=\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
                error_count++;
                return Unknown;
            }
        }
    } else if (root->castType == 3){
        UnaryExpression *unaryExpression = dynamic_cast<UnaryExpression *>(root);
        Expression *expr = unaryExpression->expr;
        return traverse_single_chain_expression(dynamic_cast<SingleChainExpression *>(expr),scope);
    }
    else if (root->castType == 4)
    {
        UnaryExpression *unaryExpression = dynamic_cast<UnaryExpression *>(root);
        Expression *expr = unaryExpression->expr;
        vector<DataType> v = traverse_multi_chain_expression(dynamic_cast<MultiChainExpression *>(expr),scope);
        return Dataset;
    }
    return DataType::Unknown;
}

void traverse_assignment(AssignmentStatement *assignmentStatement)
{
    cout << "entering assignment" << endl;
    if (assignmentStatement->declarator == nullptr)
    {
        cout << "\e[31m Error: \e[0m Declarator is missing in assignment statement\n";
        error_count++;
        return;
    }
    if (assignmentStatement->expression == nullptr)
    {
        cout << "\e[31m Error: \e[0m Expression is missing in assignment statement\n";
        error_count++;
        return;
    }
    string scope = assignmentStatement->get_scope();    
    DataType lhs = traverse_single_chain_expression(assignmentStatement->declarator,scope);
    DataType rhs = traverse_operations(assignmentStatement->expression,scope);
    if (lhs != rhs)
    {
        cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to " << dataTypeToString(lhs) << "!\n";
        error_count++;
    }
}

void traverse_if_statement(ConditionalStatement *cond_stmt,bool isFunction,bool isLoop)
{
    cout << "entering if" << endl;
    if (cond_stmt->ConditionStatements != nullptr)
    {
        for (auto &[expr, stmt_list] : *(cond_stmt->ConditionStatements))
        {
            // check if the expression is of type boolean
            if (expr != nullptr)
            {
                string scope = string(cond_stmt->get_scope());
                DataType temp = traverse_operations(expr,scope);
                if (temp != Boolean || temp != Integer)
                {
                    std::cout << "\e[31m Error: \e[0m Expression in conditional statement is not of type boolean\n";
                    error_count++;
                }
            }
            
            for (auto &in_stmt : *stmt_list)
            {
                // do the same for the statements in the conditional statement
                traverse_statement(dynamic_cast<Statement *>(in_stmt),isFunction,isLoop);
            }
        }
    }
    // cout << "exiting if" << endl;
}

void traverse(Start *start)
{
    if (start == nullptr)
        return;
    buildScope(start, ".g");
    for (auto &func_dec : *(start->FunctionList))
    {
        traverse_function_declaration(dynamic_cast<FunctionDeclaration *>(func_dec));
    }
    for (auto &stmt : *(start->StatementList))
    {
        Statement* statement= dynamic_cast<Statement *>(stmt);
        if(statement->statementType==6){
            cout << "Error: Break statement not inside loop!\n";
        } else if(statement->statementType==7){
            cout << "Error: Continue statement not inside loop!\n";
        } else if (statement->statementType==5){
            cout << "Error: Return statement not inside function!\n";   
        } else {
            traverse_statement(statement,false,false);
        }
    }
    // cout << "exiting traverse" << endl;
}