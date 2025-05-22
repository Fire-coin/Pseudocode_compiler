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

bool handleDeclare(const string& line, unordered_map<string, string>& dataTypes, const unordered_map<string, int>& keywords, unordered_set<string>& variableNames, const string& outputFile);
bool handleInitializing(const string& line);

// This function supports only single declaration on each line.
bool handleDeclare(const string& line, unordered_map<string, string>& dataTypes, const unordered_map<string, int>& keywords, unordered_set<string>& variableNames, const string& outputFile) {
    vector<string> args = split(line, ':');
    string message;
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




bool handleInitializing(const string& line) {
    vector<string> args = split(line, "<-");
    string message;
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
    
    // vector<string> args = split(line, "<-");
    return true;
}

#endif
#pragma once