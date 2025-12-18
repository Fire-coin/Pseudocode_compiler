#ifndef PSCD_TRANSPILER_PARSER_HPP
#define PSCD_TRANSPILER_PARSER_HPP

#include <iostream>
#include "lexer.hpp"

enum Symbol {not_token= -1, ident, plus, minus, star, slash, lparen, rparen, num_lit};

class Parser {
    private:
        const std::vector<Token>& tokens;
        long long index;
    public:
        Parser(const std::vector<Token>&);
        void nextSym();
};

Symbol lexerToParser(Token token);

int accept(Symbol s);
int expect(Symbol s);

void factor();
void term();
void expr();

#endif
#pragma once