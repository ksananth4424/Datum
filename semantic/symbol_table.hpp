#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <functional>
#include <cstring>

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
    Array_Dataset,
    Function,
    Any, //it means the type could be integer or float or string or char or boolean
    Unknown, 
    Void
};

class FunctionArgument {
public:
    bool isDataType;
    bool isPf;
    bool isFromToAlso;
    DataType dataType;
    std::vector<DataType> *pfDataTypes;
    std::string print();

    FunctionArgument(DataType dataType) {
        this->isDataType = true;
        this->dataType = dataType;
        this->isPf = false;
        this->isFromToAlso = false;
    }

    FunctionArgument(std::vector<DataType> *pfDataTypes) {
        this->isDataType = false;
        this->isPf = true;
        this->pfDataTypes = pfDataTypes;
        this->isFromToAlso = false;
    }

    FunctionArgument(bool isFromToAlso) {
        this->isDataType = false;
        this->isPf = false;
        this->isFromToAlso = isFromToAlso;
    }
};


class SymbolTableEntry {
public:
    std::string name;
    DataType dataType;
    std::string scope;
    std::vector<DataType> *returnParameters;
    std::vector<std::vector<FunctionArgument>*> *otherParameters;
    std::vector<DataType>* inputParameters;

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

    SymbolTableEntry(const std::string& name, std::vector<DataType>* inputParameters, std::vector<std::vector<FunctionArgument>*>* otherParameters, std::vector<DataType>* returnParameters, const std::string& scope, int rowNum, int colNum) {
        this->name = name;
        this->dataType = DataType::Function;
        this->scope = scope;
        this->rowNum = rowNum;
        this->colNum = colNum;

        this->inputParameters = inputParameters;
        this->otherParameters = otherParameters;
        this->returnParameters = returnParameters;

    }

    SymbolTableEntry(std::string& name, std::vector<DataType> *inputParameters, std::vector<std::vector<DataType>*> *otherParameters, std::vector<DataType>* returnParameters, std::string& scope, int rowNum, int colNum) {
        this->name = name;
        this->dataType = DataType::Function;
        this->scope = scope;
        this->rowNum = rowNum;
        this->colNum = colNum;

        std::vector<std::vector<FunctionArgument>*> *oParameters = new std::vector<std::vector<FunctionArgument>*>();
        for (auto &param : *otherParameters) {
            std::vector<FunctionArgument> *fArgs = new std::vector<FunctionArgument>();
            for (auto &type : *param) {
                FunctionArgument fArg(type);
                fArgs->push_back(fArg);
            }
            oParameters->push_back(fArgs);
        }
        this->inputParameters = inputParameters;
        this->otherParameters = oParameters;
        this->returnParameters = returnParameters;
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
    bool insert(std::string&, std::vector<DataType>*,std::vector<std::vector<DataType>*>*,std::vector<DataType>*, std::string, int, int);
    bool insert(std::string&, std::vector<DataType>*,std::vector<std::vector<FunctionArgument>*>*,std::vector<DataType>*, std::string, int, int);
    SymbolTableEntry* search(std::string&, std::string);
    SymbolTableEntry* searchFunction(std::string);
    void print();
};

#endif