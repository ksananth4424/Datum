#include "symbol_table.hpp"

bool SymbolTable::insert(std::string& name, DataType dataType, std::string scope, int rowNum, int colNum) {
    SymbolTableEntry symEntry(name, dataType, scope, rowNum, colNum);
    std::pair<std::string, std::string> key = {name, scope};

    if (SymbolTable::symtab.count(key)) {
        return false;
    }

    SymbolTable::symtab[key] = symEntry;
    return true;
}

bool SymbolTable::insert(std::string& name, std::vector<DataType>* inputParameters, std::vector<std::vector<DataType>*> * otherParameters, std::vector<DataType>* returnParameters, std::string scope, int colNum, int rowNum) {
    SymbolTableEntry symEntry(name, inputParameters, otherParameters, returnParameters, scope, rowNum, colNum);
    std::pair<std::string, std::string> key = {name, scope};

    if (SymbolTable::symtab.count(key)) {
        return false;
    }

    SymbolTable::symtab[key] = symEntry;
    return true;
}

bool SymbolTable::insert(std::string& name, std::vector<DataType>* inputParameters, std::vector<std::vector<FunctionArgument>*> * otherParameters, std::vector<DataType>* returnParameters, std::string scope, int colNum, int rowNum) {
    SymbolTableEntry symEntry(name, inputParameters, otherParameters, returnParameters, scope, rowNum, colNum);
    std::pair<std::string, std::string> key = {name, scope};

    if (SymbolTable::symtab.count(key)) {
        return false;
    }

    SymbolTable::symtab[key] = symEntry;
    return true;
}

SymbolTableEntry* SymbolTable::search(std::string& name, std::string scope) {
    std::pair<std::string, std::string> key = {name, scope};
    while (scope != "" and SymbolTable::symtab.count(key) == 0) {
        size_t pos = scope.find_last_of(".");
        scope = scope.substr(0, pos);
        key = {name, scope};
    }
    if (scope == "") {
        return nullptr;
    }
    return &(SymbolTable::symtab[key]);
}

SymbolTableEntry* SymbolTable::searchFunction(std::string functionName) {
    return search(functionName, ".g");
}

void SymbolTable::print() {
    std::cout << "\n<-------------------Symbol Table:"<<SymbolTable::symtab.size()<<" ------------------->\n";
    std::cout << std::setw(20) << std::left << "Name";
    std::cout << std::setw(20) << std::left << "Scope";
    std::cout << std::setw(15) << std::left << "DataType";
    std::cout << std::setw(15) << std::left << "I Parameters";
    std::cout << std::setw(15) << std::left << "O Parameters";
    std::cout << std::setw(15) << std::left << "R Parameters";
    std::cout << std::setw(15) << std::left << "Row";
    std::cout << std::setw(15) << std::left << "Col";
    std::cout << "\n";
    for (const auto &x : SymbolTable::symtab)
    {
        std::cout << std::setw(20) << x.first.first;
        std::cout << std::setw(20) << x.first.second;
        std::cout << std::setw(15) << x.second.dataType;
        // loop through the input parameters and print them
        if (x.second.inputParameters != nullptr) {
            for (auto &param : *(x.second.inputParameters)) {
                std::cout << std::setw(15) << param;
            }
        } else {
            std::cout << std::setw(15) << "NULL";
        }
        // loop through the other parameters and print them
        if (x.second.otherParameters != nullptr) {
            for (auto &param : *(x.second.otherParameters->at(0))) {
                std::cout << std::setw(15) << param.print();
            }
        } else {
            std::cout << std::setw(15) << "NULL";
        }
        // loop through the return parameters and print them
        if (x.second.returnParameters != nullptr) {
            for (auto &param : *(x.second.returnParameters)) {
                std::cout << std::setw(15) << param;
            }
        } else {
            std::cout << std::setw(15) << "NULL";
        }
        std::cout << std::setw(15) << x.second.rowNum;
        std::cout << std::setw(15) << x.second.colNum;
        std::cout << "\n";
    }
    std::cout<<"<----------------------------------------------------->\n";
    std::cout << "\n";
}

std::string FunctionArgument::print() {
    if (isDataType) {
        return "dataType";
    } else if (isFromToAlso) {
        return "fromToAlso";
    } else if (isPf) {
        return "pfunction";
    } else {
        return "unknown";
    }
}