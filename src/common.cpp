#include "common.h"

#include <stdarg.h>  // For va_start, etc.
#include <memory>    // For unique_ptr

using namespace std;

void Common::error(string error_msg) {
    cout << error_msg << endl;
    exit(1);
}

string Common::string_format(const string fmt_str, ...) {
    int final_n, n = ((int)fmt_str.size()) * 2;
    string str;
    unique_ptr<char[]> formatted;
    va_list ap;
    while(true) {
        formatted.reset(new char[n]);
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    return string(formatted.get());
}

bool Common::is_char(char a) {
    return (a >= 'a' && a <= 'z')||(a >= 'A' && a <= 'Z') || (a >= '0' && a <= '9');
}

bool Common::is_num(string s) {
    for (int i = 0; i < s.size(); i++) {
        if (s[i] < '0' || s[i] > '9') return false;
    }
    return true;
}

bool Common::is_symbol(char a) {
    if (is_char(a)) return false;
    else if (a <= 176 && a >= 40) return true;
    else return false;
}

int Common::string_to_int(string s) {
    int num = 0;
    for (int i = 0; i < s.size(); i++) {
        num = num * 10 + (s[i] - '0');
    }
    return num;
}

string Common::get_file_name(char const *full_file_name) {
    string full_name(full_file_name);
    string name = "";
    for (int i = full_name.size(); i >= 0; i--) {
        if (full_name[i] != '/') name = string_format("%c%s", full_name[i], name.c_str());
        else break;
    }
    return name;
}