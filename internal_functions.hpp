#ifndef INTERNAL_FUNCTIONS_HPP
#define INTERNAL_FUNCTIONS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <fstream>
#include <sstream>
#include <unordered_map>

using std::string, std::vector, std::stringstream,
      std::getline, std::unordered_map, std::exception,
      std::ifstream, std::ofstream, std::cout;


enum class dataTypes {
    INTEGER, REAL, CHAR, STRING, BOOLEAN, EMPTY,
    INVALID, INVALID_INTEGER, INVALID_REAL,
    INVALID_CHAR, INVALID_BOOLEAN, INVALID_STRING,
    INVALID_VARIABLE
};

unordered_map<dataTypes, string> typesToStringMap = {{dataTypes::INTEGER, "INTEGER"}, {dataTypes::REAL, "REAL"},
                                    {dataTypes::CHAR, "CHAR"}, {dataTypes::STRING, "STRING"},
                                    {dataTypes::BOOLEAN, "BOOLEAN"}};

bool loadKeywords(const string& filename, unordered_map<string, int>& keywords);
vector<string> split(const string& line, char delimiter);
vector<string> split(const string& line, const string& delimiter);
string join(const vector<string>& args);
string join(const vector<string>::iterator& start, const vector<string>::iterator& end);
void writeToFile(const string& message, const string& outputFile);
bool validName(const string& name, const unordered_map<string, int>& keywords);
void logError(const unsigned int lineNum, const string& line, const exception& e, bool& validCode);
string toLower(const string& str);
dataTypes getDataType(const string& token, const unordered_map<string, dataTypes>& variables);
vector<string> getTokens(const string& line);



bool loadKeywords(const string& filename, unordered_map<string, int>& keywords) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        return false;
    }
    string buffer;
    vector<string> pairs;
    while (getline(fin, buffer, '\n')) {
        pairs = split(buffer, ':');
        keywords[pairs[0]] = stoi(pairs[1]);
    }
    return true;
}

vector<string> split(const string& line, char delimiter) {
    vector<string> output;
    string buffer;
    stringstream ss(line);
    while (getline(ss, buffer, delimiter)) {
        output.push_back(buffer);
    }

    return output;
}

vector<string> split(const string& line, const string& delimiter) {
    vector<string> output;
    int length = delimiter.length();
    int current = 0;
    int start = 0;
    while (current < line.length() - length) {
        if (line.substr(current, length) == delimiter) {
            output.push_back(line.substr(start, current - start));
            start = current + length;
            current++;
        }
        current++;
    }
    output.push_back(line.substr(start, line.length() - start));
    return output;
}

string join(const vector<string>& args) {
    string output;
    for (string s : args) {
        output += s;
    }
    return output;
}

string join(const vector<string>::iterator& start, const vector<string>::iterator& end) {
    string output;
    for (auto position = start; position != end; ++position) {
        output += *position;
    }
    return output;
}

void writeToFile(const string& message, const string& outputFile) {
    ofstream fon;
    fon.open(outputFile, std::ios::app);
    if (fon.is_open()) {
        fon << message << '\n';
    }
    fon.close();
}

bool validName(const string& name, const unordered_map<string, int>& keywords) {
    if (keywords.find(name) != keywords.end()) return false;
    // Returns false if first character of name is not a letter
    if ((name[0] > 'Z' || name[0] < 'A') && (name[0] > 'z' || name[0] < 'a')) return false;

    for (int i = 0; i < name.length(); ++i) {
        if ((name[i] >= 'a' && name[i] <= 'z') || // Checks if character is lower case letter
            (name[i] >= 'A' && name[i] <= 'Z') || // Checks if character is upper case letter
            (name[i] >= '0' && name[i] <= '9') || // Checks if character is numeric
            (name[i] == '_')) { // Checks if character is underscore
            continue;
        } else return false;
    }
    return true;
}

void logError(const unsigned int lineNum, const string& line, const exception& e, bool& validCode) {
    cout << "Error: " << e.what();
    cout << "\t" << lineNum << " |\t" << line << '\n';
    validCode = false;
}

string toLower(const string& str) {
    string output;
    for (const char& c : str) {
        if (c >= 'A' && c <= 'Z') {
            output += (c + 32);
        } else 
            output += c;
    }
    return output;
}


dataTypes getDataType(const string& token, const unordered_map<string, dataTypes>& variables) {
    string message;
    // Checks if currently processed token is string or character literal
    if (token.find('"') != string::npos || token.find('\'') != string::npos) {
        // Checks if token is fully inside of double quotes
        if (token.find_first_of('"') == 0 && token.find_last_of('"') == token.length() - 1) return dataTypes::STRING;
        // Checks if token is fully inside of single quotes
        else if (token.find_first_of('\'') == 0 && token.find_last_of('\'') == token.length() - 1) {
            if (token.length() > 3) return dataTypes::INVALID_CHAR; // character has lenght of 1 + 2 single quotes
            return dataTypes::CHAR;
        }
        else return dataTypes::INVALID_STRING;
    } else if (token.find('.') != string::npos) { // Checks for possible real token
        // Checks if only 1 dot is present in token
        if (token.find_first_of('.') != token.find_last_of('.')) return dataTypes::INVALID_REAL;
        // Scanning through token to see if it consists of only numbers and dot
        for (const char& c : token) {
            if (c == '.') continue;
            if (c < '0' || c > '9') return dataTypes::INVALID_REAL;
        }
        return dataTypes::REAL;
    } else if (toLower(token) == "true" || toLower(token) == "false") { // Checks for possile boolean token
        if (token == "TRUE" || token == "FALSE") return dataTypes::BOOLEAN; // Returns boolean if case is correct
        return dataTypes::INVALID_BOOLEAN; // Returns invalid boolean when case is mismatched
    } else {
        for (const char& c : token) {
            if (c < '0' || c > '9') { // Checks if c is not a number
                if (variables.find(token) != variables.end()) return variables.at(token);
                return dataTypes::INVALID_VARIABLE; // Returns invalid if token is not a variable name
            }
        }
        return dataTypes::INTEGER;
    }
    return dataTypes::INVALID;
}

vector<string> getTokens(const string& line) {
    string pureLine = ""; // This is a line without any operators and brackets
    string operators = "+-*/()";

    // Deleting any possible operators, to get only tokens
    for (const char& c : line) {
        if (operators.find(c) != string::npos) {
            pureLine += ' ';
        } else {
            pureLine += c;
        }
    }

    return split(pureLine, ' ');
}

#endif
#pragma once