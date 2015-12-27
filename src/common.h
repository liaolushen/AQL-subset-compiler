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
    bool is_symbol(char c);
    int string_to_int(string s);
    string get_file_name(char const *full_file_name);
};
#endif