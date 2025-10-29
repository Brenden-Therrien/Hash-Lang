#include "error_reporter.h"
#include <iostream>
#include <sstream>
#include <iomanip>

namespace hash {

// ANSI color codes
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define GRAY    "\033[90m"

ErrorReporter::ErrorReporter(const std::string& sourceCode, const std::string& filename)
    : sourceCode(sourceCode), filename(filename) {}

void ErrorReporter::error(const std::string& message, int line, int column, int length) {
    diagnostics.emplace_back(DiagnosticLevel::Error, message, filename, line, column, length);
    errorCount++;
}

void ErrorReporter::warning(const std::string& message, int line, int column, int length) {
    diagnostics.emplace_back(DiagnosticLevel::Warning, message, filename, line, column, length);
    warningCount++;
}

void ErrorReporter::note(const std::string& message, int line, int column, int length) {
    diagnostics.emplace_back(DiagnosticLevel::Note, message, filename, line, column, length);
}

void ErrorReporter::addSuggestion(const std::string& suggestion) {
    if (!diagnostics.empty()) {
        diagnostics.back().suggestion = suggestion;
    }
}

std::string ErrorReporter::getSourceLine(int lineNumber) const {
    if (lineNumber < 1) return "";
    
    std::istringstream iss(sourceCode);
    std::string line;
    int currentLine = 1;
    
    while (std::getline(iss, line)) {
        if (currentLine == lineNumber) {
            return line;
        }
        currentLine++;
    }
    
    return "";
}

std::string ErrorReporter::createUnderline(int column, int length) const {
    if (column < 1) return "";
    
    std::string underline(column - 1, ' ');
    underline += BOLD RED;
    underline += std::string(length > 0 ? length : 1, '^');
    underline += RESET;
    
    return underline;
}

std::string ErrorReporter::getLevelString(DiagnosticLevel level) const {
    switch (level) {
        case DiagnosticLevel::Error:   return "error";
        case DiagnosticLevel::Warning: return "warning";
        case DiagnosticLevel::Note:    return "note";
        default: return "diagnostic";
    }
}

std::string ErrorReporter::getLevelColor(DiagnosticLevel level) const {
    switch (level) {
        case DiagnosticLevel::Error:   return BOLD RED;
        case DiagnosticLevel::Warning: return BOLD YELLOW;
        case DiagnosticLevel::Note:    return BOLD CYAN;
        default: return RESET;
    }
}

void ErrorReporter::printDiagnostic(const Diagnostic& diag) const {
    // Print: error: message
    std::cerr << getLevelColor(diag.level) << getLevelString(diag.level) << ": "
              << RESET BOLD << diag.message << RESET << "\n";
    
    // Print: --> filename:line:column
    if (diag.line >= 1) {
        std::cerr << BOLD BLUE << "  --> " << RESET 
                  << diag.filename << ":" << diag.line << ":" << diag.column << "\n";
        
        // Print the source line with line number
        std::string sourceLine = getSourceLine(diag.line);
        if (!sourceLine.empty()) {
            std::cerr << BOLD BLUE << std::setw(5) << diag.line << " | " << RESET
                      << sourceLine << "\n";
            
            // Print underline pointing to the error
            std::cerr << BOLD BLUE << "      | " << RESET
                      << createUnderline(diag.column, diag.length) << "\n";
        }
    }
    
    // Print suggestion if available
    if (!diag.suggestion.empty()) {
        std::cerr << BOLD CYAN << "  help: " << RESET << diag.suggestion << "\n";
    }
    
    std::cerr << "\n";
}

void ErrorReporter::printDiagnostics() const {
    if (diagnostics.empty()) return;
    
    for (const auto& diag : diagnostics) {
        printDiagnostic(diag);
    }
    
    // Print summary
    if (errorCount > 0 || warningCount > 0) {
        std::cerr << BOLD;
        if (errorCount > 0) {
            std::cerr << RED << "✗ " << errorCount << " error" 
                      << (errorCount > 1 ? "s" : "") << RESET;
        }
        if (errorCount > 0 && warningCount > 0) {
            std::cerr << ", ";
        }
        if (warningCount > 0) {
            std::cerr << BOLD YELLOW << "⚠ " << warningCount << " warning" 
                      << (warningCount > 1 ? "s" : "") << RESET;
        }
        std::cerr << " generated.\n";
    }
}

} // namespace hash
