# A Pseudocode Transpiler
### Cambridge International AS & A Level syllabus (9618)
### This program takes a file written in pseudocode and translates it into C++ code.
### Currently only lexer is finished, so no code generation yet.

# Using Transpiler
### Currently the only thing that there is, is a lexer. So you can siply watch how Pseudocode gets converted into tokens, which will be passed to analyzer as next.
### The input file is in resources/input.pscd. After building, you can change it's contents, and run the binary. Or you can change the input file location in main.cpp, however you will need to rebuild, but this time faster than the first time.

# Building
## 1. Installing C++ compiler
### The code for this transpiler is written in C++, so you need one. Any compiler supporting C++17 should be good. If you are using VSCode, the GCC with MinGW works well: https://code.visualstudio.com/docs/cpp/config-mingw.
## 2. Installing CMake
### It can be installed here: https://cmake.org/download/.
### After installing open terminal and run this command: 
#### <code>cmake --version</code>
### you should then see the version of the cmake, if you installed it correctly.
## 3. Cloning the repository
### In terminal write: 
#### <code>git clone https://github.com/Fire-coin/Pseudocode_compiler</code>
### <code>cd Pseudocode_compiler</code>
## 4. Building with CMake
### In the repository folder open terminal and write:
#### <code>cmake -B build</code>
#### <code>cmake --build build</code>
### This may take up to few minutes, to fetch all libraries and compile them.
### Once it is done, the binary will be located in <code>build/Debug</code>
## 5. Done, you can go to the section about how to use it.