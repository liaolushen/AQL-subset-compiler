#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "common.h"
#include "regex.h"
#include "code_token.h"

using namespace std;

class Lexer {
public:
    Lexer() {}
    Lexer(const char * file_name);
    vector<CodeToken> get_code_tokens();

    void print();
    
private:
    void create_token(int for_point, int to_point, int &line_number);
    string content;
    vector<CodeToken> code_tokens;
};
#endif