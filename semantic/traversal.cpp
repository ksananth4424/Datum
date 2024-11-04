#include <iostream>
#include "symbol_table.hpp"
#include "traversal.hpp"
using namespace std;

SymbolTable symtab;

DataType mapTypeToDataType(int type)
{
    switch (type)
    {
    case 0:
        return Integer;
    case 1:
        return Float;
    case 2:
        return String;
    case 3:
        return Char;
    case 4:
        return Boolean;
    case 5:
        return Dataset;
    case 6:
        return Array;
    default:
        throw std::invalid_argument("Unknown type");
    }
}

string dataTypeToString(DataType type)
{
    switch (type)
    {
    case Integer:
        return "Integer";
    case Float:
        return "Float";
    case String:
        return "String";
    case Char:
        return "Char";
    case Boolean:
        return "Boolean";
    case Dataset:
        return "Dataset";
    case Array:
        return "Array";
    default:
        return "Unknown";
    }
}

string mapInbuiltFunctionToString(InbuiltFunctions func){
    switch (func){
    case func_show_bar: return "show_bar";
    case func_show_line: return "show_line";
    case func_show_scatter: return "show_scatter";
    case func_show_box: return "show_box";
    case func_row: return "row";
    case func_col: return "col";
    case func_filter: return "filter";
    case func_sum: return "sum";
    case func_max: return "max";
    case func_min: return "min";
    case func_mean: return "mean";
    case func_join: return "join";
    case func_read: return "read";
    case func_write: return "write";
    case func_unique: return "unique";
    case func_show: return "show";
    case func_split: return "split";
    case func_sort: return "sort";
    case func_shuffle: return "shuffle";
    case func_add: return "add";
    case func_shape: return "shape";
    case func_drop: return "drop";
    case none: return "none";
    default: return "none";
    }
}
void traverse(Start *start)
{
    if (stmt == nullptr) {
        return;
    }
    cout << stmt->statementType << endl;
    switch (stmt->statementType)
    {
    case 1:
        // declaration
        cout << "declaration" << endl;
        traverse_declaration(dynamic_cast<DeclarationStatement *>(stmt->declarationStatement));
        break;
    case 2:
        // assignment
        cout << "assignment" << endl;
        traverse_assignment(dynamic_cast<AssignmentStatement *>(stmt->assignmentStatement));
        break;
    case 5:
        // return
        break;
    case 6:
        // break
        break;
    case 7:
        // continue
        break;

    case 3:
        // conditional
        cout << "conditional" << endl;
        traverse_if_statement(stmt->conditionalStatement);
        break;
    case 4:
        // loop
        cout << "loop" << endl;
        traverse_loop_statement(stmt->loopStatement);
        break;
    default:
        // compound
        {
            cout << "compound" << endl;
            auto comp_stmt = stmt->compoundStatement;
            for (auto &in_stmt : *comp_stmt)
            {
                traverse_statement(dynamic_cast<Statement *>(in_stmt));
            }
            break;
        }
    }
}

void buildScope(Node *node, string scope)
{
    cout << "entering buildscope" << endl;
    if (node == nullptr)
        return;
    int child_scope = 0;
    if (Start *start = dynamic_cast<Start *>(node))
    {

        start->scope = scope;
        for (auto &func_dec : *(start->FunctionList))
        {
            buildScope(func_dec, scope + "." + to_string(child_scope++));
        }
        for (auto &stmt : *(start->StatementList))
        {
            buildScope(stmt, scope + "." + to_string(child_scope++));
        }
    }
    else if (FunctionDeclaration *func_dec = dynamic_cast<FunctionDeclaration *>(node))
    {
        func_dec->scope = scope;
        for (auto &param : *(func_dec->inpParameter))
        {
            buildScope(param, scope + "." + to_string(child_scope++));
        }
        for (auto &param : *(func_dec->otherParameter))
        {
            buildScope(param, scope + "." + to_string(child_scope++));
        }
        for (auto &stmt : *(func_dec->statements))
        {
            buildScope(stmt, scope + "." + to_string(child_scope++));
        }
    }
    else if (Statement *stmt = dynamic_cast<Statement *>(node))
    {
        stmt->scope = scope;
        switch (stmt->statementType)
        {
        case 1:
            // declaration
            stmt->declarationStatement->scope = scope;
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
                ConditionalStatement *cond_stmt = stmt->conditionalStatement;
                cond_stmt->scope;
                for (auto &[expr, stmt_list] : *cond_stmt->ConditionStatements)
                {
                    for (auto &in_stmt : *stmt_list)
                    {
                        buildScope(in_stmt, scope + "." + to_string(child_scope++));
                    }
                }
                break;
            }
        case 4:
            // loop
            {
                LoopStatement *loop_stmt = stmt->loopStatement;
                loop_stmt->scope = scope;
                for (auto &in_stmt : *loop_stmt->statements)
                {
                    buildScope(in_stmt, scope + "." + to_string(child_scope++));
                }
                break;
            }
        default:
            // compound
            {
                auto comp_stmt = stmt->compoundStatement;
                for (auto &in_stmt : *comp_stmt)
                {
                    buildScope(in_stmt, scope + "." + to_string(child_scope++));
                }
                break;
            }
        }
    }
    else
    {
        node->scope = scope;
    }
}

