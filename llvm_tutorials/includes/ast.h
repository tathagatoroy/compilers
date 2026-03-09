#ifndef AST_H
#define AST_H
#include <memory>
#include <string>
#include <utility>
#include <vector>



class ExprAST{
public:
    virtual ~ExprAST() = default;
    
};

class NumberExprAST : public ExprAST {
    double val;
public:
    NumberExprAST(double valx) : val(valx) {}
};

class VariableExprAST: public ExprAST {
    std::string name;
public:
    VariableExprAST(const std::string &Name) : name(Name) {}
};

class BinaryExprAST : public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> lhs, rhs;
public:
    BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS , std::unique_ptr<ExprAST> RHS) : Op(op) , lhs(std::move(LHS)) , rhs(std::move(RHS)) {}
    
};

class CallExprAST : public ExprAST {
    std::string callee;
    std::vector<std::unique_ptr<ExprAST>> args;
public:
    CallExprAST(std::string &Callee, std::vector<std::unique_ptr<ExprAST>> Args) : callee(Callee) , args(std::move(Args)) {}
    
};

// prototype of a function 
class PrototypeExprAST : public ExprAST {
    std::string name;
    std::vector<std::string> args;
public:
    PrototypeExprAST(const std::string &Name, std::vector<std::string> Args) : name(Name) , args(std::move(Args)) {} // no copy for rvalue 
    const std::string& getName() const { return name; }
    
};

class FunctionExprAST : public ExprAST {
    std::unique_ptr<PrototypeExprAST> prototype;
    std::unique_ptr<ExprAST> body;
public: 
FunctionExprAST(std::unique_ptr<PrototypeExprAST> Prototype , std::unique_ptr<ExprAST> Body) : prototype(std::move(Prototype)) , body(std::move(Body)) {}

};


#endif