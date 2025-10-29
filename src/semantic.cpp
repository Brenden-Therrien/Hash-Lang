#include "semantic.h"
#include <sstream>
#include <algorithm>

namespace hash {

SemanticAnalyzer::SemanticAnalyzer()
    : currentFunction(nullptr), currentFunctionHasSideEffects(false) {}

bool SemanticAnalyzer::analyze(Program& program) {
    errors.clear();
    warnings.clear();
    
    program.accept(*this);
    
    return errors.empty();
}

void SemanticAnalyzer::visit(Program& node) {
    pushScope(); // Global scope
    
    // Register built-in functions
    FunctionInfo printI32Info("print_i32", Type::getVoid(), false);
    printI32Info.paramTypes = {Type::getI32()};
    functions["print_i32"] = printI32Info;
    
    FunctionInfo printI64Info("print_i64", Type::getVoid(), false);
    printI64Info.paramTypes = {Type::getI64()};
    functions["print_i64"] = printI64Info;
    
    FunctionInfo printF64Info("print_f64", Type::getVoid(), false);
    printF64Info.paramTypes = {Type::getF64()};
    functions["print_f64"] = printF64Info;
    
    FunctionInfo printBoolInfo("print_bool", Type::getVoid(), false);
    printBoolInfo.paramTypes = {Type::getBool()};
    functions["print_bool"] = printBoolInfo;
    
    FunctionInfo printStrInfo("print_str", Type::getVoid(), false);
    printStrInfo.paramTypes = {Type::getStr()};
    functions["print_str"] = printStrInfo;
    
    FunctionInfo printlnInfo("println", Type::getVoid(), false);
    printlnInfo.paramTypes = {};
    functions["println"] = printlnInfo;
    
    // Type conversion built-ins
    FunctionInfo i32ToI64Info("i32_to_i64", Type::getI64(), true);
    i32ToI64Info.paramTypes = {Type::getI32()};
    functions["i32_to_i64"] = i32ToI64Info;
    
    FunctionInfo i64ToI32Info("i64_to_i32", Type::getI32(), true);
    i64ToI32Info.paramTypes = {Type::getI64()};
    functions["i64_to_i32"] = i64ToI32Info;
    
    FunctionInfo i32ToF64Info("i32_to_f64", Type::getF64(), true);
    i32ToF64Info.paramTypes = {Type::getI32()};
    functions["i32_to_f64"] = i32ToF64Info;
    
    FunctionInfo f64ToI32Info("f64_to_i32", Type::getI32(), true);
    f64ToI32Info.paramTypes = {Type::getF64()};
    functions["f64_to_i32"] = f64ToI32Info;
    
    FunctionInfo i64ToF64Info("i64_to_f64", Type::getF64(), true);
    i64ToF64Info.paramTypes = {Type::getI64()};
    functions["i64_to_f64"] = i64ToF64Info;
    
    FunctionInfo f64ToI64Info("f64_to_i64", Type::getI64(), true);
    f64ToI64Info.paramTypes = {Type::getF64()};
    functions["f64_to_i64"] = f64ToI64Info;
    
    // Math built-ins
    FunctionInfo absI32Info("abs_i32", Type::getI32(), true);
    absI32Info.paramTypes = {Type::getI32()};
    functions["abs_i32"] = absI32Info;
    
    FunctionInfo minI32Info("min_i32", Type::getI32(), true);
    minI32Info.paramTypes = {Type::getI32(), Type::getI32()};
    functions["min_i32"] = minI32Info;
    
    FunctionInfo maxI32Info("max_i32", Type::getI32(), true);
    maxI32Info.paramTypes = {Type::getI32(), Type::getI32()};
    functions["max_i32"] = maxI32Info;
    
    FunctionInfo sqrtF64Info("sqrt_f64", Type::getF64(), true);
    sqrtF64Info.paramTypes = {Type::getF64()};
    functions["sqrt_f64"] = sqrtF64Info;
    
    // Python-like aliases for better ergonomics
    // print() - overloaded versions with shorter names
    functions["print"] = printI32Info;  // Default to i32
    
    // Simpler conversion names (just use int(), float(), etc.)
    FunctionInfo intInfo("int", Type::getI32(), true);
    intInfo.paramTypes = {Type::getF64()};
    functions["int"] = intInfo;  // float to int conversion
    
    FunctionInfo floatInfo("float", Type::getF64(), true);
    floatInfo.paramTypes = {Type::getI32()};
    functions["float"] = floatInfo;  // int to float conversion
    
    // Simpler math names
    FunctionInfo absInfo("abs", Type::getI32(), true);
    absInfo.paramTypes = {Type::getI32()};
    functions["abs"] = absInfo;
    
    FunctionInfo minInfo("min", Type::getI32(), true);
    minInfo.paramTypes = {Type::getI32(), Type::getI32()};
    functions["min"] = minInfo;
    
    FunctionInfo maxInfo("max", Type::getI32(), true);
    maxInfo.paramTypes = {Type::getI32(), Type::getI32()};
    functions["max"] = maxInfo;
    
    FunctionInfo sqrtInfo("sqrt", Type::getF64(), true);
    sqrtInfo.paramTypes = {Type::getF64()};
    functions["sqrt"] = sqrtInfo;
    
    // Input functions
    FunctionInfo readI32Info("read_i32", Type::getI32(), false);
    readI32Info.paramTypes = {};
    functions["read_i32"] = readI32Info;
    
    FunctionInfo readF64Info("read_f64", Type::getF64(), false);
    readF64Info.paramTypes = {};
    functions["read_f64"] = readF64Info;
    
    // Additional Math Functions
    FunctionInfo powInfo("pow", Type::getF64(), true);
    powInfo.paramTypes = {Type::getF64(), Type::getF64()};
    functions["pow"] = powInfo;
    
    FunctionInfo floorInfo("floor", Type::getF64(), true);
    floorInfo.paramTypes = {Type::getF64()};
    functions["floor"] = floorInfo;
    
    FunctionInfo ceilInfo("ceil", Type::getF64(), true);
    ceilInfo.paramTypes = {Type::getF64()};
    functions["ceil"] = ceilInfo;
    
    FunctionInfo roundInfo("round", Type::getF64(), true);
    roundInfo.paramTypes = {Type::getF64()};
    functions["round"] = roundInfo;
    
    // Trigonometric functions
    FunctionInfo sinInfo("sin", Type::getF64(), true);
    sinInfo.paramTypes = {Type::getF64()};
    functions["sin"] = sinInfo;
    
    FunctionInfo cosInfo("cos", Type::getF64(), true);
    cosInfo.paramTypes = {Type::getF64()};
    functions["cos"] = cosInfo;
    
    FunctionInfo tanInfo("tan", Type::getF64(), true);
    tanInfo.paramTypes = {Type::getF64()};
    functions["tan"] = tanInfo;
    
    FunctionInfo asinInfo("asin", Type::getF64(), true);
    asinInfo.paramTypes = {Type::getF64()};
    functions["asin"] = asinInfo;
    
    FunctionInfo acosInfo("acos", Type::getF64(), true);
    acosInfo.paramTypes = {Type::getF64()};
    functions["acos"] = acosInfo;
    
    FunctionInfo atanInfo("atan", Type::getF64(), true);
    atanInfo.paramTypes = {Type::getF64()};
    functions["atan"] = atanInfo;
    
    // Logarithmic and exponential functions
    FunctionInfo expInfo("exp", Type::getF64(), true);
    expInfo.paramTypes = {Type::getF64()};
    functions["exp"] = expInfo;
    
    FunctionInfo logInfo("log", Type::getF64(), true);
    logInfo.paramTypes = {Type::getF64()};
    functions["log"] = logInfo;
    
    FunctionInfo log2Info("log2", Type::getF64(), true);
    log2Info.paramTypes = {Type::getF64()};
    functions["log2"] = log2Info;
    
    FunctionInfo log10Info("log10", Type::getF64(), true);
    log10Info.paramTypes = {Type::getF64()};
    functions["log10"] = log10Info;
    
    // System functions
    FunctionInfo exitInfo("exit", Type::getVoid(), false);
    exitInfo.paramTypes = {Type::getI32()};
    functions["exit"] = exitInfo;
    
    FunctionInfo hashTimeInfo("hash_time", Type::getI64(), false);
    hashTimeInfo.paramTypes = {};
    functions["hash_time"] = hashTimeInfo;
    
    FunctionInfo hashClockInfo("hash_clock", Type::getF64(), false);
    hashClockInfo.paramTypes = {};
    functions["hash_clock"] = hashClockInfo;
    
    FunctionInfo randomInfo("random", Type::getF64(), false);
    randomInfo.paramTypes = {};
    functions["random"] = randomInfo;
    
    FunctionInfo seedRandomInfo("seed_random", Type::getVoid(), false);
    seedRandomInfo.paramTypes = {Type::getI32()};
    functions["seed_random"] = seedRandomInfo;
    
    FunctionInfo randomRangeInfo("random_range", Type::getI32(), false);
    randomRangeInfo.paramTypes = {Type::getI32(), Type::getI32()};
    functions["random_range"] = randomRangeInfo;
    
    // String manipulation functions
    FunctionInfo lenInfo("len", Type::getI32(), true);  // Pure - just reads length
    lenInfo.paramTypes = {Type::getStr()};
    functions["len"] = lenInfo;
    
    FunctionInfo strConcatInfo("str_concat", Type::getStr(), true);  // Pure - creates new string
    strConcatInfo.paramTypes = {Type::getStr(), Type::getStr()};
    functions["str_concat"] = strConcatInfo;
    
    FunctionInfo strEqInfo("str_eq", Type::getBool(), true);  // Pure - just compares
    strEqInfo.paramTypes = {Type::getStr(), Type::getStr()};
    functions["str_eq"] = strEqInfo;
    
    FunctionInfo upperInfo("upper", Type::getStr(), true);  // Pure - creates new string
    upperInfo.paramTypes = {Type::getStr()};
    functions["upper"] = upperInfo;
    
    FunctionInfo lowerInfo("lower", Type::getStr(), true);  // Pure - creates new string
    lowerInfo.paramTypes = {Type::getStr()};
    functions["lower"] = lowerInfo;
    
    // File I/O functions
    FunctionInfo fileReadInfo("file_read", Type::getStr(), false);  // Impure - reads from disk
    fileReadInfo.paramTypes = {Type::getStr()};
    functions["file_read"] = fileReadInfo;
    
    FunctionInfo fileWriteInfo("file_write", Type::getBool(), false);  // Impure - writes to disk
    fileWriteInfo.paramTypes = {Type::getStr(), Type::getStr()};
    functions["file_write"] = fileWriteInfo;
    
    FunctionInfo fileExistsInfo("file_exists", Type::getBool(), false);  // Impure - checks filesystem
    fileExistsInfo.paramTypes = {Type::getStr()};
    functions["file_exists"] = fileExistsInfo;
    
    FunctionInfo fileDeleteInfo("file_delete", Type::getBool(), false);  // Impure - modifies filesystem
    fileDeleteInfo.paramTypes = {Type::getStr()};
    functions["file_delete"] = fileDeleteInfo;
    
    // First pass: collect all function signatures
    for (auto& func : node.functions) {
        std::vector<std::shared_ptr<Type>> paramTypes;
        for (auto& param : func->parameters) {
            paramTypes.push_back(param.type);
        }
        
        FunctionInfo info(func->name, func->returnType, func->isPure);
        info.paramTypes = paramTypes;
        
        if (functions.find(func->name) != functions.end()) {
            error("Function '" + func->name + "' already declared", func->line, func->column);
        } else {
            functions[func->name] = info;
        }
    }
    
    // Declare global variables
    for (auto& global : node.globals) {
        global->accept(*this);
    }
    
    // Second pass: analyze function bodies
    for (auto& func : node.functions) {
        func->accept(*this);
    }
    
    popScope();
}

void SemanticAnalyzer::visit(FunctionDecl& node) {
    currentFunction = &functions[node.name];
    currentFunctionHasSideEffects = false;
    modifiedVariables.clear();
    
    pushScope(); // Function scope
    
    // Declare parameters
    for (auto& param : node.parameters) {
        Symbol symbol(param.name, param.type, false, false, true);
        declareVariable(param.name, symbol);
    }
    
    // Analyze body
    for (auto& stmt : node.body) {
        stmt->accept(*this);
    }
    
    // Check if pure function has side effects
    if (node.isPure && currentFunctionHasSideEffects) {
        error("Pure function '" + node.name + "' has side effects", node.line, node.column);
    }
    
    // Update function info
    currentFunction->hasSideEffects = currentFunctionHasSideEffects;
    
    popScope();
    currentFunction = nullptr;
}

void SemanticAnalyzer::visit(VariableDecl& node) {
    // Check if variable already declared in current scope
    if (!scopes.empty() && scopes.back().find(node.name) != scopes.back().end()) {
        error("Variable '" + node.name + "' already declared in this scope", node.line, node.column);
        return;
    }
    
    // Analyze initializer
    if (node.initializer) {
        node.initializer->accept(*this);
        
        // Type check
        if (node.initializer->type && !typesMatch(node.varType, node.initializer->type)) {
            error("Type mismatch in variable initialization: expected " + 
                  typeToString(node.varType) + ", got " + typeToString(node.initializer->type),
                  node.line, node.column);
            
            // Add helpful suggestion based on the type mismatch
            std::string initType = typeToString(node.initializer->type);
            std::string expectedType = typeToString(node.varType);
            
            if (initType == "i32" && expectedType == "i64") {
                structuredErrors.back().suggestion = "Change the variable type to 'i32', or cast the value to i64";
            } else if (initType == "i64" && expectedType == "i32") {
                structuredErrors.back().suggestion = "Change the variable type to 'i64', or ensure the value fits in i32 range";
            } else {
                structuredErrors.back().suggestion = "Change the variable type to '" + initType + "' or provide a value of type '" + expectedType + "'";
            }
        }
    }
    
    // Declare variable
    Symbol symbol(node.name, node.varType, node.isMutable, node.isPureLocal);
    declareVariable(node.name, symbol);
}

void SemanticAnalyzer::visit(Assignment& node) {
    // Look up variable
    Symbol* symbol = lookupVariable(node.name);
    if (!symbol) {
        error("Undefined variable '" + node.name + "'", node.line, node.column);
        structuredErrors.back().suggestion = "Make sure '" + node.name + "' is declared before use, or check for typos";
        return;
    }
    
    // Check if variable is mutable
    if (!symbol->isMutable && !symbol->isParameter) {
        error("Cannot assign to immutable variable '" + node.name + "'", node.line, node.column);
        structuredErrors.back().suggestion = "Declare the variable as mutable with 'let mut " + node.name + ": <type>' instead of 'let " + node.name + ": <type>'";
    }
    
    // Check pure_local access
    if (symbol->isPureLocal && currentFunction && !currentFunction->isPure) {
        error("Cannot modify pure_local variable '" + node.name + "' in non-pure function", 
              node.line, node.column);
        structuredErrors.back().suggestion = "Pure_local variables can only be modified by pure functions. Mark this function as 'pure fn' or use a regular variable";
    }
    
    // Analyze value
    node.value->accept(*this);
    
    // Type check
    if (node.value->type && !typesMatch(symbol->type, node.value->type)) {
        error("Type mismatch in assignment to '" + node.name + "': expected " + 
              typeToString(symbol->type) + ", got " + typeToString(node.value->type), 
              node.line, node.column);
        structuredErrors.back().suggestion = "Ensure the assigned value matches the variable's type '" + typeToString(symbol->type) + "'";
    }
    
    // Mark as side effect
    markSideEffect("Assignment to variable '" + node.name + "'");
    modifiedVariables.insert(node.name);
}

void SemanticAnalyzer::visit(ReturnStmt& node) {
    if (!currentFunction) {
        error("Return statement outside of function", node.line, node.column);
        structuredErrors.back().suggestion = "Return statements can only be used inside functions";
        return;
    }
    
    if (node.value) {
        node.value->accept(*this);
        
        // Type check
        if (node.value->type && !typesMatch(currentFunction->returnType, node.value->type)) {
            error("Return type mismatch: expected " + typeToString(currentFunction->returnType) + 
                  ", got " + typeToString(node.value->type), node.line, node.column);
            
            std::string returnType = typeToString(node.value->type);
            std::string expectedType = typeToString(currentFunction->returnType);
            structuredErrors.back().suggestion = "Change the return value to type '" + expectedType + "', or change the function's return type to '" + returnType + "'";
        }
    } else {
        // Check if function expects void
        if (currentFunction->returnType->kind != Type::Kind::VOID) {
            error("Function expects return value of type " + typeToString(currentFunction->returnType), 
                  node.line, node.column);
            structuredErrors.back().suggestion = "Add a return value: 'return <value>', or change function return type to 'void'";
        }
    }
}

void SemanticAnalyzer::visit(IfStmt& node) {
    node.condition->accept(*this);
    
    // Check condition is bool
    if (node.condition->type && node.condition->type->kind != Type::Kind::BOOL) {
        warning("If condition should be of type bool");
    }
    
    for (auto& stmt : node.thenBody) {
        stmt->accept(*this);
    }
    
    for (auto& stmt : node.elseBody) {
        stmt->accept(*this);
    }
}

void SemanticAnalyzer::visit(WhileStmt& node) {
    node.condition->accept(*this);
    
    // Check condition is bool
    if (node.condition->type && node.condition->type->kind != Type::Kind::BOOL) {
        warning("While condition should be of type bool");
    }
    
    for (auto& stmt : node.body) {
        stmt->accept(*this);
    }
}

void SemanticAnalyzer::visit(ExprStmt& node) {
    node.expression->accept(*this);
}

void SemanticAnalyzer::visit(IntegerLiteral& node) {
    // Type already set by parser
}

void SemanticAnalyzer::visit(FloatLiteral& node) {
    // Type already set by parser
}

void SemanticAnalyzer::visit(StringLiteral& node) {
    // Type already set by parser
}

void SemanticAnalyzer::visit(BoolLiteral& node) {
    // Type already set by parser
}

void SemanticAnalyzer::visit(Identifier& node) {
    Symbol* symbol = lookupVariable(node.name);
    if (!symbol) {
        ErrorInfo err("Undefined variable '" + node.name + "'", node.line, node.column);
        err.suggestion = "Make sure '" + node.name + "' is declared before use, or check for typos in the variable name.";
        structuredErrors.push_back(err);
        node.type = Type::getI32(); // Default type to continue analysis
        return;
    }
    
    // Check pure_local access
    if (symbol->isPureLocal && currentFunction && !currentFunction->isPure) {
        ErrorInfo err("Cannot access pure_local variable '" + node.name + "' in non-pure function", node.line, node.column);
        err.suggestion = "Pure_local variables can only be accessed by pure functions. Either make function '" + 
                         currentFunction->name + "' pure by adding the 'pure' keyword, or remove 'pure_local' from variable '" + 
                         node.name + "'.";
        structuredErrors.push_back(err);
    }
    
    node.type = symbol->type;
}

void SemanticAnalyzer::visit(BinaryOp& node) {
    node.left->accept(*this);
    node.right->accept(*this);
    
    if (!node.left->type || !node.right->type) {
        return; // Already have errors
    }
    
    // Type checking for operators
    switch (node.op) {
        case BinaryOp::Op::ADD:
        case BinaryOp::Op::SUB:
        case BinaryOp::Op::MUL:
        case BinaryOp::Op::DIV:
        case BinaryOp::Op::MOD:
            node.type = getCommonType(node.left->type, node.right->type);
            break;
        
        case BinaryOp::Op::EQ:
        case BinaryOp::Op::NE:
        case BinaryOp::Op::LT:
        case BinaryOp::Op::LE:
        case BinaryOp::Op::GT:
        case BinaryOp::Op::GE:
            node.type = Type::getBool();
            break;
        
        case BinaryOp::Op::AND:
        case BinaryOp::Op::OR:
            if (node.left->type->kind != Type::Kind::BOOL || node.right->type->kind != Type::Kind::BOOL) {
                ErrorInfo err("Logical operators require boolean operands", node.line, node.column);
                err.suggestion = "Use comparison operators (==, !=, <, >, <=, >=) to create boolean expressions, or use bitwise operators (&, |, ^) for integer operations.";
                structuredErrors.push_back(err);
            }
            node.type = Type::getBool();
            break;
        
        case BinaryOp::Op::BIT_AND:
        case BinaryOp::Op::BIT_OR:
        case BinaryOp::Op::BIT_XOR:
        case BinaryOp::Op::SHL:
        case BinaryOp::Op::SHR:
            node.type = getCommonType(node.left->type, node.right->type);
            break;
    }
}

void SemanticAnalyzer::visit(UnaryOp& node) {
    node.operand->accept(*this);
    
    if (!node.operand->type) return;
    
    switch (node.op) {
        case UnaryOp::Op::NEG:
            node.type = node.operand->type;
            break;
        case UnaryOp::Op::NOT:
            if (node.operand->type->kind != Type::Kind::BOOL) {
                ErrorInfo err("Logical NOT requires boolean operand", node.line, node.column);
                err.suggestion = "Use a comparison operator to create a boolean expression, or use bitwise NOT (~) for integer values.";
                structuredErrors.push_back(err);
            }
            node.type = Type::getBool();
            break;
        case UnaryOp::Op::BIT_NOT:
            node.type = node.operand->type;
            break;
    }
}

void SemanticAnalyzer::visit(CallExpr& node) {
    FunctionInfo* funcInfo = lookupFunction(node.functionName);
    if (!funcInfo) {
        ErrorInfo err("Undefined function '" + node.functionName + "'", node.line, node.column);
        err.suggestion = "Make sure the function '" + node.functionName + "' is declared before calling it, or check for typos in the function name.";
        structuredErrors.push_back(err);
        node.type = Type::getVoid();
        return;
    }
    
    // Check argument count
    if (node.arguments.size() != funcInfo->paramTypes.size()) {
        ErrorInfo err("Function '" + node.functionName + "' expects " + 
                     std::to_string(funcInfo->paramTypes.size()) + " arguments, got " +
                     std::to_string(node.arguments.size()), node.line, node.column);
        err.suggestion = "Provide exactly " + std::to_string(funcInfo->paramTypes.size()) + 
                         " argument" + (funcInfo->paramTypes.size() == 1 ? "" : "s") + 
                         " when calling '" + node.functionName + "'.";
        structuredErrors.push_back(err);
        node.type = funcInfo->returnType;
        return;
    }
    
    // Check argument types
    for (size_t i = 0; i < node.arguments.size(); i++) {
        node.arguments[i]->accept(*this);
        
        if (node.arguments[i]->type && !typesMatch(funcInfo->paramTypes[i], node.arguments[i]->type)) {
            std::string expectedType = typeToString(funcInfo->paramTypes[i]);
            std::string actualType = typeToString(node.arguments[i]->type);
            
            ErrorInfo err("Argument " + std::to_string(i + 1) + " type mismatch in call to '" + node.functionName + "'",
                         node.arguments[i]->line, node.arguments[i]->column);
            err.message += ": expected " + expectedType + ", got " + actualType;
            
            // Provide specific suggestions for common math function errors
            if (node.functionName == "pow" || node.functionName == "floor" || 
                node.functionName == "ceil" || node.functionName == "round" || 
                node.functionName == "sqrt" || node.functionName == "sin" || 
                node.functionName == "cos" || node.functionName == "tan" ||
                node.functionName == "asin" || node.functionName == "acos" || 
                node.functionName == "atan" || node.functionName == "exp" ||
                node.functionName == "log" || node.functionName == "log2" || 
                node.functionName == "log10") {
                if (actualType == "i32" && expectedType == "f64") {
                    err.suggestion = "Use " + expectedType + " literal (e.g., 2.0 instead of 2) or convert with float().";
                } else {
                    err.suggestion = "Function '" + node.functionName + "' expects " + expectedType + " but got " + actualType + ".";
                }
            } else if (node.functionName == "int" || node.functionName == "float") {
                err.suggestion = "Type conversion function '" + node.functionName + "()' expects " + expectedType + " but got " + actualType + ".";
            } else {
                err.suggestion = "Ensure argument " + std::to_string(i + 1) + " matches the expected parameter type, or add an explicit type conversion.";
            }
            
            structuredErrors.push_back(err);
        }
    }
    
    // If calling a function with side effects from a pure function, error
    if (currentFunction && currentFunction->isPure && funcInfo->hasSideEffects) {
        ErrorInfo err("Pure function '" + currentFunction->name + "' cannot call function '" + 
                     node.functionName + "' which has side effects", node.line, node.column);
        err.suggestion = "Either remove the 'pure' keyword from function '" + currentFunction->name + 
                         "', or only call pure functions from within it.";
        structuredErrors.push_back(err);
    }
    
    // If the called function has side effects, current function has side effects
    if (funcInfo->hasSideEffects) {
        markSideEffect("Call to function '" + node.functionName + "' with side effects");
    }
    
    node.type = funcInfo->returnType;
}

void SemanticAnalyzer::pushScope() {
    scopes.emplace_back();
}

void SemanticAnalyzer::popScope() {
    if (!scopes.empty()) {
        scopes.pop_back();
    }
}

void SemanticAnalyzer::declareVariable(const std::string& name, const Symbol& symbol) {
    if (scopes.empty()) {
        pushScope();
    }
    scopes.back()[name] = symbol;
}

Symbol* SemanticAnalyzer::lookupVariable(const std::string& name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return &found->second;
        }
    }
    return nullptr;
}

