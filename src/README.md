# Hash Compiler Source Code

This directory contains the implementation of the Hash language compiler.

## Files Overview

### Core Components

**lexer.h / lexer.cpp**
- Tokenizes Hash source code
- Handles Python-like indentation
- Manages keywords, operators, and literals
- Error position tracking

**parser.h / parser.cpp**
- Recursive descent parser
- Constructs Abstract Syntax Tree (AST)
- Operator precedence handling
- Syntax error recovery

**ast.h / ast.cpp**
- AST node definitions
- Visitor pattern for AST traversal
- Type representations
- Expression and statement nodes

**semantic.h / semantic.cpp**
- Type checking
- Symbol table management
- Pure function verification
- Behavior-aware access control enforcement
- Scope analysis

**codegen.h / codegen.cpp**
- LLVM IR generation
- Type mapping (Hash → LLVM)
- Control flow emission
- Function and variable code generation

**main.cpp**
- Compiler driver
- Command-line argument parsing
- Pipeline orchestration
- Error reporting with colors

## Data Flow

```
Source File
    ↓
[Lexer] → Tokens
    ↓
[Parser] → AST
    ↓
[Semantic Analyzer] → Validated AST
    ↓
[Code Generator] → LLVM IR
    ↓
LLVM Backend → Object File
    ↓
Clang++ → Executable
```

## Key Classes

### Lexer
- `Lexer::tokenize()` - Main entry point
- Tracks line/column for error messages
- Handles INDENT/DEDENT tokens for Python-like blocks

### Parser
- `Parser::parse()` - Returns Program AST
- Recursive descent with precedence climbing
- Error recovery with synchronization

### AST Nodes
- `Expression` - Base for all expressions
- `Statement` - Base for all statements
- `FunctionDecl` - Function declarations
- `Program` - Top-level AST node

### SemanticAnalyzer
- Implements `ASTVisitor` pattern
- Maintains scope stack
- Tracks pure function constraints
- Validates type compatibility

### CodeGenerator
- Implements `ASTVisitor` pattern
- Uses LLVM IR Builder
- Manages named values and functions
- Emits optimized code

## Building

The compiler is built using CMake. See the root CMakeLists.txt and build.ps1.

## Dependencies

- C++17 standard library
- LLVM 14.0+ (core, support, target, etc.)

## Error Handling

Each component reports errors differently:

**Lexer**: Invalid tokens, malformed strings
**Parser**: Syntax errors, unexpected tokens
**Semantic**: Type errors, undefined symbols, pure function violations
**CodeGen**: Code generation failures (rare)

## Extending the Compiler

### Adding a New Token Type
1. Add to `TokenType` enum in lexer.h
2. Update `Lexer::keywords` if it's a keyword
3. Handle in `Lexer::tokenize()`

### Adding a New AST Node
1. Define class in ast.h inheriting from Expression or Statement
2. Implement `accept()` method
3. Add visitor method to `ASTVisitor`
4. Implement visitor in semantic.cpp and codegen.cpp

### Adding a New Type
1. Add to `Type::Kind` enum in ast.h
2. Update `Parser::parseType()`
3. Handle in `SemanticAnalyzer::typesMatch()`
4. Map to LLVM type in `CodeGenerator::getLLVMType()`

### Adding a New Operator
1. Add token type to `TokenType` enum
2. Add to binary/unary op enums
3. Update parser precedence
4. Handle in semantic analyzer
5. Generate LLVM code in code generator

## Testing

To test changes:

```powershell
# Rebuild
cmake --build build --config Release

# Test with examples
.\build\Release\hashc.exe examples\hello.hash
.\build\Release\hashc.exe examples\fibonacci.hash

# Check LLVM IR output
.\build\Release\hashc.exe --emit-ir examples\pure_functions.hash
cat pure_functions.ll
```

## Code Style

- Use C++17 features
- Prefer `std::shared_ptr` for AST nodes
- Use meaningful variable names
- Comment complex algorithms
- Keep functions focused and small
- Use RAII for resource management

## Common Patterns

### AST Traversal
```cpp
void MyVisitor::visit(BinaryOp& node) {
    node.left->accept(*this);   // Visit left
    node.right->accept(*this);  // Visit right
    // Process this node
}
```

### Symbol Lookup
```cpp
Symbol* sym = lookupVariable(name);
if (!sym) {
    error("Undefined variable: " + name);
    return;
}
```

### LLVM Code Generation
```cpp
llvm::Value* leftVal = /* generate left */;
llvm::Value* rightVal = /* generate right */;
currentValue = builder->CreateAdd(leftVal, rightVal, "addtmp");
```

## Future Enhancements

Planned improvements to the compiler:

- [ ] Better error messages with source context
- [ ] Source location tracking in AST
- [ ] Constant folding in semantic pass
- [ ] Dead code elimination
- [ ] Optimization flags
- [ ] Debug symbol generation
- [ ] Incremental compilation support

## Resources

- [LLVM Documentation](https://llvm.org/docs/)
- [Kaleidoscope Tutorial](https://llvm.org/docs/tutorial/)
- [Crafting Interpreters](https://craftinginterpreters.com/)
- [Engineering a Compiler](https://www.elsevier.com/books/engineering-a-compiler/cooper/978-0-12-088478-0)
