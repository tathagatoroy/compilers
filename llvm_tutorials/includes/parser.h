#ifndef PARSER_H
#define PARSER_H

#include <cstdio>
#include<lexer.h>
#include<ast.h>
#include <memory>

class parser {
private:
    std::string program;
    lexer l;
    int curToken;


public:
    // Takes ownership of the program string and initializes the lexer from it.
    parser(std::string Program) : program(std::move(Program)), l(program) {}

    int getNextToken();
    std::unique_ptr<ExprAST> logError(const char* str) ;
    std::unique_ptr<PrototypeExprAST> LogErrorP(const char* str);
};

#endif