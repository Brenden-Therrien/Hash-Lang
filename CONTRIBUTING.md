# Contributing to Hash Language

Thank you for your interest in contributing to Hash! This document provides guidelines for contributing to the project.

## How to Contribute

### Reporting Bugs

If you find a bug, please create an issue with:

1. A clear, descriptive title
2. Steps to reproduce the bug
3. Expected behavior
4. Actual behavior
5. Hash code that demonstrates the issue
6. Your environment (OS, LLVM version, compiler version)

### Suggesting Features

We welcome feature suggestions! Please create an issue with:

1. A clear description of the feature
2. Use cases and examples
3. Why this feature would be valuable
4. Possible implementation approach (optional)

### Pull Requests

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes
4. Add tests if applicable
5. Ensure code compiles without warnings
6. Commit your changes (`git commit -m 'Add amazing feature'`)
7. Push to the branch (`git push origin feature/amazing-feature`)
8. Open a Pull Request

### Code Style

- Use C++17 features where appropriate
- Follow existing code formatting
- Use meaningful variable and function names
- Add comments for complex logic
- Keep functions focused and small

### Testing

- Test your changes with multiple Hash programs
- Verify that existing examples still work
- Test edge cases
- Check for memory leaks (if modifying core compiler)

## Development Setup

1. Install dependencies (LLVM, CMake, C++ compiler)
2. Clone the repository
3. Build using `.\build.ps1` (Windows) or CMake directly
4. Make changes to source files in `src/`
5. Rebuild and test

## Areas for Contribution

### High Priority

- [ ] Standard library functions (I/O, strings, etc.)
- [ ] Better error messages with source code context
- [ ] Struct types
- [ ] Enum types
- [ ] Pattern matching

### Medium Priority

- [ ] Module/import system
- [ ] Generic types
- [ ] Array types
- [ ] More comprehensive testing
- [ ] Documentation improvements

### Future Features

- [ ] Self-hosting compiler (compiler written in Hash)
- [ ] Package manager
- [ ] IDE/editor plugins
- [ ] Debugger integration
- [ ] Foreign function interface (FFI)

## Architecture Overview

### Compiler Pipeline

1. **Lexer** (`lexer.cpp`) - Tokenizes source code
2. **Parser** (`parser.cpp`) - Builds AST from tokens
3. **Semantic Analyzer** (`semantic.cpp`) - Type checking and pure function analysis
4. **Code Generator** (`codegen.cpp`) - Generates LLVM IR
5. **LLVM** - Optimizes and compiles to native code

### Key Files

- `src/lexer.h/cpp` - Lexical analysis
- `src/parser.h/cpp` - Syntax analysis
- `src/ast.h/cpp` - Abstract Syntax Tree definitions
- `src/semantic.h/cpp` - Semantic analysis and type checking
- `src/codegen.h/cpp` - LLVM IR generation
- `src/main.cpp` - Compiler driver

## Behavior-Aware Features

When working on features related to pure functions or pure_local variables:

1. Ensure the semantic analyzer properly checks constraints
2. Add tests for both valid and invalid cases
3. Provide clear error messages
4. Update documentation

## Questions?

Feel free to open an issue with your question or reach out to the maintainers.

## License

By contributing, you agree that your contributions will be licensed under the MIT License.
