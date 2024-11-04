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

void traverse_if_statement(ConditionalStatement* root);

void traverse_loop_statement(LoopStatement* root);

void traverse_return_statement(Start* root);

DataType traverse_single_chain_expression(SingleChainExpression* singleChainExpression);

vector<DataType> traverse_multi_chain_expression(MultiChainExpression* multiChainExpression);

DataType traverse_operations(Expression* root);

void traverse_assignment(AssignmentStatement* root);

std::string get_line_number(int);

void traverse(Start* start);

void buildScope(Node*, std::string scope="g");