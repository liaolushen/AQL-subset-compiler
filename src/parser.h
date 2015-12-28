#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "code_token.h"
#include "lexer.h"
#include "doc_token.h"
#include "tokenizer.h"
#include "common.h"
#include "regex.h"

using namespace std;


class Parser {
public:
    Parser(Lexer lexer, Tokenizer tokenizer);
    void begin_parser();

private:
    Lexer lexer;
    Tokenizer tokenizer;
    vector<vector<CodeToken> > aql_stmts;
    map<string, map<string, vector<DocToken> > > views;
    vector<CodeToken> stmt;

    void aql_stmt(int point);
    void create_stmt(int point);
    void output_stmt(int point);
    map<string, vector<DocToken> > view_stmt(int point);
    map<string, vector<DocToken> > select_stmt(int point);
    map<string, vector<DocToken> > select_list(int point, map<string, string> from_list_map);
    void select_item(int point, map<string, string> from_list_map, map<string, vector<DocToken> > &pre_result);
    map<string, vector<DocToken> > extract_stmt(int point);
    map<string, string> from_list(int from_list_point);
    map<string, vector<DocToken> > extract_spec(int point, map<string, string> from_list_map);
    map<string, vector<DocToken> > regex_spec(int point);
    map<string, vector<DocToken> > pattern_spec(int point, map<string, string> from_list_map);
    vector<vector<DocToken> > pattern_expr(int point, map<string, string> from_list_map);
    vector<vector<DocToken> > pattern_pkg(int point, map<string, string> from_list_map, vector<vector<DocToken> > pre_result);
    vector<string> name_spec(int point);
    vector<DocToken> atom(int point, map<string, string> from_list_map);
    vector<DocToken> atom_by_range(int point, map<string, string> from_list_map, int min, int max);

    vector<vector<DocToken> > link(vector<vector<DocToken> > pre_result, vector<DocToken> to_link, bool is_group);
    bool is_connect(int start_pos, int end_pos);
    vector<DocToken> find_all_to_token(string reg);
    void view_output(string view_name);
};
#endif