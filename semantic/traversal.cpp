#include<iostream>
#include "traversal.hpp"
using namespace std;
void traverse(Start* start){
    if(start->FunctionList!=nullptr) cout<<"FunctionList\n";
    else cout<<"No FunctionList\n";
            if (start->StatementList != nullptr) {
            cout << "StatementList\n";
            for (auto* statement : *(start->StatementList)) {  
                // Dereference the pointer to vector
                if (statement->statementType == 1) {
                    cout << "DeclarationStatement\n";
                } else if (statement->statementType == 2) {
                    cout << "AssignmentStatement\n";
                } else if (statement->statementType == 3) {
                    cout << "ConditionalStatement\n";
                } else if (statement->statementType == 4) {
                    cout << "LoopStatement\n";
                } else if (statement->statementType == 5) {
                    cout << "ReturnStatement\n";
                } else if (statement->statementType == 6) {
                    cout << "BreakStatement\n";
                } else if (statement->statementType == 7) {
                    cout << "ContinueStatement\n";
                } else if (statement->statementType == 8) {
                    cout << "CompoundStatement\n";
                }
            }
        }

    else cout<<"No StatementList\n";
    
}