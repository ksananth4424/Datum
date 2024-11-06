#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <functional>

enum DataType {
    Integer,
    Float,
    String,
    Char,
    Boolean,
    Dataset,
    Array,
    Array_Integer,
    Array_Float,
    Array_String,
    Array_Char,
    Array_Boolean,
    Function,
    Unknown
};

class SymbolTableEntry {
public:
    std::string name;
    DataType dataType;
    std::string scope;
    std::vector<std::vector<DataType>*> *returnParameters;
    std::vector<std::vector<DataType>*> *inputParameters;
    std::vector<std::vector<DataType>*> *otherParameters;

    int rowNum;
	int colNum;

    SymbolTableEntry() {} 

    SymbolTableEntry(std::string& name, DataType dataType, std::string& scope, int rowNum, int colNum) {
        this->name = name;
        this->dataType = dataType;
        this->inputParameters = nullptr;
        this->otherParameters = nullptr;
        this->returnParameters = nullptr;
        this->scope = scope;
        this->rowNum = rowNum;
        this->colNum = colNum;
    }

    SymbolTableEntry(std::string& name, std::vector<std::vector<DataType>*> *inputParameters, std::vector<std::vector<DataType>*> *otherParameters, std::vector<std::vector<DataType>*> *returnParameters, std::string& scope, int rowNum, int colNum) {
        this->name = name;
        this->dataType = DataType::Function;
        this->inputParameters = inputParameters;
        this->otherParameters = otherParameters;
        this->returnParameters = returnParameters;
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
    bool insert(std::string&, DataType, std::string, int, int);
    bool insert(std::string&, std::vector<std::vector<DataType>*>*,std::vector<std::vector<DataType>*>*,std::vector<std::vector<DataType>*>*, std::string, int, int);
    SymbolTableEntry* search(std::string&, std::string);
    void print();
};

#endif