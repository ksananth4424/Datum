#include <iostream>
#include "symbol_table.hpp"
#include <unistd.h>
#include <limits.h>
#include <cstring>
#include "traversal.hpp"
using namespace std;

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
    case 7:
        return Array_Integer;
    case 8:
        return Array_Float;
    case 9:
        return Array_String;
    case 10:
        return Array_Char;
    case 11:
        return Array_Boolean;
    case 12:
        return Array_Dataset;   
    case 13:
        return Function;
    case 14:
        return Unknown;
    default:
        return Void;
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
    case Array_Integer:
        return "Array_Integer"; 
    case Array_Float:   
        return "Array_Float";   
    case Array_String:
        return "Array_String";  
    case Array_Char:
        return "Array_Char";    
    case Array_Boolean:
        return "Array_Boolean";
    case Array_Dataset:
        return "Array_Dataset"; 
    case Any:
        return "Any";
    case Unknown:
        return "Unknown";
    default:
        return "Void";
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

bool matchDataType(DataType type1, DataType type2)
{
    if (debug) {
        std::cout << "Matching " << dataTypeToString(type1) << " with " << dataTypeToString(type2) << std::endl;
    }
    if (type1 == type2)
    {
        return true;
    }
    if(type1 == Array && (type2 == Array_Integer || type2 == Array_Float || type2 == Array_String || type2 == Array_Char || type2 == Array_Boolean || type2 == Array_Dataset)){
        return true;
    }
    if (type2== Array && (type1 == Array_Integer || type1 == Array_Float || type1 == Array_String || type1 == Array_Char || type1 == Array_Boolean || type1 == Array_Dataset)){
        return true;
    }
    if(type1 == Any && (type2 == Integer || type2 == Float || type2 == String || type2 == Char || type2 == Boolean)){
        return true;
    }
    if(type2 == Any && (type1 == Integer || type1 == Float || type1 == String || type1 == Char || type1 == Boolean)){
        return true;
    }
    if (type1 == Integer && type2 == Float || type1 == Float && type2 == Integer)
    {
        return true;
    }
    if (type1 == Dataset && type2 == Array || type1 == Array && type2 == Dataset)
    {
        return true;
    }
    return false;
}       

