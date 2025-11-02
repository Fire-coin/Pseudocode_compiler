#include "lexer.hpp"
#include "string_utils.hpp"
#include <iostream>
#include <fstream>

preset keywords = {};
preset operators = {};
preset separators = {};

extern std::vector<std::pair<TokenName, std::string>> patterns = {
    {TokenName::IDENTIFIER, "^[a-zA-Z][a-zA-Z_0-9]+"},
    {TokenName::KEYWORD, "^[A-Z]+"},
    {TokenName::OPERATOR, "^[+-*/<>=]|->|<=|>=|<>"},
    {TokenName::SEPARATOR, "^(;|:|,)"},
    {TokenName::COMMENT, "^//.*\n"},
    {TokenName::LITERAL, "^\".\"|\\d+(.\\d+)?"}
};

int loadPreset(preset& preset, std::string path) {
    std::ifstream fin(path);
    if (!fin.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        return -1;
    }
    std::string line;
    while (std::getline(fin, line)) {
        std::vector<std::string> pair = split(line, ';');
        preset[pair[0]] = pair[1];
        
    }
    return 0;
}


int loadKeywords() {
    return loadPreset(keywords, "resources/keywords.txt");
}

int loadOperators() {
    return loadPreset(operators, "resources/operators.txt");
}

int loadSeparators() {
    return loadPreset(separators, "resources/separators.txt");
}

int loadAllPresets() {
    return loadKeywords() |
        loadOperators()  |
        loadSeparators();
}

int getToken(std::string& source, const std::string& pattern, std::string& buffer) {
    absl::string_view input(source);     // create persistent view
    RE2 re(pattern);                     // compile regex once

    if (RE2::Consume(&input, re, &buffer))
        return 0;
    else 
        return -1;
}