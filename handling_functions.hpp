#ifndef HANDLING_FUNCTIONS_HPP
#define HANDLING_FUNCTIONS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <stdexcept> // for std::invalid_argument
#include "internal_functions.hpp"
#include "custom_exceptions.hpp"

using std::string, std::vector, std::unordered_map, std::unordered_set;

bool handleDeclare(const string& line, unordered_map<string, string>& dataTypesMap, const unordered_map<string, int>& keywords, unordered_map<string, dataTypes>& variables, const string& outputFile);
dataTypes handleExpression(const string& line, const unordered_map<string, dataTypes>& variables);
bool handleInitializing(const string& line, const unordered_map<string, dataTypes>& variables);

// This function supports only single declaration on each line.
bool handleDeclare(const string& line, unordered_map<string, string>& dataTypesMap, const unordered_map<string, int>& keywords, unordered_map<string, dataTypes>& variables, const string& outputFile) {
    vector<string> args = split(line, ':');
    string message;
    if (args.size() != 2) { // Checks if exactly 2 arguments were supplied for declaration
        message = "There should be 2 arguments for declaration; <identifier> : <data_type>; however " + std::to_string(args.size()) + " were given" + '\n';
        throw std::invalid_argument(message);
    }
    if (dataTypesMap.find(args[1]) == dataTypesMap.end()) { // Checks if data type is valid
        message = "Invalid data type: " + args[1] + '\n';
        throw data_type_exception(message.c_str());
    }
    if (!validName(args[0], keywords)) { // Checks if <identifier> name is valid
        message = "Invalid identifier name: " + args[0] + '\n';
        throw invalid_identifier_name(message.c_str());
    }

    message += dataTypesMap[args[1]] + ' '; // Adding <data_type> of variable to the message
    message += args[0] + ';'; // Addings <identifier> to the message
    dataTypes dataType;
    if (args[1] == "INTEGER") {
        dataType = dataTypes::INTEGER;
    } else if (args[1] == "REAL") {
        dataType = dataTypes::REAL;
    } else if (args[1] == "STRING") {
        dataType = dataTypes::STRING;
    } else if (args[1] == "CHAR") {
        dataType = dataTypes::CHAR;
    } else if (args[1] == "BOOLEAN") {
        dataType = dataTypes::BOOLEAN;
    }
    variables[args[0]] = dataType;// Adding variable into list of existing variables
    //TODO make splited variable sets for local function variables; priority = low
    writeToFile(message, outputFile);

    return true;
}


dataTypes handleExpression(const string& line, const unordered_map<string, dataTypes>& variables) {
    //TODO add bracket checks and impement function
    // that returns which type of char was previously.
    // And make checks so no 2 operators will be after each other
    //TODO add recognision of functions such as DIV, MOD, SUBSTRING, LENGTH, etc.
    string message;
    vector<string> args;
    string pureLine = ""; // This is a line wothout any operators and brackets
    string operators = "+-*/()";

    // Deleting any possible operators, to get only
    // identifiers and constants
    for (const char& c : line) {
        if (operators.find(c) != string::npos) {
            pureLine += ' ';
        } else {
            pureLine += c;
        }
    }

    args = split(pureLine, ' ');
    dataTypes currentDataType = dataTypes::INVALID;
    for (const string& str : args) {
        // Checks if currently processed token is string literal
        if (str.find('\"') == 0) { // If token beggins by double quote
            //TODO add test if double quote is inside of single quotes
            // Checks if token is both beggining and ending by double quote
            if (str.find_first_of('\"') != 0 || str.find_last_of('\"') != str.length() - 1) {
                message = "String not closed\n";
                throw quote_not_closed(message.c_str());
            }
            // If currentDataType is either string, char or invalid, it will be assigned string
            // Else it return invalid because it is incompatible with string
            switch (currentDataType) {
                case dataTypes::STRING:
                case dataTypes::CHAR:
                case dataTypes::INVALID:
                    currentDataType = dataTypes::STRING;
                    break;
                default:
                    return dataTypes::INVALID;
                    break;
            }
        // Checks if curently processed argumen is character literal
        } else if (str.find('\'') != string::npos) {
            if (str.find_first_of('\'') == 0 && str.find_last_of('\'') == str.length() - 1) {
                if (currentDataType == dataTypes::CHAR ||
                    currentDataType == dataTypes::STRING) {
                        currentDataType == dataTypes::STRING; // Only valid operation with 2 chars
                        // is appending, so it will result in string
                } else if (currentDataType == dataTypes::INVALID) {
                    currentDataType = dataTypes::CHAR;
                } else {
                        return dataTypes::INVALID;
                }
            }
        }
    }

    return currentDataType; // Currently not functioning data type detection
}


bool handleInitializing(const string& line, const unordered_map<string, dataTypes>& variables) {
    vector<string> args = split(line, "<-");
    string message;
    //TODO Fix this
    if (args.size() == 1) {
        message = "Expected <expression> or <identifier> after <-\n";
        throw std::invalid_argument(message);
    }
    if (args.size() != 2) {
        message = "Multiple <- (assighments) not supported on one line\n";
        throw std::invalid_argument(message);
    }
    dataTypes lineDataType = handleExpression(args[1], variables);


    //TODO create handleExpression function to handle possible expressions
    // check is variable name is already declared,
    // TODO implement data type checking inside of handleExpression; priority = low
    
    return true;
}

#endif
#pragma once