#include "parser.h"

using namespace std;
using namespace Common;

extern vector<vector<int> > findall(const char *regex, const char *content);

Parser::Parser(Lexer lexer, Tokenizer tokenizer) {
    this->lexer = lexer;
    this->tokenizer = tokenizer;
    vector<CodeToken> code_tokens = lexer.get_code_tokens();
    vector<CodeToken> aql_stmt;
    for (int i = 0; i < code_tokens.size(); i++) {
        aql_stmt.push_back(code_tokens[i]);
        if (code_tokens[i].type == ";") {
            aql_stmts.push_back(aql_stmt);
            aql_stmt = vector<CodeToken>();
        }
    }
}

void Parser::begin_parser() {
    for (int i = 0; i < aql_stmts.size(); ++i) {
        stmt = aql_stmts[i];
        aql_stmt(0);
    }
}

void Parser::aql_stmt(int point) {
    if (stmt[point].type == "create") create_stmt(point+1);
    else if (stmt[point].type == "output") output_stmt(point+1);
    else error("fail in aql_stmt");
}

void Parser::create_stmt(int point) {
    if (stmt[point + 1].type != "ID") error("fail in create_stmt");
    string view_name = stmt[point + 1].value;
    views[view_name] = view_stmt(point + 3);
}

map<string, vector<DocToken> > Parser::view_stmt(int point) {
    if (stmt[point].type == "select") {
        return select_stmt(point);
    } else if (stmt[point].type == "extract") {
        return extract_stmt(point);
    } else {
        error("fail in view_stmt");
    }
}

map<string, vector<DocToken> > Parser::select_stmt(int point) {
    int from_list_point = point;
    while (stmt[from_list_point].type != "from") {
        from_list_point++;
        if (from_list_point == stmt.size()) error("fail in select_stmt");
    }
    map<string, string> from_list_map = from_list(from_list_point);
    return select_list(point + 1, from_list_map);
}

map<string, vector<DocToken> > Parser::select_list(int point, map<string, string> from_list_map) {
    map<string, vector<DocToken> > result;
    select_item(point, from_list_map, result);
    return result;
}

void Parser::select_item(int point, map<string, string> from_list_map, map<string, vector<DocToken> > &result) {
    if (stmt[point].type == "from") return;
    if (stmt[point].type != "ID" || stmt[point + 1].type != "." || stmt[point + 2].type != "ID") {
        error("fail in select_item");
    }
    string view_name = from_list_map[stmt[point].value];
    string group_name = stmt[point + 2].value;
    point += 3;
    string name;
    if (stmt[point].type == "as") {
        name = stmt[point + 1].value;
        point += 2;
    } else {
        name = group_name;
    }
    result[name] = views[view_name][group_name];
    select_item(point, from_list_map, result);
}

map<string, vector<DocToken> > Parser::extract_stmt(int point) {
    int from_list_point = point;
    while (stmt[from_list_point].type != "from") {
        from_list_point++;
        if (from_list_point == stmt.size()) error("fail in extract_stmt");
    }
    map<string, string> from_list_map = from_list(from_list_point);
    return extract_spec(point + 1, from_list_map);
}

map<string, string> Parser::from_list(int point) {
    map<string, string> result;
    if (stmt[point].type != "from") error("fail in from_list");
    while (stmt[point].type != ";" && point < stmt.size()) {
        result[stmt[point + 2].value] = stmt[point + 1].value;
        point += 3;
    }
    return result;
}

map<string, vector<DocToken> > Parser::extract_spec(int point, map<string, string> from_list_map) {
    if (stmt[point].type == "regex") return regex_spec(point + 1);
    else if (stmt[point].type == "pattern") return pattern_spec(point + 1, from_list_map);
    else error("fail in extract_stmt");
}

