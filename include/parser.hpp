#ifndef PSCD_TRANSPILER_PARSER_HPP
#define PSCD_TRANSPILER_PARSER_HPP

#include <iostream>
#include "lexer.hpp"

enum Symbol {not_token= -1, ident, plus, minus, star, slash, lparen, rparen, num_lit, eq, neq, less, great, lesseq, greateq, notsym, orsym, andsym};


struct Parser {
    std::vector<Token> tokens;
    long long index;

    Parser(std::vector<Token> arr);
    void nextSym();

    int accept(Symbol);
    int expect(Symbol);
    void factor();
    void term();
    void expr();

    void condition();
    void bool_expr();
};

Symbol lexerToParser(Token token);

int accept(Symbol s);
int expect(Symbol s);

void factor();
void term();
void expr();

#endif
#pragma once