void traverse_declaration(DeclarationStatement *decl_stmt)
{
    if (decl_stmt == nullptr)
        return; 
    if (decl_stmt->initDeclarations != nullptr)
    {
        for (auto *init_decl : *(decl_stmt->initDeclarations))
        {
            if (init_decl->declarator != nullptr)
            {
                std::string name = init_decl->declarator->identifier;
                cout << name << endl;
                std::string scope = decl_stmt->get_scope();
                cout << scope << endl;
                DataType type = mapTypeToDataType(decl_stmt->type->type->at(0));
                SymbolTableEntry *entry = symtab.search(name, scope);
                if (entry != nullptr)
                {
                    std::cout << "Error: Identifier " << name << " in declaration statement is already declared\n";
                    continue;
                }else
                {
                    symtab.insert(name, type, scope, 0, 0);
                }
                //  type checking here. example: int a = 5.1 should give error here
                // if (init_decl->initializer->assignmentExpression != nullptr)
                // {
                //     DataType rhs = traverse_operations(init_decl->initializer->assignmentExpression->expression);
                //     if (type == Integer)
                //     {
                //         if (rhs != Integer && rhs != Boolean)
                //         {
                //             std::cout << "Error: Cannot assign " << rhs << " to integer\n";
                //         }
                //     }
                //     else if (type == Float)
                //     {
                //         if (rhs != Float && rhs != Integer && rhs != Boolean)
                //         {
                //             std::cout << "Error: Cannot assign " << rhs << " to float\n";
                //         }
                //     }
                //     else if (type == Boolean)
                //     {
                //         if (rhs != Boolean && rhs != Integer)
                //         {
                //             std::cout << "Error: Cannot assign " << rhs << " to boolean\n";
                //         }
                //     }
                //     else if (type == String && rhs != String)
                //     {
                //         std::cout << "Error: Cannot assign " << rhs << " to string\n";
                //     }
                //     else if (type == Char && rhs != Char)
                //     {
                //         std::cout << "Error: Cannot assign " << rhs << " to char\n";
                //     }
                //     else if (type == Dataset && rhs != Dataset)
                //     {
                //         std::cout << "Error: Cannot assign " << rhs << " to dataset\n";
                //     }
                //     else if (type == Array && rhs != Array)
                //     {
                //         std::cout << "Error: Cannot assign " << rhs << " to array\n";
                //     }
                // }
                // type checking in case of array

            }
        }
    }
    cout << "exiting declaration" << endl;
}

// function to do semantic checks on the loop statement
void traverse_loop_statement(LoopStatement *loop_stmt)
{
    cout << "entering loop" << endl;
    std::string name = loop_stmt->identifier;
    std::string scope = loop_stmt->scope;
    cout << scope << endl;
    SymbolTableEntry *entry = symtab.search(name, scope);
    if (entry != nullptr)
    {
        std::cout << "Error: Identifier in loop statement is already declared\n";
    }else{
        DataType dataType = Integer;
        symtab.insert(name, dataType, scope, 0, 0);
    }                
    
    // fromtopair check
    if (loop_stmt->fromToPairs != nullptr)
    {
        for (auto &pair : *(loop_stmt->fromToPairs))
        {
            // check if the from and to expressions are of type integer
            if (std::get<0>(pair) != nullptr)
            {
                if (std::get<0>(pair)->castType != 0)
                {
                    std::cout << "Error: From expression in loop statement is not of type integer\n";
                }
            }
            if (std::get<1>(pair) != nullptr)
            {
                if (std::get<1>(pair)->castType != 0)
                {
                    std::cout << "Error: To expression in loop statement is not of type integer\n";
                }
            }
            if (std::get<2>(pair) != nullptr)
            {
                if (std::get<2>(pair)->castType != 0)
                {
                    std::cout << "Error: Step expression in loop statement is not of type integer\n";
                }
            }
        }
    }

    // check if the statements in the loop are valid
    if (loop_stmt->statements != nullptr)
    {
        for (auto &stmt : *(loop_stmt->statements))
        {
            traverse_statement(dynamic_cast<Statement *>(stmt));
        }
    }
    cout << "exiting loop" << endl;
}

