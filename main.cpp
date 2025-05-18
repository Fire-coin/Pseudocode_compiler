#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility> // for std::pair
#include <unordered_map>


bool loadKeywords(std::string filename);

std::vector<std::string> split(std::string line, char delimiter);
bool handleDeclare(std::string line);
std::string join(std::vector<std::string> args);

std::pair<std::string, std::string> dataTypes[5] = {{"INTEGER", "int"}, {"REAL", "double"},
                                                    {"CHAR", "char"}, {"STRING", "string"},
                                                    {"BOOLEAN", "bool"}};

std::unordered_map<std::string, int> keywords = {{"DECLARE", 0}};

int main() {
    // Loading kaywords from a file
    bool success = loadKeywords("keywords.txt");
    if (!success) {
        std::cerr << "Could not load keywords.\n";
        return 0;
    }

    std::string inputFile;
    std::string outputFile = "output.cpp";
    std::cout << "Enter filename: ";
    std::getline(std::cin, inputFile);

    std::ifstream fin(inputFile);

    if (!fin.is_open()) {
        std::cerr << "Could not open file " << inputFile << '\n';
    }

    std::string line;
    std::vector<std::string> args;
    while (std::getline(fin, line, '\n')) {
        args = split(line, ' ');
        for (auto i : args) {
            std::cout << i << ',';
        }
        // std::cout << join(args) << '\n';
        std::cout << '\n';
    }

    return 0;
}


bool loadKeywords(std::string filename) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        return false;
    }
    std::string buffer;
    std::vector<std::string> pairs;
    while (std::getline(fin, buffer, '\n')) {
        pairs = split(buffer, ':');
        keywords[pairs[0]] = stoi(pairs[1]);
    }
    return true;
}


std::vector<std::string> split(std::string line, char delimiter) {
    std::vector<std::string> output;
    std::string buffer;
    std::stringstream ss(line);
    while (std::getline(ss, buffer, delimiter)) {
        output.push_back(buffer);
    }

    return output;
}


std::string join(std::vector<std::string> args) {
    std::string output;
    for (std::string s : args) {
        output += s;
    }
    return output;
}


// This function supports only single declaration on each line.
bool handleDeclare(std::string line) {
    std::vector<std::string> args = split(line, ':');
    if (args.size() != 2) {
        throw std::invalid_argument("There should be 2 argumenst for declaration; <identifier> : <data_type>");
    }
    

    return true;
}