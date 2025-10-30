#ifndef PSCD_TRANSPILER_LEXER_HPP
#define PSCD_TRANSPILER_LEXER_HPP

#include <re2/re2.h>
#include <unordered_map>
#include <string>

enum TokenName {IDENTIFIER, KEYWORD, OPERATOR, SEPARATOR, COMMENT, LITERAL};

/* Preset maps a string pattern found in input, to the value which will be passed to the parser. 
   E.G DECLARE is mapped to DECLARE, but * is mapped to MUL.
   These values will be stored in text files and loaded during runtime. */
using preset = std::unordered_map<std::string, std::string>;

extern std::unordered_map<TokenName, RE2> patterns;

extern preset keywords;
extern preset operators;
extern preset separators;

int loadPreset(preset& preset, std::string path);

int loadKeywords(preset& keywords);
int loadOperators(preset& operators);
int loadSeparators(preset& separators);

int loadAllPresets(preset& keywords, preset& operators, preset& separators);

#endif
#pragma once