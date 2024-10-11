#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <functional>

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
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

class SymbolTable {
private:
    std::unordered_map<std::pair<std::string, std::string>, SymbolTableEntry, pair_hash> symtab;

public:
    SymbolTable() {}
    bool insert(std::string&, DataType&, std::string&, int, int);
    SymbolTableEntry* search(std::string&, std::string&);
    void print();
};