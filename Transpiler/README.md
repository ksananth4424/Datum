# Transpiler

The language has been transpiled to C++, due to short notice of deadline and shortage of time

## Installations 

In order to make plots work we have used a library matplotlibcpp.h \
In order to use this follow the followint steps \

    - Download matplotlibcpp.h from the official github repo \

To make this header file work we would have to downgrade the numpy version

    $ pip uninstall numpy \
    $ pip install numpy==1.24.3 \
    $ sudo apt-get install python3-tk

Make sure matplotlib is installed

    $ pip install matplotlib

## Make

make the folder by running the following command

    $ make

To run a particular test file run

    $ make run input.txt

Run the following to clean the repository with all the files formed:

    $ make clean

Note: The transpiling code is almot complete except for a few bugs causing the code to not being generated properly. Due to shortage of time this is being submitted.