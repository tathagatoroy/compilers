#include "parser.h"
#include "ast.h"
#include "lexer.h"
#include <complex>
#include <cstdio>
#include <execution>
#include <memory>
#include <string>
#include <vector>

/*
 * returns the next token from from lexer
 */
int parser::getNextToken(){
    return curToken = l.getToken();
}

/*
 * prints error and returns nullptr
 * this is helpful as function calling it expects a pointer return type for all calls
 */
std::unique_ptr<ExprAST> parser::logError(const char* str) {
    fprintf(stderr, "Error : %s " , str);
    return nullptr;
}

/*
 * prints error and returns nullptr
 * this is helpful as function calling it expects a pointer return type for all calls
 * have separate type of return 
 */
std::unique_ptr<PrototypeExprAST> parser::LogErrorP(const char* str){
    logError(str);
    return nullptr;
}
/*
 * checks the precedence of curtoken when it is being is used as
 * binary operator
*/
int parser::getTokenPrecedence(){
    if(!isascii(curToken)){
        return -1; // binary ops are ascii chars
    }
    // check if its a declared binops
    int tokPrec = binOpPrecedence[curToken];
    if(tokPrec <= 0) return -1;
    return tokPrec;

}
// helper function to route to the correct parse based on current token
std::unique_ptr<ExprAST> parser::parsePrimary(){
    switch(curToken){
        default:
            return logError("Unknown token when expecting an expression");
        case tok_identifier:
            return parseIdentifierExpr();
        case tok_num:
            return parseNumberExpr();
        case '(':
            return parseParenExpr();
    }
}
/*
 * called when number literal is encountered
 */
std::unique_ptr<ExprAST> parser::parseNumberExpr(){
    auto result = std::make_unique<NumberExprAST>(l.numVal);
    //update nextToken
    getNextToken();
    return std::move(result);

}

/*
 * not called for functions
 * only expressions
 */
std::unique_ptr<ExprAST> parser:: parseParenExpr(){
    getNextToken(); // eat (
    auto v = parseExpression();
    if(!v) return nullptr;
    if(curToken != ')') return logError("expected )");
    getNextToken(); // eat )
    return v;
}

// calls when variable name is encountered
// either return a variableAST when a;
// return call(name, args) for a function call when a(5, 7)
std::unique_ptr<ExprAST> parser::parseIdentifierExpr(){
    std::string name = l.identifierStr;
    getNextToken(); // eat the identifierStr
    if(curToken != '(') return std::make_unique<VariableExprAST>(name); // not a function just a variable
    
    getNextToken(); // eat (
    std::vector<std::unique_ptr<ExprAST>> Args; // stores the function Args
    if(curToken != ')'){
        while(true){
            if( auto arg = parseExpression()) Args.push_back(std::move(arg));
            else return nullptr;
            if(curToken == ')') break;
            if(curToken != ',') return logError("Expected ')' or ',' in argument list");
            getNextToken();
        }

    }
    getNextToken();
    return std::make_unique<CallExprAST>(name, std::move(Args));
}

std::unique_ptr<ExprAST> parser::parseExpression() {
    auto LHS = parsePrimary();
    if(!LHS) return nullptr;
    return parseBinOpRHS(0, std::move(LHS));
}

std::unique_ptr<ExprAST> parser::parseBinOpRHS(int expPrec, std::unique_ptr<ExprAST> LHS){
    // find precedence in case binops
    while(true){
        int tokPrec = getTokenPrecedence();
        if(tokPrec < expPrec) return LHS; // this is baseCondition
        // parse the primaryExpression after the operator
        // eat the binOP
        int binOp = curToken;
        getNextToken();
        
        auto RHS = parsePrimary();
        if(!RHS) return nullptr;
        
        // check current binop precedence vs next 
        // If BinOp binds less tightly with RHS than the operator after RHS,
        // let the pending operator take RHS as its LHS.
        int nextPrec = getTokenPrecedence();
        if(tokPrec < nextPrec){
            RHS = parseBinOpRHS(tokPrec + 1, std::move(RHS));
            if(!RHS) return nullptr;
        }
        //merge LHS and RHS 
        LHS = std::make_unique<BinaryExprAST>(binOp, std::move(LHS), std::move(RHS));

    }
}
/*
 * parses a functioon prototype of the form fn(arg1, arg2 , ..)
 */
std::unique_ptr<PrototypeExprAST> parser::parsePrototype(){
    if(curToken != tok_identifier) return LogErrorP("Expected function name in the prototype");
    std::string fnName = l.identifierStr;
    getNextToken(); // eat (
    if(curToken != '(') return LogErrorP("Expected '(' in prototype");
    std::vector<std::string> argNames;
    while(getNextToken() == tok_identifier) argNames.push_back(l.identifierStr);
    if(curToken != ')') return LogErrorP("Expected ')' in prototype");
    
    // eat )
    getNextToken();
    return std::make_unique<PrototypeExprAST>(fnName, argNames);
}
/*
 * parses function def fn(arg1, arg2, arg3 expr(arg1, arg2,arg3) 
 * Kaleidoscope only handles one expression in a function
 */
std::unique_ptr<FunctionExprAST> parser::parseDefinition(){
    getNextToken(); // eat def
    auto prototype = parsePrototype();
    if(!prototype) return nullptr;
    if(auto E = parseExpression()) return std::make_unique<FunctionExprAST>(std::move(prototype), std::move(E));
    return nullptr;
    
}
/*
 * allows the defintinition of extern func as prototype
 */
std::unique_ptr<PrototypeExprAST> parser::parseExtern(){
    getNextToken(); // eat extern
    return parsePrototype(); 
    
}

/*
 * allows expression to be handled as functions
 */
std::unique_ptr<FunctionExprAST> parser::parseTopLevelExpr(){
    if(auto E = parseExpression()){
        auto proto = std::make_unique<PrototypeExprAST>("__anon_expr", std::vector<std::string>());
        return std::make_unique<FunctionExprAST>(std::move(proto), std::move(E));
    }
    return nullptr;
}

/*
 * main driver loop - batch compiler mode
 */
void parser::parse(){
    getNextToken();
    while(true){
        switch(curToken){
            case tok_eof:
                return;
            case ';':
                getNextToken();
                break;
            case tok_def:
                handleDefinition();
                break;
            case tok_extern:
                handleExtern();
                break;
            default:
                handleTopLevelExpression();
                break;
        }
    }
}

/*
 * abort compilation on error - no recovery in batch mode
 */
void parser::handleDefinition(){
    if(parseDefinition()){
        // IR generation or AST processing happens here
    } else {
        throw std::runtime_error(
            "Error: failed to parse function definition at line " 
           
        );
    }
}

void parser::handleExtern(){
    if(parseExtern()){
        // IR generation or AST processing happens here
    } else {
        throw std::runtime_error(
            "Error: failed to parse extern declaration at line " 
            
        );
    }
}

void parser::handleTopLevelExpression(){
    if(parseTopLevelExpr()){
        // In batch mode, top-level expressions outside functions
        // are typically illegal — warn or error depending on your spec
        throw std::runtime_error(
            "Error: top-level expressions are not allowed outside functions at line "
        );
    } else {
        throw std::runtime_error(
            "Error: unexpected token '" + std::to_string(curToken) 
            + "' at line " 
        );
    }
}