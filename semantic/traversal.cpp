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

void traverse(Start *start)
{
    if (start->FunctionList != nullptr)
    {
        cout << "FunctionList\n";
    }
    else
        cout << "No FunctionList\n";
    if (start->StatementList != nullptr)
    {
        cout << "StatementList\n";
        for (auto *statement : *(start->StatementList))
        {
            // Dereference the pointer to vector
            if (statement->statementType == 1)
            {
                cout << "DeclarationStatement\nscope: " << statement->get_scope() << "\n";
            }
            else if (statement->statementType == 2)
            {
                cout << "AssignmentStatement\nscope: " << statement->get_scope() << "\n";
                // cast to AssignmentStatement
                AssignmentStatement *assignmentStatement = statement->assignmentStatement;
                if (assignmentStatement->expression != nullptr)
                {
                    cout << "Expression\n";
                    DataType rhs = traverse_operations(assignmentStatement->expression);
                }
            }
            else if (statement->statementType == 3)
            {
                cout << "ConditionalStatement\nscope: " << statement->get_scope() << "\n";
            }
            else if (statement->statementType == 4)
            {
                cout << "LoopStatement\nscope: " << statement->get_scope() << "\n";
            }
            else if (statement->statementType == 5)
            {
                cout << "ReturnStatement\nscope: " << statement->get_scope() << "\n";
            }
            else if (statement->statementType == 6)
            {
                cout << "BreakStatement\nscope: " << statement->get_scope() << "\n";
            }
            else if (statement->statementType == 7)
            {
                cout << "ContinueStatement\nscope: " << statement->get_scope() << "\n";
            }
            else if (statement->statementType == 8)
            {
                cout << "CompoundStatement\nscope: " << statement->get_scope() << "\n";
            }
        }
    }

    else
        cout << "No StatementList\n";
}

