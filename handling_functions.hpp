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
dataTypes handleExpression(const vector<string>& tokens, const unordered_map<string, dataTypes>& variables);
bool handleInitializing(const string& line, const unordered_map<string, dataTypes>& variables);
void throwDataTypeException(unordered_map<dataTypes, string> typesToStringMap, const dataTypes& dataType1, const dataTypes& dataType2);

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


void throwDataTypeException(unordered_map<dataTypes, string> typesToStringMap, const dataTypes& dataType1, const dataTypes& dataType2) {
    string message;
    message = "Mismatched data types of " + 
        typesToStringMap.at(dataType1) + " and "
        + typesToStringMap.at(dataType2) + '\n';
    throw data_type_exception(message.c_str());
}


dataTypes handleExpression(const vector<string>& tokens, const unordered_map<string, dataTypes>& variables) {
    //TODO add bracket checks
    //TODO add recognision of functions such as DIV, MOD, SUBSTRING, LENGTH, etc.
    
    string message;
    dataTypes currentDataType = dataTypes::EMPTY;
    dataTypes tokenDataType = dataTypes::EMPTY;

    for (const string& token : tokens) {
        tokenDataType = getDataType(token, variables);
        if (tokenDataType == currentDataType && tokenDataType != dataTypes::CHAR) continue;

        // Checking for all possible error types token can have
        switch(tokenDataType) {
            case dataTypes::INVALID_BOOLEAN:
                message = "BOOLEAN has to be capitalized\n"; // When boolean is not writen all in capitals
                break;
            case dataTypes::INVALID_CHAR:
                message = "CHAR has to be of length 1\n"; // When single quotes are used for string
                break;
            case dataTypes::INVALID_REAL:
                message = "Invalid initialization of REAL\n"; // When there are either more than 1 dot or non numeric characters
                break;
            case dataTypes::INVALID_STRING:
                message = "Quotes not closed\n";
                break;
            case dataTypes::INVALID_VARIABLE:
                message = "Variable \"" + token + "\" not initialized\n";
                throw uninitialized_variable(message.c_str());
            case dataTypes::INVALID:
                message = "Unknown data type\n";
                break;
        }
        if (message != "") throw data_type_exception(message.c_str());

        if (currentDataType == dataTypes::EMPTY) {
            currentDataType = tokenDataType;
            continue;
        }

        switch(tokenDataType) {
            case dataTypes::CHAR:
                if (currentDataType != dataTypes::STRING && currentDataType != dataTypes::CHAR) {
                    throwDataTypeException(typesToStringMap, currentDataType, tokenDataType);
                }
                currentDataType = dataTypes::STRING;
                break;    
            case dataTypes::STRING:
                if (currentDataType != dataTypes::STRING && currentDataType != dataTypes::CHAR) {
                    throwDataTypeException(typesToStringMap, currentDataType, tokenDataType);
                }
                currentDataType = dataTypes::STRING;
                break;
            case dataTypes::INTEGER:
                if (currentDataType == dataTypes::REAL) continue;
                throwDataTypeException(typesToStringMap, currentDataType, tokenDataType);
            case dataTypes::REAL:
                if (currentDataType == dataTypes::INTEGER) currentDataType = dataTypes::REAL;
                throwDataTypeException(typesToStringMap, currentDataType, tokenDataType);
            case dataTypes::BOOLEAN:
                throwDataTypeException(typesToStringMap, currentDataType, tokenDataType);
        }
    }
    return currentDataType;
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
    vector<string> tokens = getTokens(args[1]);
    dataTypes lineDataType = handleExpression(tokens, variables);
    // std::cout << line << "; Line data type: " << int(lineDataType) << '\n';
    
    return true;
}

#endif
#pragma once