#include "parser.h"
#include <sstream>

namespace hash {

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens), current(0) {}

std::shared_ptr<Program> Parser::parse() {
    auto program = std::make_shared<Program>();
    
    while (!isAtEnd()) {
        try {
            if (match(TokenType::FN) || match(TokenType::PURE)) {
                // Reset to check for pure
                if (tokens[current - 1].type == TokenType::PURE || 
                    (current > 0 && tokens[current - 2].type == TokenType::PURE)) {
                    program->functions.push_back(parseFunction());
                } else {
                    program->functions.push_back(parseFunction());
                }
            } else if (match(TokenType::LET)) {
                program->globals.push_back(parseGlobalVariable());
            } else if (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {
                continue;
            } else {
                error("Expected function or global variable declaration");
                synchronize();
            }
        } catch (const std::exception& e) {
            error(e.what());
            synchronize();
        }
    }
    
    return program;
}

Token Parser::peek(int offset) const {
    if (current + offset >= tokens.size()) {
        return tokens.back();
    }
    return tokens[current + offset];
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return tokens[current - 1];
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::match(const std::vector<TokenType>& types) {
    for (auto type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::END_OF_FILE;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    error(message);
    throw std::runtime_error(message);
}

void Parser::synchronize() {
    while (!isAtEnd()) {
        if (tokens[current - 1].type == TokenType::NEWLINE) return;
        
        switch (peek().type) {
            case TokenType::FN:
            case TokenType::LET:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::RETURN:
                return;
            default:
                advance();
        }
    }
}

void Parser::error(const std::string& message) {
    Token token = peek();
    std::ostringstream oss;
    oss << "Error at line " << token.line << ", column " << token.column 
        << ": " << message;
    errors.push_back(oss.str());
}

std::shared_ptr<FunctionDecl> Parser::parseFunction() {
    bool isPure = false;
    
    // Check if we already consumed PURE
    if (current > 0 && tokens[current - 1].type == TokenType::PURE) {
        isPure = true;
        consume(TokenType::FN, "Expected 'fn' after 'pure'");
    } else if (current > 0 && tokens[current - 1].type == TokenType::FN) {
        // Already consumed FN, check if there was PURE before it
        if (current > 1 && tokens[current - 2].type == TokenType::PURE) {
            isPure = true;
        }
    }
    
    Token name = consume(TokenType::IDENTIFIER, "Expected function name");
    auto func = std::make_shared<FunctionDecl>(name.value, isPure);
    func->line = name.line;
    func->column = name.column;
    
    consume(TokenType::LPAREN, "Expected '(' after function name");
    
    // Parse parameters
    if (!check(TokenType::RPAREN)) {
        do {
            Token paramName = consume(TokenType::IDENTIFIER, "Expected parameter name");
            consume(TokenType::COLON, "Expected ':' after parameter name");
            auto paramType = parseType();
            func->parameters.emplace_back(paramName.value, paramType);
        } while (match(TokenType::COMMA));
    }
    
    consume(TokenType::RPAREN, "Expected ')' after parameters");
    
    // Parse return type
    if (match(TokenType::ARROW)) {
        func->returnType = parseType();
    } else {
        func->returnType = Type::getVoid();
    }
    
    consume(TokenType::COLON, "Expected ':' before function body");
    
    // Parse body
    func->body = parseBlock();
    
    return func;
}

std::shared_ptr<VariableDecl> Parser::parseGlobalVariable() {
    bool isMutable = false;
    bool isPureLocal = false;
    
    // Check for mut or pure_local
    if (match(TokenType::MUT)) {
        isMutable = true;
    } else if (match(TokenType::PURE_LOCAL)) {
        isPureLocal = true;
    }
    
    Token name = consume(TokenType::IDENTIFIER, "Expected variable name");
    consume(TokenType::COLON, "Expected ':' after variable name");
    
    auto type = parseType();
    auto varDecl = std::make_shared<VariableDecl>(name.value, type, isMutable, isPureLocal);
    varDecl->line = name.line;
    varDecl->column = name.column;
    
    if (match(TokenType::ASSIGN)) {
        varDecl->initializer = parseExpression();
    }
    
    return varDecl;
}

std::shared_ptr<Statement> Parser::parseStatement() {
    // Skip newlines
    while (match(TokenType::NEWLINE)) {}
    
    if (match(TokenType::LET)) {
        return parseVariableDecl();
    }
    if (match(TokenType::IF)) {
        return parseIfStatement();
    }
    if (match(TokenType::WHILE)) {
        return parseWhileStatement();
    }
    if (match(TokenType::RETURN)) {
        int returnLine = tokens[current - 1].line;
        int returnCol = tokens[current - 1].column;
        return parseReturnStatement(returnLine, returnCol);
    }
    
    // Check for assignment
    if (check(TokenType::IDENTIFIER) && peek(1).type == TokenType::ASSIGN) {
        Token name = advance();
        advance(); // consume '='
        auto value = parseExpression();
        auto assignment = std::make_shared<Assignment>(name.value, value);
        assignment->line = name.line;
        assignment->column = name.column;
        return assignment;
    }
    
    // Expression statement
    auto expr = parseExpression();
    return std::make_shared<ExprStmt>(expr);
}

std::shared_ptr<VariableDecl> Parser::parseVariableDecl() {
    bool isMutable = false;
    bool isPureLocal = false;
    
    if (match(TokenType::MUT)) {
        isMutable = true;
    } else if (match(TokenType::PURE_LOCAL)) {
        isPureLocal = true;
    }
    
    Token name = consume(TokenType::IDENTIFIER, "Expected variable name");
    consume(TokenType::COLON, "Expected ':' after variable name");
    
    auto type = parseType();
    auto varDecl = std::make_shared<VariableDecl>(name.value, type, isMutable, isPureLocal);
    varDecl->line = name.line;
    varDecl->column = name.column;
    
    if (match(TokenType::ASSIGN)) {
        varDecl->initializer = parseExpression();
    }
    
    return varDecl;
}

std::shared_ptr<Statement> Parser::parseIfStatement() {
    auto condition = parseExpression();
    consume(TokenType::COLON, "Expected ':' after if condition");
    
    auto ifStmt = std::make_shared<IfStmt>(condition);
    ifStmt->thenBody = parseBlock();
    
    if (match(TokenType::ELSE)) {
        consume(TokenType::COLON, "Expected ':' after else");
        ifStmt->elseBody = parseBlock();
    }
    
    return ifStmt;
}

std::shared_ptr<Statement> Parser::parseWhileStatement() {
    auto condition = parseExpression();
    consume(TokenType::COLON, "Expected ':' after while condition");
    
    auto whileStmt = std::make_shared<WhileStmt>(condition);
    whileStmt->body = parseBlock();
    
    return whileStmt;
}

std::shared_ptr<Statement> Parser::parseReturnStatement(int line, int column) {
    if (check(TokenType::NEWLINE) || check(TokenType::DEDENT)) {
        auto stmt = std::make_shared<ReturnStmt>();
        stmt->line = line;
        stmt->column = column;
        return stmt;
    }
    
    auto value = parseExpression();
    auto stmt = std::make_shared<ReturnStmt>(value);
    stmt->line = line;
    stmt->column = column;
    return stmt;
}

std::shared_ptr<Expression> Parser::parseExpression() {
    return parseLogicalOr();
}

std::shared_ptr<Expression> Parser::parseLogicalOr() {
    auto expr = parseLogicalAnd();
    
    while (match(TokenType::OR)) {
        auto right = parseLogicalAnd();
        expr = std::make_shared<BinaryOp>(BinaryOp::Op::OR, expr, right);
    }
    
    return expr;
}

std::shared_ptr<Expression> Parser::parseLogicalAnd() {
    auto expr = parseBitwiseOr();
    
    while (match(TokenType::AND)) {
        auto right = parseBitwiseOr();
        expr = std::make_shared<BinaryOp>(BinaryOp::Op::AND, expr, right);
    }
    
    return expr;
}

std::shared_ptr<Expression> Parser::parseBitwiseOr() {
    auto expr = parseBitwiseXor();
    
    while (match(TokenType::BITWISE_OR)) {
        auto right = parseBitwiseXor();
        expr = std::make_shared<BinaryOp>(BinaryOp::Op::BIT_OR, expr, right);
    }
    
    return expr;
}

std::shared_ptr<Expression> Parser::parseBitwiseXor() {
    auto expr = parseBitwiseAnd();
    
    while (match(TokenType::BITWISE_XOR)) {
        auto right = parseBitwiseAnd();
        expr = std::make_shared<BinaryOp>(BinaryOp::Op::BIT_XOR, expr, right);
    }
    
    return expr;
}

std::shared_ptr<Expression> Parser::parseBitwiseAnd() {
    auto expr = parseEquality();
    
    while (match(TokenType::BITWISE_AND)) {
        auto right = parseEquality();
        expr = std::make_shared<BinaryOp>(BinaryOp::Op::BIT_AND, expr, right);
    }
    
    return expr;
}

std::shared_ptr<Expression> Parser::parseEquality() {
    auto expr = parseComparison();
    
    while (true) {
        if (match(TokenType::EQUAL)) {
            auto right = parseComparison();
            expr = std::make_shared<BinaryOp>(BinaryOp::Op::EQ, expr, right);
        } else if (match(TokenType::NOT_EQUAL)) {
            auto right = parseComparison();
            expr = std::make_shared<BinaryOp>(BinaryOp::Op::NE, expr, right);
        } else {
            break;
        }
    }
    
    return expr;
}

std::shared_ptr<Expression> Parser::parseComparison() {
    auto expr = parseShift();
    
    while (true) {
        if (match(TokenType::LESS)) {
            auto right = parseShift();
            expr = std::make_shared<BinaryOp>(BinaryOp::Op::LT, expr, right);
        } else if (match(TokenType::LESS_EQUAL)) {
            auto right = parseShift();
            expr = std::make_shared<BinaryOp>(BinaryOp::Op::LE, expr, right);
        } else if (match(TokenType::GREATER)) {
            auto right = parseShift();
            expr = std::make_shared<BinaryOp>(BinaryOp::Op::GT, expr, right);
        } else if (match(TokenType::GREATER_EQUAL)) {
            auto right = parseShift();
            expr = std::make_shared<BinaryOp>(BinaryOp::Op::GE, expr, right);
        } else {
            break;
        }
    }
    
    return expr;
}

std::shared_ptr<Expression> Parser::parseShift() {
    auto expr = parseTerm();
    
    while (true) {
        if (match(TokenType::SHIFT_LEFT)) {
            auto right = parseTerm();
            expr = std::make_shared<BinaryOp>(BinaryOp::Op::SHL, expr, right);
        } else if (match(TokenType::SHIFT_RIGHT)) {
            auto right = parseTerm();
            expr = std::make_shared<BinaryOp>(BinaryOp::Op::SHR, expr, right);
        } else {
            break;
        }
    }
    
    return expr;
}

std::shared_ptr<Expression> Parser::parseTerm() {
    auto expr = parseFactor();
    
    while (true) {
        if (match(TokenType::PLUS)) {
            auto right = parseFactor();
            expr = std::make_shared<BinaryOp>(BinaryOp::Op::ADD, expr, right);
        } else if (match(TokenType::MINUS)) {
            auto right = parseFactor();
            expr = std::make_shared<BinaryOp>(BinaryOp::Op::SUB, expr, right);
        } else {
            break;
        }
    }
    
    return expr;
}

std::shared_ptr<Expression> Parser::parseFactor() {
    auto expr = parseUnary();
    
    while (true) {
        if (match(TokenType::MULTIPLY)) {
            auto right = parseUnary();
            expr = std::make_shared<BinaryOp>(BinaryOp::Op::MUL, expr, right);
        } else if (match(TokenType::DIVIDE)) {
            auto right = parseUnary();
            expr = std::make_shared<BinaryOp>(BinaryOp::Op::DIV, expr, right);
        } else if (match(TokenType::MODULO)) {
            auto right = parseUnary();
            expr = std::make_shared<BinaryOp>(BinaryOp::Op::MOD, expr, right);
        } else {
            break;
        }
    }
    
    return expr;
}

std::shared_ptr<Expression> Parser::parseUnary() {
    if (match(TokenType::MINUS)) {
        auto operand = parseUnary();
        return std::make_shared<UnaryOp>(UnaryOp::Op::NEG, operand);
    }
    if (match(TokenType::NOT)) {
        auto operand = parseUnary();
        return std::make_shared<UnaryOp>(UnaryOp::Op::NOT, operand);
    }
    if (match(TokenType::BITWISE_NOT)) {
        auto operand = parseUnary();
        return std::make_shared<UnaryOp>(UnaryOp::Op::BIT_NOT, operand);
    }
    
    return parsePrimary();
}

std::shared_ptr<Expression> Parser::parsePrimary() {
    if (match(TokenType::INTEGER)) {
        auto lit = std::make_shared<IntegerLiteral>(std::stoll(tokens[current - 1].value));
        lit->type = Type::getI32();  // Default to i32 like C
        return lit;
    }
    
    if (match(TokenType::FLOAT)) {
        auto lit = std::make_shared<FloatLiteral>(std::stod(tokens[current - 1].value));
        lit->type = Type::getF64();
        return lit;
    }
    
    if (match(TokenType::STRING)) {
        auto lit = std::make_shared<StringLiteral>(tokens[current - 1].value);
        lit->type = Type::getStr();
        return lit;
    }
    
    if (match(TokenType::TRUE)) {
        auto lit = std::make_shared<BoolLiteral>(true);
        lit->type = Type::getBool();
        return lit;
    }
    
    if (match(TokenType::FALSE)) {
        auto lit = std::make_shared<BoolLiteral>(false);
        lit->type = Type::getBool();
        return lit;
    }
    
    if (match(TokenType::IDENTIFIER)) {
        std::string name = tokens[current - 1].value;
        
        // Check for function call
        if (match(TokenType::LPAREN)) {
            auto call = std::make_shared<CallExpr>(name);
            
            if (!check(TokenType::RPAREN)) {
                do {
                    call->arguments.push_back(parseExpression());
                } while (match(TokenType::COMMA));
            }
            
            consume(TokenType::RPAREN, "Expected ')' after arguments");
            return call;
        }
        
        return std::make_shared<Identifier>(name);
    }
    
    if (match(TokenType::LPAREN)) {
        auto expr = parseExpression();
        consume(TokenType::RPAREN, "Expected ')' after expression");
        return expr;
    }
    
    error("Expected expression");
    throw std::runtime_error("Expected expression");
}

std::shared_ptr<Type> Parser::parseType() {
    if (match(TokenType::TYPE_I8)) return std::make_shared<Type>(Type::Kind::I8);
    if (match(TokenType::TYPE_I16)) return std::make_shared<Type>(Type::Kind::I16);
    if (match(TokenType::TYPE_I32)) return std::make_shared<Type>(Type::Kind::I32);
    if (match(TokenType::TYPE_I64)) return std::make_shared<Type>(Type::Kind::I64);
    if (match(TokenType::TYPE_U8)) return std::make_shared<Type>(Type::Kind::U8);
    if (match(TokenType::TYPE_U16)) return std::make_shared<Type>(Type::Kind::U16);
    if (match(TokenType::TYPE_U32)) return std::make_shared<Type>(Type::Kind::U32);
    if (match(TokenType::TYPE_U64)) return std::make_shared<Type>(Type::Kind::U64);
    if (match(TokenType::TYPE_F32)) return std::make_shared<Type>(Type::Kind::F32);
    if (match(TokenType::TYPE_F64)) return std::make_shared<Type>(Type::Kind::F64);
    if (match(TokenType::TYPE_BOOL)) return std::make_shared<Type>(Type::Kind::BOOL);
    if (match(TokenType::TYPE_VOID)) return std::make_shared<Type>(Type::Kind::VOID);
    if (match(TokenType::TYPE_STR)) return std::make_shared<Type>(Type::Kind::STR);
    
    error("Expected type");
    throw std::runtime_error("Expected type");
}

std::vector<std::shared_ptr<Statement>> Parser::parseBlock() {
    std::vector<std::shared_ptr<Statement>> statements;
    
    // Expect INDENT
    if (!match(TokenType::INDENT) && !match(TokenType::NEWLINE)) {
        // Single-line block (for simple statements)
        statements.push_back(parseStatement());
        return statements;
    }
    
    // Skip additional newlines
    while (match(TokenType::NEWLINE)) {}
    
    // Parse statements until DEDENT
    while (!check(TokenType::DEDENT) && !isAtEnd()) {
        if (match(TokenType::NEWLINE)) continue;
        statements.push_back(parseStatement());
    }
    
    consume(TokenType::DEDENT, "Expected dedent after block");
    
    return statements;
}

} // namespace hash