void traverse_function_declaration(FunctionDeclaration *func_dec)
{
    if (func_dec == nullptr)
        return;
    
    // add all the function declarations to the symbol table
    // symbol table format: name, input parameters, other parameters, return parameters, scope, row number, column number

    std::cout << func_dec->identifier << std::endl;
    // get the input parameters
    std::vector<DataType> *inputParameters;
    for (auto *param : *(func_dec->inpParameter))
    {
        // map the type (which is a number)to DataType enum
        DataType dataType = mapTypeToDataType(param->type->type->at(0));
        inputParameters->push_back(dataType);
    }
    // get the other parameters
    std::vector<DataType> *otherParameters;
    for (auto *param : *(func_dec->otherParameter))
    {
        // map the type (which is a number)to DataType enum
        DataType dataType = mapTypeToDataType(param->type->type->at(0));
        otherParameters->push_back(dataType);
    }
    // get the return parameters
    std::vector<DataType> *returnParameters;
    for (auto *param : *(func_dec->outParameter))
    {
        // map the type (which is a number)to DataType enum
        DataType dataType = mapTypeToDataType(param->type->type->at(0));
        returnParameters->push_back(dataType);
    }
    if (func_dec->identifier != nullptr)
    {
        std::string name = func_dec->identifier;
        std::string scope = func_dec->get_scope();
        symtab.insert(name, inputParameters, otherParameters, returnParameters, scope, 0, 0);
    }        
   
    for (auto &param : *(func_dec->inpParameter))
    {
        if (param->identifier == nullptr)
        {
            cout << "error: identifier name missing in function declaration input parameter" << endl;
        }
    }
    for (auto &param : *(func_dec->otherParameter))
    {
        if (param->identifier == nullptr)
        {
            cout << "error: identifier name missing in function declaration input parameter" << endl;
        }
    }

    std::cout << func_dec->identifier << std::endl;

    for (auto *param : *(func_dec->outParameter))
    {
        if (param->identifier == nullptr)
        {
            cout << "error: unecessary identifier name in function declaration output parameter" << endl;
        }
    }

    for (auto *stmt : *(func_dec->statements))
    {
        traverse_statement(dynamic_cast<Statement *>(stmt));
    }
}

void traverse_function_call(FunctionCall* functionCall,SymbolTableEntry* entry){

}

DataType traverse_single_chain_expression(SingleChainExpression *singleChainExpression)
{
    char *identifier = singleChainExpression->identifier;
    std::string scope = singleChainExpression->get_scope();
    std::string identifier_str = std::string(identifier);
    SymbolTableEntry *entry = symtab.search(identifier_str, scope);
    if (entry == nullptr){
        cout << "Error: Identifier " << identifier << " not declared\n";
        return Unknown;
    }
    DataType currentDataType = entry->dataType;
    for(auto &funcPair : *(singleChainExpression->functionCallList)){
        FunctionCall *functionCall = funcPair.first;
        std::string functionName = std::string(functionCall->identifier);
        SymbolTableEntry *functionEntry = symtab.search(functionName, scope);
        if(functionEntry==nullptr){
            cout << "Error: Function " << functionName << " not declared\n";
            return Unknown;
        }
        vector<DataType> *inputParameters = functionEntry->inputParameters;
        vector<DataType> *returnParameters = functionEntry->returnParameters;
        traverse_function_call(functionCall,functionEntry);
        if (functionEntry->inputParameters->size()!=1){
            cout << "Error: Function " << functionName << " expects more than one input parameter\n";
        } else if(returnParameters->size()!=1){
            cout << "Error: Function " << functionName << " returns more than one return parameter\n";
        } else {
            DataType functionInputParameter = functionEntry->inputParameters->at(0);
            if (functionInputParameter != currentDataType){
                cout << "Error: Function " << functionName << " input parameter is not of the same type as the identifier\n"<<functionName<<" expects "<<dataTypeToString(functionInputParameter)<<" but input is of type "<<dataTypeToString(currentDataType)<<"\n";
            } 
            currentDataType = returnParameters->at(0);
        }
    }
    return currentDataType;
}

