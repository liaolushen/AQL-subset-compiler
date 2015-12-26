#include "code_token.h"

using namespace std;

CodeToken::CodeToken(string type, string value,int line_number) {
    this->type = type;
    this->value = value;
    this->line_number = line_number;
}

void CodeToken::print() {
    cout << type << "->" << value << ":" << line_number << endl;
}