#include<iostream>
#include "symbol_table.hpp"
#include "traversal.hpp"
using namespace std;

SymbolTable symtab;

void traverse(Start* start){
    if(start->FunctionList!=nullptr){
        cout<<"FunctionList\n";
    }    
    else cout<<"No FunctionList\n";
            if (start->StatementList != nullptr) {
            cout << "StatementList\n";
            for (auto* statement : *(start->StatementList)) {  
                // Dereference the pointer to vector
                if (statement->statementType == 1) {
                    cout << "DeclarationStatement\nscope: " << statement->get_scope() << "\n";
                } else if (statement->statementType == 2) {
                    cout << "AssignmentStatement\nscope: " << statement->get_scope() << "\n";
                } else if (statement->statementType == 3) {
                    cout << "ConditionalStatement\nscope: " << statement->get_scope() << "\n";
                } else if (statement->statementType == 4) {
                    cout << "LoopStatement\nscope: " << statement->get_scope() << "\n";
                } else if (statement->statementType == 5) {
                    cout << "ReturnStatement\nscope: " << statement->get_scope() << "\n";
                } else if (statement->statementType == 6) {
                    cout << "BreakStatement\nscope: " << statement->get_scope() << "\n";
                } else if (statement->statementType == 7) {
                    cout << "ContinueStatement\nscope: " << statement->get_scope() << "\n";
                } else if (statement->statementType == 8) {
                    cout << "CompoundStatement\nscope: " << statement->get_scope() << "\n";
                }
            }
        }

    else cout<<"No StatementList\n";
    
}

void buildScope(Node* node, string scope) {
    if (node == nullptr) return;
    int child_scope = 0;
    if (Start* start = dynamic_cast<Start*>(node)) {
        start->scope = scope;
        for (auto &func_dec : *(start->FunctionList)) {
            buildScope(func_dec, scope + "." + to_string(child_scope++));
        }
        for (auto &stmt : *(start->StatementList)) {
            buildScope(stmt, scope + "." + to_string(child_scope++));
        }
    } else if (FunctionDeclaration* func_dec = dynamic_cast<FunctionDeclaration*>(node)) {
        func_dec->scope = scope;
        for (auto &param : *(func_dec->inpParameter)) {
            buildScope(param, scope + "." + to_string(child_scope++));
        }
        for (auto &param : *(func_dec->otherParameter)) {
            buildScope(param, scope + "." + to_string(child_scope++));
        }
        for (auto &stmt : *(func_dec->statements)) {
            buildScope(stmt, scope + "." + to_string(child_scope++));
        }
        // add all the function declarations to the symbol table
        // symbol table format: name, input parameters, other parameters, return parameters, scope, row number, column number

        cout<<func_dec->identifier<<endl;
        std::string name = func_dec->identifier;
        // get the input parameters
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        for(auto* param : *(func_dec->inpParameter)){
            // map the type (which is a number)to DataType enum
            DataType dataType;
            switch (param->type->type->at(0)) {
            case 0:
                dataType = Integer;
                break;
            case 1:
                dataType = Float;
                break;
            case 2:
                dataType = String;
                break; 
            case 3:
                dataType = Char;
                break;
            case 4:
                dataType = Boolean;
                break;
            case 5:
                dataType = Dataset;
                break;
            case 6:
                dataType = Array;
                break;
            }
            inputParameters->push_back(dataType);
        }
        // get the other parameters
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        for(auto* param : *(func_dec->otherParameter)){
            // map the type (which is a number)to DataType enum
            DataType dataType;
            switch (param->type->type->at(0)) {
            case 0:
                dataType = Integer;
                break;
            case 1:
                dataType = Float;
                break;
            case 2:
                dataType = String;
                break; 
            case 3:
                dataType = Char;
                break;
            case 4:
                dataType = Boolean;
                break;
            case 5:
                dataType = Dataset;
                break;
            case 6:
                dataType = Array;
                break;
            }
            otherParameters->push_back(dataType);
        }
        // get the return parameters
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        for(auto* param : *(func_dec->outParameter)){
            // map the type (which is a number)to DataType enum
            DataType dataType;
            switch (param->type->type->at(0)) {
            case 0:
                dataType = Integer;
                break;
            case 1:
                dataType = Float;
                break;
            case 2:
                dataType = String;
                break;
            case 3:
                dataType = Char;
                break;
            case 4:
                dataType = Boolean;
                break;
            case 5:
                dataType = Dataset;
                break;
            case 6:
                dataType = Array;
                break;
            }
        }

    } else if (Statement* stmt = dynamic_cast<Statement*>(node)) {
        stmt->scope = scope;
        switch (stmt->statementType) {
        case 1:
            // declaration
            stmt->declarationStatement->scope = scope;
            for(auto &decl : *(stmt->declarationStatement->initDeclarations)){
                cout << decl->declarator->identifier << endl;
                std::string identifier = decl->declarator->identifier;
                // map the type (which is a number)to DataType enum
                DataType dataType;
                switch (stmt->declarationStatement->type->type->at(0)) {
                case 0:
                    dataType = Integer;
                    break;
                case 1:
                    dataType = Float;
                    break;
                case 2:
                    dataType = String;
                    break; 
                case 3:
                    dataType = Char;
                    break;
                case 4:
                    dataType = Boolean;
                    break;
                case 5:
                    dataType = Dataset;
                    break;
                case 6:
                    dataType = Array;
                    break;
                }
                std::string scope = stmt->declarationStatement->get_scope();
                symtab.insert(identifier, dataType, scope, 0, 0);
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
                ConditionalStatement* cond_stmt = stmt->conditionalStatement;
                cond_stmt->scope;
                for (auto &[expr, stmt_list] : *cond_stmt->ConditionStatements) {
                    for (auto &in_stmt : *stmt_list) {
                        buildScope(in_stmt, scope + "." + to_string(child_scope++));
                    }
                }
                break;
            }
        case 4:
            // loop
            {
                LoopStatement* loop_stmt = stmt->loopStatement;
                loop_stmt->scope = scope;
                // for (auto &from_to : *loop_stmt->fromToPairs) {
                // }

                // add all loop variable init to symbol table
                // for (auto &from_to : *loop_stmt->fromToPairs) {
                    cout << loop_stmt->identifier << endl;
                    std::string identifier = (loop_stmt)->identifier;
                    DataType dataType = Integer;
                    std::string scope = loop_stmt->get_scope();
                    symtab.insert(identifier, dataType, scope, 0, 0);
                // }

                for (auto &in_stmt : *loop_stmt->statements) {
                    buildScope(in_stmt, scope + "." + to_string(child_scope++));
                }
                break;
            }
        default:
            // compound
            {
                auto comp_stmt = stmt->compoundStatement;
                for (auto &in_stmt : *comp_stmt) {
                    buildScope(in_stmt, scope + "." + to_string(child_scope++));
                }
                break;
            }
        }
    } else {
        node->scope = scope;
    }
}
