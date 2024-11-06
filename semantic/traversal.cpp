#include <iostream>
#include "symbol_table.hpp"
#include <unistd.h>
#include <limits.h>
#include <cstring>
#include "traversal.hpp"
using namespace std;

// I want a variable that can be triggered by the makefile to print the error messages
#ifdef DEBUG
bool debug = true;
#else
bool debug = false;
#endif

extern FILE *yyin;

SymbolTable symtab;
int error_count = 0;

void printerror(int row, int column){
    int fd = fileno(yyin);
    if (fd == -1) {
        perror("fileno");
        return;
    }
    char filename[PATH_MAX];
    snprintf(filename, sizeof(filename), "/proc/self/fd/%d", fd);
    char actual_filename[PATH_MAX];
    ssize_t len = readlink(filename, actual_filename, sizeof(actual_filename) - 1);
    if (len == -1) {
        perror("readlink");
        return;
    }
    actual_filename[len] = '\0';

    fprintf(stderr, "In File %s:%d:%d\n", actual_filename, row, column);

    long current_pos = ftell(yyin);

    fseek(yyin, 0, SEEK_SET);

    char line[1024];
    int current_line = 1;
    while (fgets(line, sizeof(line), yyin)) {
        if (current_line == row) {
            break;
        }
        current_line++;
    }

    fseek(yyin, current_pos, SEEK_SET);
    line[strcspn(line, "\n")] = '\0';

    //remove all spaces at the start of the line
    int i = 0;
    while (line[i] == ' ' || line[i] == '\t') {
        i++;
    }
    fprintf(stderr, "%s\n", line + i);

    for(int i=0; i<column;i++){
        std::cout << "\e[31m-\e[0m";
    }
    std::cout << "\e[31m^\e[0m\n";
}

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

void insert_inbuiltfunction_symtab(FunctionCall* function, std::string func){
    std::string scope = "g";
    if(func == "show_bar"){
        std::vector<std::vector<DataType>*> *inputParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *returnParameters = new std::vector<std::vector<DataType>*>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, function->row,function->column);
    } else if(func == "show_line"){
        std::vector<std::vector<DataType>*> *inputParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *returnParameters = new std::vector<std::vector<DataType>*>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, function->row,function->column);
    } else if(func == "show_scatter"){
        std::vector<std::vector<DataType>*> *inputParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *returnParameters = new std::vector<std::vector<DataType>*>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, function->row,function->column);
    } else if(func == "show_box"){
        std::vector<std::vector<DataType>*> *inputParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *returnParameters = new std::vector<std::vector<DataType>*>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, function->row,function->column);
    } else if(func == "row"){
        std::vector<std::vector<DataType>*> *inputParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
        otherParameters->push_back(new std::vector<DataType>({Array_Integer}));
        std::vector<std::vector<DataType>*> *returnParameters = new std::vector<std::vector<DataType>*>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, function->row,function->column);
    } else if(func == "col"){
        std::vector<std::vector<DataType>*> *inputParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
        otherParameters->push_back(new std::vector<DataType>({Array_Integer}));
        std::vector<std::vector<DataType>*> *returnParameters = new std::vector<std::vector<DataType>*>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, function->row,function->column);
    } else if(func == "filter"){
        std::vector<std::vector<DataType>*> *inputParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *returnParameters = new std::vector<std::vector<DataType>*>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, function->row,function->column);
    } else if(func == "sum"){
        std::vector<std::vector<DataType>*> *inputParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *returnParameters = new std::vector<std::vector<DataType>*>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, function->row,function->column);
    } else if(func == "max"){
        std::vector<std::vector<DataType>*> *inputParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *returnParameters = new std::vector<std::vector<DataType>*>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, function->row,function->column);
    } else if(func == "min"){
        std::vector<std::vector<DataType>*> *inputParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *returnParameters = new std::vector<std::vector<DataType>*>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, function->row,function->column);
    } else if(func == "read"){
        std::vector<std::vector<DataType>*> *inputParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *returnParameters = new std::vector<std::vector<DataType>*>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, function->row,function->column);
    } else if(func == "write"){
        std::vector<std::vector<DataType>*> *inputParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *returnParameters = new std::vector<std::vector<DataType>*>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, function->row,function->column);
    } else if(func == "unique"){
        std::vector<std::vector<DataType>*> *inputParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *returnParameters = new std::vector<std::vector<DataType>*>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, function->row,function->column);
    } else if(func == "show"){
        std::vector<std::vector<DataType>*> *inputParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *returnParameters = new std::vector<std::vector<DataType>*>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, function->row,function->column);
    } else if(func == "split"){
        std::vector<std::vector<DataType>*> *inputParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *returnParameters = new std::vector<std::vector<DataType>*>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, function->row,function->column);
    } else if(func == "sort"){
        std::vector<std::vector<DataType>*> *inputParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *returnParameters = new std::vector<std::vector<DataType>*>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, function->row,function->column);
    } else if(func == "shuffle"){
        std::vector<std::vector<DataType>*> *inputParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *returnParameters = new std::vector<std::vector<DataType>*>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, function->row,function->column);
    } else if(func == "add"){
        std::vector<std::vector<DataType>*> *inputParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *returnParameters = new std::vector<std::vector<DataType>*>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, function->row,function->column);
    } else if(func == "shape"){
        std::vector<std::vector<DataType>*> *inputParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *returnParameters = new std::vector<std::vector<DataType>*>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, function->row,function->column);
    } else if(func == "drop"){
        std::vector<std::vector<DataType>*> *inputParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
        std::vector<std::vector<DataType>*> *returnParameters = new std::vector<std::vector<DataType>*>();
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, function->row,function->column);
    }else{
        cout << "\e[31m Error: \e[0m Inbuilt function " << func << " not supported.\n";
        printerror(function->row, function->column);
        error_count++;
    }
}

