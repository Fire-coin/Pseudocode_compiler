#ifndef PSCD_TRANSPILER_LEXER_HPP
#define PSCD_TRANSPILER_LEXER_HPP

#include <re2/re2.h>
#include <unordered_map>
#include <string>
#include <utility>

/* TODO: Create errors enum, to track them easier. */

enum TokenName {IDENTIFIER, KEYWORD, OPERATOR, SEPARATOR, COMMENT, LITERAL, WHITESPACE};

/* Preset maps a string pattern found in input, to the value which will be passed to the parser. 
   E.G DECLARE is mapped to DECLARE, but * is mapped to MUL.
   These values will be stored in text files and loaded during runtime. */
using preset = std::unordered_map<std::string, std::string>;

extern std::vector<std::pair<TokenName, std::string>> patterns;

extern preset keywordss;
extern std::vector<std::string> keywords;
extern preset operators;
extern preset separators;

struct Token {
   TokenName tokenName;
   std::string tokenValue;
};

int loadPreset(preset& preset, std::string path);

int loadKeywords();
int loadOperators();
int loadSeparators();

int loadAllPresets();

/* Scans for pattern in the begging of the source. If it has found a match, it returns it in buffer.
   Else the buffer stays empty. */
int getToken(std::string& source, const std::string& pattern, std::string& buffer);


void getTokens(std::string& source, std::vector<Token>& tokens);

#endif
#pragma once