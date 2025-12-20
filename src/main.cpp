#include "lexer.hpp"
#include <iostream>
#include <re2/re2.h>
#include <fstream>
#include "parser.hpp"

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
    int error = readFile(source, "resources/parser_test.pscd");
    if (error) {
        std::cerr << "Could not open file.\n";
        return -1;
    }
    std::vector<Token> tokens;
    getTokens(source, tokens, 4);
    for (auto i : tokens) {
        std::cout << "Token Name: " << showTokenName(i.tokenName) << "; Token Value: " << i.tokenValue << std::endl;
    }

    Parser parser(tokens);
    parser.expr();
    parser.nextSym();
    std::cout << "\n\n\n";
    parser.bool_expr();
}