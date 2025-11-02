#include "lexer.hpp"
#include "string_utils.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

std::string showTokenName(TokenName tokenName) {
    switch (tokenName) {
        case TokenName::IDENTIFIER:
        return "IDENTIFIER";
        break;
        case TokenName::KEYWORD:
        return "KEYWORD";
        break;
        case TokenName::OPERATOR:
        return "OPERATOR";
        break;
        case TokenName::SEPARATOR:
        return "SEPARATOR";
        break;
        case TokenName::COMMENT:
        return "COMMENT";
        break;
        case TokenName::LITERAL:
        return "LITERAL";
        break;
        case TokenName::WHITESPACE:
        return "WHITESPACE";
        break;
        default:
        return "__ERROR__";
        break;
    }
}

std::vector<std::string> keywords = {
    "DECLARE",
    "INTEGER",
    "BOOLEAN",
    "STRING", 
    "WHILE",
    "ENDWHILE"
};

extern std::vector<std::pair<TokenName, std::string>> patterns = {
    {TokenName::KEYWORD, "^([A-Z]+)"},
    {TokenName::IDENTIFIER, "^([a-zA-Z][a-zA-Z_0-9]+)"},
    {TokenName::OPERATOR, "^(<--|<=|>=|<>|[+\\-*/<>=])"},
    {TokenName::SEPARATOR, "^(;|:|,)"},
    {TokenName::COMMENT, "^(//.*\n)"},
    {TokenName::LITERAL, "^(\".*\"|\'[ -~]{1}\'|\\d+(.\\d+)?)"},
    {TokenName::WHITESPACE, "^(\\s)"}
};


int getToken(std::string& source, const std::string& pattern, std::string& buffer) {
    absl::string_view input(source);     // create persistent view
    RE2 re(pattern);                     // compile regex once

    if (RE2::Consume(&input, re, &buffer))
        return 0;
    else 
        return -1;
}
/* TODO: Add invalid token */
void getTokens(std::string& source, std::vector<Token>& tokens) {
    while (source != "") {
        for (auto it = patterns.begin(); it != patterns.end(); ++it) {
            std::string buffer;
            int success = getToken(source, it->second, buffer);
            if (success == 0) {
                switch (it->first) {
                    case TokenName::KEYWORD: /* XXX: Not sure about continue inside of switch, maybe it can cause error. */
                        /* Checking wheter it is actually a keyword or only identifier */
                        if (std::find(keywords.begin(), keywords.end(), buffer) == keywords.end()) continue;
                        break;
                    default:
                        break;
                }
                Token token;
                token.tokenName = it->first;
                token.tokenValue = buffer;
                tokens.push_back(token);

                source = source.substr(buffer.size(), source.size() - buffer.size());
            }
        }
        std::cout << std::endl << source << std::endl;
    }
}