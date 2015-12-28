#include "doc_token.h"

using namespace std;
using namespace Common;

DocToken::DocToken(string value, int start_pos, int end_pos) {
    this->value = value;
    this->start_pos = start_pos;
    this->end_pos = end_pos;
}

void DocToken::print() {
    cout << value << "(" << start_pos << "," << end_pos << ")" << endl;
}

string DocToken::to_string() {
    return string_format("%s:(%d,%d)", value.c_str(), start_pos, end_pos);
}