void traverse_statement(Statement *stmt,bool isFunction=false,bool isLoop=false)
{
    if(debug) {cout << "entering statement" << endl;}
    if (stmt == nullptr) {
        return;
    }
    if(debug) {cout << stmt->statementType << endl;}
    switch (stmt->statementType)
    {
    case 1:
        // declaration
        if(debug) {cout << "declaration" << endl;}
        traverse_declaration(dynamic_cast<DeclarationStatement *>(stmt->declarationStatement));
        break;
    case 2:
        // assignment
        if(debug) {cout << "assignment" << endl;}
        traverse_assignment(dynamic_cast<AssignmentStatement *>(stmt->assignmentStatement));
        break;
    case 5:
        // return
        if(!isFunction){
            cout<<"\e[31m Error: \e[0m Return statement is not allowed outside of function!\n";
            printerror(stmt->row,stmt->column);
            error_count++;
        }
        break;
    case 6:
        // break
        if(!isLoop){
            cout<<"\e[31m Error: \e[0m Break statement is not allowed outside of loop!\n";
            printerror(stmt->row,stmt->column);
            error_count++;
        }
        break;
    case 7:
        // continue
        if(!isLoop){
            cout<<"\e[31m Error: \e[0m Continue statement is not allowed outside of loop!\n";
            printerror(stmt->row,stmt->column);
            error_count++;
        }
        break;

    case 3:
        // conditional
        if(debug) {cout << "conditional" << endl;}
        traverse_if_statement(stmt->conditionalStatement,isFunction,isLoop);
        break;
    case 4:
        // loop
        if(debug) {cout << "loop" << endl;}
        traverse_loop_statement(stmt->loopStatement,isFunction);
        break;
    default:
        // compound
        {
            if(debug) {cout << "compound" << endl;}
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
    if(debug) {cout << "entering buildscope" << endl;}
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
    if(debug) {cout << "entering declaration" << endl;}
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
                    printerror(decl_stmt->row,decl_stmt->column);
                    error_count++;
                    continue;
                }else
                {
                    if(debug) {cout << "inserting into symbol table" << endl;}
                    symtab.insert(name, type, scope, decl_stmt->row, decl_stmt->column);
                }
                //  type checking here
                if (init_decl->initializer != nullptr && init_decl->initializer->assignmentExpression!=nullptr && init_decl->initializer->assignmentExpression->expression!=nullptr){
                    Expression* expr = init_decl->initializer->assignmentExpression->expression;
                    DataType rhs = traverse_operations(expr,scope);
                    if (type == Integer)
                    {
                        if (rhs != Integer && rhs != Boolean)
                        {
                            std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to integer\n";
                            printerror(init_decl->initializer->row,init_decl->initializer->column);
                            error_count++;
                        }
                    }
                    else if (type == Float)
                    {
                        if (rhs != Float && rhs != Integer && rhs != Boolean)
                        {
                            std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to float\n";
                            printerror(init_decl->initializer->row,init_decl->initializer->column);
                            error_count++;
                        }
                    }
                    else if (type == Boolean)
                    {
                        if (rhs != Boolean && rhs != Integer)
                        {
                            std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to boolean\n";
                            printerror(init_decl->initializer->row,init_decl->initializer->column);
                            error_count++;
                        }
                    }
                    else if (type == String && rhs != String)
                    {
                        std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to string\n";
                        printerror(init_decl->initializer->row,init_decl->initializer->column);
                        error_count++;
                    }
                    else if (type == Char && rhs != Char)
                    {
                        std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to char\n";
                        printerror(init_decl->initializer->row,init_decl->initializer->column);
                        error_count++;
                    }
                    else if (type == Dataset && rhs != Dataset)
                    {
                        std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to dataset\n";
                        printerror(init_decl->initializer->row,init_decl->initializer->column);
                        error_count++;
                    }
                    else if (type == Array && rhs != Array)
                    {
                        std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to array\n";
                        //print the error line by opening the file given as arugment
                        printerror(init_decl->initializer->row,init_decl->initializer->column);
                        error_count++;
                    }
                }
                // type checking in case of array
                else if (init_decl->initializer != nullptr && init_decl->initializer->initializerList != nullptr)
                {
                    for (auto *initializer : *(init_decl->initializer->initializerList))
                    {
                        if (initializer->assignmentExpression != nullptr && initializer->assignmentExpression->expression != nullptr)
                        {
                            Expression *expr = initializer->assignmentExpression->expression;
                            DataType rhs = traverse_operations(expr,scope);
                            if (type == Integer)
                            {
                                if (rhs != Integer && rhs != Boolean)
                                {
                                    std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to integer\n";
                                    printerror(initializer->row,initializer->column);
                                    error_count++;
                                }
                            }
                            else if (type == Float)
                            {
                                if (rhs != Float && rhs != Integer && rhs != Boolean)
                                {
                                    std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to float\n";
                                    printerror(initializer->row,initializer->column);
                                    error_count++;
                                }
                            }
                            else if (type == Boolean)
                            {
                                if (rhs != Boolean && rhs != Integer)
                                {
                                    std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to boolean\n";
                                    printerror(initializer->row,initializer->column);
                                    error_count++;
                                }
                            }
                            else if (type == String && rhs != String)
                            {
                                std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to string\n";
                                printerror(initializer->row,initializer->column);
                                error_count++;
                            }
                            else if (type == Char && rhs != Char)
                            {
                                std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to char\n";
                                printerror(initializer->row,initializer->column);
                                error_count++;
                            }
                            else if (type == Dataset && rhs != Dataset)
                            {
                                std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to dataset\n";
                                printerror(initializer->row,initializer->column);
                                error_count++;
                            }
                            else if (type == Array && rhs != Array)
                            {
                                std::cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to array\n";
                                printerror(initializer->row,initializer->column);
                                error_count++;
                            }
                        }
                    }
                } 
                else cout<<"initilizer is null"<<endl;
            }
        }
    }
    if(debug) {cout << "exiting declaration" << endl;}
}

