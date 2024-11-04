#include "symbol_table.hpp"

bool SymbolTable::insert(std::string& name, DataType dataType, std::string& scope, int rowNum, int colNum) {
    SymbolTableEntry symEntry(name, dataType, scope, rowNum, colNum);

    if (SymbolTable::search(name, scope) != nullptr) {
        return false;
    }

    std::pair<std::string, std::string> key = {name, scope};
    SymbolTable::symtab[key] = symEntry;
    return true;
}

bool SymbolTable::insert(std::string& name, std::vector<DataType>* inputParameters, std::vector<DataType>* otherParameters, std::vector<DataType>* returnParameters, std::string& scope, int colNum, int rowNum) {
    SymbolTableEntry symEntry(name, inputParameters, otherParameters, returnParameters, scope, rowNum, colNum);

    if (SymbolTable::search(name, scope) != nullptr) {
        return false;
    }

    std::pair<std::string, std::string> key = {name, scope};
    SymbolTable::symtab[key] = symEntry;
    return true;
}

SymbolTableEntry* SymbolTable::search(std::string& name, std::string& scope) {
    std::pair<std::string, std::string> key = {name, scope};
    if (SymbolTable::symtab.count(key) == 0) {
        return nullptr;
    }
    return &(SymbolTable::symtab[key]);
}

void SymbolTable::print() {
    std::cout << "\n<-------------------Symbol Table:"<<SymbolTable::symtab.size()<<" ------------------->\n";
    printf("Name\tScope\tDatatype\tVartype\tRow\tCol\n");
    for (const auto &x : SymbolTable::symtab)
    {
        std::cout << x.first.first;
        std::cout << std::setw(10) << x.first.second;
        std::cout << std::setw(10) << x.second.dataType;
        // loop through the input parameters and print them
        if (x.second.inputParameters != nullptr) {
            for (auto &param : *(x.second.inputParameters)) {
                std::cout << std::setw(10) << param;
            }
        }
        // loop through the other parameters and print them
        if (x.second.otherParameters != nullptr) {
            for (auto &param : *(x.second.otherParameters)) {
                std::cout << std::setw(10) << param;
            }
        }
        // loop through the return parameters and print them
        if (x.second.returnParameters != nullptr) {
            for (auto &param : *(x.second.returnParameters)) {
                std::cout << std::setw(10) << param;
            }
        }
        std::cout << std::setw(10) << x.second.scope;
        std::cout << std::setw(10) << x.second.rowNum;
        std::cout << std::setw(10) << x.second.colNum;
        std::cout << "\n";
    }
    std::cout<<"<----------------------------------------------------->\n";
    std::cout << "\n";
}