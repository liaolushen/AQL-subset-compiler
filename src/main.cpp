#include "tokenizer.h"
#include "lexer.h"
#include "parser.h"
#include "common.h"

using namespace std;

int main(int argc, char const *argv[]) {
    cout << "Processing " << Common::get_file_name(argv[2]) << endl;
    Lexer lexer = Lexer(argv[1]);
    Tokenizer tokenizer = Tokenizer(argv[2]);
    Parser parser = Parser(lexer, tokenizer);
    parser.begin_parser();

    return 0;
}