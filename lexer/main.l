%{
#include <stdio.h>
#include "y.tab.h"

void count();
%}

D			[0-9]
L			[a-zA-Z_]

%%

"$!"    {}
"$$"    {}   

"integer"   {return (INTEGER);}
"float"     {return (FLOAT);}
"string"    {return (STRING);}
"char"      {return (CHAR);}
"bool"      {return (BOOL);}
"dataset"   {return (DATASET);}
"array"     {return (ARRAY);}

"if"        {return (IF);}
"else"      {return (ELSE);}
"loop"      {return (LOOP);}
"break"     {return (BREAK);}
"continue"  {return (CONTINUE);}
"return"    {return (RETURN);}

"or"        {return (OR);}
"and"       {return (AND);}
"not"       {return (NOT);}

"from"      {return (FROM);}
"to"        {return (TO);}
"step"      {return (STEP);}
"also"      {return (ALSO);}

"row"       {return (ROW);}
"col"       {return (COL);}
"filter"    {return (FILTER);}
"sum"       {return (SUM);}
"max"       {return (MAX);}
"min"       {return (MIN);}
"mean"      {return (MEAN);}
"join"      {return (JOIN);}
"read"      {return (READ);}
"write"     {return (WRITE);}
"unique"    {return (UNIQUE);}
"show"      {return (SHOW);}
"split"     {return (SPLIT);}
"sort"      {return (SORT);}
"shuffle"   {return (SHUFFLE);}
"add"       {return (ADD);}
"shape"     {return (SHAPE);}
"drop"      {return (DROP);}