void insert_inbuiltfunction_symtab() {
    std::string func;
    std::string scope = ".g";
    {
        func = "show_bar";
        std::vector<DataType> *inputParameters = new std::vector<DataType>({Array, Array, Array});
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>({new std::vector<DataType>({Array_Integer}),new std::vector<DataType>({Array_Float}),new std::vector<DataType>({})});
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Void, Void, Void});
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    }
    // TODO
    {
        func = "show_line";
        std::vector<DataType> *inputParameters = new std::vector<DataType>({Void, Array, Array, Void});
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>({new std::vector<DataType>({Array, Array}),new std::vector<DataType>({Integer,Integer,Integer,Integer}),new std::vector<DataType>({}),new std::vector<DataType>({Array, Array, Integer,Integer,Integer,Integer})});
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Void, Void, Void, Void});
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    } 
    {
        func = "show_scatter";
        std::vector<DataType> *inputParameters = new std::vector<DataType>({Void , Array, Array, Void});
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>({new std::vector<DataType>({Array, Array}),new std::vector<DataType>({Integer,Integer}),new std::vector<DataType>({}),new std::vector<DataType>({Array, Array, Integer,Integer}),});
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Void, Void, Void, Void});
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    } 
    {
        func = "show_box";
        std::vector<DataType> *inputParameters = new std::vector<DataType>({Void,Array,Array,Void});
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>({new std::vector<DataType>({Array, Array}),new std::vector<DataType>({Integer,Integer}),new std::vector<DataType>({}),new std::vector<DataType>({Array, Array, Integer,Integer}),});
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Void, Void, Void, Void});
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    }
    {
        func = "row";

        std::vector<std::vector<FunctionArgument>*> *otherParameters = new std::vector<std::vector<FunctionArgument>*>();
        otherParameters->push_back(new std::vector<FunctionArgument>({FunctionArgument(Integer)}));
        otherParameters->push_back(new std::vector<FunctionArgument>({FunctionArgument(Array_Integer)}));
        otherParameters->push_back(new std::vector<FunctionArgument>({FunctionArgument(String)}));
        otherParameters->push_back(new std::vector<FunctionArgument>({FunctionArgument(Array_String)}));
        otherParameters->push_back(new std::vector<FunctionArgument>({FunctionArgument(new std::vector<DataType>({Integer}))}));

        std::vector<DataType> *inputParameters = new std::vector<DataType>({Dataset,Dataset, Dataset, Dataset, Dataset});
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Array,Dataset, Array, Dataset, Dataset});

        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    } 
    {
        func = "col";

        std::vector<std::vector<FunctionArgument>*> *otherParameters = new std::vector<std::vector<FunctionArgument>*>();
        otherParameters->push_back(new std::vector<FunctionArgument>({FunctionArgument(Integer)}));
        otherParameters->push_back(new std::vector<FunctionArgument>({FunctionArgument(Array_Integer)}));
        otherParameters->push_back(new std::vector<FunctionArgument>({FunctionArgument(String)}));
        otherParameters->push_back(new std::vector<FunctionArgument>({FunctionArgument(Array_String)}));
        otherParameters->push_back(new std::vector<FunctionArgument>({FunctionArgument(new std::vector<DataType>({Integer}))}));

        std::vector<DataType> *inputParameters = new std::vector<DataType>({Dataset,Dataset, Dataset, Dataset, Dataset});
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Array,Dataset, Array, Dataset, Dataset});
        
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    } 
    {
        func = "filter";

        std::vector<std::vector<FunctionArgument>*> *otherParameters = new std::vector<std::vector<FunctionArgument>*>();
        otherParameters->push_back(new std::vector<FunctionArgument>({FunctionArgument(new std::vector<DataType>({Integer, Integer}))}));
        otherParameters->push_back(new std::vector<FunctionArgument>({FunctionArgument(new std::vector<DataType>({Integer}))}));


        std::vector<DataType> *inputParameters = new std::vector<DataType>({Dataset,Array});
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Dataset,Array});
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    } 
    {
        func = "sum";
        std::vector<DataType> *inputParameters = new std::vector<DataType>({Dataset,Dataset,Array_Integer,Array_Float});
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>({new std::vector<DataType>({Integer}),new std::vector<DataType>({}),new std::vector<DataType>({}),new std::vector<DataType>({})});
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Array,Any,Integer,Float});
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    } 
    {
        func = "max";
        std::vector<DataType> *inputParameters = new std::vector<DataType>({Dataset,Dataset,Array_Integer,Array_Float});
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>({new std::vector<DataType>({Integer}),new std::vector<DataType>({}),new std::vector<DataType>({}), new std::vector<DataType>({})});
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Array,Any,Integer,Float});
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    } 
    {
        func = "min";
        std::vector<DataType> *inputParameters = new std::vector<DataType>({Dataset,Dataset,Array_Integer,Array_Float});
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>({new std::vector<DataType>({Integer}),new std::vector<DataType>({}),new std::vector<DataType>({}), new std::vector<DataType>({})});
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Array,Any,Integer,Float});
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    } 
    {
        func = "mean";
        std::vector<DataType> *inputParameters = new std::vector<DataType>({Dataset,Dataset,Array_Integer,Array_Float});
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>({new std::vector<DataType>({Integer}),new std::vector<DataType>({}),new std::vector<DataType>({}), new std::vector<DataType>({})});
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Array,Any,Integer,Float});
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    } 
    {
        func = "read";
        std::vector<DataType> *inputParameters = new std::vector<DataType>({Void});
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>({new std::vector<DataType>({String,String})});
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Dataset});
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    } 
    {
        func = "write";
        std::vector<DataType> *inputParameters = new std::vector<DataType>({Dataset});
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>({new std::vector<DataType>({String,String})});
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Void});
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    } 
    {
        func = "unique";
        std::vector<DataType> *inputParameters = new std::vector<DataType>({Dataset, Array});
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>({new std::vector<DataType>({}), new std::vector<DataType>({})});
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Dataset, Array});
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    } 
    {
        func = "show";
        std::vector<DataType> *inputParameters = new std::vector<DataType>({Dataset,Array,Array_Integer,Array_String,Array_Float,Array_Char,Array_Boolean,String,Integer,Char,Float,Boolean});
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>({new std::vector<DataType>({}),new std::vector<DataType>({}),new std::vector<DataType>({}),new std::vector<DataType>({}),new std::vector<DataType>({}),new std::vector<DataType>({}),new std::vector<DataType>({}),new std::vector<DataType>({}),new std::vector<DataType>({}),new std::vector<DataType>({}),new std::vector<DataType>({}),new std::vector<DataType>({})});
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Dataset,Array,Array_Integer,Array_String,Array_Float,Array_Char,Array_Boolean,String,Integer,Char,Float,Boolean});
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    } 
    {
        func = "split";
        std::vector<DataType> *inputParameters = new std::vector<DataType>({Dataset});
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>({new std::vector<DataType>({Array_Float})});
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Array_Dataset});
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    } 
    {
        func = "sort";
        std::vector<DataType> *inputParameters = new std::vector<DataType>({Dataset,Dataset,Array_Integer,Array_String,Array_Float,Array_Char,Array_Boolean});
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>({new std::vector<DataType>({Integer}),new std::vector<DataType>({}),new std::vector<DataType>({}),new std::vector<DataType>({}),new std::vector<DataType>({}),new std::vector<DataType>({}),new std::vector<DataType>({})});
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Dataset,Dataset,Array_Integer,Array_String,Array_Float,Array_Char,Array_Boolean});
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    } 
    {
        func = "shuffle";
        std::vector<DataType> *inputParameters = new std::vector<DataType>({Dataset,Array_Integer,Array_String,Array_Float,Array_Char,Array_Boolean});
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>({new std::vector<DataType>({}),new std::vector<DataType>({}),new std::vector<DataType>({}),new std::vector<DataType>({}),new std::vector<DataType>({}),new std::vector<DataType>({}) });
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Dataset,Array_Integer,Array_String,Array_Float,Array_Char,Array_Boolean});
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    } 
    {
        //this is left
        func = "add";
        std::vector<DataType> *inputParameters = new std::vector<DataType>({Dataset,Array});
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>({new std::vector<DataType>({Array}), new std::vector<DataType>({Any})});
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Dataset,Array});
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    } 
    {
        func = "shape";
        std::vector<DataType> *inputParameters = new std::vector<DataType>({Dataset, Array});
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>({new std::vector<DataType>({}), new std::vector<DataType>({})});
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Array_Integer, Integer});
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    } 
    {
        func = "drop";
        std::vector<DataType> *inputParameters = new std::vector<DataType>({Dataset,Dataset});
        std::vector<std::vector<DataType>*> *otherParameters = new std::vector<std::vector<DataType>*>({new std::vector<DataType>({}),new std::vector<DataType>({Integer})});
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Void, Void});
        symtab.insert(func,inputParameters,otherParameters,returnParameters, scope, -1, -1);
    }
    {
        func = "join";
        std::vector<DataType> *inputParameters = new std::vector<DataType>({Void, Void});
        std::vector<std::vector<FunctionArgument>*> *otherParameters = new std::vector<std::vector<FunctionArgument>*>();
        otherParameters->push_back(new std::vector<FunctionArgument>({FunctionArgument(Dataset),FunctionArgument(Dataset)}));
        otherParameters->push_back(new std::vector<FunctionArgument>({FunctionArgument(Dataset), FunctionArgument(Dataset), FunctionArgument(new std::vector<DataType>({Integer, Integer}))}));
        std::vector<DataType> *returnParameters = new std::vector<DataType>({Dataset,Dataset});
    }
}

