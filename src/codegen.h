#ifndef HASH_CODEGEN_H
#define HASH_CODEGEN_H

#include "ast.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <unordered_map>
#include <string>

namespace hash {

class CodeGenerator : public ASTVisitor {
public:
    CodeGenerator();
    ~CodeGenerator();
    
    bool generate(Program& program, const std::string& moduleName);
    void emitObjectFile(const std::string& filename);
    void emitLLVMIR(const std::string& filename);
    llvm::Module* getModule() { return module.get(); }
    
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
    std::unique_ptr<llvm::LLVMContext> context;
    std::unique_ptr<llvm::Module> module;
    std::unique_ptr<llvm::IRBuilder<>> builder;
    
    std::unordered_map<std::string, llvm::AllocaInst*> namedValues;
    std::unordered_map<std::string, llvm::Function*> functions;
    
    llvm::Value* currentValue;
    llvm::Function* currentFunction;
    
    llvm::Type* getLLVMType(const std::shared_ptr<Type>& type);
    llvm::AllocaInst* createEntryBlockAlloca(llvm::Function* function, const std::string& varName, llvm::Type* type);
};

} // namespace hash

#endif // HASH_CODEGEN_H
