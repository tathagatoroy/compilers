#ifndef PARSER_H
#define PARSER_H

#include <cstdio>
#include<lexer.h>
#include<ast.h>
#include <memory>
#include <map>
#include <sys/stat.h>
class parser {
private:
    std::string program;
    lexer l;     
    int curToken;


public:
    // Takes ownership of the program string and initializes the lexer from it.
    parser(std::string Program) : program(std::move(Program)), l(program) {
        binOpPrecedence['<'] = 10;
        binOpPrecedence['+'] = 20;
        binOpPrecedence['-'] = 20;
        binOpPrecedence['*'] = 40;
    }
    std::map<char, int> binOpPrecedence;
    int getNextToken();
    int getTokenPrecedence();
    std::unique_ptr<ExprAST> logError(const char* str) ;
    std::unique_ptr<PrototypeExprAST> LogErrorP(const char* str);
    std::unique_ptr<ExprAST> parseExpression();
    std::unique_ptr<ExprAST> parsePrimary();
    std::unique_ptr<ExprAST> parseNumberExpr();
    std::unique_ptr<ExprAST> parseParenExpr();
    std::unique_ptr<ExprAST> parseNumbeExpression();
    std::unique_ptr<ExprAST> parseParenthesisExpression();
    std::unique_ptr<ExprAST> parseIdentifierExpr();
    std::unique_ptr<ExprAST> parseBinOpRHS(int exprPrec, std::unique_ptr<ExprAST> LHS);
    std::unique_ptr<PrototypeExprAST> parsePrototype();
    std::unique_ptr<FunctionExprAST> parseDefinition();
    std::unique_ptr<FunctionExprAST> parseTopLevelExpr();
    std::unique_ptr<PrototypeExprAST> parseExtern();
    void parse();
    void handleDefinition();
    void handleExtern();
    void handleTopLevelExpression();
    
};

#endif