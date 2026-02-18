Reference : https://llvm.org/docs/tutorial/index.html

Language properties:
  1. Data type : 64 bit float 
     This means no type declaration needed
  2.Lexer : parses text file and returns token 
            Tokens can be of type identifiers, keywords, EOF, numbers and operators(punctuations)
  3. parsing the output of the lexer gives AST
     Two algorithms is used for this.
     a. Recursive Descent Parsing 
     b. Operator precedence parsing 
     Classes of AST
        1. Number : Values
        2. VariableName : name of Var;
        3. BinaryExpression : op , LHS, RHS
        4. FunctionCaller : callee, vector of Args
        5. prototype (Function prototype) : function Name, vector<String> arguments
        6. Function : Proto, Body(Expr)
  