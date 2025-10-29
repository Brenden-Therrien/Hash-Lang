#ifndef HASH_AST_H
#define HASH_AST_H

#include <memory>
#include <string>
#include <vector>

namespace hash {

// Forward declarations
class ASTVisitor;

// Base class for all AST nodes
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
    
    int line = 0;
    int column = 0;
};

// Type representation
struct Type {
    enum class Kind {
        I8, I16, I32, I64,
        U8, U16, U32, U64,
        F32, F64,
        BOOL, VOID, STR,
        POINTER, ARRAY, STRUCT, FUNCTION
    };
    
    Kind kind;
    std::shared_ptr<Type> elementType; // For pointers and arrays
    std::vector<std::shared_ptr<Type>> paramTypes; // For functions
    std::shared_ptr<Type> returnType; // For functions
    std::string structName; // For struct types
    int arraySize = -1; // For arrays (-1 means dynamic/unknown size)
    
    Type(Kind k) : kind(k) {}
    
    static std::shared_ptr<Type> getI32() { return std::make_shared<Type>(Kind::I32); }
    static std::shared_ptr<Type> getI64() { return std::make_shared<Type>(Kind::I64); }
    static std::shared_ptr<Type> getF64() { return std::make_shared<Type>(Kind::F64); }
    static std::shared_ptr<Type> getBool() { return std::make_shared<Type>(Kind::BOOL); }
    static std::shared_ptr<Type> getVoid() { return std::make_shared<Type>(Kind::VOID); }
    static std::shared_ptr<Type> getStr() { return std::make_shared<Type>(Kind::STR); }
};

// Expression nodes
class Expression : public ASTNode {
public:
    std::shared_ptr<Type> type;
};

class IntegerLiteral : public Expression {
public:
    int64_t value;
    IntegerLiteral(int64_t v) : value(v) {}
    void accept(ASTVisitor& visitor) override;
};

class FloatLiteral : public Expression {
public:
    double value;
    FloatLiteral(double v) : value(v) {}
    void accept(ASTVisitor& visitor) override;
};

class StringLiteral : public Expression {
public:
    std::string value;
    StringLiteral(const std::string& v) : value(v) {}
    void accept(ASTVisitor& visitor) override;
};

class BoolLiteral : public Expression {
public:
    bool value;
    BoolLiteral(bool v) : value(v) {}
    void accept(ASTVisitor& visitor) override;
};

class Identifier : public Expression {
public:
    std::string name;
    Identifier(const std::string& n) : name(n) {}
    void accept(ASTVisitor& visitor) override;
};

class BinaryOp : public Expression {
public:
    enum class Op {
        ADD, SUB, MUL, DIV, MOD,
        EQ, NE, LT, LE, GT, GE,
        AND, OR,
        BIT_AND, BIT_OR, BIT_XOR,
        SHL, SHR
    };
    
    Op op;
    std::shared_ptr<Expression> left;
    std::shared_ptr<Expression> right;
    
    BinaryOp(Op o, std::shared_ptr<Expression> l, std::shared_ptr<Expression> r)
        : op(o), left(l), right(r) {}
    void accept(ASTVisitor& visitor) override;
};

class UnaryOp : public Expression {
public:
    enum class Op { NEG, NOT, BIT_NOT };
    
    Op op;
    std::shared_ptr<Expression> operand;
    
    UnaryOp(Op o, std::shared_ptr<Expression> operand)
        : op(o), operand(operand) {}
    void accept(ASTVisitor& visitor) override;
};

class CallExpr : public Expression {
public:
    std::string functionName;
    std::vector<std::shared_ptr<Expression>> arguments;
    
    CallExpr(const std::string& name) : functionName(name) {}
    void accept(ASTVisitor& visitor) override;
};

// Statement nodes
class Statement : public ASTNode {
};

class VariableDecl : public Statement {
public:
    std::string name;
    std::shared_ptr<Type> varType;
    std::shared_ptr<Expression> initializer;
    bool isMutable;
    bool isPureLocal; // Behavior-aware access control
    
    VariableDecl(const std::string& n, std::shared_ptr<Type> t, bool mut = false, bool pureLocal = false)
        : name(n), varType(t), isMutable(mut), isPureLocal(pureLocal) {}
    void accept(ASTVisitor& visitor) override;
};

class Assignment : public Statement {
public:
    std::string name;
    std::shared_ptr<Expression> value;
    
    Assignment(const std::string& n, std::shared_ptr<Expression> v)
        : name(n), value(v) {}
    void accept(ASTVisitor& visitor) override;
};

class ReturnStmt : public Statement {
public:
    std::shared_ptr<Expression> value;
    
    ReturnStmt(std::shared_ptr<Expression> v = nullptr) : value(v) {}
    void accept(ASTVisitor& visitor) override;
};

class IfStmt : public Statement {
public:
    std::shared_ptr<Expression> condition;
    std::vector<std::shared_ptr<Statement>> thenBody;
    std::vector<std::shared_ptr<Statement>> elseBody;
    
    IfStmt(std::shared_ptr<Expression> cond) : condition(cond) {}
    void accept(ASTVisitor& visitor) override;
};

class WhileStmt : public Statement {
public:
    std::shared_ptr<Expression> condition;
    std::vector<std::shared_ptr<Statement>> body;
    
    WhileStmt(std::shared_ptr<Expression> cond) : condition(cond) {}
    void accept(ASTVisitor& visitor) override;
};

class ExprStmt : public Statement {
public:
    std::shared_ptr<Expression> expression;
    
    ExprStmt(std::shared_ptr<Expression> expr) : expression(expr) {}
    void accept(ASTVisitor& visitor) override;
};

// Function parameter
struct Parameter {
    std::string name;
    std::shared_ptr<Type> type;
    
    Parameter(const std::string& n, std::shared_ptr<Type> t)
        : name(n), type(t) {}
};

// Function declaration
class FunctionDecl : public ASTNode {
public:
    std::string name;
    std::vector<Parameter> parameters;
    std::shared_ptr<Type> returnType;
    std::vector<std::shared_ptr<Statement>> body;
    bool isPure; // Behavior-aware: pure function marker
    
    FunctionDecl(const std::string& n, bool pure = false)
        : name(n), isPure(pure) {}
    void accept(ASTVisitor& visitor) override;
};

// Program (top-level)
class Program : public ASTNode {
public:
    std::vector<std::shared_ptr<FunctionDecl>> functions;
    std::vector<std::shared_ptr<VariableDecl>> globals;
    
    void accept(ASTVisitor& visitor) override;
};

// Visitor pattern for AST traversal
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    virtual void visit(IntegerLiteral& node) = 0;
    virtual void visit(FloatLiteral& node) = 0;
    virtual void visit(StringLiteral& node) = 0;
    virtual void visit(BoolLiteral& node) = 0;
    virtual void visit(Identifier& node) = 0;
    virtual void visit(BinaryOp& node) = 0;
    virtual void visit(UnaryOp& node) = 0;
    virtual void visit(CallExpr& node) = 0;
    
    virtual void visit(VariableDecl& node) = 0;
    virtual void visit(Assignment& node) = 0;
    virtual void visit(ReturnStmt& node) = 0;
    virtual void visit(IfStmt& node) = 0;
    virtual void visit(WhileStmt& node) = 0;
    virtual void visit(ExprStmt& node) = 0;
    
    virtual void visit(FunctionDecl& node) = 0;
    virtual void visit(Program& node) = 0;
};

} // namespace hash

#endif // HASH_AST_H
