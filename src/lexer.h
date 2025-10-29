#ifndef HASH_LEXER_H
#define HASH_LEXER_H

#include <string>
#include <vector>
#include <unordered_map>

namespace hash {

enum class TokenType {
    // Literals
    INTEGER,
    FLOAT,
    STRING,
    IDENTIFIER,
    
    // Keywords
    FN,           // function declaration
    PURE,         // pure function marker
    LET,          // variable declaration
    MUT,          // mutable variable
    IF,
    ELSE,
    WHILE,
    FOR,
    RETURN,
    IMPORT,
    STRUCT,
    ENUM,
    MATCH,
    TRUE,
    FALSE,
    
    // Types
    TYPE_I8, TYPE_I16, TYPE_I32, TYPE_I64,
    TYPE_U8, TYPE_U16, TYPE_U32, TYPE_U64,
    TYPE_F32, TYPE_F64,
    TYPE_BOOL,
    TYPE_VOID,
    TYPE_STR,
    
    // Access modifiers (behavior-aware)
    PURE_LOCAL,   // only accessible by pure functions
    SIDE_EFFECT,  // can be accessed by functions with side effects
    
    // Operators
    PLUS, MINUS, MULTIPLY, DIVIDE, MODULO,
    ASSIGN, EQUAL, NOT_EQUAL,
    LESS, LESS_EQUAL, GREATER, GREATER_EQUAL,
    AND, OR, NOT,
    BITWISE_AND, BITWISE_OR, BITWISE_XOR, BITWISE_NOT,
    SHIFT_LEFT, SHIFT_RIGHT,
    
    // Delimiters
    LPAREN, RPAREN,
    LBRACE, RBRACE,
    LBRACKET, RBRACKET,
    COMMA, SEMICOLON, COLON,
    ARROW,        // ->
    DOT,
    
    // Special
    NEWLINE,
    INDENT,
    DEDENT,
    END_OF_FILE,
    INVALID
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
    
    Token(TokenType t, const std::string& v, int l, int c)
        : type(t), value(v), line(l), column(c) {}
};

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> tokenize();
    
private:
    std::string source;
    size_t pos;
    int line;
    int column;
    int currentIndent;
    std::vector<int> indentStack;
    
    static std::unordered_map<std::string, TokenType> keywords;
    
    char peek(int offset = 0) const;
    char advance();
    void skipWhitespace();
    void skipComment();
    
    Token makeToken(TokenType type, const std::string& value);
    Token readNumber();
    Token readString();
    Token readIdentifier();
    std::vector<Token> handleIndentation(int spaces);
    
    bool isAtEnd() const;
    bool isDigit(char c) const;
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;
};

} // namespace hash

#endif // HASH_LEXER_H
