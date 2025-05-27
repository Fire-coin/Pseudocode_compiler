#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <stdlib.h> // for std::system
#include <exception> // for std::exception
#include <stdexcept> // for std::invalid_argument
#include <unordered_set>
#include "internal_functions.hpp"
#include "handling_functions.hpp"
#include "custom_exceptions.hpp"
// Globals
std::string outputFile = "output.cpp";
std::unordered_map<std::string, std::string> dataTypesMap = {{"INTEGER", "int"}, {"REAL", "double"},
                                                    {"CHAR", "char"}, {"STRING", "string"},
                                                    {"BOOLEAN", "bool"}};

std::unordered_map<std::string, int> keywords = {{"DECLARE", 0}};
std::unordered_map<std::string, dataTypes> variables;
bool validCode = true;


int main() {
    // Loading keywords from a file
    bool success = loadKeywords("keywords.txt", keywords);
    if (!success) {
        std::cerr << "Could not load keywords.\n";
        return 0;
    }

    std::string inputFile;
    std::cout << "Enter filename: ";
    std::getline(std::cin, inputFile);

    std::ifstream fin(inputFile);

    if (!fin.is_open()) {
        std::cerr << "Could not open file " << inputFile << '\n';
    }

    // Emptying the output file
    std::ofstream fon(outputFile);
    fon << "";
    fon.close();

    // Adding headers to the output file
    writeToFile("#include <iostream>", outputFile);
    writeToFile("#include <string>", outputFile);

    writeToFile("using namespace std;", outputFile);

    // Beggining of main function in output file
    writeToFile("int main() {", outputFile);

    std::string line; // Current line being processed
    std::vector<std::string> args;
    unsigned int lineNum = 0; // Number of current line bding processed
    while (std::getline(fin, line, '\n')) {
        lineNum++;
        args = split(line, ' '); // TODO remove spliting by space
        for (auto position = args.begin(); position != args.end(); ++position) {
            if (keywords.find(*position) != keywords.end()) {
                switch (keywords[*position]) {
                    case 0: // DECLARE
                        try {
                            handleDeclare(join(args.begin() + 1, args.end()), 
                                          dataTypesMap, keywords, variables, outputFile);
                        } catch (const std::invalid_argument& e) {
                            logError(lineNum, line, e, validCode);
                        } catch (const data_type_exception& e) {
                            logError(lineNum, line, e, validCode);
                        } catch (const invalid_identifier_name& e) {
                            logError(lineNum, line, e, validCode);
                        }
                        break;
                    case 1: // <- (assignment operator)
                        try{
                            handleInitializing(join(args.begin(), args.end()), variables);
                        } catch (const std::invalid_argument& e) {
                            logError(lineNum, line, e, validCode);
                        } catch (const quote_not_closed& e) {
                            logError(lineNum, line, e, validCode);
                        }
                        break;
                    default:
                        std::cerr << "Invalid keyword: " << *position << ':' 
                        << keywords[*position] << '\n';
                }
                break; // TODO if multiline expressions are allowed it need flag variable
            }
        }
    }

    // closing main function in output file
    writeToFile("}", outputFile);
    // compiling the output file
    if (validCode) {
        std::string command = "g++ -o" + outputFile.substr(0, outputFile.size() - 3) + ".exe " + outputFile;
        std::system(command.c_str());
    }
    return 0;
}