vector<DataType> traverse_function_call_list_multi(vector<pair<FunctionCall *, vector<Expression *>*>> functionCallList,vector<DataType> currentDataType,string scope)
{
    for(auto &funcPair : (functionCallList)){
            FunctionCall *functionCall = funcPair.first;
            std::string functionName = std::string(functionCall->identifier);
            SymbolTableEntry *functionEntry = symtab.search(functionName, scope);
            if(functionEntry==nullptr){
                cout << "Error: Function " << functionName << " not declared\n";
                return currentDataType;
            }
            vector<DataType> *inputParameters = functionEntry->inputParameters;
            vector<DataType> *returnParameters = functionEntry->returnParameters;
            if(inputParameters->size()!=currentDataType.size()){
                cout << "Error: Function " << functionName << " expects more than the provided input parameter\n";

            } else {
                for(int i=0;i<inputParameters->size();i++){
                    if(inputParameters->at(i)!=currentDataType.at(i)){
                        cout << "Error: Function " << functionName << " input parameter is not of the same type as the "<<functionName<<" expects "<<dataTypeToString(inputParameters->at(i))<<" but input is of type "<<dataTypeToString(currentDataType.at(i))<<"\n";
                    }
                }
            }
            currentDataType.clear();
            currentDataType = *returnParameters;
    }
    return currentDataType;
}

//intbuilt functions must be initialized by default in symbol table
vector<DataType> traverse_multi_chain_expression(MultiChainExpression *multiChainExpression)
{
    if(multiChainExpression->functionCall!=nullptr || multiChainExpression->inbuiltFunc!=InbuiltFunctions::none){
        std::string functionCallName;
        if(multiChainExpression->functionCall!=nullptr){
            functionCallName = string(multiChainExpression->functionCall->identifier) ;
        } else {
            functionCallName = string(mapInbuiltFunctionToString(multiChainExpression->inbuiltFunc));
        }
        std::string scope = string(scope);
        SymbolTableEntry *entry = symtab.search(functionCallName,scope);
        if(entry==nullptr){
            cout << "Error: Function " << functionCallName << " not declared\n";
            return vector<DataType>();
        }
        traverse_function_call(multiChainExpression->functionCall,entry);
        return traverse_function_call_list_multi(*multiChainExpression->functionCallList,*entry->returnParameters,scope);
    } else {
        vector<Expression*> *inputList = multiChainExpression->functionCallStart.second;
        vector<DataType> inputTypes;
        for(auto &expr : *inputList){
            DataType exprType = traverse_operations(expr);
            inputTypes.push_back(exprType);
        }
        return traverse_function_call_list_multi(*(multiChainExpression->functionCallList),inputTypes,multiChainExpression->get_scope());
    }
}

