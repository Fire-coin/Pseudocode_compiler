#include "string_utils.hpp"

std::vector<std::string> split(std::string s, char delimiter) {
    std::vector<std::string> output;
    std::stringstream ss(s);
    std::string buffer;
    while(std::getline(ss, buffer, delimiter)) {
        output.push_back(buffer);
    }
    return output;
}