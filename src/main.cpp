#include "lexer.hpp"
#include <iostream>
#include <re2/re2.h>

int main() {
    loadKeywords(keywords);

    std::cout << "Hello from CMake project!" << std::endl;
}