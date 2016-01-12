#ifndef _PARSER_H_
#define _PARSER_H_

#include "../csc321.h"
#include <cstdlib>
#include <fstream>
#include <string>

// This is a very simple parsing class which breaks up a text file into tokens.
// You should not need to modify this file.

class Parser {
public:
    Parser(std::string filename) : file(filename.c_str()), token() {}
  
    ~Parser() {}
  
    bool nextToken() {
        // returns false when end of file is reached
        return !(file >> token);
    }
  
    std::string getToken() {
        // return the token we just read
        return token;
    }
  
    double getValue() {
        // return the token we just read interpreted as a floating point value
        return atof(token.c_str());
    }
  
private:
    std::ifstream file;
    std::string token;
};

#endif /* _PARSER_H_*/
