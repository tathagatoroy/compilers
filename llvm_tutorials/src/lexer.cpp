#include<string>
#include "lexer.h"

lexer::lexer(std::string program) : input(std::move(program)) , lastIndex(0) {}; // move more efficient than copy for large source text
int lexer::getNextChar(){
    if(lastIndex >= input.length()){
        return EOF; // why not tok_eof;
    }
    return input[lastIndex];
}
void lexer::ungetChar(){
    if(lastIndex > 0){
        lastIndex--;
    }
}