// function to do semantic checks on the loop statement
void traverse_loop_statement(LoopStatement *loop_stmt,bool isFunction)
{
    if(debug) {cout << "entering loop" << endl;}
    std::string name = loop_stmt->variable->identifier;
    std::string scope = loop_stmt->scope;
    SymbolTableEntry *entry = symtab.search(name, scope);
    if (entry != nullptr)
    {
        std::cout << "\e[31m Error: \e[0m Identifier in loop statement is already declared\n";
        printerror(loop_stmt->variable->row,loop_stmt->variable->column);
        error_count++;
    }else{
        DataType dataType = Integer;
        symtab.insert(name, dataType, scope, loop_stmt->row, loop_stmt->column);
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
                    printerror(std::get<0>(pair)->row,std::get<0>(pair)->column);
                    error_count++;
                }
            }
            if (std::get<1>(pair) != nullptr)
            {
                if (traverse_operations(std::get<1>(pair),scope) != 0)
                {
                    std::cout << "\e[31m Error: \e[0m To expression in loop statement is not of type integer\n";
                    printerror(std::get<1>(pair)->row,std::get<1>(pair)->column);
                    error_count++;
                }
            }
            if (std::get<2>(pair) != nullptr)
            {
                if (traverse_operations(std::get<2>(pair),scope) != 0)
                {
                    std::cout << "\e[31m Error: \e[0m Step expression in loop statement is not of type integer\n";
                    printerror(std::get<2>(pair)->row,std::get<2>(pair)->column);
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
    if(debug) {cout << "exiting loop" << endl;}
}

// need to check the return statement
void traverse_function_declaration(FunctionDeclaration *func_dec) {
    if(debug) {cout << "entering function declaration" << endl;}
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
            cout << "\e[31m Error: \e[0m identifier name missing in function declaration input parameter!" << endl;
            printerror(param->row,param->column);
            error_count++;
        } else {
            string name = param->identifier->identifier;
            symtab.insert(name, dataType, scope, param->identifier->row, param->identifier->column);
        }
    }
    // get the other parameters
    std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>();
    otherParameters->push_back(new std::vector<DataType>());
    for (auto *param : *(func_dec->otherParameter)) {
        // map the type (which is a number)to DataType enum
        DataType dataType = mapTypeToDataType(param->type->type->at(0));
        otherParameters->at(0)->push_back(dataType);
        string scope = param->get_scope();
        if (param->identifier == nullptr) {
            cout << "\e[31m Error: \e[0m identifier name missing in function declaration other parameter!" << endl;
            printerror(param->row,param->column);
            error_count++;
        } else {
            string name = param->identifier->identifier;
            symtab.insert(name, dataType, scope, param->identifier->row, param->identifier->column);
        }
    }
    // get the return parameters
    std::vector<DataType> *returnParameters = new std::vector<DataType>();
    for (auto *param : *(func_dec->outParameter)) {
        // map the type (which is a number)to DataType enum
        DataType dataType = mapTypeToDataType(param->type->type->at(0));
        returnParameters->push_back(dataType);
        if (param->identifier != nullptr) {
            cout << "\e[31m Error: \e[0m unecessary identifier missing in function declaration return parameter!" << endl;
            printerror(param->row,param->column);
            error_count++;
        }
    }
    if (func_dec->identifier != nullptr) {
        std::string name = func_dec->identifier;
        std::string scope = func_dec->get_scope();
        symtab.insert(name, inputParameters, otherParameters, returnParameters, scope, func_dec->row, func_dec->column);
    } else {
        cout << "\e[31m Error: \e[0m identifier name missing in function declaration!" << endl;
        printerror(func_dec->row,func_dec->column);
        error_count++;
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

vector<int> match_function_symb_entry(SymbolTableEntry* entry,vector<DataType>* inputP){
    if(debug) {cout << "Entering match function symbol entry" << endl;}
    vector<vector<DataType>*>* inputParameters = entry->inputParameters;
    vector<int> matchedIndex = vector<int>();
    for(int i=0;i<inputParameters->size();i++){
        vector<DataType>* inputParameter = inputParameters->at(i);
        if(inputParameter->size() == inputP->size()){
            bool match = true;
            for(int j=0;j<inputParameter->size();j++){
                if(inputParameter->at(j) != inputP->at(j)){
                    match = false;
                    break;
                }
            }
            if(match){
                matchedIndex.push_back(i);
            }
        }
    }
    return matchedIndex;

}
//IT MUST RETURN TRUE IF THE FUNCTION CALL IS MATCHED CORRECTLY. Match the ith one
bool traverse_function_call(FunctionCall* functionCall,SymbolTableEntry* entry,int i){
    if(debug) {cout << "Entering function call" << endl;}
    vector<Argument *> *argumentList = functionCall->argumentList;
    vector<DataType> *otherParameters = entry->otherParameters->at(0);
    // vector<DataType> *outParameters = entry->returnParameters;

    if (argumentList->size() != otherParameters->size()) {
        cout << "\e[31m Error: \e[0m Function " << entry->name << " expects " << otherParameters->size() << " arguments but " << argumentList->size() << " provided\n";
        printerror(functionCall->row,functionCall->column);
        error_count++;
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
                    cout << "\e[31m Error: \e[0m Function " << entry->name << " expects " << dataTypeToString(otherParameters->at(i)) << " but " << dataTypeToString(exprType) << " provided\n";
                    printerror(argument->row,argument->column);
                    error_count++;
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
                        cout << "\e[31m Error: \e[0m Function " << entry->name << " expects integer arguments\n";
                        printerror(argument->row,argument->column);
                        error_count++;
                    }
                }
            } else if (argument->statements != nullptr) {
                for (auto &stmt : *(argument->statements)) {
                    Statement *statement = dynamic_cast<Statement *>(stmt);
                    traverse_statement(statement,true,false);
                }
            } else {
                cout << "\e[31m Error: \e[0m Argument is null\n";
                printerror(argument->row,argument->column);
                error_count++;
            }
        }
    }
    return true;
    
}

