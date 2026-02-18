#include "parser.h"

int parser::getNextToken(){
    return curToken = l.getToken();
}

std::unique_ptr<ExprAST> parser::logError(const char* str) {
    fprintf(stderr, "Error : %s " , str);
    return nullptr;
}

std::unique_ptr<PrototypeExprAST> parser::LogErrorP(const char* str){
    logError(str);
    return nullptr;
}