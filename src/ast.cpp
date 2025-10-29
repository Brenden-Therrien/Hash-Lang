#include "ast.h"

namespace hash {

// Expression implementations
void IntegerLiteral::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void FloatLiteral::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void StringLiteral::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void BoolLiteral::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void Identifier::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void BinaryOp::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void UnaryOp::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void CallExpr::accept(ASTVisitor& visitor) { visitor.visit(*this); }

// Statement implementations
void VariableDecl::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void Assignment::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void ReturnStmt::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void IfStmt::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void WhileStmt::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void ExprStmt::accept(ASTVisitor& visitor) { visitor.visit(*this); }

// Function and Program implementations
void FunctionDecl::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void Program::accept(ASTVisitor& visitor) { visitor.visit(*this); }

} // namespace hash
