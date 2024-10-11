#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <iomanip>

enum DataType {
    Integer,
    Float,
    Char,
    Boolean,
    String,
    Dataset,
    Array,
    Function
};

class SymbolTableEntry {
public:
    std::string name;
    DataType dataType;
    std::string scope;
    int rowNum;
	int colNum;

    SymbolTableEntry(std::string& name, DataType& dataType, std::string& scope, int rowNum, int colNum) {
        this->name = name;
        this->dataType = dataType;
        this->scope = scope;
        this->rowNum = rowNum;
        this->colNum = colNum;
    }
};

class SymbolTable {
private:
    std::unordered_map<std::pair<std::string, std::string>, SymbolTableEntry> symtab;

public:
    bool insert(std::string&, DataType&, std::string&, int, int);
    SymbolTableEntry* search(std::string&, std::string&);
    void print();
};