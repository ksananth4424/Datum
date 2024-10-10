# Semantic Analysis

## Introduction
This section of the DSL compiler is the process of analysing the semantics of the code.

## File Format in Tests directory
correct_{test_case_num}.dt is to test the correct codes. \
Similarly wrong_{test_case_num}.dt is to test the wrong code.

## Testing

Go to the parser folder and run the following

    $ make \
    $$ make test

This should run the test cases and report their results

Run the following to clean the repository with all the files formed:

    $ make clean

## References

ANSI C Grammar