map<string, vector<DocToken> > Parser::regex_spec(int point) {
    map<string, vector<DocToken> > result;
    if (stmt[point].type != "REG") error("fail in regex_spec");
    string reg = stmt[point].value;
    vector<DocToken> group_value = find_all_to_token(reg);
    int name_spec_point = point;
    while (stmt[name_spec_point].type != "return" && stmt[name_spec_point].type != "as") {
        name_spec_point++;
        if (name_spec_point == stmt.size()) error("fail in regex_spec");
    }
    vector<string> group_name = name_spec(name_spec_point);
    if (group_name.size() != 1) error("group name not equal to group matched!");
    result[group_name[0]] = group_value;
    return result;
}

map<string, vector<DocToken> > Parser::pattern_spec(int point, map<string, string> from_list_map) {
    map<string, vector<DocToken> > result;
    vector<vector<DocToken> > group_value = pattern_expr(point, from_list_map);
    int name_spec_point = point;
    while (stmt[name_spec_point].type != "return" && stmt[name_spec_point].type != "as") {
        name_spec_point++;
        if (name_spec_point == stmt.size()) error("fail in pattern_spec");
    }
    vector<string> group_name = name_spec(name_spec_point);
    if (group_name.size() != group_value.size()) error("group name not equal to group matched!");
    for (int i = 0; i < group_name.size(); i++) {
        result[group_name[i]] = group_value[i];
    }
    return result;
}

vector<vector<DocToken> > Parser::pattern_expr(int point, map<string, string> from_list_map) {
    vector<vector<DocToken> > result;
    return pattern_pkg(point, from_list_map, result);
}

vector<vector<DocToken> > Parser::pattern_pkg(int point, map<string, string> from_list_map, vector<vector<DocToken> > pre_result) {
    bool is_group = false;
    if (stmt[point].type == "as" || stmt[point].type == "return") return pre_result;
    if (stmt[point].type == "(") {
        is_group = true;
        point++;
    }
    vector<DocToken> to_link;
    if (stmt[point].type == "REG") {
        to_link = find_all_to_token(stmt[point].value);
        point++;
    } else if (stmt[point].type == "<") {
        int look_forward = point;
        while (stmt[look_forward].type != ">") look_forward++;
        look_forward++;
        if (stmt[look_forward].type == "{") {
            int min = string_to_int(stmt[look_forward + 1].value);
            int max = string_to_int(stmt[look_forward + 3].value);
            while (stmt[look_forward].type != "}") look_forward++;
            look_forward++;
            to_link = atom_by_range(point, from_list_map, min, max);
        } else {
            to_link = atom(point, from_list_map);
        }
        point = look_forward;
    }
    if (stmt[point].type == ")") point++;
    return pattern_pkg(point, from_list_map, link(pre_result, to_link, is_group));
}

vector<DocToken> Parser::atom(int point, map<string, string> from_list_map) {
    point++;
    string view_name;
    if (stmt[point].type == "ID") {
        view_name = from_list_map[stmt[point].value];
        return views[view_name][stmt[point + 2].value];
    } else if (stmt[point].type == "Token"){
        return tokenizer.get_doc_tokens(1, 1);
    } else {
        error("fail in atom");
    }
}
vector<DocToken> Parser::atom_by_range(int point, map<string, string> from_list_map, int min, int max) {
    return tokenizer.get_doc_tokens(min, max);
}

