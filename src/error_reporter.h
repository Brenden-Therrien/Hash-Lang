#pragma once
#include <string>
#include <vector>
#include <memory>

namespace hash {

enum class DiagnosticLevel {
    Error,
    Warning,
    Note
};

struct Diagnostic {
    DiagnosticLevel level;
    std::string message;
    std::string filename;
    int line;
    int column;
    int length;  // Length of the problematic token/expression
    std::string suggestion;  // Optional helpful suggestion
    
    Diagnostic(DiagnosticLevel level, const std::string& message, 
               const std::string& filename = "", int line = -1, int column = -1, int length = 1)
        : level(level), message(message), filename(filename), 
          line(line), column(column), length(length) {}
};

class ErrorReporter {
public:
    ErrorReporter(const std::string& sourceCode, const std::string& filename);
    
    void error(const std::string& message, int line, int column, int length = 1);
    void warning(const std::string& message, int line, int column, int length = 1);
    void note(const std::string& message, int line, int column, int length = 1);
    
    void addSuggestion(const std::string& suggestion);
    
    void printDiagnostics() const;
    void printDiagnostic(const Diagnostic& diag) const;
    
    bool hasErrors() const { return errorCount > 0; }
    int getErrorCount() const { return errorCount; }
    int getWarningCount() const { return warningCount; }
    
    const std::vector<Diagnostic>& getDiagnostics() const { return diagnostics; }
    
private:
    std::string sourceCode;
    std::string filename;
    std::vector<Diagnostic> diagnostics;
    int errorCount = 0;
    int warningCount = 0;
    
    std::string getSourceLine(int lineNumber) const;
    std::string createUnderline(int column, int length) const;
    std::string getLevelString(DiagnosticLevel level) const;
    std::string getLevelColor(DiagnosticLevel level) const;
};

} // namespace hash
