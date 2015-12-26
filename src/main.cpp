#include "tokenizer.h"
#include "lexer.h"
//#include "parser.h"

using namespace std;

int main(int argc, char const *argv[]) {
    Lexer lexer = Lexer(argv[1]);
    lexer.print();
/*    Tokenizer tokenizer = Tokenizer(argv[2]);
    Parser parser = Parser(lexer, tokenizer);
    parser.begin_parser();*/

    return 0;
}