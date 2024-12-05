# Syntax Analysis
(a.k.a parsing)

## Introduction
This section of the DSL compiler is the process of parsing a string of tokens into a parse tree. We use yacc for this purpose.

## File Format in Tests directory
correct_{test_case_num}.dt is to test the correct codes. \
Similarly wrong_{test_case_num}.dt is to test the wrong code.

## Installations

In order to run this run the following command

    $ sudo apt install bison

## Testing

Go to the parser folder and run the following

    $ make \
    $$ make test

This should run the test cases and report their results

Run the following to clean the repository with all the files formed:

    $ make clean

## References

ANSI C Grammar

Note: Syntax phase completed