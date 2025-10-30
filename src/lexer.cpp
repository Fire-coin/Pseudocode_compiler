#include "lexer.hpp"
#include "string_utils.hpp"
#include <iostream>
#include <fstream>

preset keywords = {};
preset operators = {};
preset separators = {};


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


int loadKeywords(preset& keywords) {
    return loadPreset(keywords, "resources/keywords.txt");
}

int loadOperators(preset& operators) {
    return loadPreset(operators, "resources/operators.txt");
}

int loadSeparators(preset& separators) {
    return loadPreset(separators, "resources/separators.txt");
}

int loadAllPresets(preset& keywords, preset& operators, preset& separators) {
    return loadKeywords(keywords) |
        loadOperators(operators)  |
        loadSeparators(separators);
}