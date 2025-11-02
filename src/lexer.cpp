#include "lexer.hpp"
#include "string_utils.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

preset keywordss = {};
preset operators = {};
preset separators = {};

std::vector<std::string> keywords = {
    "DECLARE"
};

extern std::vector<std::pair<TokenName, std::string>> patterns = {
    {TokenName::KEYWORD, "^([A-Z]+)"},
    {TokenName::IDENTIFIER, "^([a-zA-Z][a-zA-Z_0-9]+)"},
    {TokenName::OPERATOR, "^([+\\-*/<>=]|->|<=|>=|<>)"},
    {TokenName::SEPARATOR, "^(;|:|,)"},
    {TokenName::COMMENT, "^(//.*\n)"},
    {TokenName::LITERAL, "^(\".\"|\\d+(.\\d+)?)"},
    {TokenName::WHITESPACE, "^(\\s)"}
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
    return loadPreset(keywordss, "resources/keywords.txt");
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

void getTokens(std::string& source, std::vector<Token>& tokens) {
    while (source != "") {
        for (auto it = patterns.begin(); it != patterns.end(); ++it) {
            std::string buffer;
            int success = getToken(source, it->second, buffer);
            std::cout << success << it->second << std::endl;
            if (success == 0) {
                if (it->first == TokenName::KEYWORD) {
                    /* Checking wheter it is actually a keyword or only identifier */
                    if (std::find(keywords.begin(), keywords.end(), buffer) == keywords.end()) continue;
                }
                Token token;
                token.tokenName = it->first;
                token.tokenValue = buffer;
                tokens.push_back(token);

                source = source.substr(buffer.size(), source.size() - buffer.size());
            }
        }
        std::cout << source << std::endl;
    }
}