# Semantic Analysis

## Introduction
This section of the DSL compiler is the process of analysing the semantics of the code.

## File Format in Tests directory
correct_{test_case_num}.dt is to test the correct codes. \
Similarly wrong_{test_case_num}.dt is to test the wrong code.

## Installations

Make sure you have g++

    $ sudo apt install g++

## Testing

Go to the parser folder and run the following

    $ make \
    $ make test \

This should run the test cases and report their results

To test with debug mode

    $ make debug

To run a particular test file run

    $ make run input.txt

This will print all the flow of the semantic analysis

Run the following to clean the repository with all the files formed:

    $ make clean


Note: Semantic phase completed