vector<vector<DocToken> > Parser::link(vector<vector<DocToken> > pre_result, vector<DocToken> to_link, bool is_group) {
    if (pre_result.size() == 0) {
        if (is_group) return vector<vector<DocToken> >(2, to_link);
        else return vector<vector<DocToken> >(1, to_link);
    }
    vector<vector<DocToken> > result(pre_result.size(), vector<DocToken>());
    if (is_group) result.push_back(vector<DocToken>());
    int pre_result_index = 0;
    int to_link_index = 0;
    while (pre_result_index < pre_result[0].size() && to_link_index < to_link.size()) { 
        if (pre_result[0][pre_result_index].end_pos > to_link[to_link_index].start_pos) {
            to_link_index++;
        } else if (pre_result[0][pre_result_index].end_pos <= to_link[to_link_index].start_pos) {
            if (is_connect(pre_result[0][pre_result_index].end_pos, to_link[to_link_index].start_pos)) {
                result[0].push_back(DocToken(tokenizer.get_content().substr(pre_result[0][pre_result_index].start_pos, to_link[to_link_index].end_pos - pre_result[0][pre_result_index].start_pos),
                    pre_result[0][pre_result_index].start_pos, to_link[to_link_index].end_pos));
                for (int i = 1; i < pre_result.size(); i++) result[i].push_back(pre_result[i][pre_result_index]);
                if (is_group) result[pre_result.size()].push_back(to_link[to_link_index]);
                to_link_index++;
            } else {
                pre_result_index++;
                while (pre_result[0][pre_result_index].end_pos <= to_link[to_link_index].start_pos) {
                    if (to_link_index == 0) break;
                    else to_link_index--;
                }
            }
        }
    }
    return result;
}

bool Parser::is_connect(int start_pos, int end_pos) {
    bool connect = true;
    string content = tokenizer.get_content();
    for (int i = start_pos + 1; i < end_pos; i++) {
        if (content[i] != ' ' || content[i] != '\t') connect = false;
    }
    return connect;
}

vector<string> Parser::name_spec(int point) {
    vector<string> result;
    for (int i = point; i < stmt.size(); i++) {
        if (stmt[i].type == "as") result.push_back(stmt[i + 1].value);
    }
    return result;
}

void Parser::output_stmt(int point) {
    string view_name = stmt[point + 1].value;
//    cout << "output_stmt" << endl;
    view_output(view_name);
}

vector<DocToken> Parser::find_all_to_token(string reg) {
    vector<DocToken> tokens;
    vector<vector<int> > result;
    string content = tokenizer.get_content();
    result = findall(reg.c_str(), content.c_str());
    string token_value;
    int token_begin = 0;
    int token_end = 0;
    for (int i = 0; i < result.size(); i++) {
        token_begin = result[i][0];
        token_end = result[i][1];
        token_value = content.substr(token_begin, token_end - token_begin);
        tokens.push_back(DocToken(token_value, token_begin, token_end));
    }
    return tokens;
}

void print_border(vector<int> groups_size) {
    for (int i = 0; i < groups_size.size(); i++) {
        cout << "+";
        for (int j = 0; j < groups_size[i] + 2; j++) cout << "-";
    }
    cout << "+" << endl;
}

void Parser::view_output(string view_name) {
    map<string, vector<DocToken> > groups = views[view_name];
    cout << "View: " << view_name << endl;

    vector<string> key_list;
    for (map<string, vector<DocToken> >::iterator it = groups.begin(); it != groups.end(); ++it) {
        key_list.push_back(it->first);
    }

    int rows = groups[key_list[0]].size();

    vector<int> groups_size;
    for (int i = 0; i < key_list.size(); i++) {
        int group_size = 0;
        for (int j = 0; j < rows; j++) {
            if (groups[key_list[i]][j].get_value().size() > group_size) group_size = groups[key_list[i]][j].get_value().size();
        }
        groups_size.push_back(group_size);
    }

    print_border(groups_size);
    for (int i = 0; i < groups_size.size(); i++) {
        cout << "| ";
        cout << key_list[i];
        for (int j = key_list[i].size() + 1; j < groups_size[i] + 2; j++) cout << " ";
    }
    cout << "|" << endl;

    print_border(groups_size);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < key_list.size(); j++) {
            cout << "| ";
            cout << groups[key_list[j]][i].get_value();
            for (int k = groups[key_list[j]][i].get_value().size() + 1; k < groups_size[j] + 2; k++) cout << " ";
        }
        cout << "|" << endl;
    }
    print_border(groups_size);
    cout << rows << " rows in set" << endl << endl;
}