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

void traverse_declaration(Start* root);

void traverse_function_declaration(Start* root);

void traverse_if_statement(Start* root);

void traverse_loop_statement(Start* root);

void traverse_return_statement(Start* root);

void traverse_operations(Start* root);

void traverse_assignment(Start* root);

std::string get_line_number(int);

void traverse(Start* start);

void buildScope(Node*, string scope="g");