void traverse_statement(Statement *stmt,bool isFunction=false,bool isLoop=false, string functionName="")
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
        // return NOT DONE
        if (!isFunction){
            cout<<"here"<<endl;
            cout<<"\e[31m Error: \e[0m Return statement is not allowed outside of function\n";
            printerror(stmt->row,stmt->column);
            error_count++;
            break;
        } else {
            SymbolTableEntry* function = symtab.searchFunction(functionName);
            if (function != nullptr){
                if (function->returnParameters->size() == 0 and stmt->returnStatement->expression != nullptr){
                    cout<<"\e[31m Error: \e[0m Function expects no return type\n";
                    printerror(stmt->row,stmt->column);
                    error_count++;
                } else {
                    if (stmt->returnStatement->expression != nullptr){
                        Expression* expr = stmt->returnStatement->expression;
                        DataType rhs = traverse_operations(expr,stmt->scope);
                        if(rhs==Unknown) return;
                        bool flag = false;
                        for (auto &param : *function->returnParameters){
                            if (rhs == param){
                                flag = true;
                                break;
                            }
                        }

                        if (!flag){
                            std::cout << "\e[31m Error: \e[0m Cannot return " << dataTypeToString(rhs) << " from function with return type ";
                            for (auto &param : *function->returnParameters){
                                std::cout << dataTypeToString(param) << " ";
                            }
                            std::cout << "\n";
                            printerror(stmt->returnStatement->row,stmt->returnStatement->column);
                            error_count++;
                        }
                    }
                }
            }
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
        int stmt_scope = child_scope;
        child_scope = 0;
        for (auto &stmt : *(start->StatementList))
        {
            buildScope(stmt, scope + "." + to_string(stmt_scope) + "." + to_string(child_scope++));
        }
    }
    else if (FunctionDeclaration *func_dec = dynamic_cast<FunctionDeclaration *>(node))
    {
        func_dec->scope = scope;
        if (func_dec->inpParameter != nullptr) {
            buildScope(func_dec->inpParameter, scope + "." + to_string(child_scope++));
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
            if (stmt->assignmentStatement->expression != nullptr) {
                buildScope(stmt->assignmentStatement->expression, scope);
            }
            if (stmt->assignmentStatement->declarator != nullptr) {
                buildScope(stmt->assignmentStatement->declarator, scope);
            }
            break;
        case 5:
            // return
            stmt->returnStatement->scope = scope;
            if (stmt->returnStatement->expression != nullptr) {
                buildScope(stmt->returnStatement->expression, scope);
            }
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
                    for (auto &in_stmt : *stmt_list) {
                        buildScope(in_stmt, scope + "." + to_string(child_scope++));
                    }
                    if (expr != nullptr) {
                        buildScope(expr, scope);
                    }
                }
                break;
            }
        case 4:
            // loop
            {
                LoopStatement *loop_stmt = stmt->loopStatement;
                loop_stmt->scope = scope;
                for (auto &[expr1, expr2, expr3] : *loop_stmt->fromToPairs) {
                    if (expr1 != nullptr) {
                        buildScope(expr1, scope);
                    }
                    if (expr2 != nullptr) {
                        buildScope(expr2, scope);
                    }
                    if (expr3 != nullptr) {
                        buildScope(expr3, scope);
                    }
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
    } else if (BinaryExpression* bin_expr = dynamic_cast<BinaryExpression*>(node)){
        bin_expr->scope = scope;
        buildScope(bin_expr->lhs, scope + "." + to_string(child_scope++));
        buildScope(bin_expr->rhs, scope + "." + to_string(child_scope++));
    } else if (UnaryExpression* un_expr = dynamic_cast<UnaryExpression*>(node)){
        un_expr->scope = scope;
        buildScope(un_expr->expr, scope + "." + to_string(child_scope++));
    } else if (FunctionCall* func_call = dynamic_cast<FunctionCall*>(node)){
        func_call->scope = scope;
        if (func_call->argumentList == nullptr) return;
        for (auto &expr : *(func_call->argumentList)){
            buildScope(expr, scope + "." + to_string(child_scope++));
        }
    } else if (SingleChainExpression* single_chain = dynamic_cast<SingleChainExpression*>(node)){
        single_chain->scope = scope;
        // cout << scope << endl;

        if (single_chain->access != nullptr){
            buildScope(single_chain->access, scope + "." + to_string(child_scope++));
        }

        if (single_chain->functionCallList == nullptr) return;
        for (auto &[func, expr] : *(single_chain->functionCallList)){
            buildScope(func, scope + "." + to_string(child_scope++));
            if (expr == nullptr) continue;
            buildScope(expr, scope + "." + to_string(child_scope++));
        }
    } else {
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
                std::string scope = decl_stmt->get_scope();
                DataType type = mapTypeToDataType(decl_stmt->type->type->at(0));
                if (!symtab.insert(name, type, scope, decl_stmt->row, decl_stmt->column))
                {
                    std::cout << "\e[31m Error: \e[0m Identifier " << name << " in declaration statement is already declared\n";
                    printerror(decl_stmt->row,decl_stmt->column);
                    error_count++;
                    continue;
                } else {
                    if(debug) {cout << "inserting into symbol table" << endl;}
                }
                //  type checking here
                if (init_decl->initializer != nullptr && init_decl->initializer->assignmentExpression!=nullptr && init_decl->initializer->assignmentExpression->expression!=nullptr){
                    if (debug) {cout << "checking type" << endl;}
                    Expression* expr = init_decl->initializer->assignmentExpression->expression;
                    DataType rhs = traverse_operations(expr,scope);
                    if( rhs == Unknown) return;
                    if(matchDataType(type,rhs)){

                    } else if (type == Integer)
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
                            if( rhs == Unknown) return;
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
                // else cout<<"initilizer is null"<<endl;
            }
        }
    } else {
        std::cout << "\e[31m Error: \e[0m Expected identifier after type specifier\n";
        printerror(decl_stmt->row,decl_stmt->column);
        error_count++;
    }
    if(debug) {cout << "exiting declaration" << endl;}
}

