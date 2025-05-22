#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <stdlib.h> // for std::system
#include <exception> // for std::exception
#include <stdexcept> // for std::invalid_argument
#include <unordered_set>
#include "internal.hpp"
// Globals
std::string outputFile = "output.cpp";
std::unordered_map<std::string, std::string> dataTypes = {{"INTEGER", "int"}, {"REAL", "double"},
                                                    {"CHAR", "char"}, {"STRING", "string"},
                                                    {"BOOLEAN", "bool"}};

std::unordered_map<std::string, int> keywords = {{"DECLARE", 0}};
std::unordered_set<std::string> variableNames;
bool validCode = true;

bool handleExpression(const std::string& line);

// Functions for translation
bool handleDeclare(std::string line);
bool handleInitializing(std::string line);

// Exceptions
class data_type_exception : public std::exception {
    private:
        std::string message;
    
    public:
        data_type_exception(const char * msg) : message(msg) {};
        const char * what() const noexcept override {
            return message.c_str();
        }
};

class invalid_identifier_name : public std::exception {
    private:
        std::string message;
    
    public:
        invalid_identifier_name(const char * msg) : message(msg) {};
        const char * what() const noexcept override {
            return message.c_str();
        }
};

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

    std::string line;
    std::vector<std::string> args;
    unsigned int lineNum = 0;
    while (std::getline(fin, line, '\n')) {
        lineNum++;
        args = split(line, ' ');
        for (auto position = args.begin(); position != args.end(); ++position) {
            if (keywords.find(*position) != keywords.end()) {
                switch (keywords[*position]) {
                    case 0: // DECLARE
                        try {
                            handleDeclare(join(args.begin() + 1, args.end()));
                        } catch (const std::invalid_argument& e) {
                            logError(lineNum, line, e, validCode);
                        } catch (const data_type_exception& e) {
                            logError(lineNum, line, e, validCode);
                        } catch (const invalid_identifier_name& e) {
                            logError(lineNum, line, e, validCode);
                        }
                        break;
                    case 1: // <- (assignment operator)
                        handleInitializing(join(args.begin(), args.end()));
                        break;
                    default:
                        std::cerr << "Invalid keyword: " << *position << ':' 
                        << keywords[*position] << '\n';
                }
                break; // TODO if multiline expressions are allowed it need flag variable
            }
        }
    }
    fin.close();

    // closing main function in output file
    writeToFile("}", outputFile);
    // compiling the output file
    if (validCode) {
        std::string command = "g++ -o" + outputFile.substr(0, outputFile.size() - 3) + ".exe " + outputFile;
        std::system(command.c_str());
    }
    return 0;
}


// This function supports only single declaration on each line.
bool handleDeclare(std::string line) {
    std::vector<std::string> args = split(line, ':');
    std::string message;
    if (args.size() != 2) { // Checks if exactly 2 arguments were supplied for declaration
        message = "There should be 2 arguments for declaration; <identifier> : <data_type>; however " + std::to_string(args.size()) + " were given" + '\n';
        throw std::invalid_argument(message);
    }
    if (dataTypes.find(args[1]) == dataTypes.end()) { // Checks if data type is valid
        message = "Invalid data type: " + args[1] + '\n';
        throw data_type_exception(message.c_str());
    }
    if (!validName(args[0], keywords)) { // Checks if <identifier> name is valid
        message = "Invalid identifier name: " + args[0] + '\n';
        throw invalid_identifier_name(message.c_str());
    }

    message += dataTypes[args[1]] + ' '; // Adding <data_type> of variable to the message
    message += args[0] + ';'; // Addings <identifier> to the message
    variableNames.insert(args[0]); // Adding variable into list of existing variables
    //TODO make splited variable sets for local function variables; priority = low
    writeToFile(message, outputFile);

    return true;
}


bool handleExpression(const std::string& line) {
    //TODO add bracket checks and impement function
    // that returns which type of char was previously.
    // And make checks so no 2 operators will be after each other
    std::string varName = "";

    for (const char& i : line) {
        if ((i >= 97 && i <= 122) || // Checks if character is lower case letter
            (i >= 65 && i <= 90)) { // Checks if character is upper case letter

        } 
    }

    return true;
}


bool handleInitializing(std::string line) {
    std::vector<std::string> args = split(line, "<-");
    std::string message;
    if (args.size() == 1) {
        message = "Expected <expression> or <identifier> after <-\n";
        throw std::invalid_argument(message);
    }
    if (args.size() != 2) {
        message = "Multiple <- (assighments) not supported on one line\n";
        throw std::invalid_argument(message);
    }


    //TODO create handleExpression function to handle possible expressions
    // check is variable name is already declared,
    // TODO implement data type checking inside of handleExpression; priority = low
    
    // std::vector<std::string> args = split(line, "<-");
    return true;
}