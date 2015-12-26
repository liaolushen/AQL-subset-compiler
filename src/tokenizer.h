#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "doc_token.h"
#include "common.h"

using namespace std;


class Tokenizer {
public:
    Tokenizer(const char * file_name);

    string get_content();
    vector<DocToken> get_tokens();

    void print();
private:
    string content;
    vector<DocToken> doc_tokens;
    void generate_tokens();
};
#endif