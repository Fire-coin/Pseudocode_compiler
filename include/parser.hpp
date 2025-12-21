#ifndef PSCD_TRANSPILER_PARSER_HPP
#define PSCD_TRANSPILER_PARSER_HPP

#include <vector>
#include <string>
#include "lexer.hpp"

enum Symbol {not_token= -1, ident, plus, minus, star, slash, lparen, rparen, num_lit, eq, neq, less, great, lesseq, greateq,
             notsym, orsym, andsym, declsym, comma, colon, dtype, eol, assignsym, inputsym, ifsym, eifsym, elsesym, whilesym,
             ewhilesym, thensym, indent, truesym, falsesym};


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

    int statement();
    void block();
};

Symbol lexerToParser(Token token);

int is_dtype(std::string);

#endif
#pragma once