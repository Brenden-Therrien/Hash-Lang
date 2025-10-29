#include "lexer.h"
#include <cctype>
#include <stdexcept>

namespace hash {

std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"fn", TokenType::FN},
    {"pure", TokenType::PURE},
    {"let", TokenType::LET},
    {"mut", TokenType::MUT},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"while", TokenType::WHILE},
    {"for", TokenType::FOR},
    {"return", TokenType::RETURN},
    {"import", TokenType::IMPORT},
    {"struct", TokenType::STRUCT},
    {"enum", TokenType::ENUM},
    {"match", TokenType::MATCH},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},
    
    // Types
    {"i8", TokenType::TYPE_I8},
    {"i16", TokenType::TYPE_I16},
    {"i32", TokenType::TYPE_I32},
    {"i64", TokenType::TYPE_I64},
    {"u8", TokenType::TYPE_U8},
    {"u16", TokenType::TYPE_U16},
    {"u32", TokenType::TYPE_U32},
    {"u64", TokenType::TYPE_U64},
    {"f32", TokenType::TYPE_F32},
    {"f64", TokenType::TYPE_F64},
    {"bool", TokenType::TYPE_BOOL},
    {"void", TokenType::TYPE_VOID},
    {"str", TokenType::TYPE_STR},
    
    // Access modifiers
    {"pure_local", TokenType::PURE_LOCAL},
    {"side_effect", TokenType::SIDE_EFFECT},
};

Lexer::Lexer(const std::string& source)
    : source(source), pos(0), line(1), column(1), currentIndent(0) {
    indentStack.push_back(0);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    bool atLineStart = true;
    
    while (!isAtEnd()) {
        // Handle indentation at the start of a line
        if (atLineStart && peek() != '\n' && peek() != '\r') {
            int spaces = 0;
            while (peek() == ' ') {
                advance();
                spaces++;
            }
            
            // Skip empty lines or comment-only lines
            if (peek() == '\n' || peek() == '\r' || peek() == '#') {
                skipComment();
                if (peek() == '\n' || peek() == '\r') {
                    advance();
                    continue;
                }
            } else {
                auto indentTokens = handleIndentation(spaces);
                tokens.insert(tokens.end(), indentTokens.begin(), indentTokens.end());
                atLineStart = false;
            }
        }
        
        skipWhitespace();
        
        if (isAtEnd()) break;
        
        char c = peek();
        
        // Skip comments
        if (c == '#') {
            skipComment();
            continue;
        }
        
        // Newline
        if (c == '\n' || c == '\r') {
            advance();
            atLineStart = true;
            continue;
        }
        
        // Numbers
        if (isDigit(c)) {
            tokens.push_back(readNumber());
            continue;
        }
        
        // Strings
        if (c == '"' || c == '\'') {
            tokens.push_back(readString());
            continue;
        }
        
        // Identifiers and keywords
        if (isAlpha(c) || c == '_') {
            tokens.push_back(readIdentifier());
            continue;
        }
        
        // Operators and delimiters
        switch (c) {
            case '+': tokens.push_back(makeToken(TokenType::PLUS, std::string(1, advance()))); break;
            case '*': tokens.push_back(makeToken(TokenType::MULTIPLY, std::string(1, advance()))); break;
            case '%': tokens.push_back(makeToken(TokenType::MODULO, std::string(1, advance()))); break;
            case '(': tokens.push_back(makeToken(TokenType::LPAREN, std::string(1, advance()))); break;
            case ')': tokens.push_back(makeToken(TokenType::RPAREN, std::string(1, advance()))); break;
            case '{': tokens.push_back(makeToken(TokenType::LBRACE, std::string(1, advance()))); break;
            case '}': tokens.push_back(makeToken(TokenType::RBRACE, std::string(1, advance()))); break;
            case '[': tokens.push_back(makeToken(TokenType::LBRACKET, std::string(1, advance()))); break;
            case ']': tokens.push_back(makeToken(TokenType::RBRACKET, std::string(1, advance()))); break;
            case ',': tokens.push_back(makeToken(TokenType::COMMA, std::string(1, advance()))); break;
            case ';': tokens.push_back(makeToken(TokenType::SEMICOLON, std::string(1, advance()))); break;
            case ':': tokens.push_back(makeToken(TokenType::COLON, std::string(1, advance()))); break;
            case '.': tokens.push_back(makeToken(TokenType::DOT, std::string(1, advance()))); break;
            case '~': tokens.push_back(makeToken(TokenType::BITWISE_NOT, std::string(1, advance()))); break;
            
            case '-':
                advance();
                if (peek() == '>') {
                    advance();
                    tokens.push_back(makeToken(TokenType::ARROW, "->"));
                } else {
                    tokens.push_back(makeToken(TokenType::MINUS, "-"));
                }
                break;
            
            case '=':
                advance();
                if (peek() == '=') {
                    advance();
                    tokens.push_back(makeToken(TokenType::EQUAL, "=="));
                } else {
                    tokens.push_back(makeToken(TokenType::ASSIGN, "="));
                }
                break;
            
            case '!':
                advance();
                if (peek() == '=') {
                    advance();
                    tokens.push_back(makeToken(TokenType::NOT_EQUAL, "!="));
                } else {
                    tokens.push_back(makeToken(TokenType::NOT, "!"));
                }
                break;
            
            case '<':
                advance();
                if (peek() == '=') {
                    advance();
                    tokens.push_back(makeToken(TokenType::LESS_EQUAL, "<="));
                } else if (peek() == '<') {
                    advance();
                    tokens.push_back(makeToken(TokenType::SHIFT_LEFT, "<<"));
                } else {
                    tokens.push_back(makeToken(TokenType::LESS, "<"));
                }
                break;
            
            case '>':
                advance();
                if (peek() == '=') {
                    advance();
                    tokens.push_back(makeToken(TokenType::GREATER_EQUAL, ">="));
                } else if (peek() == '>') {
                    advance();
                    tokens.push_back(makeToken(TokenType::SHIFT_RIGHT, ">>"));
                } else {
                    tokens.push_back(makeToken(TokenType::GREATER, ">"));
                }
                break;
            
            case '&':
                advance();
                if (peek() == '&') {
                    advance();
                    tokens.push_back(makeToken(TokenType::AND, "&&"));
                } else {
                    tokens.push_back(makeToken(TokenType::BITWISE_AND, "&"));
                }
                break;
            
            case '|':
                advance();
                if (peek() == '|') {
                    advance();
                    tokens.push_back(makeToken(TokenType::OR, "||"));
                } else {
                    tokens.push_back(makeToken(TokenType::BITWISE_OR, "|"));
                }
                break;
            
            case '^':
                tokens.push_back(makeToken(TokenType::BITWISE_XOR, std::string(1, advance())));
                break;
            
            case '/':
                advance();
                if (peek() == '/') {
                    skipComment();
                } else {
                    tokens.push_back(makeToken(TokenType::DIVIDE, "/"));
                }
                break;
            
            default:
                tokens.push_back(makeToken(TokenType::INVALID, std::string(1, advance())));
                break;
        }
    }
    
    // Add dedent tokens for any remaining indentation
    while (indentStack.size() > 1) {
        indentStack.pop_back();
        tokens.push_back(makeToken(TokenType::DEDENT, ""));
    }
    
    tokens.push_back(makeToken(TokenType::END_OF_FILE, ""));
    return tokens;
}

