#include "tokenizer.h"

using namespace std;
using namespace Common;

Tokenizer::Tokenizer(const char *file_name) {
    ifstream in(file_name);
    if (!in.is_open()) error("Error opening file");
    stringstream buffer;  
    buffer << in.rdbuf();  
    content = buffer.str();

    in.close();
    in.clear();

    generate_tokens();
}

string Tokenizer::get_content() {
    return content;
}

vector<DocToken> Tokenizer::get_doc_tokens(int min, int max) {
    vector<DocToken> result;
    for (int i = 0; i < doc_tokens.size(); i++) {
        for (int n = min - 1; n < max; n++) {
            if (i + n >= doc_tokens.size()) break;
            result.push_back(
                DocToken(content.substr(doc_tokens[i].start_pos, doc_tokens[i + n].end_pos - doc_tokens[i].start_pos),
                    doc_tokens[i].start_pos, doc_tokens[i + n].end_pos));
        }
    }
    return result;
}

void Tokenizer::generate_tokens() {
    int start_pos, end_pos;
    for (start_pos = 0, end_pos = 1; end_pos < content.size(); end_pos++) {
        if ((is_char(content[end_pos]) && is_char(content[end_pos - 1]))
            || (is_symbol(content[end_pos]) && is_symbol(content[end_pos - 1]))) {
            continue;
        } else {
            string result = content.substr(start_pos, end_pos - start_pos);
            if (result != "\t" && result != " " && result != "\n") {
                doc_tokens.push_back(DocToken(result, start_pos, end_pos));
            }
            start_pos = end_pos;
        }
    }
    if (end_pos > start_pos) {
        string result = content.substr(start_pos, end_pos - start_pos);
        if (result != "\t" && result != " " && result != "\n") {
            doc_tokens.push_back(DocToken(result, start_pos, end_pos));
        }
    }

    // print();
}

void Tokenizer::print() {
    for (int i = 0; i < doc_tokens.size(); i++) {
        doc_tokens[i].print();
    }
}