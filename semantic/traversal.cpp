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

        std::cout << func_dec->identifier << std::endl;
        // get the input parameters
        std::vector<DataType> *inputParameters;
        for(auto* param : *(func_dec->inpParameter)){
            // map the type (which is a number)to DataType enum
            DataType dataType;
            switch (param->type->type->at(0)) {
            case 0: dataType = Integer; break;
            case 1: dataType = Float; break;
            case 2: dataType = String; break; 
            case 3: dataType = Char; break;
            case 4: dataType = Boolean; break;
            case 5: dataType = Dataset; break;
            case 6: dataType = Array; break;
            }
            inputParameters->push_back(dataType);
        }
        // get the other parameters
        std::vector<DataType> *otherParameters;
        for(auto* param : *(func_dec->otherParameter)){
            // map the type (which is a number)to DataType enum
            DataType dataType;
            switch (param->type->type->at(0)) {
            case 0: dataType = Integer; break;
            case 1: dataType = Float; break;
            case 2: dataType = String; break; 
            case 3: dataType = Char; break;
            case 4: dataType = Boolean; break;
            case 5: dataType = Dataset; break;
            case 6: dataType = Array; break;
            }
            otherParameters->push_back(dataType);
        }
        // get the return parameters
        std::vector<DataType> *returnParameters;
        for(auto* param : *(func_dec->outParameter)){
            // map the type (which is a number)to DataType enum
            DataType dataType;
            switch (param->type->type->at(0)) {
            case 0: dataType = Integer; break;
            case 1: dataType = Float; break;
            case 2: dataType = String; break;
            case 3: dataType = Char; break;
            case 4: dataType = Boolean; break;
            case 5: dataType = Dataset; break;
            case 6: dataType = Array; break;
            }
            returnParameters->push_back(dataType);
        }
        if(func_dec->identifier != nullptr){
            std::string name = func_dec->identifier;
            std::string scope = func_dec->get_scope();
            symtab.insert(name, inputParameters, otherParameters, returnParameters, scope, 0, 0);
        }

    } else if (Statement* stmt = dynamic_cast<Statement*>(node)) {
        stmt->scope = scope;
        switch (stmt->statementType) {
        case 1:
            // declaration
            stmt->declarationStatement->scope = scope;
            for(auto &decl : *(stmt->declarationStatement->initDeclarations)){
                cout << decl->declarator->identifier << endl;
                // map the type (which is a number)to DataType enum
                DataType dataType;
                switch (stmt->declarationStatement->type->type->at(0)) {
                case 0: dataType = Integer; break;
                case 1: dataType = Float; break;
                case 2: dataType = String; break; 
                case 3: dataType = Char; break;
                case 4: dataType = Boolean; break;
                case 5: dataType = Dataset; break;
                case 6: dataType = Array; break;
                }
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