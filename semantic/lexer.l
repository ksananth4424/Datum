%{
#include <stdio.h>
#include "y.tab.h"
#include "traversal.hpp"

int lineno = 1;
int yycolumn = 0;

void count();
%}

D			[0-9]
L			[a-zA-Z_]

%%

"$!"([^!]|(!+[^!$]))*!*"!$" ;
"$$".*        ;   

"integer"   {count(); return (INTEGER);}
"float"     {count(); return (FLOAT);}
"string"    {count(); return (STRING);}
"char"      {count(); return (CHAR);}
"bool"      {count(); return (BOOL);}
"dataset"   {count(); return (DATASET);}
"array"     {count(); return (ARRAY);}

"if"        {count(); return (IF);}
"else"      {count(); return (ELSE);}
"loop"      {count(); return (LOOP);}
"break"     {count(); return (BREAK);}
"continue"  {count(); return (CONTINUE);}
"return"    {count(); return (RETURN);}
"function"  {count(); return (FUNCTION);}

"or"        {count(); return (OR);}
"and"       {count(); return (AND);}
"not"       {count(); return (NOT);}

"from"      {count(); return (FROM);}
"to"        {count(); return (TO);}
"step"      {count(); return (STEP);}
"also"      {count(); return (ALSO);}

"row"       {count(); return (ROW);}
"col"       {count(); return (COL);}
"filter"    {count(); return (FILTER);}
"sum"       {count(); return (SUM);}
"max"       {count(); return (MAX);}
"min"       {count(); return (MIN);}
"mean"      {count(); return (MEAN);}
"join"      {count(); return (JOIN);}
"read"      {count(); return (READ);}
"write"     {count(); return (WRITE);}
"unique"    {count(); return (UNIQUE);}
"show"      {count(); return (SHOW);}
"split"     {count(); return (SPLIT);}
"sort"      {count(); return (SORT);}
"shuffle"   {count(); return (SHUFFLE);}
"add"       {count(); return (ADD);}
"shape"     {count(); return (SHAPE);}
"drop"      {count(); return (DROP);}

"show_bar"      {count(); return (SHOW_BAR);}
"show_scatter"  {count(); return (SHOW_SCATTER);}
"show_line"     {count(); return (SHOW_LINE);}
"show_box"      {count(); return (SHOW_BOX);}

"function_declarations:"    {count(); return (FUNC_LABEL);}
"start:"                    {count(); return (START_LABEL);}

"true"                  {count();  yylval.bval = 1; return (BOOL_TRUE);}
"false"                 {count();  yylval.bval = 0; return (BOOL_FALSE);}
{L}({L}|{D})*           {count();  yylval.sval = strdup(yytext); return (IDENTIFIER);}
\"(\\.|[^\\"])*\"       {count();  yylval.strval = strdup(yytext); return (STRING_LITERAL);}
'(\\.|[^\\'])'	        {count();  yylval.cval = yytext[0]; return (CONST_CHAR);}
{D}+                    {count();  yylval.ival = atoi(yytext); return (CONST_INTEGER);}
{D}*"."{D}+             {count();  yylval.fval = atof(yytext); return (CONST_FLOAT);}
{D}+"."{D}*             {count();  yylval.fval = atof(yytext); return (CONST_FLOAT);}

"+="        {count(); return (ADD_ASSIGN);}
"-="        {count(); return (SUB_ASSIGN);}
"*="        {count(); return (MUL_ASSIGN);}
"/="        {count(); return (DIV_ASSIGN);}
"%="        {count(); return (MOD_ASSIGN);}
"=="        {count(); return (EQ_OP);}
"="         {count(); return ('=');}
"+"         {count(); return ('+');}
"-"         {count(); return ('-');}
"/"         {count(); return ('/');}
"*"         {count(); return ('*');}
"%"         {count(); return ('%');}
">"         {count(); return ('>');}
"<"         {count(); return ('<');}
">="        {count(); return (GE_OP);}
"<="        {count(); return (LE_OP);}
"!="        {count(); return (NE_OP);}
"->"        {count(); return (FLOW);}
";"         {count(); return (';');}
"("         {count(); return ('(');}
")"         {count(); return (')');}
","         {count(); return (',');}
"{"         {count(); return ('{');}
"}"         {count(); return ('}');}
"["         {count(); return ('[');}
"]"         {count(); return (']');}

[\n]        {lineno++; yycolumn = 0;}
[ \t\v\f]   {}
.           {puts("error in lexing"); /* ignore */}

%%

void count() {
    /* ECHO; */
    /* printf("\t"); */
    /* yylval.sval = yytext; */
    // count linenumber and column number
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			yycolumn = 0;
		else if (yytext[i] == '\t')
			yycolumn += 8 - (yycolumn % 8);
		else
			yycolumn++;
}