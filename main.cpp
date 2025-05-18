#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility> // for std::pair
#include <unordered_map>
#include <stdlib.h> // for std::system
// Globals
std::string outputFile = "output.cpp";
std::unordered_map<std::string, std::string> dataTypes = {{"INTEGER", "int"}, {"REAL", "double"},
                                                    {"CHAR", "char"}, {"STRING", "string"},
                                                    {"BOOLEAN", "bool"}};

std::unordered_map<std::string, int> keywords = {{"DECLARE", 0}};

// Internally used functions
bool loadKeywords(std::string filename);
std::vector<std::string> split(std::string line, char delimiter);
std::string join(std::vector<std::string> args);
std::string join(std::vector<std::string>::iterator start, std::vector<std::string>::iterator end);
void writeToFile(std::string message);
bool validName(std::string);

// Functions for translation
bool handleDeclare(std::string line);

class data_type_exception : public std::exception {
    private:
        const char* message;
    
    public:
        data_type_exception(const char * msg) : message(msg) {};
        const char * what() {
            return message;
        }
};

class invalid_identifier_name : public std::exception {
    private:
        const char* message;
    
    public:
        invalid_identifier_name(const char * msg) : message(msg) {};
        const char * what() {
            return message;
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
    while (std::getline(fin, line, '\n')) {
        args = split(line, ' ');
        for (auto position = args.begin(); position != args.end(); ++position) {
            if (keywords.find(*position) != keywords.end()) {
                switch (keywords[*position]) {
                    case 0: // DECLARE
                        handleDeclare(join(args.begin() + 1, args.end()));
                        break;
                    default:
                        std::cerr << "Invalid keyword: " << *position << ':' 
                        << keywords[*position] << '\n';
                }
            }
        }
    }
    fin.close();

    // closing main function in output file
    writeToFile("}");
    // compiling the output file
    std::string command = "g++ -o" + outputFile.substr(0, outputFile.size() - 3) + ".exe " + outputFile;
    std::system(command.c_str());
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




// This function supports only single declaration on each line.
bool handleDeclare(std::string line) {
    std::vector<std::string> args = split(line, ':');
    std::string message;
    if (args.size() != 2) { // Checks if exactly 2 arguments were supplied for declaration
        message = "There should be 2 arguments for declaration; <identifier> : <data_type>; however " + std::to_string(args.size()) + " were given";
        throw std::invalid_argument(message);
    }
    if (dataTypes.find(args[1]) == dataTypes.end()) { // Checks if data type is valid
        message = "Invalid data type: " + args[1];
        throw data_type_exception(message.c_str());
    }
    if (!validName(args[0])) { // Checks if <identifier> name is valid
        message = "Invalid identifier name: " + args[0];
        throw invalid_identifier_name(message.c_str());
    }

    message += dataTypes[args[1]] + ' '; // Adding <data_type> of variable to the message
    message += args[0] + ';'; // Addings <identifier> to the message
    writeToFile(message);

    return true;
}