DataType traverse_single_chain_expression(SingleChainExpression *singleChainExpression,string scope)
{
    if(debug) {cout << "Entering single chain expression" << endl;}
    if(singleChainExpression==nullptr){
        cout << "\e[31m Error: \e[0m SingleChainExpression is null\n";
        printerror(singleChainExpression->row,singleChainExpression->column);
        error_count++;
        return Unknown;
    }
    char *identifier = singleChainExpression->identifier;
    std::string identifier_str = std::string(identifier);
    SymbolTableEntry *entry = symtab.search(identifier_str, scope);
    if(debug) {cout << "Identifier: " << identifier_str << endl;}
    if (entry == nullptr){
        cout << "\e[31m Error: \e[0m Identifier " << identifier_str << " not declared\n";
        printerror(singleChainExpression->row,singleChainExpression->column);
        error_count++;
        return Unknown;
    }
    vector<DataType> currentDataType = vector<DataType>({entry->dataType});
    return traverse_function_call_list_multi(*singleChainExpression->functionCallList,currentDataType,scope)[0];
    // for(auto &funcPair : *(singleChainExpression->functionCallList)){
    //     FunctionCall *functionCall = funcPair.first;
    //     if(debug) {cout << "Function call: " << endl;}
    
    //     std::string functionName;
    //     if (functionCall->identifier != nullptr){
    //         functionName = std::string(functionCall->identifier);
    //     } else {
    //         functionName = mapInbuiltFunctionToString(functionCall->inbuiltFunc);
    //         insert_inbuiltfunction_symtab(functionCall, functionName);
    //     }

    //     SymbolTableEntry *functionEntry = symtab.search(functionName, scope);
    //     if(functionEntry==nullptr){
    //         cout << "\e[31m Error: \e[0m Function " << functionName << " not declared\n";
    //         printerror(functionCall->row,functionCall->column);
    //         error_count++;
    //         return Unknown;
    //     }
    //     vector<int> funcIndex = match_function_symb_entry(functionEntry,&currentDataType);
        // int funcIndex = match_function_symb_entry(functionEntry,&currentDataType);
        // vector<DataType> *inputParameters = functionEntry->inputParameters;
        // vector<DataType> *returnParameters = functionEntry->returnParameters;
        // traverse_function_call(functionCall,functionEntry);
        // if (functionEntry->inputParameters->size()!=1){
        //     cout << "\e[31m Error: \e[0m Function " << functionName << " expects more than one input parameter\n";
        //     printerror(functionCall->row,functionCall->column);
        //     error_count++;
        // } else if(returnParameters->size()!=1){
        //     cout << "\e[31m Error: \e[0m Function " << functionName << " returns more than one return parameter\n";
        //     printerror(functionCall->row,functionCall->column);
        //     error_count++;
        // } else {
        //     DataType functionInputParameter = functionEntry->inputParameters->at(0);
        //     if (functionInputParameter != currentDataType){
        //         cout << "\e[31m Error: \e[0m Function " << functionName << " input parameter is not of the same type as the identifier\n"<<functionName<<" expects "<<dataTypeToString(functionInputParameter)<<" but input is of type "<<dataTypeToString(currentDataType)<<"!\n";
        //         printerror(functionCall->row,functionCall->column);
        //         error_count++;
        //     } 
        //     currentDataType = returnParameters->at(0);
        // }
    // }
    if(debug) {cout << "Exiting single chain expression" << endl;}
    return currentDataType.at(0);
}

