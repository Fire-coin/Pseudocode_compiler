#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility> // for std::pair


std::vector<std::string> split(std::string line, char delimiter);
std::pair<std::string, std::string> dataTypes[5] = {{"INTEGER", "int"}, {"REAL", "double"},
                                                    {"CHAR", "char"}, {"STRING", "string"},
                                                    {"BOOLEAN", "bool"}};

int main() {
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
        std::cout << '\n';
    }

    return 0;
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