DataType traverse_operations(Expression *root){
    if (root == nullptr)
        return DataType::Unknown;
    if (root->castType == 1)
    {
        cout << "UnaryExpression\n";
        UnaryExpression *unaryExpression = dynamic_cast<UnaryExpression *>(root);
        if(unaryExpression->constantValue==nullptr){
            std::string identifier = std::string(unaryExpression->identifier);
            std::string scope = unaryExpression->get_scope();
            SymbolTableEntry *entry = symtab.search(identifier, scope);
            if (entry == nullptr){
                cout << "Error: Identifier " << identifier << " not declared\n";
                return Unknown;
            }
            DataType datatype = entry->dataType;
            if (datatype == Integer) return Integer;
            else if (datatype == Float){
                for(auto &o:*(unaryExpression->op)){
                    if (o==not_op) cout << "Error: Cannot \"!\" perform operation on float\n";
                    return Float;
                    }
            } else if (datatype == String){
                for (auto &o : *(unaryExpression->op)){
                    if (o == minus_op){
                        cout << "Error: Cannot \"-\" perform operation on string\n";
                    }else if (o == plus_op){
                        cout << "Error: Cannot \"+\" perform operation on string\n";
                    }
                }
                return String;
            } else if (datatype == Char){
                for (auto &o : *(unaryExpression->op)){
                    if (o == minus_op){
                        cout << "Error: Cannot \"-\" perform operation on string\n";
                    } else if (o == plus_op){
                        cout << "Error: Cannot \"+\" perform operation on string\n";
                    }
                }
                return Char;
            } else if (datatype == Boolean)
            {
                for (auto &o : *(unaryExpression->op)){
                    if (o == minus_op){
                        cout << "Error: Cannot \"-\" perform operation on string\n";
                    } else if (o == plus_op){
                        cout << "Error: Cannot \"+\" perform operation on string\n";
                    }
                }
                return Boolean;
            } else if (datatype == Dataset){
                if (unaryExpression->op->size() > 0){
                    cout << "Error: Cannot perform Unary operations on dataset\n";
                }
                return Dataset;
            } else if (datatype == Array){
                if (unaryExpression->op->size() > 0){
                    cout << "Error: Cannot perform Unary operations on array\n";
                }
                return Array;
            } else {
                return DataType::Unknown;
            }  
        } else {
        DataType datatype = mapTypeToDataType(unaryExpression->constantValue->type->type->at(0));
        if (datatype == Integer) {
            return Integer;
        } else if (datatype == Float) {
            return Float;
        } else if (datatype == String) {
            for (auto &o : *(unaryExpression->op)){
                if (o == minus_op){
                    cout << "Error: Cannot \"-\" perform operation on string\n";
                }else if (o == plus_op){
                    cout << "Error: Cannot \"+\" perform operation on string\n";
                }
            }
            return String;
        } else if (datatype == Char) {
            for (auto &o : *(unaryExpression->op)){
                if (o == minus_op) {
                    cout << "Error: Cannot \"-\" perform operation on string\n";
                } else if (o == plus_op){
                    cout << "Error: Cannot \"+\" perform operation on string\n";
                }
            }
            return Char;
        } else if (datatype == Boolean){
            for (auto &o : *(unaryExpression->op))
            {
                if (o == minus_op){
                    cout << "Error: Cannot \"-\" perform operation on string\n";
                } else if (o == plus_op){
                    cout << "Error: Cannot \"+\" perform operation on string\n";
                }
            }
            return Boolean;
        } else if (datatype == Dataset){
            if (unaryExpression->op->size() > 0){
                cout << "Error: Cannot perform Unary operations on dataset\n";
            }
            return Dataset;
        } else if (datatype == Array){
            if (unaryExpression->op->size() > 0){
                cout << "Error: Cannot perform Unary operations on array\n";
            }
            return Array;
        }
        }
    } else if (root->castType == 2){
        BinaryExpression *binaryExpression = dynamic_cast<BinaryExpression *>(root);
        DataType lhs = traverse_operations(binaryExpression->lhs);
        DataType rhs = traverse_operations(binaryExpression->rhs);
        if (binaryExpression->op == add_op){
            if (lhs == Integer && rhs == Integer){
                return Integer;
            } else if (lhs == Float && rhs == Float){
                return Float;
            } else if (lhs == String && rhs == String){
                return String;
            } else if (lhs == String && rhs == Char){
                return String;
            } else if (lhs == Boolean && rhs == Boolean){
                return Boolean;
            } else if (lhs == Integer && rhs == Float){
                return Float;
            } else if (lhs == Float && rhs == Integer){
                return Float;
            } else if (lhs == Boolean && rhs == Integer){
                return Integer;
            } else if (lhs == Integer && rhs == Boolean){
                return Integer;
            } else if (lhs == Boolean && rhs == Float){
                return Float;
            } else if (lhs == Float && rhs == Boolean){
                return Float;
            } else if (lhs == Dataset && rhs == Dataset){
                return Dataset;
            } else if (lhs == Array && rhs == Array){
                return Array;
            } else {
                cout << "Error: Cannot perform \"+\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        }
        else if (binaryExpression->op == sub_op)
        {
            if (lhs == Integer && rhs == Integer){
                return Integer;
            } else if (lhs == Float && rhs == Float){
                return Float;
            } else {
                cout << "Error: Cannot perform \"-\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        } else if (binaryExpression->op == mul_op){
            if (lhs == Integer && rhs == Integer){
                return Integer;
            } else if (lhs == Float && rhs == Float){
                return Float;
            } else{
                cout << "Error: Cannot perform \"*\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        } else if (binaryExpression->op == div_op){
            if (lhs == Integer && rhs == Integer){
                return Integer;
            } else if (lhs == Float && rhs == Float){
                return Float;
            } else{
                cout << "Error: Cannot perform \"/\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        } else if (binaryExpression->op == mod_op){
            if (lhs == Integer && rhs == Integer){
                return Integer;
            } else {
                cout << "Error: Cannot perform \"%\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        } else if (binaryExpression->op == and_op){
            if (lhs == Boolean && rhs == Boolean){
                return Boolean;
            } else if (lhs == Integer && rhs == Integer){
                return Integer;
            } else {
                cout << "Error: Cannot perform \"&&\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        } else if (binaryExpression->op == or_op){
            if (lhs == Boolean && rhs == Boolean){
                return Boolean;
            } else if (lhs == Integer && rhs == Integer){
                return Integer;
            } else{
                cout << "Error: Cannot perform \"||\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        } else if (binaryExpression->op == eq_op){
            if (lhs == Integer && rhs == Integer){
                return Boolean;
            } else if (lhs == Float && rhs == Float){
                return Boolean;
            } else if (lhs == String && rhs == String){
                return Boolean;
            } else if (lhs == Char && rhs == Char){
                return Boolean;
            } else if (lhs == Boolean && rhs == Boolean){
                return Boolean;
            } else if (lhs == Dataset && rhs == Dataset){
                return Boolean;
            } else if (lhs == Array && rhs == Array){
                return Boolean;
            } else{
                cout << "Error: Cannot perform \"==\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        } else if (binaryExpression->op == neq_op){
            if (lhs == Integer && rhs == Integer){
                return Boolean;
            } else if (lhs == Float && rhs == Float){
                return Boolean;
            } else if (lhs == String && rhs == String){
                return Boolean;
            } else if (lhs == Char && rhs == Char){
                return Boolean;
            } else if (lhs == Boolean && rhs == Boolean){
                return Boolean;
            } else if (lhs == Dataset && rhs == Dataset){
                return Boolean;
            } else if (lhs == Array && rhs == Array){
                return Boolean;
            } else{
                cout << "Error: Cannot perform \"!=\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        }
    } else if (root->castType == 3){
        return traverse_single_chain_expression(dynamic_cast<SingleChainExpression *>(root));
    }
    else if (root->castType == 4)
    {
        vector<DataType> v = traverse_multi_chain_expression(dynamic_cast<MultiChainExpression *>(root));
        return Dataset;
    }
    else
    {
        return DataType::Unknown;
    }
}

void traverse_assignment(AssignmentStatement *assignmentStatement)
{
    if (assignmentStatement->declarator == nullptr)
    {
        cout << "Error: Declarator is missing in assignment statement\n";
        return;
    }
    if (assignmentStatement->expression == nullptr)
    {
        cout << "Error: Expression is missing in assignment statement\n";
        return;
    }
    DataType lhs = traverse_single_chain_expression(assignmentStatement->declarator);
    DataType rhs = traverse_operations(assignmentStatement->expression);
    if (lhs != rhs)
    {
        cout << "Error: Assignment is invalid\n";
    }
}

void traverse_if_statement(ConditionalStatement *cond_stmt)
{
    cout << "entering if" << endl;
    if (cond_stmt->ConditionStatements != nullptr)
    {
        for (auto &[expr, stmt_list] : *(cond_stmt->ConditionStatements))
        {
            // check if the expression is of type boolean
            if (expr != nullptr)
            {
                DataType temp = traverse_operations(expr);
                if (temp != Boolean || temp != Integer)
                {
                    std::cout << "Error: Expression in conditional statement is not of type boolean\n";
                }
            }
            
            for (auto &in_stmt : *stmt_list)
            {
                // do the same for the statements in the conditional statement
                traverse_statement(dynamic_cast<Statement *>(in_stmt));
            }
        }
    }
    cout << "exiting if" << endl;
}

void traverse(Start *start)
{
    if (start == nullptr)
        return;
    buildScope(start, "g");
    for (auto &func_dec : *(start->FunctionList))
    {
        traverse_function_declaration(dynamic_cast<FunctionDeclaration *>(func_dec));
    }
    for (auto &stmt : *(start->StatementList))
    {
        traverse_statement(dynamic_cast<Statement *>(stmt));
    }
    cout << "exiting traverse" << endl;
}