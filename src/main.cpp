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
    std::string file = "resources/parser_test.pscd";
    int error = readFile(source, file);
    if (error) {
        std::cerr << "Could not open file: " << file << "\n";
        return -1;
    }
    std::vector<Token> tokens;
    getTokens(source, tokens, 4);
    for (auto i : tokens) {
        std::cout << "Token Name: " << showTokenName(i.tokenName) << "; Token Value: " << i.tokenValue << std::endl;
    }

    Parser parser(tokens);
    parser.parseExpr();
    parser.nextSym();
    std::cout << "\n\n\n";
    parser.parseExpr();
    parser.nextSym();
    std::cout << "\n\n\n";
    parser.parseBoolExpr();
    parser.nextSym();
    std::cout << "\n\n\n\n";
    parser.parseStatement();
    parser.parseStatement();
}
