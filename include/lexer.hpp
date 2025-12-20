#ifndef PSCD_TRANSPILER_LEXER_HPP
#define PSCD_TRANSPILER_LEXER_HPP

#include <re2/re2.h>
#include <string>
#include <utility>

/* TODO: Create errors enum, to track them easier. */

enum TokenName {IDENTIFIER, KEYWORD, OPERATOR, SEPARATOR, COMMENT, LITERAL, WHITESPACE};

extern std::vector<std::pair<TokenName, std::string>> patterns;

extern std::vector<std::string> keywords;

struct Token {
   TokenName tokenName;
   std::string tokenValue;
};

/* Scans for pattern in the begging of the source. If it has found a match, it returns it in buffer.
   Else the buffer stays empty. */
int getToken(std::string& source, const std::string& pattern, std::string& buffer);


void getTokens(std::string& source, std::vector<Token>& tokens, int indent);

std::string showTokenName(TokenName tokenName);

#endif
#pragma once