FunctionInfo* SemanticAnalyzer::lookupFunction(const std::string& name) {
    auto it = functions.find(name);
    if (it != functions.end()) {
        return &it->second;
    }
    return nullptr;
}

void SemanticAnalyzer::error(const std::string& message, int line, int column) {
    std::ostringstream oss;
    if (line >= 0) {
        oss << "Error at line " << line << ", column " << column << ": ";
    } else {
        oss << "Error: ";
    }
    oss << message;
    errors.push_back(oss.str());
    
    // Also store structured error
    structuredErrors.emplace_back(message, line, column);
}

void SemanticAnalyzer::warning(const std::string& message, int line, int column) {
    std::ostringstream oss;
    if (line >= 0) {
        oss << "Warning at line " << line << ", column " << column << ": ";
    } else {
        oss << "Warning: ";
    }
    oss << message;
    warnings.push_back(oss.str());
    
    // Also store structured warning
    structuredWarnings.emplace_back(message, line, column);
}

bool SemanticAnalyzer::typesMatch(const std::shared_ptr<Type>& t1, const std::shared_ptr<Type>& t2) {
    if (!t1 || !t2) return false;
    return t1->kind == t2->kind;
}

std::string SemanticAnalyzer::typeToString(const std::shared_ptr<Type>& type) {
    if (!type) return "unknown";
    
    switch (type->kind) {
        case Type::Kind::I8: return "i8";
        case Type::Kind::I16: return "i16";
        case Type::Kind::I32: return "i32";
        case Type::Kind::I64: return "i64";
        case Type::Kind::U8: return "u8";
        case Type::Kind::U16: return "u16";
        case Type::Kind::U32: return "u32";
        case Type::Kind::U64: return "u64";
        case Type::Kind::F32: return "f32";
        case Type::Kind::F64: return "f64";
        case Type::Kind::BOOL: return "bool";
        case Type::Kind::VOID: return "void";
        case Type::Kind::STR: return "str";
        default: return "unknown";
    }
}