char Lexer::peek(int offset) const {
    if (pos + offset >= source.length()) return '\0';
    return source[pos + offset];
}

char Lexer::advance() {
    char c = source[pos++];
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

void Lexer::skipWhitespace() {
    while (peek() == ' ' || peek() == '\t') {
        advance();
    }
}

void Lexer::skipComment() {
    if (peek() == '#') {
        while (peek() != '\n' && peek() != '\r' && !isAtEnd()) {
            advance();
        }
    }
}

Token Lexer::makeToken(TokenType type, const std::string& value) {
    return Token(type, value, line, column - value.length());
}

Token Lexer::readNumber() {
    std::string num;
    bool isFloat = false;
    
    while (isDigit(peek())) {
        num += advance();
    }
    
    if (peek() == '.' && isDigit(peek(1))) {
        isFloat = true;
        num += advance(); // consume '.'
        while (isDigit(peek())) {
            num += advance();
        }
    }
    
    return makeToken(isFloat ? TokenType::FLOAT : TokenType::INTEGER, num);
}

Token Lexer::readString() {
    char quote = advance(); // consume opening quote
    std::string str;
    
    while (peek() != quote && !isAtEnd()) {
        if (peek() == '\\') {
            advance();
            char escaped = advance();
            switch (escaped) {
                case 'n': str += '\n'; break;
                case 't': str += '\t'; break;
                case 'r': str += '\r'; break;
                case '\\': str += '\\'; break;
                case '"': str += '"'; break;
                case '\'': str += '\''; break;
                default: str += escaped; break;
            }
        } else {
            str += advance();
        }
    }
    
    if (isAtEnd()) {
        return makeToken(TokenType::INVALID, str);
    }
    
    advance(); // consume closing quote
    return makeToken(TokenType::STRING, str);
}

Token Lexer::readIdentifier() {
    std::string id;
    
    while (isAlphaNumeric(peek()) || peek() == '_') {
        id += advance();
    }
    
    auto it = keywords.find(id);
    TokenType type = (it != keywords.end()) ? it->second : TokenType::IDENTIFIER;
    
    return makeToken(type, id);
}

std::vector<Token> Lexer::handleIndentation(int spaces) {
    std::vector<Token> tokens;
    
    if (spaces > indentStack.back()) {
        indentStack.push_back(spaces);
        tokens.push_back(makeToken(TokenType::INDENT, ""));
    } else if (spaces < indentStack.back()) {
        while (!indentStack.empty() && spaces < indentStack.back()) {
            indentStack.pop_back();
            tokens.push_back(makeToken(TokenType::DEDENT, ""));
        }
        
        if (indentStack.empty() || spaces != indentStack.back()) {
            // Indentation error - not matching any previous level
            tokens.push_back(makeToken(TokenType::INVALID, ""));
        }
    }
    
    return tokens;
}

bool Lexer::isAtEnd() const {
    return pos >= source.length();
}

bool Lexer::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

} // namespace hash