// function to do semantic checks on the loop statement
void traverse_loop_statement(LoopStatement *loop_stmt,bool isFunction)
{
    if(debug) {cout << "entering loop" << endl;}
    std::string name = loop_stmt->variable->identifier;
    std::string scope = loop_stmt->scope;
    DataType dataType = Integer;
    
    if (!symtab.insert(name, dataType, scope, loop_stmt->row, loop_stmt->column))
    {
        std::cout << "\e[31m Error: \e[0m Identifier in loop statement is already declared\n";
        printerror(loop_stmt->variable->row,loop_stmt->variable->column);
        error_count++;
    } else {
        if(debug) {cout << "inserting into symbol table" << endl;}
    }
    
    // fromtopair check
    if (loop_stmt->fromToPairs != nullptr)
    {
        for (auto &pair : *(loop_stmt->fromToPairs))
        {
            // check if the from and to expressions are of type integer
            if (std::get<0>(pair) != nullptr)
            {
                if (traverse_operations(std::get<0>(pair),scope) != Integer)
                {
                    std::cout << "\e[31m Error: \e[0m From expression in loop statement is not of type integer\n";
                    printerror(std::get<0>(pair)->row,std::get<0>(pair)->column);
                    error_count++;
                }
            }
            if (std::get<1>(pair) != nullptr)
            {
                if (traverse_operations(std::get<1>(pair),scope) != Integer)
                {
                    std::cout << "\e[31m Error: \e[0m To expression in loop statement is not of type integer\n";
                    printerror(std::get<1>(pair)->row,std::get<1>(pair)->column);
                    error_count++;
                }
            }
            if (std::get<2>(pair) != nullptr)
            {
                if (traverse_operations(std::get<2>(pair),scope) != Integer)
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
    
    // map the type (which is a number)to DataType enum
    Parameter* param = func_dec->inpParameter;
    DataType inputParameters = mapTypeToDataType(param->type->type->at(0));
    string scope = param->get_scope();
    if (param->identifier == nullptr) {
        cout << "\e[31m Error: \e[0m identifier name missing in function declaration input parameter!" << endl;
        printerror(param->row,param->column);
        error_count++;
    } else {
        string name = param->identifier->identifier;
        if (!symtab.insert(name, inputParameters, scope, param->identifier->row, param->identifier->column)) {
            cout << "\e[31m Error: \e[0m Identifier " << name << " in function declaration is already declared\n";
            printerror(param->identifier->row,param->identifier->column);
            error_count++;
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
            if (!symtab.insert(name, dataType, scope, param->identifier->row, param->identifier->column)) {
                cout << "\e[31m Error: \e[0m Identifier " << name << " in function declaration is already declared\n";
                printerror(param->identifier->row,param->identifier->column);
                error_count++;
            }
        }
    }
    // map the type (which is a number)to DataType enum
    param = func_dec->outParameter;
    DataType returnParameters = mapTypeToDataType(param->type->type->at(0));
    if (param->identifier != nullptr) {
        cout << "\e[31m Error: \e[0m unecessary identifier in function declaration return parameter!" << endl;
        printerror(param->row,param->column);
        error_count++;
    }
    if (func_dec->identifier != nullptr) {
        std::string name = func_dec->identifier;
        // std::string scope = func_dec->get_scope();
        std::string scope = ".g";
        // symtab.insert(name, inputParameters, otherParameters, returnParameters, scope, func_dec->row, func_dec->column);
        symtab.insert(name, new std::vector<DataType>({inputParameters}), otherParameters, new std::vector<DataType>({returnParameters}), scope, func_dec->row, func_dec->column);
    } else {
        cout << "\e[31m Error: \e[0m identifier name missing in function declaration!" << endl;
        printerror(func_dec->row,func_dec->column);
        error_count++;
    }
    
    for (auto *stmt : *(func_dec->statements)) {
        Statement *statement = dynamic_cast<Statement *>(stmt);
        traverse_statement(statement,true,false, func_dec->identifier);
    }
}

vector<int> match_function_symb_entry(SymbolTableEntry* entry, DataType inputP){
    if(debug) {cout << "Entering match function symbol entry" << endl;}
    vector<DataType>* inputParameters = entry->inputParameters;
    vector<int> matchedIndex;
    for(int i=0;i<inputParameters->size();i++){
        DataType inputParameter = inputParameters->at(i);
        if(matchDataType(inputParameter,inputP)){
            matchedIndex.push_back(i);
        }
    }
    return matchedIndex;

}
//IT MUST RETURN TRUE IF THE FUNCTION CALL IS MATCHED CORRECTLY. Match the ith one
bool traverse_function_call(FunctionCall* functionCall,SymbolTableEntry* entry, int index) {
    if(debug) {cout << "Entering function call" << endl;}
    // if (index >= entry->otherParameters->size()) {
    //     cout << "error" << endl;
    //     return false;
    // }
    vector<Argument *> *argumentList = functionCall->argumentList;
    vector<FunctionArgument> *otherParameters = entry->otherParameters->at(index);


    if (argumentList->size() != otherParameters->size()) {
        return false;
    }
    
    for (int i = 0; i < argumentList->size(); i++) {
        Argument *argument = argumentList->at(i);
        if (argument->expression != nullptr) {
            if (!otherParameters->at(i).isDataType) {
                return false;
            }
            Expression *expr = argument->expression;
            DataType exprType = traverse_operations(expr, entry->scope);
            if(exprType==Unknown){
                return false;
            }
            if (!matchDataType(exprType, otherParameters->at(i).dataType)) {
                return false;
            }
        } else if (argument->fromToAlsoExpression != nullptr) {
            if (!otherParameters->at(i).isFromToAlso) {
                return false;
            }
            for (auto &tuple : *(argument->fromToAlsoExpression)) {
                Expression *from = std::get<0>(tuple);
                Expression *to = std::get<1>(tuple);
                Expression *also = std::get<2>(tuple);
                DataType fromType = traverse_operations(from, entry->scope);
                DataType toType = traverse_operations(to, entry->scope);
                DataType alsoType = traverse_operations(also, entry->scope);
                if(fromType==Unknown || toType==Unknown || alsoType==Unknown){
                    return false;
                }
                if (fromType != Integer || toType != Integer || alsoType != Integer) {
                    cout << "\e[31m Error: \e[0m Function " << entry->name << " expects integer arguments\n";
                    printerror(argument->row,argument->column);
                    error_count++;
                }
            }
        } else if (argument->statements != nullptr) {
            if (debug) {cout << "checking statements" << endl;}
            if (!otherParameters->at(i).isPf) {
                return false;
            }
            // collect all the undeclared variables in the function
            vector<string> undeclaredVariables;

            for (auto &stmt : *(argument->statements)) {
                Statement *statement = dynamic_cast<Statement *>(stmt);
                traverse_statement(statement,true,false);
            }

            // if (undeclaredVariables.size() != otherParameters->at(i).pfDataTypes->size()) {
            //     return false;
            // }
            
            // for (int i = 0; i < undeclaredVariables.size(); i++) {
            //     symtab.insert(undeclaredVariables[i], otherParameters->at(i).pfDataTypes->at(i), entry->scope, argument->row, argument->column);
            // }

            // for (auto &stmt : *(argument->statements)) {
            //     Statement *statement = dynamic_cast<Statement *>(stmt);
            //     traverse_statement(statement,true,false);
            // }

        } else {
            cout << "\e[31m Error: \e[0m Argument is null\n";
            printerror(argument->row,argument->column);
            error_count++;
        }
    }
    
    return true;
}

bool validate_access_list(Expression* expr, string scope) {
    if(debug) {cout << "Entering validate access list" << endl;}

    if (expr == nullptr) {
        return true;
    }
    
    DataType dataType = traverse_operations(expr,scope);
    if(dataType==Unknown){
        return false;
    }
    if(dataType!=Integer){
        cout << "\e[31m Error: \e[0m Access operator expects integer argument\n";
        printerror(expr->row,expr->column);
        error_count++;
        return false;
    }

    return true;
}

DataType traverse_function_call_list_multi(vector<pair<FunctionCall *, Expression *>> functionCallList,DataType currentDataType,string scope)
{
    if(debug) {cout << "Entering function call list multi" << endl;}
    // cout<<"in multi call"<<endl;
    for(int i=0;i<functionCallList.size();i++){
        pair<FunctionCall *, Expression *> funcPair = functionCallList.at(i);
        FunctionCall *functionCall = funcPair.first;
        Expression *access = funcPair.second;
        std::string functionName ;
        if(functionCall->identifier!=nullptr){
            functionName = std::string(functionCall->identifier);
        } else {
            functionName = mapInbuiltFunctionToString(functionCall->inbuiltFunc);
        }
        SymbolTableEntry *functionEntry = symtab.search(functionName, ".g");

        if(functionEntry==nullptr){
            cout << "\e[31m Error: \e[0m Function " << functionName << " not declared\n";
            printerror(functionCall->row,functionCall->column);
            error_count++;
            return currentDataType;
        }
        vector<int> funcIndex = match_function_symb_entry(functionEntry, currentDataType);
        if(funcIndex.size()==0){
            cout << "\e[31m Error: \e[0m Function " << functionName << " arguments have not matched\n";
            printerror(functionCall->row,functionCall->column);
            error_count++;
            return currentDataType;
        }
        vector<int> matchedIndex;
        //search for the function call in the list of functions
        for(auto &i:funcIndex){
            if(traverse_function_call(functionCall,functionEntry,i)){
                matchedIndex.push_back(i);
                // break;
            }
        }

        if(matchedIndex.size()==0){
            cout << "\e[31m Error: \e[0m no matching call found for function " << functionName << "\n";
            printerror(functionCall->row,functionCall->column);
            error_count++;
            return Unknown;
        }
        if(i<functionCallList.size()-1) {
            // cout<<"i: "<<i<<" "<<functionCallList.size()<<endl;
            //match the return type of the function with the input type of the next function
            int isMatched = 0;
            FunctionCall *nextFunctionCall = functionCallList.at(i+1).first;
            string nextFunctionName ;
            if(nextFunctionCall->identifier!=nullptr){
                nextFunctionName = std::string(nextFunctionCall->identifier);
            } else {
                nextFunctionName = mapInbuiltFunctionToString(nextFunctionCall->inbuiltFunc);
            }
            SymbolTableEntry *nextFunctionEntry = symtab.search(nextFunctionName, ".g");
            if(nextFunctionEntry==nullptr){
                cout << "\e[31m Error: \e[0m Function " << nextFunctionName << " not declared\n";
                printerror(functionCall->row,functionCall->column);
                error_count++;
                return Unknown;
            }
            for(auto &matchi:matchedIndex){
                //print all return paramaters of the function
                // cout<<functionEntry->returnParameters->size()<<" "<<matchedIndex.at(matchi)<<endl;
                
                DataType returnParameters = functionEntry->returnParameters->at(matchi);

                for (int j=0;j<nextFunctionEntry->inputParameters->size();j++){
                // cout<<"next"<<nextFunctionEntry->inputParameters->size()<<endl;
                    DataType nextInputParameters = nextFunctionEntry->inputParameters->at(j);
                    //check if the return type of the function matches the input type of the next function
                    // cout<<returnParameters->size()<<" "<<nextInputParameters->size()<<endl;
                    if(matchDataType(returnParameters,nextInputParameters)){
                        isMatched = 1;
                        break;
                    }
                }
                if(isMatched==0){
                    continue;
                } 
                if(isMatched){
                    if(access==nullptr){
                        // cout<<"here"<<endl;
                        currentDataType = returnParameters;
                        break;
                    }
                    else {
                            // cout<<"safe"<<endl;
                        if(currentDataType == Unknown){
                            cout << "\e[31m Error: \e[0m Invalid usage of access operator\n";
                            printerror(functionCall->row,functionCall->column);
                            error_count++;
                            return Unknown;
                        } else {
                            if(!validate_access_list(access,scope)){
                                return Unknown;
                            }
                            DataType dataType = currentDataType;
                            if(dataType==Array_Integer) currentDataType = Integer;
                            else if(dataType==Array_Float) currentDataType = Float;
                            else if(dataType==Array_Char) currentDataType = Char;
                            else if(dataType==Array_String) currentDataType = String;
                            else if(dataType==Array_Dataset) currentDataType = Dataset;
                            else if(dataType==Array_Boolean) currentDataType = Boolean;
                            else if(dataType==Array) currentDataType = Any;
                            else if (access) {
                                //for other data types like integer, float, char, string, boolean we can't have access operator
                                cout << "\e[31m Error: \e[0m Invalid usage of access operator\n";
                                printerror(functionCall->row,functionCall->column);
                                error_count++;
                                return Unknown;
                            }
                        }
                    }
                }
            } 
            // cout<<"is matched"<<isMatched<<i<<endl;
            if(isMatched==0){
                cout << "\e[31m Error: \e[0m Function " << functionName << " return type does not match the input type of the next function\n";
                printerror(functionCall->row,functionCall->column);
                error_count++;
                return Unknown;
            }
        } else {
            currentDataType = Unknown;
            Expression* access = funcPair.second;
            if(functionEntry->returnParameters->size()==0) {
                currentDataType = Unknown;
            } else {
                currentDataType = functionEntry->returnParameters->at(matchedIndex.at(0));
            }
            if(access && !validate_access_list(access,scope)){
                return Unknown;
            }
            if (access != nullptr){
                DataType dataType = currentDataType;
                if(( (dataType!=Array && dataType!=Array_Integer && dataType!=Array_Float && dataType!=Array_Char && dataType!=Array_String && dataType!=Array_Boolean))){
                    cout << "\e[31m Error: \e[0m Invalid usage of Access Operator\n";
                    printerror(functionCall->row,functionCall->column);
                    error_count++;
                    return Unknown;
                } 
                if(dataType==Array_Integer){
                    dataType = Integer;
                } else if(dataType==Array_Float){
                    dataType = Float;
                } else if(dataType==Array_Char){
                    dataType = Char;
                } else if(dataType==Array_String){
                    dataType = String;
                } else if(dataType==Array_Boolean){
                    dataType = Boolean;
                } else if(dataType==Array_Dataset){
                    dataType = Dataset;
                } else if(dataType==Array){
                    cout<<"return ANY"<<endl;
                    dataType = Any;
                } else {
                    cout << "\e[31m Error: \e[0m Invalid usage of Access Operator\n";
                    printerror(functionCall->row,functionCall->column);
                    error_count++;
                    return Unknown;
                }
                currentDataType = dataType;
            }

        }
    }
    if(debug) {cout << "Exiting function call list multi" << endl;}
    return currentDataType;
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
    DataType dataType;
    if (identifier != nullptr) {
        std::string identifier_str = std::string(identifier);
        SymbolTableEntry *entry = symtab.search(identifier_str, singleChainExpression->get_scope());
        if (debug) {cout << "Identifier: " << identifier_str << ' ' << singleChainExpression->get_scope() <<  endl;}

        if (entry == nullptr){
            cout << "\e[31m Error: \e[0m Identifier " << identifier_str << " not declared\n";
            printerror(singleChainExpression->row,singleChainExpression->column);
            error_count++;
            return Unknown;
        }

        dataType = entry->dataType;
        // cout<<"DataType: "<<dataTypeToString(dataType)<<endl;
        Expression* access = singleChainExpression->access;
        if(!validate_access_list(access,scope)){
            return Unknown;
        }

        if(access != nullptr){
            if( (dataType!=Array && dataType!=Array_Integer && dataType!=Array_Float && dataType!=Array_Char && dataType!=Array_String && dataType!=Array_Boolean) ){
                cout << "\e[31m Error: \e[0m Invalid usage of Access Operator\n";
                printerror(singleChainExpression->row,singleChainExpression->column);
                error_count++;
                return Unknown;
            } 
            if(dataType==Array_Integer){
                dataType = Integer;
            } else if(dataType==Array_Float){
                dataType = Float;
            } else if(dataType==Array_Char){
                dataType = Char;
            } else if(dataType==Array_String){
                dataType = String;
            } else if(dataType==Array_Boolean){
                dataType = Boolean;
            } else if(dataType==Array_Dataset){
                dataType = Dataset;
            } else if(dataType==Array){
                dataType = Any;
            } else {
                cout << "\e[31m Error: \e[0m Invalid usage of Access Operator\n";
                printerror(singleChainExpression->row,singleChainExpression->column);
                error_count++;
                return Unknown;
            }
        }
    } else {
        dataType = Void;
    }


    return traverse_function_call_list_multi(*singleChainExpression->functionCallList,dataType,scope);
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

            if (debug) {
                cout << "scope: " << scope << endl;
                cout << "scope2: " << unaryExpression->scope << endl;
            }

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
        if (lhs == Unknown || rhs == Unknown){
            return Unknown;
        }
        if (binaryExpression->op == add_op){
            if (lhs == Integer && rhs == Integer) {
                return Integer;
            } else if (lhs == Integer && rhs == Float) {
                return Float;
            } else if (lhs == Float && rhs == Integer) {
                return Float;
            } else if (lhs == Float && rhs == Float) {
                return Float;
            } else if (lhs == Array_Integer && (rhs == Integer || rhs == Array_Integer)){
                return Array_Integer;
            } else if (lhs == Array_Integer && (rhs == Float || rhs == Array_Float)){
                return Array_Float;
            } else if (lhs == Array_Float && (rhs == Integer || rhs == Array_Integer || rhs == Float || rhs == Array_Float)){
                return Array_Float;
            } else if (lhs == Dataset && (rhs == Integer || rhs == Float || rhs == Dataset)){
                return Dataset;
            } else {
                cout << "\e[31m Error: \e[0m Cannot perform \"+\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
                printerror(binaryExpression->row,binaryExpression->column);
                error_count++;
                return Unknown;
            }
        }
        else if (binaryExpression->op == sub_op)
        {
            if (lhs == Integer && rhs == Integer) {
                return Integer;
            } else if (lhs == Integer && rhs == Float) {
                return Float;
            } else if (lhs == Float && rhs == Integer) {
                return Float;
            } else if (lhs == Float && rhs == Float) {
                return Float;
            } else if (lhs == Array_Integer && (rhs == Integer || rhs == Array_Integer)){
                return Array_Integer;
            } else if (lhs == Array_Integer && (rhs == Float || rhs == Array_Float)){
                return Array_Float;
            } else if (lhs == Array_Float && (rhs == Integer || rhs == Array_Integer || rhs == Float || rhs == Array_Float)){
                return Array_Float;
            } else if (lhs == Dataset && (rhs == Integer || rhs == Float || rhs == Dataset)){
                return Dataset;
            } else {
                cout << "\e[31m Error: \e[0m Cannot perform \"-\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
                printerror(binaryExpression->row,binaryExpression->column);
                error_count++;
                return Unknown;
            }
        } else if (binaryExpression->op == mul_op){
            if (lhs == Integer && rhs == Integer) {
                return Integer;
            } else if (lhs == Integer && rhs == Float) {
                return Float;
            } else if (lhs == Float && rhs == Integer) {
                return Float;
            } else if (lhs == Float && rhs == Float) {
                return Float;
            } else if (lhs == Array_Integer && (rhs == Integer || rhs == Array_Integer)){
                return Array_Integer;
            } else if (lhs == Array_Integer && (rhs == Float || rhs == Array_Float)){
                return Array_Float;
            } else if (lhs == Array_Float && (rhs == Integer || rhs == Array_Integer || rhs == Float || rhs == Array_Float)){
                return Array_Float;
            } else if (lhs == Dataset && (rhs == Integer || rhs == Float || rhs == Dataset)){
                return Dataset;
            } else{
                cout << "\e[31m Error: \e[0m Cannot perform \"*\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
                printerror(binaryExpression->row,binaryExpression->column);
                error_count++;
                return Unknown;
            }
        } else if (binaryExpression->op == div_op){
            if (lhs == Integer && rhs == Integer) {
                return Integer;
            } else if (lhs == Integer && rhs == Float) {
                return Float;
            } else if (lhs == Float && rhs == Integer) {
                return Float;
            } else if (lhs == Float && rhs == Float) {
                return Float;
            } else if (lhs == Array_Integer && (rhs == Integer || rhs == Array_Integer)){
                return Array_Integer;
            } else if (lhs == Array_Integer && (rhs == Float || rhs == Array_Float)){
                return Array_Float;
            } else if (lhs == Array_Float && (rhs == Integer || rhs == Array_Integer || rhs == Float || rhs == Array_Float)){
                return Array_Float;
            } else if (lhs == Dataset && (rhs == Integer || rhs == Float || rhs == Dataset)){
                return Dataset;
            } else {
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
            } else if (lhs == Boolean && rhs == Integer){
                return Boolean;
            } else if (lhs == Boolean && rhs == Float){
                return Boolean;
            } else if (lhs == Integer && rhs == Boolean){
                return Boolean;
            } else if (lhs == Integer && rhs == Float){
                return Boolean;
            } else if (lhs == Integer && rhs == Integer){
                return Boolean;
            } else if (lhs == Float && rhs == Integer){
                return Boolean;
            } else if (lhs == Float && rhs == Boolean){
                return Boolean;
            } else if (lhs == Float && rhs == Float) {
                return Boolean;
            } else {
                cout << "\e[31m Error: \e[0m Cannot perform \"&&\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
                printerror(binaryExpression->row,binaryExpression->column);
                error_count++;
                return Unknown;
            }
        } else if (binaryExpression->op == or_op){
            if (lhs == Boolean && rhs == Boolean){
                return Boolean;
            } else if (lhs == Boolean && rhs == Integer){
                return Boolean;
            } else if (lhs == Boolean && rhs == Float){
                return Boolean;
            } else if (lhs == Integer && rhs == Boolean){
                return Boolean;
            } else if (lhs == Integer && rhs == Float){
                return Boolean;
            } else if (lhs == Integer && rhs == Integer){
                return Boolean;
            } else if (lhs == Float && rhs == Integer){
                return Boolean;
            } else if (lhs == Float && rhs == Boolean){
                return Boolean;
            } else if (lhs == Float && rhs == Float) {
                return Boolean;
            } else{
                cout << "\e[31m Error: \e[0m Cannot perform \"||\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
                printerror(binaryExpression->row,binaryExpression->column);
                error_count++;
                return Unknown;
            }
        } else if (binaryExpression->op == eq_op){
            if (debug) {cout << "lhs: " << dataTypeToString(lhs) << " rhs: " << dataTypeToString(rhs) << endl;}
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
            } else if (lhs == Array && rhs == Array){
                return Boolean;
            } else if (lhs == Array_Integer && rhs == Array_Integer){
                return Boolean;
            } else if (lhs == Array_Float && rhs == Array_Float){
                return Boolean;
            } else if (lhs == Array_Char && rhs == Array_Char){
                return Boolean;
            } else if (lhs == Array_String && rhs == Array_String){
                return Boolean;
            } else if (lhs == Array_Boolean && rhs == Array_Boolean){
                return Boolean;
            } else if (lhs == Array_Dataset && rhs == Array_Dataset){
                return Boolean;
            } else if (lhs == Dataset && rhs == Dataset){
                return Boolean;
            } else {
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
            } else if (lhs == Array && rhs == Array){
                return Boolean;
            } else if (lhs == Array_Integer && rhs == Array_Integer){
                return Boolean;
            } else if (lhs == Array_Float && rhs == Array_Float){
                return Boolean;
            } else if (lhs == Array_Char && rhs == Array_Char){
                return Boolean;
            } else if (lhs == Array_String && rhs == Array_String){
                return Boolean;
            } else if (lhs == Array_Boolean && rhs == Array_Boolean){
                return Boolean;
            } else if (lhs == Array_Dataset && rhs == Array_Dataset){
                return Boolean;
            } else if (lhs == Dataset && rhs == Dataset){
                return Boolean;
            } else {
                cout << "\e[31m Error: \e[0m Cannot perform \"!=\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
                printerror(binaryExpression->row,binaryExpression->column);
                error_count++;
                return Unknown;
            }
        } else if (binaryExpression->op == gt_op){
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
            } else{
                cout << "\e[31m Error: \e[0m Cannot perform \">\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
                printerror(binaryExpression->row,binaryExpression->column);
                error_count++;
                return Unknown;
            }
        } else if (binaryExpression->op == lt_op) {
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
            } else{
                cout << "\e[31m Error: \e[0m Cannot perform \"<\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
                printerror(binaryExpression->row,binaryExpression->column);
                error_count++;
                return Unknown;
            }
        } else if (binaryExpression->op == gte_op){
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
            } else {
                cout << "\e[31m Error: \e[0m Cannot perform \">=\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
                printerror(binaryExpression->row,binaryExpression->column);
                error_count++;
                return Unknown;
            }
        } else if (binaryExpression->op == lte_op) {
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
            } else {
                cout << "\e[31m Error: \e[0m Cannot perform \"<=\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
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
    return DataType::Unknown;
}

