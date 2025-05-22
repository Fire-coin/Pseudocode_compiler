#ifndef CUSTOM_EXCEPTIONS_HPP
#define CUSTOM_EXCEPTIONS_HPP

#include <string> // for string
#include <exception> // for exception

using std::string, std::exception;

class data_type_exception : public exception {
    private:
        string message;
    
    public:
        data_type_exception(const char * msg) : message(msg) {};
        const char * what() const noexcept override {
            return message.c_str();
        }
};

class invalid_identifier_name : public exception {
    private:
        string message;
    
    public:
        invalid_identifier_name(const char * msg) : message(msg) {};
        const char * what() const noexcept override {
            return message.c_str();
        }
};
#endif
#pragma once