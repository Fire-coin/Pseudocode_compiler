#include "lexer.hpp"
#include <iostream>
#include <re2/re2.h>

int main() {
    loadKeywords();
    std::string buffer;
    std::string source = "Hello from CMake project";
    std::string pattern = "^([a-zA-Z][a-zA-Z_0-9]+)";
    // int error = getToken(source, pattern, buffer);
    std::vector<Token> tokens;
    getTokens(source, tokens);
    for (auto i : tokens) {
        std::cout << "Token Name: " << i.tokenName << " Token Value: " << i.tokenValue << std::endl;
    }
    // std::cout << error << ", " << buffer << std::endl;
    std::cout << "Hello from CMake project!" << std::endl;
}