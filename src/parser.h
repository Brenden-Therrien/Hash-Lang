#ifndef HASH_PARSER_H
#define HASH_PARSER_H

#include "lexer.h"
#include "ast.h"
#include <memory>
#include <vector>

namespace hash {

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::shared_ptr<Program> parse();
    
    const std::vector<std::string>& getErrors() const { return errors; }
    
private:
    std::vector<Token> tokens;
    size_t current;
    std::vector<std::string> errors;
    
    // Helper methods
    Token peek(int offset = 0) const;
    Token advance();
    bool match(TokenType type);
    bool match(const std::vector<TokenType>& types);
    bool check(TokenType type) const;
    bool isAtEnd() const;
    Token consume(TokenType type, const std::string& message);
    void synchronize();
    void error(const std::string& message);
    
    // Parsing methods
    std::shared_ptr<FunctionDecl> parseFunction();
    std::shared_ptr<VariableDecl> parseGlobalVariable();
    std::shared_ptr<Statement> parseStatement();
    std::shared_ptr<VariableDecl> parseVariableDecl();
    std::shared_ptr<Statement> parseIfStatement();
    std::shared_ptr<Statement> parseWhileStatement();
    std::shared_ptr<Statement> parseReturnStatement(int line, int column);
    std::shared_ptr<Expression> parseExpression();
    std::shared_ptr<Expression> parseLogicalOr();
    std::shared_ptr<Expression> parseLogicalAnd();
    std::shared_ptr<Expression> parseBitwiseOr();
    std::shared_ptr<Expression> parseBitwiseXor();
    std::shared_ptr<Expression> parseBitwiseAnd();
    std::shared_ptr<Expression> parseEquality();
    std::shared_ptr<Expression> parseComparison();
    std::shared_ptr<Expression> parseShift();
    std::shared_ptr<Expression> parseTerm();
    std::shared_ptr<Expression> parseFactor();
    std::shared_ptr<Expression> parseUnary();
    std::shared_ptr<Expression> parsePrimary();
    std::shared_ptr<Expression> parseCall(std::shared_ptr<Expression> callee);
    
    std::shared_ptr<Type> parseType();
    std::vector<std::shared_ptr<Statement>> parseBlock();
};

} // namespace hash

#endif // HASH_PARSER_H
