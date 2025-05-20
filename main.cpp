#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility> // for std::pair
#include <unordered_map>
#include <stdlib.h> // for std::system
#include <exception> // for std::exception
#include <stdexcept> // for std::invalid_argument
#include <unordered_set>
// Globals
std::string outputFile = "output.cpp";
std::unordered_map<std::string, std::string> dataTypes = {{"INTEGER", "int"}, {"REAL", "double"},
                                                    {"CHAR", "char"}, {"STRING", "string"},
                                                    {"BOOLEAN", "bool"}};

std::unordered_map<std::string, int> keywords = {{"DECLARE", 0}};
std::unordered_set<std::string> variableNames;
bool validCode = true;

// Internally used functions
bool loadKeywords(std::string filename);
std::vector<std::string> split(std::string line, char delimiter);
std::vector<std::string> split(std::string line, std::string delimiter);
std::string join(std::vector<std::string> args);
std::string join(std::vector<std::string>::iterator start, std::vector<std::string>::iterator end);
void writeToFile(std::string message);
bool validName(std::string);
void logError(unsigned int, std::string, const std::exception&);

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
    bool success = loadKeywords("keywords.txt");
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
    writeToFile("#include <iostream>");
    writeToFile("#include <string>");

    writeToFile("using namespace std;");

    // Beggining of main function in output file
    writeToFile("int main() {");

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
                            logError(lineNum, line, e);
                        } catch (const data_type_exception& e) {
                            logError(lineNum, line, e);
                        } catch (const invalid_identifier_name& e) {
                            logError(lineNum, line, e);
                        }
                        break;
                    case 1: // <- (assignment operator)
                        
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
    writeToFile("}");
    // compiling the output file
    if (validCode) {
        std::string command = "g++ -o" + outputFile.substr(0, outputFile.size() - 3) + ".exe " + outputFile;
        std::system(command.c_str());
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


std::vector<std::string> split(std::string line, std::string delimiter) {
    std::vector<std::string> output;
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


void writeToFile(std::string message) {
    std::ofstream fon;
    fon.open(outputFile, std::ios::app);
    if (fon.is_open()) {
        fon << message << '\n';
    }
    fon.close();
}


std::string join(std::vector<std::string> args) {
    std::string output;
    for (std::string s : args) {
        output += s;
    }
    return output;
}


std::string join(std::vector<std::string>::iterator start, std::vector<std::string>::iterator end) {
    std::string output;
    for (auto position = start; position != end; ++position) {
        output += *position;
    }
    return output;
}


bool validName(std::string name) {
    if (keywords.find(name) != keywords.end()) return false;
    // Returns false if first character of name is not a letter
    if ((name[0] > 90 || name[0] < 65) && (name[0] > 122 || name[0] < 97)) return false;

    for (int i = 0; i < name.length(); ++i) {
        if ((name[i] >= 97 && name[i] <= 122) || // Checks if character is lower case letter
            (name[i] >= 65 && name[i] <= 90) || // Checks if character is upper case letter
            (name[i] >= 48 && name[i] <= 57) || // Checks if character is numeric
            (name[i] == 95)) { // Checks if character is underscore
            continue;
        } else return false;
    }
    return true;
}


void logError(unsigned int lineNum, std::string line, const std::exception& e) {
    std::cout << "Error: " << e.what();
    std::cout << "\t" << lineNum << " |\t" << line << '\n';
    validCode = false;
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
    if (!validName(args[0])) { // Checks if <identifier> name is valid
        message = "Invalid identifier name: " + args[0] + '\n';
        throw invalid_identifier_name(message.c_str());
    }

    message += dataTypes[args[1]] + ' '; // Adding <data_type> of variable to the message
    message += args[0] + ';'; // Addings <identifier> to the message
    writeToFile(message);

    return true;
}


bool handleInitializing(std::string line) {
    //TODO create split function that takes sting as delimiter
    //TODO create handleExpression function to handle possible expressions
    //TODO add checks for 2 arguments only after spliting by <-,
    // check is variable name is already declared,
    // TODO implement data type checking inside of handleExpression; priority = low
    
    // std::vector<std::string> args = split(line, "<-");
    return true;
}