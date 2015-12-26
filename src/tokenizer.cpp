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

vector<DocToken> Tokenizer::get_tokens() {
    return doc_tokens;
}

void Tokenizer::generate_tokens() {
    /*get tokens form content and put in doc_tokens*/
    // doc_tokens.push_back(DocToken(content, 0, 5));
}

void Tokenizer::print() {
    for (int i = 0; i < doc_tokens.size(); i++) {
        doc_tokens[i].print();
    }
}