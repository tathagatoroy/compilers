#include "lexer.h"
#include <cctype>
#include <cstdlib>
#include <string>
#include <utility>

lexer::lexer(std::string program) : input(std::move(program)) , lastIndex(0) {};

int lexer::getNextChar(){
    if(lastIndex >= input.length()){
        return EOF; // why not tok_eof; also -1
    }
    return input[lastIndex++]; // implicit promotion to int 
}
void lexer::ungetChar(){
    if(lastIndex > 0){
        lastIndex--;
    }
}

int lexer::getToken(){
    char lastChar = getNextChar();
    
    // skip whitespace -
    while(std::isspace(lastChar)){
        lastChar  = getNextChar();
    }
    
    // identifier or keywords
    if(std::isalpha(lastChar)){
        identifierStr = lastChar;
        while(std::isalnum(lastChar = getNextChar())){
            identifierStr += lastChar;
        }
        ungetChar();
    
        // send back last char most likey whitespace as it is not part of the identifier or keyword
        if(identifierStr == "def") return tok_def;
        if(identifierStr == "extern") return tok_extern;
        return tok_identifier;
    }
    // numbers;
    if(std::isdigit(lastChar) || lastChar == '.'){
        std::string numString;
        do {
            numString += lastChar;
            lastChar = getNextChar();
        } while(isdigit(lastChar) || lastChar == '.');
        // put back last char as it is not part of the number
        ungetChar();
        numVal = std::strtod(numString.c_str(), 0);
        return tok_num;
        
    }

    // comments
    if(lastChar == '#'){
        do {
            lastChar = getNextChar();
        } while(lastChar != EOF && lastChar != '\n' && lastChar != '\r');
        
        if(lastChar != EOF) {
            // do this again
            return getToken();
        }
    }
    // EOF
    if(lastChar == EOF){
        return tok_eof;
    }

    // punctuations and operators are left 
    return lastChar;
}