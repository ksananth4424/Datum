#include "symbol_table.hpp"

bool SymbolTable::insert(std::string& name, DataType& dataType, std::string& scope, int rowNum, int colNum) {
    SymbolTableEntry symEntry(name, dataType, scope, rowNum, colNum);

    if (SymbolTable::search(name, scope) == nullptr) {
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
    std::cout << "Symbol Table: ";
    std::cout << SymbolTable::symtab.size() << "\n";
    printf("Name\tScope\tDatatype\tVartype\tRow\tCol\n");
    for (const auto &x : SymbolTable::symtab)
    {
        std::cout << x.first.first;
        std::cout << std::setw(10) << x.first.second;
        std::cout << std::setw(10) << x.second.dataType;
        std::cout << std::setw(10) << x.second.rowNum;
        std::cout << std::setw(10) << x.second.colNum;
        std::cout << "\n";
    }
    std::cout << "\n";
}