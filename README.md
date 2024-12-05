# Compilers
This repository is for course projects. Course info: CS3423 Compilers-II at CSE Department, IITH

Language name: Datum\
Supervisor Dr. Ramakrishna Upadrasta\

Abstract \
This whitepaper presents a domain-specific language(DSL) which is designed for tasks related
to data manipulation and visualization. With the recent developments in machine learning and
data science applications, the importance of data is ever growing and is needed for every model,
may it be a large model made by professionals or a small model made by a beginner. Which
is why there is a need for a language that is intuitive, brief, and user friendly so that the user
can preprocess the data as required before using the same. Therefore our DSL provides these
and more by offering features such as intuitive and chainable syntax, strong semantic analysis,
and pipeline functions, enabling users to create readable, and less verbose data transformation
processes. Possible optimizations of the DSL are lazy evaluation, vectorization, memory-efficient
data slicing, and the ability to easily integrate various data transformation functions. The
language focuses on making the code intuitive and readable by adding simple English-like syntax.
All of the above discussed aspects about the DSL is explained in detail in this whitepaper

## installations 

Refer the README files of the respective phases to run them

## make

Run the following to make the project

    $ make

In order to run all the test cases and get the results and stats run the following command

    $ make test

In order to clean the project run the following

    $ make clean

## Description of the phases

### Lexical Phase

This stage has been completed.\
For further info regarding the same proceed to the lexer folder.

### Syntax Phase

This stage has been completed.\
For further info regarding the same proceed to the parser folder.

### Semantic Phase

This stage has been completed.\
For further info regarding the same proceed to the semantic folder.

### Transpiling Phase

This stage is almost completed except a few bugs which we plan to resolve in the upcoming days.\
We decided to go ahead with transpiling to cpp due to shortage of time and had to stop our codegen with LLVM-IR halfway. To see developments in LLVM-IR refer the next section.\
For further info regarding the transpiling proceed to the Transpiler folder.

### Codegen with LLVM-IR

This stage was stopped halfway due to shortage of time and we went ahead with transpiling the code to cpp.\
To see developments in LLVM-IR refer the codegen folder.\


Note: In the next few days we will be fixing a few bugs of transpiler and adding test cases