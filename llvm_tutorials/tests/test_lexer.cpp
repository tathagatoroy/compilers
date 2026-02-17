#include <gtest/gtest.h>
#include "lexer.h"
#include <string>
#include <vector>

// Helper struct to manage the lexer and the results
struct LexerTestRunner {
    lexer l;
    std::vector<int> tokens;
    
    LexerTestRunner(std::string input) : l(std::move(input)) {
        int tok;
        do {
            tok = l.getToken();
            tokens.push_back(tok);
        } while (tok != tok_eof);
    }
};

// TEST(TestSuiteName, TestName)
TEST(LexerTest, EmptyInput) {
    // Purpose: Tests if the lexer correctly handles an empty string.
    // Expected: An immediate End-of-File token.
    LexerTestRunner runner("");
    std::vector<int> expected_tokens = {tok_eof};
    ASSERT_EQ(runner.tokens, expected_tokens);
}

TEST(LexerTest, WhitespaceOnly) {
    // Purpose: Tests if the lexer can correctly handle various forms of whitespace.
    // Expected: A clean EOF token at the end after skipping all whitespace.
    LexerTestRunner runner(" \t \n\r ");
    std::vector<int> expected_tokens = {tok_eof};
    ASSERT_EQ(runner.tokens, expected_tokens);
}

TEST(LexerTest, KeywordsAndIdentifiers) {
    // Purpose: Tests the basic tokenization of keywords and identifiers.
    LexerTestRunner runner("def main extern sin");
    
    ASSERT_EQ(runner.tokens[0], tok_def);
    
    ASSERT_EQ(runner.tokens[1], tok_identifier);
    ASSERT_EQ(runner.l.identifierStr, "main");
    
    ASSERT_EQ(runner.tokens[2], tok_extern);
    
    ASSERT_EQ(runner.tokens[3], tok_identifier);
    ASSERT_EQ(runner.l.identifierStr, "sin");
    
    ASSERT_EQ(runner.tokens[4], tok_eof);
}

TEST(LexerTest, FullExpression) {
    // Purpose: A comprehensive test involving identifiers, numbers, and operators.
    LexerTestRunner runner("a + b * 10.5");
    
    ASSERT_EQ(runner.tokens[0], tok_identifier);
    ASSERT_EQ(runner.l.identifierStr, "a");
    
    ASSERT_EQ(runner.tokens[1], '+');
    
    ASSERT_EQ(runner.tokens[2], tok_identifier);
    ASSERT_EQ(runner.l.identifierStr, "b");
    
    ASSERT_EQ(runner.tokens[3], '*');
    
    ASSERT_EQ(runner.tokens[4], tok_num);
    ASSERT_DOUBLE_EQ(runner.l.numVal, 10.5);
    
    ASSERT_EQ(runner.tokens[5], tok_eof);
}

TEST(LexerTest, CommentsHandling) {
    // Purpose: Tests if the lexer correctly ignores comments.
    LexerTestRunner runner("def foo() # this is a comment");
    
    ASSERT_EQ(runner.tokens[0], tok_def);
    ASSERT_EQ(runner.tokens[1], tok_identifier);
    ASSERT_EQ(runner.l.identifierStr, "foo");
    ASSERT_EQ(runner.tokens[2], '(');
    ASSERT_EQ(runner.tokens[3], ')');
    ASSERT_EQ(runner.tokens[4], tok_eof);
}

TEST(LexerTest, CommentBeforeEOF) {
    // Purpose: Edge case to ensure a file ending with a comment gives EOF.
    LexerTestRunner runner("# final words");
    ASSERT_EQ(runner.tokens[0], tok_eof);
}

TEST(LexerTest, NumberForms) {
    // Purpose: Tests various forms of floating-point numbers.
    LexerTestRunner runner("1.23 .5 42. 123");
    
    ASSERT_EQ(runner.tokens[0], tok_num);
    ASSERT_DOUBLE_EQ(runner.l.numVal, 1.23);
    
    ASSERT_EQ(runner.tokens[1], tok_num);
    ASSERT_DOUBLE_EQ(runner.l.numVal, 0.5);
    
    ASSERT_EQ(runner.tokens[2], tok_num);
    ASSERT_DOUBLE_EQ(runner.l.numVal, 42.0);

    ASSERT_EQ(runner.tokens[3], tok_num);
    ASSERT_DOUBLE_EQ(runner.l.numVal, 123.0);
    
    ASSERT_EQ(runner.tokens[4], tok_eof);
}

TEST(LexerTest, InvalidNumberFormat) {
    // Purpose: Confirms the lexer tokenizes an invalid number as separate tokens.
    // It's the parser's job to semantically validate this, not the lexer's.
    LexerTestRunner runner("1.2.3");
    
    ASSERT_EQ(runner.tokens[0], tok_num);
    ASSERT_DOUBLE_EQ(runner.l.numVal, 1.2);
    
    ASSERT_EQ(runner.tokens[1], '.');
    
    ASSERT_EQ(runner.tokens[2], tok_num);
    ASSERT_DOUBLE_EQ(runner.l.numVal, 3.0);
    
    ASSERT_EQ(runner.tokens[3], tok_eof);
}