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


enum class characters {
    lower, upper, number, underscore, op, bracket
};

bool loadKeywords(const string& filename, unordered_map<string, int>& keywords);
vector<string> split(const string& line, char delimiter);
vector<string> split(const string& line, const string& delimiter);
string join(const vector<string>& args);
string join(const vector<string>::iterator& start, const vector<string>::iterator& end);
void writeToFile(const string& message, const string& outputFile);
bool validName(const string& name, const unordered_map<string, int>& keywords);
void logError(const unsigned int lineNum, const string& line, const exception& e, bool& validCode);
bool handleExpression(const string& line);



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


bool handleExpression(const string& line) {
    //TODO add bracket checks and impement function
    // that returns which type of char was previously.
    // And make checks so no 2 operators will be after each other
    string varName = "";
    for (const char& i : line) {
        if ((i >= 'a' && i <= 'z') || // Checks if character is lower case letter
            (i >= 'A' && i <= 'Z')) { // Checks if character is upper case letter

        }
    }

    return true;
}

#endif
#pragma once