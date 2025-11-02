#include "lexer.hpp"
#include <iostream>
#include <re2/re2.h>

int main() {
    loadKeywords(keywords);
    std::string buffer;
    std::string source = "Hello from CMake project!";
    std::string pattern = "(^[A-Z][a-z]*)";
    int error = getToken(source, pattern, buffer);
    std::cout << "Hello from CMake project!" << std::endl;
}