void buildScope(Node *node, string scope)
{
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
        // add all the function declarations to the symbol table
        // symbol table format: name, input parameters, other parameters, return parameters, scope, row number, column number

        cout << func_dec->identifier << endl;
        std::string name = func_dec->identifier;
        // get the input parameters
        std::vector<DataType> *inputParameters = new std::vector<DataType>();
        for (auto *param : *(func_dec->inpParameter))
        {
            // map the type (which is a number)to DataType enum
            DataType dataType = mapTypeToDataType(param->type->type->at(0));
            inputParameters->push_back(dataType);
        }
        // get the other parameters
        std::vector<DataType> *otherParameters = new std::vector<DataType>();
        for (auto *param : *(func_dec->otherParameter))
        {
            // map the type (which is a number)to DataType enum
            DataType dataType;
            switch (param->type->type->at(0))
            {
            case 0:
                dataType = Integer;
                break;
            case 1:
                dataType = Float;
                break;
            case 2:
                dataType = String;
                break;
            case 3:
                dataType = Char;
                break;
            case 4:
                dataType = Boolean;
                break;
            case 5:
                dataType = Dataset;
                break;
            case 6:
                dataType = Array;
                break;
            }
            otherParameters->push_back(dataType);
        }
        // get the return parameters
        std::vector<DataType> *returnParameters = new std::vector<DataType>();
        for (auto *param : *(func_dec->outParameter))
        {
            // map the type (which is a number)to DataType enum
            DataType dataType = mapTypeToDataType(param->type->type->at(0));
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
            for (auto &decl : *(stmt->declarationStatement->initDeclarations))
            {
                cout << decl->declarator->identifier << endl;
                std::string identifier = decl->declarator->identifier;
                // map the type (which is a number)to DataType enum
                DataType dataType = mapTypeToDataType(stmt->declarationStatement->type->type->at(0));
                std::string scope = stmt->declarationStatement->get_scope();
                symtab.insert(identifier, dataType, scope, 0, 0);
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
                // for (auto &from_to : *loop_stmt->fromToPairs) {
                // }

                // add all loop variable init to symbol table
                // for (auto &from_to : *loop_stmt->fromToPairs) {
                cout << loop_stmt->identifier << endl;
                std::string identifier = (loop_stmt)->identifier;
                DataType dataType = Integer;
                std::string scope = loop_stmt->get_scope();
                symtab.insert(identifier, dataType, scope, 0, 0);
                // }

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

DataType traverse_single_chain_expression(SingleChainExpression *singleChainExpression)
{
    char* identifier = singleChainExpression->identifier;
    std::string scope = singleChainExpression->get_scope();
    std::string identifier_str = std::string(identifier);
    SymbolTableEntry* entry = symtab.search(identifier_str, scope);
    DataType currentDataType = entry->dataType;
    //loop over the function calls
    


}

DataType traverse_multi_chain_expression(MultiChainExpression *multiChainExpression)
{
}

DataType traverse_operations(Expression *root)
{
    if (root == nullptr)
        return DataType::Unknown;
    if (root->castType == 1)
    {
        cout << "UnaryExpression\n";
        UnaryExpression *unaryExpression = dynamic_cast<UnaryExpression *>(root);
        DataType datatype = mapTypeToDataType(unaryExpression->constantValue->type->type->at(0));
        if (datatype == Integer)
        {
            return Integer;
        }
        else if (datatype == Float)
        {
            return Float;
        }
        else if (datatype == String)
        {
            for (auto &o : *(unaryExpression->op))
            {
                if (o == minus_op)
                {
                    cout << "Error: Cannot \"-\" perform operation on string\n";
                }
                else if (o == plus_op)
                {
                    cout << "Error: Cannot \"+\" perform operation on string\n";
                }
            }
            return String;
        }
        else if (datatype == Char)
        {
            for (auto &o : *(unaryExpression->op))
            {
                if (o == minus_op)
                {
                    cout << "Error: Cannot \"-\" perform operation on string\n";
                }
                else if (o == plus_op)
                {
                    cout << "Error: Cannot \"+\" perform operation on string\n";
                }
            }
            return Char;
        }
        else if (datatype == Boolean)
        {
            for (auto &o : *(unaryExpression->op))
            {
                if (o == minus_op)
                {
                    cout << "Error: Cannot \"-\" perform operation on string\n";
                }
                else if (o == plus_op)
                {
                    cout << "Error: Cannot \"+\" perform operation on string\n";
                }
            }
            return Boolean;
        }
        else if (datatype == Dataset)
        {
            if (unaryExpression->op->size() > 0)
            {
                cout << "Error: Cannot perform Unary operations on dataset\n";
            }
            return Dataset;
        }
        else if (datatype == Array)
        {
            if (unaryExpression->op->size() > 0)
            {
                cout << "Error: Cannot perform Unary operations on array\n";
            }
            return Array;
        }
    }
    else if (root->castType == 2)
    {
        BinaryExpression *binaryExpression = dynamic_cast<BinaryExpression *>(root);
        DataType lhs = traverse_operations(binaryExpression->lhs);
        DataType rhs = traverse_operations(binaryExpression->rhs);
        if (binaryExpression->op == add_op)
        {
            if (lhs == Integer && rhs == Integer)
            {
                return Integer;
            }
            else if (lhs == Float && rhs == Float)
            {
                return Float;
            }
            else if (lhs == String && rhs == String)
            {
                return String;
            }
            else if (lhs == String && rhs == Char)
            {
                return String;
            }
            else if (lhs == Boolean && rhs == Boolean)
            {
                return Boolean;
            }
            else if (lhs == Integer && rhs == Float)
            {
                return Float;
            }
            else if (lhs == Float && rhs == Integer)
            {
                return Float;
            }
            else if (lhs == Boolean && rhs == Integer)
            {
                return Integer;
            }
            else if (lhs == Integer && rhs == Boolean)
            {
                return Integer;
            }
            else if (lhs == Boolean && rhs == Float)
            {
                return Float;
            }
            else if (lhs == Float && rhs == Boolean)
            {
                return Float;
            }
            else if (lhs == Dataset && rhs == Dataset)
            {
                return Dataset;
            }
            else if (lhs == Array && rhs == Array)
            {
                return Array;
            }
            else
            {
                cout << "Error: Cannot perform \"+\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        }
        else if (binaryExpression->op == sub_op)
        {
            if (lhs == Integer && rhs == Integer)
            {
                return Integer;
            }
            else if (lhs == Float && rhs == Float)
            {
                return Float;
            }
            else
            {
                cout << "Error: Cannot perform \"-\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        }
        else if (binaryExpression->op == mul_op)
        {
            if (lhs == Integer && rhs == Integer)
            {
                return Integer;
            }
            else if (lhs == Float && rhs == Float)
            {
                return Float;
            }
            else
            {
                cout << "Error: Cannot perform \"*\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        }
        else if (binaryExpression->op == div_op)
        {
            if (lhs == Integer && rhs == Integer)
            {
                return Integer;
            }
            else if (lhs == Float && rhs == Float)
            {
                return Float;
            }
            else
            {
                cout << "Error: Cannot perform \"/\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        }
        else if (binaryExpression->op == mod_op)
        {
            if (lhs == Integer && rhs == Integer)
            {
                return Integer;
            }
            else
            {
                cout << "Error: Cannot perform \"%\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        }
        else if (binaryExpression->op == and_op)
        {
            if (lhs == Boolean && rhs == Boolean)
            {
                return Boolean;
            }
            else if (lhs == Integer && rhs == Integer)
            {
                return Integer;
            }
            else
            {
                cout << "Error: Cannot perform \"&&\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        }
        else if (binaryExpression->op == or_op)
        {
            if (lhs == Boolean && rhs == Boolean)
            {
                return Boolean;
            }
            else if (lhs == Integer && rhs == Integer)
            {
                return Integer;
            }
            else
            {
                cout << "Error: Cannot perform \"||\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        }
        else if (binaryExpression->op == eq_op)
        {
            if (lhs == Integer && rhs == Integer)
            {
                return Boolean;
            }
            else if (lhs == Float && rhs == Float)
            {
                return Boolean;
            }
            else if (lhs == String && rhs == String)
            {
                return Boolean;
            }
            else if (lhs == Char && rhs == Char)
            {
                return Boolean;
            }
            else if (lhs == Boolean && rhs == Boolean)
            {
                return Boolean;
            }
            else if (lhs == Dataset && rhs == Dataset)
            {
                return Boolean;
            }
            else if (lhs == Array && rhs == Array)
            {
                return Boolean;
            }
            else
            {
                cout << "Error: Cannot perform \"==\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        }
        else if (binaryExpression->op == neq_op)
        {
            if (lhs == Integer && rhs == Integer)
            {
                return Boolean;
            }
            else if (lhs == Float && rhs == Float)
            {
                return Boolean;
            }
            else if (lhs == String && rhs == String)
            {
                return Boolean;
            }
            else if (lhs == Char && rhs == Char)
            {
                return Boolean;
            }
            else if (lhs == Boolean && rhs == Boolean)
            {
                return Boolean;
            }
            else if (lhs == Dataset && rhs == Dataset)
            {
                return Boolean;
            }
            else if (lhs == Array && rhs == Array)
            {
                return Boolean;
            }
            else
            {
                cout << "Error: Cannot perform \"!=\" operation on " << dataTypeToString(lhs) << " and " << dataTypeToString(rhs) << "\n";
                return Unknown;
            }
        }
    }
    else if (root->castType == 3)
    {
        return traverse_single_chain_expression(dynamic_cast<SingleChainExpression *>(root));
    }
    else if (root->castType == 4)
    {
        return traverse_multi_chain_expression(dynamic_cast<MultiChainExpression *>(root));
    }
    else
    {
        return DataType::Unknown;
    }
}