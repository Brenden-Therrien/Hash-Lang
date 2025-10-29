# Hash Language Architecture Visualization

## Compilation Pipeline

```
┌─────────────────────────────────────────────────────────────────┐
│                        Hash Source Code (.hash)                 │
│                    Python-like syntax with types                │
└─────────────────────────┬───────────────────────────────────────┘
                          │
                          ▼
┌─────────────────────────────────────────────────────────────────┐
│                         LEXER (lexer.cpp)                       │
│  • Tokenization                                                 │
│  • Indentation tracking                                         │
│  • Comment removal                                              │
│  • String parsing                                               │
└─────────────────────────┬───────────────────────────────────────┘
                          │
                          ▼
                    Token Stream
                  (TokenType + Value)
                          │
                          ▼
┌─────────────────────────────────────────────────────────────────┐
│                       PARSER (parser.cpp)                       │
│  • Recursive descent parsing                                    │
│  • Operator precedence                                          │
│  • AST construction                                             │
│  • Syntax error detection                                       │
└─────────────────────────┬───────────────────────────────────────┘
                          │
                          ▼
                   Abstract Syntax Tree
                   (AST nodes: ast.h)
                          │
                          ▼
┌─────────────────────────────────────────────────────────────────┐
│                 SEMANTIC ANALYZER (semantic.cpp)                │
│  • Type checking                                                │
│  • Symbol table management                                      │
│  • Pure function verification ⚡                                │
│  • pure_local access checking ⚡                                │
│  • Scope analysis                                               │
└─────────────────────────┬───────────────────────────────────────┘
                          │
                          ▼
                  Validated & Typed AST
                          │
                          ▼
┌─────────────────────────────────────────────────────────────────┐
│                  CODE GENERATOR (codegen.cpp)                   │
│  • LLVM IR generation                                           │
│  • Type mapping                                                 │
│  • Control flow translation                                     │
│  • Function emission                                            │
└─────────────────────────┬───────────────────────────────────────┘
                          │
                          ▼
                      LLVM IR (.ll)
                          │
                          ▼
┌─────────────────────────────────────────────────────────────────┐
│                      LLVM Optimizer                             │
│  • Optimization passes                                          │
│  • Dead code elimination                                        │
│  • Inlining                                                     │
│  • Constant folding                                             │
└─────────────────────────┬───────────────────────────────────────┘
                          │
                          ▼
                    Optimized LLVM IR
                          │
                          ▼
┌─────────────────────────────────────────────────────────────────┐
│                     LLVM Code Generator                         │
│  • Target-specific code generation                              │
│  • Register allocation                                          │
│  • Instruction selection                                        │
└─────────────────────────┬───────────────────────────────────────┘
                          │
                          ▼
                    Object File (.o)
                          │
                          ▼
┌─────────────────────────────────────────────────────────────────┐
│                      Clang++ Linker                             │
│  • Symbol resolution                                            │
│  • Library linking                                              │
│  • Executable generation                                        │
└─────────────────────────┬───────────────────────────────────────┘
                          │
                          ▼
                  Native Executable (.exe)
```

⚡ = Unique Hash features (behavior-aware access control)

## Component Interaction

```
┌──────────────┐
│   main.cpp   │  Compiler Driver
│              │
│  • Args      │──────┐
│  • File I/O  │      │
│  • Pipeline  │      │
└──────────────┘      │
                      │
                      ▼
        ┌─────────────────────────┐
        │  Compilation Pipeline   │
        └─────────────────────────┘
                 │
    ┌────────────┼────────────┐
    │            │            │
    ▼            ▼            ▼
┌────────┐  ┌─────────┐  ┌──────────┐
│ Lexer  │→ │ Parser  │→ │ Semantic │
└────────┘  └─────────┘  └──────────┘
                              │
                              ▼
                         ┌──────────┐
                         │ CodeGen  │
                         └──────────┘
                              │
                              ▼
                         ┌──────────┐
                         │   LLVM   │
                         └──────────┘
```

## Type System Hierarchy

```
┌─────────────────────────────────────┐
│           Hash Types                │
└─────────────────────────────────────┘
                 │
    ┌────────────┼────────────┐
    │            │            │
    ▼            ▼            ▼
┌─────────┐ ┌─────────┐ ┌─────────┐
│ Numeric │ │ Boolean │ │  Other  │
└─────────┘ └─────────┘ └─────────┘
    │            │            │
    │            │            └─── void, str
    │            │
    │            └─── bool (true/false)
    │
    ├─── Signed Integers
    │    ├─── i8  (8-bit)
    │    ├─── i16 (16-bit)
    │    ├─── i32 (32-bit)
    │    └─── i64 (64-bit)
    │
    ├─── Unsigned Integers
    │    ├─── u8  (8-bit)
    │    ├─── u16 (16-bit)
    │    ├─── u32 (32-bit)
    │    └─── u64 (64-bit)
    │
    └─── Floating Point
         ├─── f32 (single precision)
         └─── f64 (double precision)
```

