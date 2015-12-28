#ifndef DOC_TOKEN_H
#define DOC_TOKEN_H

#include <string>
#include <iostream>

#include "common.h"

using namespace std;


class DocToken {
public:
    DocToken(string value, int start_pos, int end_pos);

    void print();
    string to_string();

    string value;
    int start_pos;
    int end_pos;
};
#endif