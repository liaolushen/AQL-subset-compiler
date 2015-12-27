#include "lexer.h"

using namespace std;
using namespace Common;

Lexer::Lexer(const char *file_name) {
    ifstream in(file_name);
    if (!in.is_open()) error("Error opening file");
    stringstream buffer;  
    buffer << in.rdbuf();  
    content = buffer.str();

    in.close();
    in.clear();

    int for_point, to_point;
    int line_number = 1;
    for (for_point = 0, to_point = 1; to_point < content.size(); to_point++) {
        if (is_char(content[to_point]) && is_char(content[to_point - 1])) {
            continue;
        } else {
            create_token(for_point, to_point, line_number);
            for_point = to_point;
            if (content[to_point] == '/') {
                while (content[++to_point] != '/');
                to_point++;
                create_token(for_point, to_point, line_number);
                for_point = to_point;
            }
        }
    }
}

void Lexer::create_token(int for_point, int to_point, int &line_number) {
    string result = content.substr(for_point, to_point - for_point);
    if (result == "\t" || result == " ") return;
    if (result == "\n") {
        line_number++;
    } else if (result == "create" || result == "view" || result == "as" || result == "output"
        || result == "from" || result == "." || result == "extract" || result == "from" || result == "select"
        || result == "regex" || result == "on" || result == "return" || result == "group"
        || result == "pattern" || result == "Token" || result == "(" || result == ")" || result == "<"
        || result == ">"|| result == "{"|| result == "}" || result ==  ";" || result == ",") {
        code_tokens.push_back(CodeToken(result, result, line_number));
    } else if (result[0] == '/' && result[result.size() - 1] == '/') {
        code_tokens.push_back(CodeToken("REG", result.substr(1, result.size() - 2), line_number));
    } else if (is_num(result)) {
        code_tokens.push_back(CodeToken("NUM", result, line_number));
    } else {
        code_tokens.push_back(CodeToken("ID", result, line_number));
    }
}

vector<CodeToken> Lexer::get_code_tokens() {
    return code_tokens;
}


void Lexer::print() {
    for (int i = 0; i < code_tokens.size(); i++) {
        code_tokens[i].print();
    }
}