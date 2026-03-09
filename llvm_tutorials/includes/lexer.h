#ifndef LEXER_H
#define LEXER_H

#include<string>
#include <cctype>
#include <cstdlib>
#include <string>
#include <utility>



enum Token {
    tok_eof = -1,
    
    // commands
    tok_def = -2,
    tok_extern = -3,
    
    //primary 
    tok_identifier = -4,
    tok_num = -5,
    
    
};

class lexer{
    // will return [0,255] for unknown values
public:
    Token curTok;
    std::string identifierStr;
    double numVal;
    /*
     * returns either 
     * 1. identifier and keywords token (identifierStr is filled)
     * 2. tok_def 
     * 3. tok_extern
     * 4. numToken, numVal is filled 
     * 5. EOF 
     * 6. the last Char itself (This will be punctuations)
     */
    int getToken(); 
    lexer(std::string input);
        
private:
    std::string input;
    int lastIndex;
    
    int getNextChar();
    void ungetChar();
        
    
    
};


#endif