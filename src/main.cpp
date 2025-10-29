#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "codegen.h"
#include "error_reporter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'" << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void printError(const std::string& message, const std::string& filename = "") {
    std::cerr << "\033[1;31mError:\033[0m ";
    if (!filename.empty()) {
        std::cerr << filename << ": ";
    }
    std::cerr << message << std::endl;
}

void printWarning(const std::string& message, const std::string& filename = "") {
    std::cerr << "\033[1;33mWarning:\033[0m ";
    if (!filename.empty()) {
        std::cerr << filename << ": ";
    }
    std::cerr << message << std::endl;
}

void printSuccess(const std::string& message) {
    std::cout << "\033[1;32m!\033[0m " << message << std::endl;
}

void printUsage(const char* programName) {
    std::cout << "Hash Language Compiler\n";
    std::cout << "Usage: " << programName << " [options] <input.hash>\n\n";
    std::cout << "Options:\n";
    std::cout << "  -o <output>     Specify output file (default: a.out)\n";
    std::cout << "  --emit-llvm     Emit LLVM IR instead of object file\n";
    std::cout << "  --emit-ir       Save LLVM IR to file (.ll)\n";
    std::cout << "  --ast           Print AST and exit\n";
    std::cout << "  --tokens        Print tokens and exit\n";
    std::cout << "  -h, --help      Show this help message\n";
    std::cout << "\nExamples:\n";
    std::cout << "  " << programName << " program.hash\n";
    std::cout << "  " << programName << " -o program program.hash\n";
    std::cout << "  " << programName << " --emit-llvm program.hash\n";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string inputFile;
    #ifdef _WIN32
    std::string outputFile = "a.exe";
    #else
    std::string outputFile = "a.out";
    #endif
    bool emitLLVM = false;
    bool emitIR = false;
    bool printAST = false;
    bool printTokens = false;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-o") {
            if (i + 1 < argc) {
                outputFile = argv[++i];
            } else {
                printError("Expected output file after -o");
                return 1;
            }
        } else if (arg == "--emit-llvm") {
            emitLLVM = true;
        } else if (arg == "--emit-ir") {
            emitIR = true;
        } else if (arg == "--ast") {
            printAST = true;
        } else if (arg == "--tokens") {
            printTokens = true;
        } else if (arg[0] == '-') {
            printError("Unknown option: " + arg);
            return 1;
        } else {
            inputFile = arg;
        }
    }
    
    if (inputFile.empty()) {
        printError("No input file specified");
        printUsage(argv[0]);
        return 1;
    }
    
    // Read source file
    std::string source = readFile(inputFile);
    if (source.empty()) {
        return 1;
    }
    
    std::cout << "Compiling \033[1m" << inputFile << "\033[0m...\n" << std::endl;
    
    // Lexical analysis
    std::cout << "Lexical analysis..." << std::endl;
    hash::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    if (printTokens) {
        std::cout << "\nTokens:\n";
        for (const auto& token : tokens) {
            std::cout << "  " << static_cast<int>(token.type) << ": '"
                     << token.value << "' (line " << token.line << ")" << std::endl;
        }
        return 0;
    }
    
    printSuccess("Lexical analysis completed");
    
    // Parsing
    std::cout << "Parsing..." << std::endl;
    hash::Parser parser(tokens);
    auto program = parser.parse();
    
    if (!parser.getErrors().empty()) {
        std::cerr << "\n\033[1;31mParsing errors:\033[0m\n";
        for (const auto& error : parser.getErrors()) {
            printError(error, inputFile);
        }
        return 1;
    }
    
    printSuccess("Parsing completed");
    
    if (printAST) {
        std::cout << "\nAST generated successfully (visualization not implemented)" << std::endl;
        return 0;
    }
    
    // Semantic analysis
    std::cout << "Semantic analysis..." << std::endl;
    hash::SemanticAnalyzer analyzer;
    bool semanticSuccess = analyzer.analyze(*program);
    
    if (!semanticSuccess || !analyzer.getErrors().empty() || !analyzer.getWarnings().empty()) {
        std::cerr << "\n";
        
        hash::ErrorReporter reporter(source, inputFile);
        
        // Add structured warnings
        for (const auto& warning : analyzer.getStructuredWarnings()) {
            reporter.warning(warning.message, warning.line, warning.column, warning.length);
        }
        
        // Add structured errors
        for (const auto& error : analyzer.getStructuredErrors()) {
            reporter.error(error.message, error.line, error.column, error.length);
            if (!error.suggestion.empty()) {
                reporter.addSuggestion(error.suggestion);
            }
        }
        
        reporter.printDiagnostics();
        
        if (!semanticSuccess || !analyzer.getErrors().empty()) {
            return 1;
        }
    } else {
        printSuccess("Semantic analysis completed");
    }
    
    // Code generation
    std::cout << "Code generation..." << std::endl;
    hash::CodeGenerator codegen;
    
    std::string moduleName = fs::path(inputFile).stem().string();
    if (!codegen.generate(*program, moduleName)) {
        printError("Code generation failed");
        return 1;
    }
    
    printSuccess("Code generation completed");
    
    // Output
    if (emitLLVM || emitIR) {
        std::string irFile = outputFile;
        if (!emitLLVM) {
            irFile = moduleName + ".ll";
        }
        
        std::cout << "Emitting LLVM IR to " << irFile << "..." << std::endl;
        codegen.emitLLVMIR(irFile);
        printSuccess("LLVM IR emitted successfully");
    } else {
        std::string objFile = moduleName + ".o";
        std::cout << "Generating object file..." << std::endl;
        codegen.emitObjectFile(objFile);
        printSuccess("Object file generated: " + objFile);
        
        // Link with clang
        std::cout << "Linking..." << std::endl;
        #ifdef _WIN32
        std::string linkCmd = "clang++ " + objFile + " -o " + outputFile + " -llegacy_stdio_definitions";
        #else
        std::string linkCmd = "clang++ " + objFile + " -o " + outputFile;
        #endif
        int result = system(linkCmd.c_str());
        
        if (result == 0) {
            printSuccess("Executable created: " + outputFile);
            
            // Clean up object file
            fs::remove(objFile);
        } else {
            printError("Linking failed");
            return 1;
        }
    }
    
    std::cout << "\n\033[1;32mCompilation successful!\033[0m" << std::endl;
    return 0;
}