std::shared_ptr<Type> SemanticAnalyzer::getCommonType(const std::shared_ptr<Type>& t1, const std::shared_ptr<Type>& t2) {
    if (!t1 || !t2) return Type::getI32();
    
    // If same type, return it
    if (typesMatch(t1, t2)) return t1;
    
    // Float promotion
    if (t1->kind == Type::Kind::F64 || t2->kind == Type::Kind::F64) return Type::getF64();
    if (t1->kind == Type::Kind::F32 || t2->kind == Type::Kind::F32) return std::make_shared<Type>(Type::Kind::F32);
    
    // Integer promotion (return larger type)
    return Type::getI64();
}

void SemanticAnalyzer::markSideEffect(const std::string& reason) {
    currentFunctionHasSideEffects = true;
}

void SemanticAnalyzer::checkPureFunction(FunctionDecl& node) {
    // This is called during analysis - pure functions are checked automatically
}

void SemanticAnalyzer::checkPureLocalAccess(const std::string& varName, int line, int column) {
    Symbol* symbol = lookupVariable(varName);
    if (symbol && symbol->isPureLocal && currentFunction && !currentFunction->isPure) {
        ErrorInfo err("Cannot access pure_local variable '" + varName + "' from non-pure function", line, column);
        err.suggestion = "Pure_local variables can only be accessed by pure functions. Either make function '" + 
                         currentFunction->name + "' pure by adding the 'pure' keyword, or remove 'pure_local' from variable '" + 
                         varName + "'.";
        structuredErrors.push_back(err);
    }
}

} // namespace hash
