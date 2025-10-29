# The Hash Language Story

## How It Started

My friend and I started coding a while back, and like many developers, we were fascinated by programming languages themselves. We'd discuss the tradeoffs between Python's readability and C's performance, debate the merits of different type systems, and wonder what it would be like to design our own language from scratch.

The idea kept coming up: what if we actually tried to build a programming language?

Not just a toy language or an interpreter for simple expressions, but a real compiler that could generate native executables. Something we could actually use to write real programs.

## The Challenge

We knew it wouldn't be easy. Compiler design is notoriously complex.. lexers, parsers, semantic analysis, code generation, optimization. But we were determined to try.

The vision was clear: combine Python's clean, readable syntax with C-level performance. We wanted a language that felt natural to write but compiled to efficient native code.

## Building Hash

In May 2025, we started seriously working on it. We started with the fundamentals:
- A lexer to tokenize source code
- A parser to build abstract syntax trees
- A semantic analyzer for type checking
- LLVM for code generation and optimization

The first milestone was simple: get "Hello, World" to compile and run. When we finally saw that first executable run successfully, we knew we were onto something.

## Adding Features

Once the foundation was solid, we started adding features:

**The Type System**: We implemented a strong static type system with integers, floats, booleans, and strings. Every variable has a type, catching errors at compile time rather than runtime.

**Control Flow**: If statements, while loops, and functions. The building blocks of any real program.

**Pure Functions**: This is where Hash became unique. We added a `pure` keyword that enforces functional purity at compile time. Pure functions can't have side effects, making them easier to test, reason about, and potentially parallelize.

**Built-in Functions**: We built up a standard library of 50 functions—everything from basic I/O and math operations to string manipulation and file I/O. Each one carefully implemented and tested.

**VS Code Integration**: We wanted a professional development experience, so we built a full VS Code extension with syntax highlighting, IntelliSense, and code snippets.

## The Real Test

The true test came when we started writing actual programs in Hash:
- Algorithms (sorting, searching, Fibonacci)
- Games (number guessing, interactive programs)
- Text processing tools
- File I/O utilities

Each program revealed new edge cases and opportunities for improvement. We refined error messages, added missing features, and polished the developer experience.

## What We Learned

Building a programming language taught us more than any tutorial ever could:

**Compiler Design**: We now deeply understand lexical analysis, parsing, type systems, and code generation. Reading about these concepts is one thing; implementing them is entirely different.

**LLVM**: Working with LLVM showed us how modern compilers generate optimized machine code. We learned about intermediate representations, optimization passes, and target-specific code generation.

**Language Design**: Every decision matters. How should type inference work? What should happen when you divide by zero? Should strings be mutable? These questions don't have obvious answers.

**Developer Experience**: A language is more than just syntax and semantics. The tooling, error messages, documentation, and examples all matter. We spent as much time on the CLI and VS Code extension as we did on the compiler itself.

## The Development Timeline

**May 2025**: Started the project. Built the lexer and parser, got our first "Hello, World" to compile.

**June-July 2025**: Added the type system, semantic analyzer, and basic built-in functions. Implemented the pure function system.

**August 2025**: Expanded to 30+ built-in functions. Created the first 15 example programs to test everything.

**September 2025**: Added string manipulation and file I/O. Built the VS Code extension with syntax highlighting and IntelliSense.

**October 2025**: Professional CLI tools, build output improvements, setup wizard, and final polish. Completed all 23 examples and comprehensive documentation.

**October 29, 2025**: Official v1.0.0 release.

Five months from first line of code to a complete, usable programming language.

## What We Built

Hash Language v1.0.0, released on October 29, 2025, is the culmination of this journey:

- A full compiler with LLVM backend
- 50 built-in functions
- 23 example programs
- Complete VS Code integration
- Professional command-line tools
- Comprehensive documentation

But more importantly, we proved to ourselves that we could do it. We went from "wouldn't it be cool to build a language" to actually using that language to write real programs.

## The Programs We Wrote

Using our own language to solve real problems was incredibly satisfying:

**Text Processor**: A program that reads files, processes text, and generates reports. It handles edge cases, validates input, and produces clean output—all written in Hash.

**Algorithms**: Classic computer science algorithms implemented and tested. Seeing our bubble sort or binary search compile to efficient machine code was a proud moment.

**Interactive Games**: Programs that read user input, maintain game state, and provide feedback. The number guessing game is simple, but it demonstrates Hash's capabilities.

**File Tools**: Utilities that create, read, and manipulate files. These aren't toy examples—they're genuinely useful programs.

## What's Next

Hash is complete as a 1.0 release, but the journey doesn't end here. There are features we'd love to add:
- Structs and enums for better data modeling
- A module system for organizing larger projects
- More advanced optimizations
- Cross-platform support (Linux, macOS)

But for now, we're proud of what we built. Hash is a real programming language with a real compiler that generates real executables. It has features we're genuinely excited about (pure functions!) and a development experience we're happy to use.

## For Anyone Thinking About Building a Language

If you're considering building your own programming language, our advice is simple: do it.

It will be harder than you expect. You'll encounter problems you never anticipated. You'll rewrite major components multiple times. You'll spend hours debugging obscure segfaults and LLVM errors.

But you'll also learn more than you thought possible. You'll gain a deep understanding of how computers actually work. You'll see your code compile to assembly and run on real hardware. And eventually, you'll write a program in your own language and watch it work.

That moment makes everything worth it.

## The Technical Stack

For those interested in the technical details:

**Compiler**: C++17 with LLVM 21.1.4
**Build System**: CMake
**Tooling**: PowerShell scripts for CLI and installation
**Editor Support**: VS Code extension with TextMate grammar
**Documentation**: Markdown with 15+ comprehensive guides
**Examples**: 23 working programs demonstrating all features

Everything is built from scratch except for LLVM, which handles the low-level code generation and optimization.

## Questions You Might Have

### Why not release every version on GitHub before the 1.0 release?

We wanted to get it right before going public. 

Early versions were messy. We made breaking changes constantly, rewrote major components, and had half-implemented features everywhere. The code worked, but it wasn't something we wanted to share yet.

We decided to focus on building rather than maintaining public releases. No pressure to maintain backward compatibility. No need to write migration guides for changes we'd probably undo next week. Just us, the code, and figuring things out.

By the time we hit 1.0, we had something stable, documented, and genuinely useful. That's what we wanted people's first impression to be.

Could we have done it differently? Sure. But for a project built by two people learning as we went, launching with a complete 1.0 felt right.

## Final Thoughts

Hash Language started as a conversation between me and my buddy: "Wouldn't it be cool if..."

It became months of learning, designing, coding, debugging, and refining.

And now it's a complete programming language that we can use to build real programs.

We set out to prove we could build a language. We ended up building something we're genuinely proud to use.

---