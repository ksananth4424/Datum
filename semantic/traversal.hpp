#pragma once

#include <iostream>
#include "ast.hpp"
#include "symbol_table.hpp"

// Forward declaration of Program class
#include <map>
#include <vector>
#include <string>
#include <algorithm>

extern SymbolTable symtab;

extern char filename[100];

void traverse_declaration(DeclarationStatement* root);

void traverse_function_declaration(FunctionDeclaration* root);

void traverse_if_statement(ConditionalStatement* root,bool isFunction=false,bool isLoop=false);

void traverse_loop_statement(LoopStatement* root,bool isFunction=false);

void traverse_return_statement(Start* root);

DataType traverse_single_chain_expression(SingleChainExpression* singleChainExpression,string scope);

vector<DataType> traverse_multi_chain_expression(MultiChainExpression* multiChainExpression,string scope);

DataType traverse_operations(Expression* root,string scope);

void traverse_assignment(AssignmentStatement* root);

std::string get_line_number(int);

void traverse(Start* start);

void buildScope(Node*, std::string);