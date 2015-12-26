#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <cstring>
#include <iostream>

using namespace std;

namespace Common {
    void error(string error_msg);
    string string_format(const string fmt_str, ...);
    bool is_char(char c);
    bool is_num(string s);
};
#endif