vector<DataType> traverse_function_call_list_multi(vector<pair<FunctionCall *, vector<Expression *>*>> functionCallList,vector<DataType> currentDataType,string scope)
{
    if(debug) {cout << "Entering function call list multi" << endl;}
    for(int i=0;i<functionCallList.size();i++){
        pair<FunctionCall *, vector<Expression *>*> funcPair = functionCallList.at(i);
        FunctionCall *functionCall = funcPair.first;
        std::string functionName = std::string(functionCall->identifier);
        SymbolTableEntry *functionEntry = symtab.search(functionName, scope);
        if(functionEntry==nullptr){
            cout << "\e[31m Error: \e[0m Function " << functionName << " not declared\n";
            printerror(functionCall->row,functionCall->column);
            error_count++;
            return currentDataType;
        }
        vector<int> funcIndex = match_function_symb_entry(functionEntry,&currentDataType);
        if(funcIndex.size()==0){
            cout << "\e[31m Error: \e[0m Function " << functionName << " arguments have not matched\n";
            printerror(functionCall->row,functionCall->column);
            error_count++;
            return currentDataType;
        }
        vector<int> matchedIndex = vector<int>();
        //search for the function call in the list of functions
        for(auto &i:funcIndex){
            if(traverse_function_call(functionCall,functionEntry,i)){
                matchedIndex.push_back(i);
                break;
            }
        }
        if(matchedIndex.size()==0){
            cout << "\e[31m Error: \e[0m no matching call found for function " << functionName << "\n";
            printerror(functionCall->row,functionCall->column);
            error_count++;
            return vector<DataType>({Unknown});
        }
        if(i<functionCallList.size()-1){
            //match the return type of the function with the input type of the next function
            int isMatched = 0;
            string nextFunctionName = string(functionCallList.at(i+1).first->identifier);
            SymbolTableEntry *nextFunctionEntry = symtab.search(nextFunctionName, scope);
            if(nextFunctionEntry==nullptr){
                cout << "\e[31m Error: \e[0m Function " << nextFunctionName << " not declared\n";
                printerror(functionCall->row,functionCall->column);
                error_count++;
                return vector<DataType>({Unknown});
            }
            for(auto &matchi:matchedIndex){
                vector<DataType> *returnParameters = functionEntry->returnParameters->at(matchedIndex.at(matchi));
                for (int j=0;i<nextFunctionEntry->inputParameters->size();j++){
                    vector<DataType> *nextInputParameters = nextFunctionEntry->inputParameters->at(j);
                    //check if the return type of the function matches the input type of the next function
                    if(returnParameters->size()==nextInputParameters->size()){
                        bool match = true;
                        for(int k=0;k<returnParameters->size();k++){
                            if(returnParameters->at(k)!=nextInputParameters->at(k)){
                                match = false;
                                break;
                            }
                        }
                        if(match){
                            isMatched = 1;
                            break;
                        }
                    }
                }
                if(isMatched==0){
                    continue;
                } else {
                    currentDataType.clear();
                    currentDataType = *returnParameters;
                    break;
                }
                if(isMatched){
                    break;
                }
            }
            if(isMatched==0){
                cout<<"\e[31m Error: \e[0m Function calls to "<<functionName<<" and "<<nextFunctionName<<" do not match\n";
                printerror(functionCall->row,functionCall->column);
                error_count++;
                return vector<DataType>({Unknown});
            }
            }
            else {
                return currentDataType;
            }
    }
    if(debug) {cout << "Exiting function call list multi\n";}
    return currentDataType;
}

