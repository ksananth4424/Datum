%{
#include <stdio.h>
// #include "y.tab.h"
void count();
%}

D			[0-9]
L			[a-zA-Z_]

%%

"$!"([^!]|(!+[^!$]))*!*"!$" ;
"$$".*\n        ;   

"integer"   {count(); printf("INTEGER");}
"float"     {count(); printf("FLOAT");}
"string"    {count(); printf("STRING");}
"char"      {count(); printf("CHAR");}
"bool"      {count(); printf("BOOL");}
"dataset"   {count(); printf("DATASET");}
"array"     {count(); printf("ARRAY");}

"if"        {count(); printf("IF");}
"else"      {count(); printf("ELSE");}
"loop"      {count(); printf("LOOP");}
"break"     {count(); printf("BREAK");}
"continue"  {count(); printf("CONTINUE");}
"return"    {count(); printf("RETURN");}
"function"  {count(); printf("FUNCTION");}

"or"        {count(); printf("OR");}
"and"       {count(); printf("AND");}
"not"       {count(); printf("NOT");}
"from"      {count(); printf("FROM");}
"to"        {count(); printf("TO");}
"step"      {count(); printf("STEP");}
"also"      {count(); printf("ALSO");}

"row"       {count(); printf("ROW");}
"col"       {count(); printf("COL");}
"filter"    {count(); printf("FILTER");}
"sum"       {count(); printf("SUM");}
"max"       {count(); printf("MAX");}
"min"       {count(); printf("MIN");}
"mean"      {count(); printf("MEAN");}
"join"      {count(); printf("JOIN");}
"read"      {count(); printf("READ");}
"write"     {count(); printf("WRITE");}
"unique"    {count(); printf("UNIQUE");}
"show"      {count(); printf("SHOW");}
"split"     {count(); printf("SPLIT");}
"sort"      {count(); printf("SORT");}
"shuffle"   {count(); printf("SHUFFLE");}
"add"       {count(); printf("ADD");}
"shape"     {count(); printf("SHAPE");}
"drop"      {count(); printf("DROP");}

"show_bar"      {count();printf("SHOW_BAR");}
"show_scatter"  {count();printf("SHOW_SCATTER");}
"show_line"     {count();printf("SHOW_LINE");}
"show_box"      {count(); printf("SHOW_BOX");}
"function_declarations:"    {count(); printf("FUNC_LABEL");}
"start:"                    {count(); printf("START_LABEL");}
{L}({L}|{D})*           {count(); printf("IDENTIFIER");}	
\"(\\.|[^\\"])*\"       {count(); printf("STRING_LITERAL");}
'(\\.|[^\\'])'	        {count(); printf("CONSTANT");}
{D}+                    {count(); printf("CONSTANT");}
{D}*"."{D}+             {count(); printf("CONSTANT");}
{D}+"."{D}*             {count(); printf("CONSTANT");}


"+="        {count(); printf("ADD_ASSIGN");}
"-="        {count(); printf("SUB_ASSIGN");}
"*="        {count(); printf("MUL_ASSIGN");}
"/="        {count(); printf("DIV_ASSIGN");}
"%="        {count(); printf("MOD_ASSIGN");}
"=="        {count(); printf("EQ_OP");}
"="         {count(); printf("=");}
"+"         {count(); printf("+");}
"-"         {count(); printf("-");}
"/"         {count(); printf("/");}
"*"         {count(); printf("*");}
"%"         {count(); printf("%%");}
"++"        {count(); printf("INC_OP");}
"--"        {count(); printf("DEC_OP");}
">"         {count(); printf(">");}
"<"         {count(); printf("<");}
">="        {count(); printf("GE_OP");}
"<="        {count(); printf("LE_OP");}
"!="        {count(); printf("NE_OP");}
"->"        {count(); printf("FLOW");}
";"         {count(); printf(";");}
"("         {count(); printf("(");}
")"         {count(); printf(")");}
"!"         {count(); printf("!");}
","         {count(); printf(",");}
"{"         {count(); printf("{");}
"}"         {count(); printf("}");}
"["         {count(); printf("[");}
"]"         {count(); printf("]");}


[ \t\n\v\f] {ECHO;}
.           {puts("error in lexing"); /* ignore */}

%%

void count() {
    // ECHO;
    printf(" ");
}

int yywrap() {
    return 1;
}

int main(int argc, char** argv) {
	if(argc >= 2){
		yyin = fopen(argv[1] ,"r");
		if(yyin ==NULL){
			printf("File not found\n");
			return 0;
		}
	}
	else{
		puts("error: usage: ./lexer.out <filename>");
		return 0;
	}
	yylex();
    fclose(yyin);
	return 0;
}