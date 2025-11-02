#include "lexer.hpp"
#include <iostream>
#include <re2/re2.h>
#include <fstream>

int readFile(std::string& output, const std::string& path) {
    std::ifstream fin(path);
    if (!fin.is_open())
        return 1;
    
    std::string line;
    while(std::getline(fin, line))
        output += line + '\n';
    fin.close();

    return 0;
}

int main() {
    std::string source;
    int error = readFile(source, "resources/input.pscd");
    if (error) {
        std::cerr << "Could not open file.\n";
        return -1;
    }
    std::vector<Token> tokens;
    getTokens(source, tokens);
    for (auto i : tokens) {
        std::cout << "Token Name: " << showTokenName(i.tokenName) << "; Token Value: " << i.tokenValue << std::endl;
    }
}