//intbuilt functions must be initialized by default in symbol table
// CHECK: init of functionCallName
vector<DataType> traverse_multi_chain_expression(MultiChainExpression *multiChainExpression,string scope)
{
    if(debug) {cout << "Entering multi chain expression" << endl;}
    if(multiChainExpression->functionCall!=nullptr || multiChainExpression->inbuiltFunc!=InbuiltFunctions::none){
        if(debug) {cout << "hi" << endl;}
        std::string functionCallName;
        if(multiChainExpression->functionCall!=nullptr){
            functionCallName = string(multiChainExpression->functionCall->identifier) ;
        } else {
            functionCallName = string(mapInbuiltFunctionToString(multiChainExpression->inbuiltFunc));
        }
        SymbolTableEntry *entry = symtab.search(functionCallName,scope);
        if(entry==nullptr){
            cout << "\e[31m Error: \e[0m Function " << functionCallName << " not declared\n";
            printerror(multiChainExpression->row,multiChainExpression->column);
            error_count++;
            return vector<DataType>({Unknown});
        }
        vector<int> funcIndex = match_function_symb_entry(entry,new vector<DataType>());
        if(funcIndex.size()==0){
            cout << "\e[31m Error: \e[0m Function " << functionCallName << " expects more than the provided input parameter\n";
            printerror(multiChainExpression->row,multiChainExpression->column);
            error_count++;
            return vector<DataType>({Unknown});
        }
        //create a funtionCall object
        //convert the function name to char* type
        char* funcName = new char[functionCallName.length()+1];
        strcpy(funcName,functionCallName.c_str());
        pair<FunctionCall *, vector<Expression *>*> funcPair = make_pair(multiChainExpression->functionCall,multiChainExpression->access);
        multiChainExpression->functionCallList->insert(multiChainExpression->functionCallList->begin(),funcPair);

        return traverse_function_call_list_multi(*multiChainExpression->functionCallList,vector<DataType>(),scope);
    } else {
        vector<Expression*> *inputList = multiChainExpression->inputExprList;
        vector<DataType> inputTypes;
        for(auto &expr : *inputList){
            DataType exprType = traverse_operations(expr,scope);
            inputTypes.push_back(exprType);
        }
        return traverse_function_call_list_multi(*(multiChainExpression->functionCallList),inputTypes,scope);
    }
}

