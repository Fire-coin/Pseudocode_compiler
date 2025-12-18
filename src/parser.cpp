#include "parser.hpp"
#include <iostream>

Symbol sym;

Parser::Parser(const std::vector<Token>& arr): tokens(arr), index(0) {}

void Parser::nextSym() {
    if (index < tokens.size())
        sym = lexerToParser(tokens[index++]);
    std::cout << sym << std::endl;
}

Symbol lexerToParser(Token token) {
    switch (token.tokenName)
    {
    case IDENTIFIER:
        return Symbol::ident;
        break;
    case OPERATOR:
        if (token.tokenValue == "+") 
            return Symbol::plus;
        else if (token.tokenValue == "-")
            return Symbol::minus;
        else if (token.tokenValue == "*")
            return Symbol::star;
        else if (token.tokenValue == "/")
            return Symbol::slash;
        else
            return Symbol::not_token;
        break;
    case SEPARATOR:
        if (token.tokenValue == "(")
            return Symbol::lparen;
        else if (token.tokenValue == ")")
            return Symbol::rparen;
        else
            return Symbol::not_token;
        break;
    case LITERAL:
        if (token.tokenValue.find('\'') == token.tokenValue.npos && token.tokenValue.find('"') == token.tokenValue.npos)
            return Symbol::num_lit;
        break;
    default:
        return Symbol::not_token;
        break;
    }
}

int accept(Symbol s);
int expect(Symbol s);

void factor();
void term();
void expr();