## Behavior-Aware Access Control Model

```
┌────────────────────────────────────────────────────────────┐
│                    FUNCTION BEHAVIOR                       │
└────────────────────────────────────────────────────────────┘
                          │
              ┌───────────┴───────────┐
              │                       │
              ▼                       ▼
    ┌─────────────────┐     ┌─────────────────┐
    │  Pure Function  │     │ Regular Function │
    │  (pure keyword) │     │  (no keyword)    │
    └─────────────────┘     └─────────────────┘
              │                       │
              │                       │
    ┌─────────┴─────────┐   ┌─────────┴─────────┐
    │ CAN:              │   │ CAN:              │
    │ • Read any var    │   │ • Read normal vars│
    │ • Read pure_local │   │ • Modify mut vars │
    │ • Call pure fn    │   │ • Call any fn     │
    │ • Return values   │   │ • Side effects    │
    └───────────────────┘   └───────────────────┘
    ┌─────────┴─────────┐   ┌─────────┴─────────┐
    │ CANNOT:           │   │ CANNOT:            │
    │ • Modify any var  │   │ • Read pure_local  │
    │ • Call impure fn  │   │ • Modify pure_local│
    │ • Side effects    │   │                    │
    └───────────────────┘   └────────────────────┘

┌────────────────────────────────────────────────────────────┐
│                    VARIABLE ACCESS                         │
└────────────────────────────────────────────────────────────┘
                          │
          ┌───────────────┼───────────────┐
          │               │               │
          ▼               ▼               ▼
    ┌──────────┐   ┌──────────┐   ┌──────────┐
    │  Normal  │   │   mut    │   │pure_local│
    │ Variable │   │ Variable │   │ Variable │
    └──────────┘   └──────────┘   └──────────┘
          │               │               │
          │               │               │
    Readable by     Modifiable      Only accessible
    any function    by impure        by pure
                    functions        functions
```

## Symbol Table Structure

```
┌─────────────────────────────────────────┐
│          Global Scope                   │
│  • Functions                            │
│  • Global variables                     │
└─────────────────┬───────────────────────┘
                  │
      ┌───────────┼───────────┐
      │                       │
      ▼                       ▼
┌──────────────┐      ┌──────────────┐
│  Function 1  │      │  Function 2  │
│  Scope       │      │  Scope       │
│              │      │              │
│  • Params    │      │  • Params    │
│  • Locals    │      │  • Locals    │
└──────┬───────┘      └──────┬───────┘
       │                     │
       ▼                     ▼
   ┌────────┐           ┌────────┐
   │ Block  │           │ Block  │
   │ Scope  │           │ Scope  │
   └────────┘           └────────┘
```

## Example: Compilation Flow

```
Source:                  Tokens:              AST:

fn add(a, b):           FN                   FunctionDecl
    return a + b        IDENTIFIER(add)      ├─ name: "add"
                        LPAREN               ├─ params:
                        IDENTIFIER(a)        │  ├─ a: i32
                        COMMA                │  └─ b: i32
                        IDENTIFIER(b)        ├─ return: i32
                        RPAREN               └─ body:
                        COLON                   └─ ReturnStmt
                        INDENT                     └─ BinaryOp(+)
                        RETURN                        ├─ left: a
                        IDENTIFIER(a)                 └─ right: b
                        PLUS
                        IDENTIFIER(b)
                        DEDENT

LLVM IR:                                 Assembly:

define i32 @add(i32 %a, i32 %b) {       add:
entry:                                      mov eax, edi
  %0 = add i32 %a, %b                       add eax, esi
  ret i32 %0                                ret
}
```

## Project File Organization

```
HashLang/
│
├── src/                    Core compiler implementation
│   ├── lexer.{h,cpp}      Tokenization
│   ├── parser.{h,cpp}     Parsing & AST
│   ├── ast.{h,cpp}        AST definitions
│   ├── semantic.{h,cpp}   Type checking & analysis
│   ├── codegen.{h,cpp}    LLVM code generation
│   └── main.cpp           Compiler driver
│
├── examples/              Sample programs
│   ├── hello.hash
│   ├── fibonacci.hash
│   ├── pure_functions.hash
│   ├── types.hash
│   └── comprehensive.hash
│
├── Documentation
│   ├── README.md          Main docs
│   ├── QUICKSTART.md      Getting started
│   ├── SPEC.md            Language spec
│   ├── SUMMARY.md         Project overview
│   └── ARCHITECTURE.md    This file
│
├── Build System
│   ├── CMakeLists.txt     CMake config
│   └── build.ps1          Build script
│
└── Project Meta
    ├── LICENSE            MIT License
    ├── CONTRIBUTING.md    Contribution guide
    └── .gitignore         Git ignore rules
```

---

This architecture enables Hash to provide:
- **Low-level control** through LLVM backend
- **High-level expressiveness** through Python-like syntax
- **Safety guarantees** through behavior-aware access control
- **Performance** through native compilation and optimization