DataType traverse_operations(Expression *root,string scope){
    if(debug) {cout << "Entering traverse operations" << endl;}
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
                printerror(unaryExpression->row,unaryExpression->column);
                error_count++;
                return Unknown;
            }
            DataType datatype = entry->dataType;
            if (datatype == Integer) return Integer;
            else if (datatype == Float){
                for(auto &o:*(unaryExpression->op)){
                    if (o==not_op) {
                        cout << "\e[31m Error: \e[0m Cannot \"!\" perform operation on float\n";
                        printerror(unaryExpression->row,unaryExpression->column);
                        error_count++;
                    }
                    return Float;
                    }
            } else if (datatype == String){
                for (auto &o : *(unaryExpression->op)){
                    if (o == minus_op){
                        cout << "\e[31m Error: \e[0m Cannot \"-\" perform operation on string\n";
                        printerror(unaryExpression->row,unaryExpression->column);
                        error_count++;
                    }else if (o == plus_op){
                        cout << "\e[31m Error: \e[0m Cannot \"+\" perform operation on string\n";
                        printerror(unaryExpression->row,unaryExpression->column);
                        error_count++;
                    }
                }
                return String;
            } else if (datatype == Char){
                for (auto &o : *(unaryExpression->op)){
                    if (o == minus_op){
                        cout << "\e[31m Error: \e[0m Cannot \"-\" perform operation on string\n";
                        printerror(unaryExpression->row,unaryExpression->column);
                        error_count++;
                    } else if (o == plus_op){
                        cout << "\e[31m Error: \e[0m Cannot \"+\" perform operation on string\n";
                        printerror(unaryExpression->row,unaryExpression->column);
                        error_count++;
                    }
                }
                return Char;
            } else if (datatype == Boolean)
            {
                for (auto &o : *(unaryExpression->op)){
                    if (o == minus_op){
                        cout << "\e[31m Error: \e[0m Cannot \"-\" perform operation on string\n";
                        printerror(unaryExpression->row,unaryExpression->column);
                        error_count++;
                    } else if (o == plus_op){
                        cout << "\e[31m Error: \e[0m Cannot \"+\" perform operation on string\n";
                        printerror(unaryExpression->row,unaryExpression->column);
                        error_count++;
                    }
                }
                return Boolean;
            } else if (datatype == Dataset){
                if (unaryExpression->op->size() > 0){
                    cout << "\e[31m Error: \e[0m Cannot perform Unary operations on dataset\n";
                    printerror(unaryExpression->row,unaryExpression->column);
                    error_count++;
                }
                return Dataset;
            } else if (datatype == Array){
                if (unaryExpression->op->size() > 0){
                    cout << "\e[31m Error: \e[0m Cannot perform Unary operations on array\n";
                    printerror(unaryExpression->row,unaryExpression->column);
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
                    printerror(unaryExpression->row,unaryExpression->column);
                    error_count++;
                }else if (o == plus_op){
                    cout << "\e[31m Error: \e[0m Cannot \"+\" perform operation on string\n";
                    printerror(unaryExpression->row,unaryExpression->column);
                    error_count++;
                }
            }
            return String;
        } else if (datatype == Char) {
            for (auto &o : *(unaryExpression->op)){
                if (o == minus_op) {
                    cout << "\e[31m Error: \e[0m Cannot \"-\" perform operation on string\n";
                    printerror(unaryExpression->row,unaryExpression->column);
                    error_count++;
                } else if (o == plus_op){
                    cout << "\e[31m Error: \e[0m Cannot \"+\" perform operation on string\n";
                    printerror(unaryExpression->row,unaryExpression->column);
                    error_count++;
                }
            }
            return Char;
        } else if (datatype == Boolean){
            for (auto &o : *(unaryExpression->op))
            {
                if (o == minus_op){
                    cout << "\e[31m Error: \e[0m Cannot \"-\" perform operation on string\n";
                    printerror(unaryExpression->row,unaryExpression->column);
                    error_count++;
                } else if (o == plus_op){
                    cout << "\e[31m Error: \e[0m Cannot \"+\" perform operation on string\n";
                    printerror(unaryExpression->row,unaryExpression->column);
                    error_count++;
                }
            }
            return Boolean;
        } else if (datatype == Dataset){
            if (unaryExpression->op->size() > 0){
                cout << "\e[31m Error: \e[0m Cannot perform Unary operations on dataset\n";
                printerror(unaryExpression->row,unaryExpression->column);
                error_count++;
            }
            return Dataset;
        } else if (datatype == Array){
            if (unaryExpression->op->size() > 0){
                cout << "\e[31m Error: \e[0m Cannot perform Unary operations on array\n";
                printerror(unaryExpression->row,unaryExpression->column);
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
                printerror(binaryExpression->row,binaryExpression->column);
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
                printerror(binaryExpression->row,binaryExpression->column);
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
                printerror(binaryExpression->row,binaryExpression->column);
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
                printerror(binaryExpression->row,binaryExpression->column);
                error_count++;
                return Unknown;
            }
        } else if (binaryExpression->op == mod_op){
            if (lhs == Integer && rhs == Integer){
                return Integer;
            } else {
                cout << "\e[31m Error: \e[0m Cannot perform \"%\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
                printerror(binaryExpression->row,binaryExpression->column);
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
                printerror(binaryExpression->row,binaryExpression->column);
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
                printerror(binaryExpression->row,binaryExpression->column);
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
                printerror(binaryExpression->row,binaryExpression->column);
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
                printerror(binaryExpression->row,binaryExpression->column);
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
    if(debug) {cout << "entering assignment" << endl;}
    if (assignmentStatement->declarator == nullptr)
    {
        cout << "\e[31m Error: \e[0m Declarator is missing in assignment statement\n";
        printerror(assignmentStatement->row,assignmentStatement->column);
        error_count++;
        return;
    }
    if (assignmentStatement->expression == nullptr)
    {
        cout << "\e[31m Error: \e[0m Expression is missing in assignment statement\n";
        printerror(assignmentStatement->row,assignmentStatement->column);
        error_count++;
        return;
    }
    string scope = assignmentStatement->get_scope();    
    DataType lhs = traverse_single_chain_expression(assignmentStatement->declarator,scope);
    DataType rhs = traverse_operations(assignmentStatement->expression,scope);
    AssignmentOperator operator_type = assignmentStatement->op;
    if(operator_type == assign)
    {
        if (lhs != rhs)
        {
            cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to " << dataTypeToString(lhs) << "!\n";
            error_count++;
        }
    }else if(operator_type == add_assign)
    {
        if(lhs != Integer && lhs != Float)
        {
            cout << "\e[31m Error: \e[0m Cannot perform \"+=\" operation on " << dataTypeToString(lhs) << "!\n";
            error_count++;
        }
        if(rhs != Integer && rhs != Float)
        {
            cout << "\e[31m Error: \e[0m Cannot perform \"+=\" operation with " << dataTypeToString(rhs) << "!\n";
            error_count++;
        }
        if(lhs != rhs)
        {
            cout << "\e[31m Error: \e[0m Cannot perform \"+=\" operation with " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
            error_count++;
        }
    }else if(operator_type == sub_assign)
    {
        if(lhs != Integer && lhs != Float)
        {
            cout << "\e[31m Error: \e[0m Cannot perform \"-=\" operation on " << dataTypeToString(lhs) << "!\n";
            error_count++;
        }
        if(rhs != Integer && rhs != Float)
        {
            cout << "\e[31m Error: \e[0m Cannot perform \"-=\" operation on " << dataTypeToString(rhs) << "!\n";
            error_count++;
        }
        if(lhs != rhs)
        {
            cout << "\e[31m Error: \e[0m Cannot perform \"-=\" operation with " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
            error_count++;
        }
    }else if(operator_type == mul_assign)
    {
        if(lhs != Integer && lhs != Float)
        {
            cout << "\e[31m Error: \e[0m Cannot perform \"*=\" operation on " << dataTypeToString(lhs) << "!\n";
            error_count++;
        }
        if(rhs != Integer && rhs != Float)
        {
            cout << "\e[31m Error: \e[0m Cannot perform \"*=\" operation on " << dataTypeToString(rhs) << "!\n";
            error_count++;
        }
        if(lhs != rhs)
        {
            cout << "\e[31m Error: \e[0m Cannot perform \"*=\" operation with " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
            error_count++;
        }
    }else if(operator_type == div_assign)
    {
        if(lhs != Integer && lhs != Float)
        {
            cout << "\e[31m Error: \e[0m Cannot perform \"/=\" operation on " << dataTypeToString(lhs) << "!\n";
            error_count++;
        }
        if(rhs != Integer && rhs != Float)
        {
            cout << "\e[31m Error: \e[0m Cannot perform \"/=\" operation on " << dataTypeToString(rhs) << "!\n";
            error_count++;
        }
        if(lhs != rhs)
        {
            cout << "\e[31m Error: \e[0m Cannot perform \"/=\" operation with " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
            error_count++;
        }
    }else if(operator_type == mod_assign)
    {
        if(lhs != Integer && lhs != Float)
        {
            cout << "\e[31m Error: \e[0m Cannot perform \"%=\" operation on " << dataTypeToString(lhs) << "!\n";
            error_count++;
        }
        if(rhs != Integer && rhs != Float)
        {
            cout << "\e[31m Error: \e[0m Cannot perform \"%=\" operation on " << dataTypeToString(rhs) << "!\n";
            error_count++;
        }
        if(lhs != rhs)
        {
            cout << "\e[31m Error: \e[0m Cannot perform \"%=\" operation with " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
            error_count++;
        }
    }
}

void traverse_if_statement(ConditionalStatement *cond_stmt,bool isFunction,bool isLoop)
{
    if(debug) {cout << "entering if" << endl;}
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
                    printerror(cond_stmt->row,cond_stmt->column);
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
    if(debug) {cout << "exiting if" << endl;}
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
            cout << "\e[31m Error: \e[0m Break statement not inside loop!\n";
            printerror(statement->row,statement->column);
            error_count++;
        } else if(statement->statementType==7){
            cout << "\e[31m Error: \e[0m Continue statement not inside loop!\n";
            printerror(statement->row,statement->column);
            error_count++;
        } else if (statement->statementType==5){
            cout << "\e[31m Error: \e[0m Return statement not inside function!\n";   
            printerror(statement->row,statement->column);
            error_count++;
        } else {
            traverse_statement(statement,false,false);
        }
    }
    if(debug) {cout << "exiting traverse" << endl;}
}