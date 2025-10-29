#ifndef HASH_SEMANTIC_H
#define HASH_SEMANTIC_H

#include "ast.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

namespace hash {

// Symbol information
struct Symbol {
    std::string name;
    std::shared_ptr<Type> type;
    bool isMutable;
    bool isPureLocal;
    bool isParameter;
    
    Symbol() : name(""), type(nullptr), isMutable(false), isPureLocal(false), isParameter(false) {}
    Symbol(const std::string& n, std::shared_ptr<Type> t, bool mut = false, bool pureLocal = false, bool param = false)
        : name(n), type(t), isMutable(mut), isPureLocal(pureLocal), isParameter(param) {}
};

// Function information
struct FunctionInfo {
    std::string name;
    std::shared_ptr<Type> returnType;
    std::vector<std::shared_ptr<Type>> paramTypes;
    bool isPure;
    bool hasSideEffects; // Analyzed during semantic analysis
    
    FunctionInfo() : name(""), returnType(nullptr), isPure(false), hasSideEffects(false) {}
    FunctionInfo(const std::string& n, std::shared_ptr<Type> ret, bool pure = false)
        : name(n), returnType(ret), isPure(pure), hasSideEffects(false) {}
};

// Semantic analyzer with behavior-aware scope checking
class SemanticAnalyzer : public ASTVisitor {
public:
    struct ErrorInfo {
        std::string message;
        int line;
        int column;
        int length;
        std::string suggestion;
        
        ErrorInfo(const std::string& msg, int l = -1, int c = -1, int len = 1)
            : message(msg), line(l), column(c), length(len), suggestion("") {}
    };
    
    SemanticAnalyzer();
    
    bool analyze(Program& program);
    const std::vector<std::string>& getErrors() const { return errors; }
    const std::vector<std::string>& getWarnings() const { return warnings; }
    const std::vector<ErrorInfo>& getStructuredErrors() const { return structuredErrors; }
    const std::vector<ErrorInfo>& getStructuredWarnings() const { return structuredWarnings; }
    
    // Visitor methods
    void visit(IntegerLiteral& node) override;
    void visit(FloatLiteral& node) override;
    void visit(StringLiteral& node) override;
    void visit(BoolLiteral& node) override;
    void visit(Identifier& node) override;
    void visit(BinaryOp& node) override;
    void visit(UnaryOp& node) override;
    void visit(CallExpr& node) override;
    
    void visit(VariableDecl& node) override;
    void visit(Assignment& node) override;
    void visit(ReturnStmt& node) override;
    void visit(IfStmt& node) override;
    void visit(WhileStmt& node) override;
    void visit(ExprStmt& node) override;
    
    void visit(FunctionDecl& node) override;
    void visit(Program& node) override;
    
private:
    std::vector<std::unordered_map<std::string, Symbol>> scopes;
    std::unordered_map<std::string, FunctionInfo> functions;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    std::vector<ErrorInfo> structuredErrors;
    std::vector<ErrorInfo> structuredWarnings;
    
    FunctionInfo* currentFunction;
    bool currentFunctionHasSideEffects;
    std::unordered_set<std::string> modifiedVariables; // Track variables modified in current function
    
    void pushScope();
    void popScope();
    void declareVariable(const std::string& name, const Symbol& symbol);
    Symbol* lookupVariable(const std::string& name);
    FunctionInfo* lookupFunction(const std::string& name);
    
    void error(const std::string& message, int line = -1, int column = -1);
    void warning(const std::string& message, int line = -1, int column = -1);
    
    std::string typeToString(const std::shared_ptr<Type>& type);
    bool typesMatch(const std::shared_ptr<Type>& t1, const std::shared_ptr<Type>& t2);
    std::shared_ptr<Type> getCommonType(const std::shared_ptr<Type>& t1, const std::shared_ptr<Type>& t2);
    
    // Behavior-aware analysis
    void checkPureFunction(FunctionDecl& node);
    void checkPureLocalAccess(const std::string& varName, int line, int column);
    void markSideEffect(const std::string& reason);
};

} // namespace hash

#endif // HASH_SEMANTIC_H
