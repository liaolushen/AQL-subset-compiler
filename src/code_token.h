#ifndef CODE_TOKEN_H
#define CODE_TOKEN_H

#include <string>
#include <iostream>

using namespace std;


class CodeToken {
public:
    /**
     * The types of code_token:
     * "create", "view", "ID", "as", "output", "as", "select", "from",
     * ".", "extract", "from", "regex", "REG", "on", "return", "group",
     * "NUM", "pattern", "Token", "(", ")", "<", ">", "{", "}", ";"
     */
    CodeToken(string type, string value, int line_number);

    void print();

    string type;
    string value;
    int line_number;
};
#endif