void traverse_assignment(AssignmentStatement *assignmentStatement)
{
    if(debug) {cout << "entering assignment" << endl;}
    if (assignmentStatement->declarator == nullptr)
    {
        DataType rhs = traverse_operations(assignmentStatement->expression,assignmentStatement->scope);
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
    if(lhs==Unknown ){
        return;
    }
    DataType rhs = traverse_operations(assignmentStatement->expression,scope);
    if(rhs==Unknown){
        return;
    }
    AssignmentOperator operator_type = assignmentStatement->op;
    if(operator_type == assign)
    {
        if (!matchDataType(lhs, rhs))
        {
            cout<<"ehere"<<endl;
            cout << "\e[31m Error: \e[0m Cannot assign " << dataTypeToString(rhs) << " to " << dataTypeToString(lhs) << "!\n";
            printerror(assignmentStatement->row,assignmentStatement->column);
            error_count++;
            return;
        }
    }else if(operator_type == add_assign)
    {
        if(lhs == Integer && lhs == Float)
        {
            return;
        }
        if(lhs == Integer && rhs == Float)
        {
            return;
        }
        if(matchDataType(lhs, rhs))
        {
            return;
        }
        if(lhs==Array && (rhs==Integer || rhs==Float || rhs==Char || rhs==String || rhs==Boolean || rhs==Dataset))
        {
            return;
        }
        if((lhs==Array_Integer && rhs==Integer) || (lhs==Array_Float && rhs==Float) || (lhs==Array_Char && rhs==Char) || (lhs==Array_String && rhs==String) || (lhs==Array_Boolean && rhs==Boolean) || (lhs==Array_Dataset && rhs==Dataset))
        {
            return;
        } 
        cout << "\e[31m Error: \e[0m Cannot perform \"+=\" operation with " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
        printerror(assignmentStatement->row,assignmentStatement->column);
        error_count++;
        return;

    }else if(operator_type == sub_assign)
    {
        if(lhs == Integer && lhs == Float)
        {
            return;
        }
        if(matchDataType(lhs, rhs))
        {
            return;
        }
        if(rhs == Integer && rhs == Float)
        {
            return;
        }
        if(lhs == rhs)
        {
            return;
        }
        cout << "\e[31m Error: \e[0m Cannot perform \"-=\" operation with " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
        error_count++;
    }else if(operator_type == mul_assign)
    {

        if(lhs == Integer && (lhs == Float || rhs == Any))
        {
            return;
        }
        if(rhs == Integer && (rhs == Float || rhs == Any))
        {
            return;
        }
        if(matchDataType(lhs, rhs))
        {
            return;
        }
        if(lhs == rhs)
        {
            return;
        }
        if((lhs==Array || lhs==Array_Integer ) && (rhs==Integer || rhs==Any))
        {
            return;
        }
        if((lhs==Array || lhs==Array_Float ) && (rhs==Float || rhs==Any))
        {
            return;
        }
        cout << "\e[31m Error: \e[0m Cannot perform \"*=\" operation with " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
        error_count++;
        printerror(assignmentStatement->row,assignmentStatement->column);
        return;
    }else if(operator_type == div_assign)
    {
        if(lhs == Integer && lhs == Float)
        {
            return;
        }
        if(rhs == Integer && rhs == Float)
        {
            return;
        }
        if(matchDataType(lhs, rhs))
        {
            return;
        }
        if(lhs == rhs)
        {
            return;
        }
        cout << "\e[31m Error: \e[0m Cannot perform \"/=\" operation with " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
        error_count++;
        printerror(assignmentStatement->row,assignmentStatement->column);
        return;
    }else if(operator_type == mod_assign)
    {
        if(lhs == Integer && lhs == Float)
        {
            return;
        }
        if(matchDataType(lhs, rhs))
        {
            return;
        }
        if(rhs == Integer && rhs == Float)
        {
            return;
        }
        if(lhs == rhs)
        {
            return;
        }
        cout << "\e[31m Error: \e[0m Cannot perform \"%=\" operation with " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "!\n";
        error_count++;
        printerror(assignmentStatement->row,assignmentStatement->column);
        return;
    }
}

void traverse_if_statement(ConditionalStatement *cond_stmt,bool isFunction,bool isLoop)
{
    if(debug) {cout << "entering if" << endl;}
    if (cond_stmt->ConditionStatements != nullptr)
    {
        for (auto &[expr, stmt_list] : *(cond_stmt->ConditionStatements)) {
            // check if the expression is of type boolean
            if (expr != nullptr) {
                string scope = string(cond_stmt->get_scope());
                DataType temp = traverse_operations(expr,scope);
                if(temp==Unknown) {
                    return;
                }
                if (temp != Boolean && temp != Integer) {
                    std::cout << "\e[31m Error: \e[0m Expression in conditional statement is not of type boolean\n";
                    printerror(cond_stmt->row,cond_stmt->column);
                    error_count++;
                }
            } else {
                if (debug) {
                    cout << "cond statement Expression is null" << endl;
                }
            }
            
            for (auto &in_stmt : *stmt_list) {
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
    start->buildScope(".g");
    // buildScope(start, ".g");
    //for functions in inbuilt function, add eacg of them to symbol table
    insert_inbuiltfunction_symtab();
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