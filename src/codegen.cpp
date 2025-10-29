#include "codegen.h"
#include <llvm/IR/Verifier.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/CodeGen/TargetPassConfig.h>
#include <optional>
#include <iostream>
#include <system_error>

namespace hash {

CodeGenerator::CodeGenerator()
    : context(std::make_unique<llvm::LLVMContext>()),
      module(nullptr),
      builder(std::make_unique<llvm::IRBuilder<>>(*context)),
      currentValue(nullptr),
      currentFunction(nullptr) {
    // Initialize LLVM targets
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
}

CodeGenerator::~CodeGenerator() = default;

bool CodeGenerator::generate(Program& program, const std::string& moduleName) {
    module = std::make_unique<llvm::Module>(moduleName, *context);
    
    // Set target triple - LLVM 21 requires llvm::Triple object
    module->setTargetTriple(llvm::Triple(llvm::sys::getDefaultTargetTriple()));
    
    program.accept(*this);
    
    // Verify the module
    std::string errorStr;
    llvm::raw_string_ostream errorStream(errorStr);
    if (llvm::verifyModule(*module, &errorStream)) {
        std::cerr << "Module verification failed:\n" << errorStr << std::endl;
        return false;
    }
    
    return true;
}

void CodeGenerator::emitObjectFile(const std::string& filename) {
    auto targetTriple = llvm::sys::getDefaultTargetTriple();
    
    std::string error;
    auto target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
    
    if (!target) {
        std::cerr << "Failed to lookup target: " << error << std::endl;
        return;
    }
    
    auto CPU = "generic";
    auto features = "";
    
    llvm::TargetOptions opt;
    std::optional<llvm::Reloc::Model> RM;
    auto targetMachine = target->createTargetMachine(targetTriple, CPU, features, opt, RM);
    
    module->setDataLayout(targetMachine->createDataLayout());
    
    std::error_code EC;
    llvm::raw_fd_ostream dest(filename, EC, llvm::sys::fs::OF_None);
    
    if (EC) {
        std::cerr << "Could not open file: " << EC.message() << std::endl;
        return;
    }
    
    llvm::legacy::PassManager pass;
    auto fileType = llvm::CodeGenFileType::ObjectFile;
    
    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType)) {
        std::cerr << "TargetMachine can't emit a file of this type" << std::endl;
        return;
    }
    
    pass.run(*module);
    dest.flush();
}

void CodeGenerator::emitLLVMIR(const std::string& filename) {
    std::error_code EC;
    llvm::raw_fd_ostream dest(filename, EC, llvm::sys::fs::OF_None);
    
    if (EC) {
        std::cerr << "Could not open file: " << EC.message() << std::endl;
        return;
    }
    
    module->print(dest, nullptr);
    dest.flush();
}

llvm::Type* CodeGenerator::getLLVMType(const std::shared_ptr<Type>& type) {
    switch (type->kind) {
        case Type::Kind::I8: return llvm::Type::getInt8Ty(*context);
        case Type::Kind::I16: return llvm::Type::getInt16Ty(*context);
        case Type::Kind::I32: return llvm::Type::getInt32Ty(*context);
        case Type::Kind::I64: return llvm::Type::getInt64Ty(*context);
        case Type::Kind::U8: return llvm::Type::getInt8Ty(*context);
        case Type::Kind::U16: return llvm::Type::getInt16Ty(*context);
        case Type::Kind::U32: return llvm::Type::getInt32Ty(*context);
        case Type::Kind::U64: return llvm::Type::getInt64Ty(*context);
        case Type::Kind::F32: return llvm::Type::getFloatTy(*context);
        case Type::Kind::F64: return llvm::Type::getDoubleTy(*context);
        case Type::Kind::BOOL: return llvm::Type::getInt1Ty(*context);
        case Type::Kind::VOID: return llvm::Type::getVoidTy(*context);
        case Type::Kind::STR: return llvm::PointerType::get(*context, 0);
        default: return llvm::Type::getInt32Ty(*context);
    }
}

llvm::AllocaInst* CodeGenerator::createEntryBlockAlloca(llvm::Function* function, 
                                                          const std::string& varName, 
                                                          llvm::Type* type) {
    llvm::IRBuilder<> tmpBuilder(&function->getEntryBlock(), function->getEntryBlock().begin());
    return tmpBuilder.CreateAlloca(type, nullptr, varName);
}

void CodeGenerator::visit(Program& node) {
    // Declare external functions (printf for basic I/O)
    std::vector<llvm::Type*> printfArgs = {llvm::PointerType::get(*context, 0)};
    llvm::FunctionType* printfType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context), printfArgs, true);
    llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf", module.get());
    
    // Declare built-in print_i32 function
    std::vector<llvm::Type*> printI32Args = {llvm::Type::getInt32Ty(*context)};
    llvm::FunctionType* printI32Type = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*context), printI32Args, false);
    llvm::Function* printI32Func = llvm::Function::Create(
        printI32Type, llvm::Function::ExternalLinkage, "print_i32", module.get());
    
    // Implement print_i32 using printf
    llvm::BasicBlock* printBlock = llvm::BasicBlock::Create(*context, "entry", printI32Func);
    builder->SetInsertPoint(printBlock);
    
    // Create format string "%d\n"
    llvm::Value* formatStr = builder->CreateGlobalStringPtr("%d\n");
    
    // Get the parameter
    llvm::Value* value = printI32Func->getArg(0);
    
    // Call printf
    llvm::Function* printfFunc = module->getFunction("printf");
    builder->CreateCall(printfFunc, {formatStr, value});
    builder->CreateRetVoid();
    
    // Declare and implement print_i64
    std::vector<llvm::Type*> printI64Args = {llvm::Type::getInt64Ty(*context)};
    llvm::FunctionType* printI64Type = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*context), printI64Args, false);
    llvm::Function* printI64Func = llvm::Function::Create(
        printI64Type, llvm::Function::ExternalLinkage, "print_i64", module.get());
    
    llvm::BasicBlock* printI64Block = llvm::BasicBlock::Create(*context, "entry", printI64Func);
    builder->SetInsertPoint(printI64Block);
    llvm::Value* formatI64Str = builder->CreateGlobalStringPtr("%lld\n");
    llvm::Value* i64Value = printI64Func->getArg(0);
    builder->CreateCall(printfFunc, {formatI64Str, i64Value});
    builder->CreateRetVoid();
    
    // Declare and implement print_f64
    std::vector<llvm::Type*> printF64Args = {llvm::Type::getDoubleTy(*context)};
    llvm::FunctionType* printF64Type = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*context), printF64Args, false);
    llvm::Function* printF64Func = llvm::Function::Create(
        printF64Type, llvm::Function::ExternalLinkage, "print_f64", module.get());
    
    llvm::BasicBlock* printF64Block = llvm::BasicBlock::Create(*context, "entry", printF64Func);
    builder->SetInsertPoint(printF64Block);
    llvm::Value* formatF64Str = builder->CreateGlobalStringPtr("%f\n");
    llvm::Value* f64Value = printF64Func->getArg(0);
    builder->CreateCall(printfFunc, {formatF64Str, f64Value});
    builder->CreateRetVoid();
    
    // Declare and implement print_bool
    std::vector<llvm::Type*> printBoolArgs = {llvm::Type::getInt1Ty(*context)};
    llvm::FunctionType* printBoolType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*context), printBoolArgs, false);
    llvm::Function* printBoolFunc = llvm::Function::Create(
        printBoolType, llvm::Function::ExternalLinkage, "print_bool", module.get());
    
    llvm::BasicBlock* printBoolBlock = llvm::BasicBlock::Create(*context, "entry", printBoolFunc);
    builder->SetInsertPoint(printBoolBlock);
    llvm::Value* boolValue = printBoolFunc->getArg(0);
    
    // Convert bool to i32 (0 or 1) and then select string
    llvm::Value* boolAsInt = builder->CreateZExt(boolValue, llvm::Type::getInt32Ty(*context));
    llvm::Value* trueStr = builder->CreateGlobalStringPtr("true\n");
    llvm::Value* falseStr = builder->CreateGlobalStringPtr("false\n");
    llvm::Value* selectedStr = builder->CreateSelect(boolValue, trueStr, falseStr);
    builder->CreateCall(printfFunc, {selectedStr});
    builder->CreateRetVoid();
    
    // Declare and implement print_str
    std::vector<llvm::Type*> printStrArgs = {llvm::PointerType::get(*context, 0)};
    llvm::FunctionType* printStrType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*context), printStrArgs, false);
    llvm::Function* printStrFunc = llvm::Function::Create(
        printStrType, llvm::Function::ExternalLinkage, "print_str", module.get());
    
    llvm::BasicBlock* printStrBlock = llvm::BasicBlock::Create(*context, "entry", printStrFunc);
    builder->SetInsertPoint(printStrBlock);
    llvm::Value* formatStrStr = builder->CreateGlobalStringPtr("%s\n");
    llvm::Value* strValue = printStrFunc->getArg(0);
    builder->CreateCall(printfFunc, {formatStrStr, strValue});
    builder->CreateRetVoid();
    
    // Declare and implement println (just prints newline)
    llvm::FunctionType* printlnType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*context), {}, false);
    llvm::Function* printlnFunc = llvm::Function::Create(
        printlnType, llvm::Function::ExternalLinkage, "println", module.get());
    
    llvm::BasicBlock* printlnBlock = llvm::BasicBlock::Create(*context, "entry", printlnFunc);
    builder->SetInsertPoint(printlnBlock);
    llvm::Value* newlineStr = builder->CreateGlobalStringPtr("\n");
    builder->CreateCall(printfFunc, {newlineStr});
    builder->CreateRetVoid();
    
    // Type conversion built-ins
    
    // i32_to_i64: convert i32 to i64 (sign extension)
    llvm::FunctionType* i32ToI64Type = llvm::FunctionType::get(
        llvm::Type::getInt64Ty(*context),
        {llvm::Type::getInt32Ty(*context)},
        false);
    llvm::Function* i32ToI64Func = llvm::Function::Create(
        i32ToI64Type, llvm::Function::ExternalLinkage, "i32_to_i64", module.get());
    llvm::BasicBlock* i32ToI64Block = llvm::BasicBlock::Create(*context, "entry", i32ToI64Func);
    builder->SetInsertPoint(i32ToI64Block);
    llvm::Value* i32Val = i32ToI64Func->getArg(0);
    llvm::Value* i64Val = builder->CreateSExt(i32Val, llvm::Type::getInt64Ty(*context));
    builder->CreateRet(i64Val);
    
    // i64_to_i32: convert i64 to i32 (truncation)
    llvm::FunctionType* i64ToI32Type = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        {llvm::Type::getInt64Ty(*context)},
        false);
    llvm::Function* i64ToI32Func = llvm::Function::Create(
        i64ToI32Type, llvm::Function::ExternalLinkage, "i64_to_i32", module.get());
    llvm::BasicBlock* i64ToI32Block = llvm::BasicBlock::Create(*context, "entry", i64ToI32Func);
    builder->SetInsertPoint(i64ToI32Block);
    llvm::Value* i64Input = i64ToI32Func->getArg(0);
    llvm::Value* i32Result = builder->CreateTrunc(i64Input, llvm::Type::getInt32Ty(*context));
    builder->CreateRet(i32Result);
    
    // i32_to_f64: convert i32 to f64
    llvm::FunctionType* i32ToF64Type = llvm::FunctionType::get(
        llvm::Type::getDoubleTy(*context),
        {llvm::Type::getInt32Ty(*context)},
        false);
    llvm::Function* i32ToF64Func = llvm::Function::Create(
        i32ToF64Type, llvm::Function::ExternalLinkage, "i32_to_f64", module.get());
    llvm::BasicBlock* i32ToF64Block = llvm::BasicBlock::Create(*context, "entry", i32ToF64Func);
    builder->SetInsertPoint(i32ToF64Block);
    llvm::Value* i32ForFloat = i32ToF64Func->getArg(0);
    llvm::Value* f64Result = builder->CreateSIToFP(i32ForFloat, llvm::Type::getDoubleTy(*context));
    builder->CreateRet(f64Result);
    
    // f64_to_i32: convert f64 to i32
    llvm::FunctionType* f64ToI32Type = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        {llvm::Type::getDoubleTy(*context)},
        false);
    llvm::Function* f64ToI32Func = llvm::Function::Create(
        f64ToI32Type, llvm::Function::ExternalLinkage, "f64_to_i32", module.get());
    llvm::BasicBlock* f64ToI32Block = llvm::BasicBlock::Create(*context, "entry", f64ToI32Func);
    builder->SetInsertPoint(f64ToI32Block);
    llvm::Value* f64Input = f64ToI32Func->getArg(0);
    llvm::Value* i32FromFloat = builder->CreateFPToSI(f64Input, llvm::Type::getInt32Ty(*context));
    builder->CreateRet(i32FromFloat);
    
    // i64_to_f64: convert i64 to f64
    llvm::FunctionType* i64ToF64Type = llvm::FunctionType::get(
        llvm::Type::getDoubleTy(*context),
        {llvm::Type::getInt64Ty(*context)},
        false);
    llvm::Function* i64ToF64Func = llvm::Function::Create(
        i64ToF64Type, llvm::Function::ExternalLinkage, "i64_to_f64", module.get());
    llvm::BasicBlock* i64ToF64Block = llvm::BasicBlock::Create(*context, "entry", i64ToF64Func);
    builder->SetInsertPoint(i64ToF64Block);
    llvm::Value* i64ForFloat = i64ToF64Func->getArg(0);
    llvm::Value* f64FromI64 = builder->CreateSIToFP(i64ForFloat, llvm::Type::getDoubleTy(*context));
    builder->CreateRet(f64FromI64);
    
    // f64_to_i64: convert f64 to i64
    llvm::FunctionType* f64ToI64Type = llvm::FunctionType::get(
        llvm::Type::getInt64Ty(*context),
        {llvm::Type::getDoubleTy(*context)},
        false);
    llvm::Function* f64ToI64Func = llvm::Function::Create(
        f64ToI64Type, llvm::Function::ExternalLinkage, "f64_to_i64", module.get());
    llvm::BasicBlock* f64ToI64Block = llvm::BasicBlock::Create(*context, "entry", f64ToI64Func);
    builder->SetInsertPoint(f64ToI64Block);
    llvm::Value* f64ForI64 = f64ToI64Func->getArg(0);
    llvm::Value* i64FromFloat = builder->CreateFPToSI(f64ForI64, llvm::Type::getInt64Ty(*context));
    builder->CreateRet(i64FromFloat);
    
    // Math built-ins
    
    // abs_i32: absolute value for i32
    llvm::FunctionType* absI32Type = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        {llvm::Type::getInt32Ty(*context)},
        false);
    llvm::Function* absI32Func = llvm::Function::Create(
        absI32Type, llvm::Function::ExternalLinkage, "abs_i32", module.get());
    llvm::BasicBlock* absI32Block = llvm::BasicBlock::Create(*context, "entry", absI32Func);
    builder->SetInsertPoint(absI32Block);
    llvm::Value* absInput = absI32Func->getArg(0);
    // If value < 0, return -value, else return value
    llvm::Value* isNeg = builder->CreateICmpSLT(absInput, llvm::ConstantInt::get(*context, llvm::APInt(32, 0)));
    llvm::Value* negated = builder->CreateNeg(absInput);
    llvm::Value* absResult = builder->CreateSelect(isNeg, negated, absInput);
    builder->CreateRet(absResult);
    
    // min_i32: minimum of two i32 values
    llvm::FunctionType* minI32Type = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        {llvm::Type::getInt32Ty(*context), llvm::Type::getInt32Ty(*context)},
        false);
    llvm::Function* minI32Func = llvm::Function::Create(
        minI32Type, llvm::Function::ExternalLinkage, "min_i32", module.get());
    llvm::BasicBlock* minI32Block = llvm::BasicBlock::Create(*context, "entry", minI32Func);
    builder->SetInsertPoint(minI32Block);
    llvm::Value* minA = minI32Func->getArg(0);
    llvm::Value* minB = minI32Func->getArg(1);
    llvm::Value* isLess = builder->CreateICmpSLT(minA, minB);
    llvm::Value* minResult = builder->CreateSelect(isLess, minA, minB);
    builder->CreateRet(minResult);
    
    // max_i32: maximum of two i32 values
    llvm::FunctionType* maxI32Type = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        {llvm::Type::getInt32Ty(*context), llvm::Type::getInt32Ty(*context)},
        false);
    llvm::Function* maxI32Func = llvm::Function::Create(
        maxI32Type, llvm::Function::ExternalLinkage, "max_i32", module.get());
    llvm::BasicBlock* maxI32Block = llvm::BasicBlock::Create(*context, "entry", maxI32Func);
    builder->SetInsertPoint(maxI32Block);
    llvm::Value* maxA = maxI32Func->getArg(0);
    llvm::Value* maxB = maxI32Func->getArg(1);
    llvm::Value* isGreater = builder->CreateICmpSGT(maxA, maxB);
    llvm::Value* maxResult = builder->CreateSelect(isGreater, maxA, maxB);
    builder->CreateRet(maxResult);
    
    // sqrt_f64: square root for f64
    llvm::FunctionType* sqrtF64Type = llvm::FunctionType::get(
        llvm::Type::getDoubleTy(*context),
        {llvm::Type::getDoubleTy(*context)},
        false);
    llvm::Function* sqrtF64Func = llvm::Function::Create(
        sqrtF64Type, llvm::Function::ExternalLinkage, "sqrt_f64", module.get());
    llvm::BasicBlock* sqrtF64Block = llvm::BasicBlock::Create(*context, "entry", sqrtF64Func);
    builder->SetInsertPoint(sqrtF64Block);
    llvm::Value* sqrtInput = sqrtF64Func->getArg(0);
    // Use LLVM's intrinsic sqrt function
    llvm::Function* sqrtIntrinsic = llvm::Intrinsic::getDeclaration(
        module.get(), llvm::Intrinsic::sqrt, {llvm::Type::getDoubleTy(*context)});
    llvm::Value* sqrtResult = builder->CreateCall(sqrtIntrinsic, {sqrtInput});
    builder->CreateRet(sqrtResult);
    
    // ========================================
    // Python-like aliases for better UX
    // ========================================
    
    // int(f64) -> i32 (wrapper for f64_to_i32)
    llvm::FunctionType* intType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        {llvm::Type::getDoubleTy(*context)},
        false);
    llvm::Function* intFunc = llvm::Function::Create(
        intType, llvm::Function::ExternalLinkage, "int", module.get());
    llvm::BasicBlock* intBlock = llvm::BasicBlock::Create(*context, "entry", intFunc);
    builder->SetInsertPoint(intBlock);
    llvm::Value* intInput = intFunc->getArg(0);
    llvm::Value* intResult = builder->CreateFPToSI(intInput, llvm::Type::getInt32Ty(*context));
    builder->CreateRet(intResult);
    
    // float(i32) -> f64 (wrapper for i32_to_f64)
    llvm::FunctionType* floatType = llvm::FunctionType::get(
        llvm::Type::getDoubleTy(*context),
        {llvm::Type::getInt32Ty(*context)},
        false);
    llvm::Function* floatFunc = llvm::Function::Create(
        floatType, llvm::Function::ExternalLinkage, "float", module.get());
    llvm::BasicBlock* floatBlock = llvm::BasicBlock::Create(*context, "entry", floatFunc);
    builder->SetInsertPoint(floatBlock);
    llvm::Value* floatInput = floatFunc->getArg(0);
    llvm::Value* floatResult = builder->CreateSIToFP(floatInput, llvm::Type::getDoubleTy(*context));
    builder->CreateRet(floatResult);
    
    // abs(i32) -> i32 (wrapper for abs_i32)
    llvm::FunctionType* absType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        {llvm::Type::getInt32Ty(*context)},
        false);
    llvm::Function* absFunc = llvm::Function::Create(
        absType, llvm::Function::ExternalLinkage, "abs", module.get());
    llvm::BasicBlock* absBlock = llvm::BasicBlock::Create(*context, "entry", absFunc);
    builder->SetInsertPoint(absBlock);
    llvm::Value* absIn = absFunc->getArg(0);
    llvm::Value* absIsNeg = builder->CreateICmpSLT(absIn, llvm::ConstantInt::get(*context, llvm::APInt(32, 0)));
    llvm::Value* absNeg = builder->CreateNeg(absIn);
    llvm::Value* absRes = builder->CreateSelect(absIsNeg, absNeg, absIn);
    builder->CreateRet(absRes);
    
    // min(i32, i32) -> i32
    llvm::FunctionType* minType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        {llvm::Type::getInt32Ty(*context), llvm::Type::getInt32Ty(*context)},
        false);
    llvm::Function* minFunc = llvm::Function::Create(
        minType, llvm::Function::ExternalLinkage, "min", module.get());
    llvm::BasicBlock* minBlock = llvm::BasicBlock::Create(*context, "entry", minFunc);
    builder->SetInsertPoint(minBlock);
    llvm::Value* minArg1 = minFunc->getArg(0);
    llvm::Value* minArg2 = minFunc->getArg(1);
    llvm::Value* minLess = builder->CreateICmpSLT(minArg1, minArg2);
    llvm::Value* minRes = builder->CreateSelect(minLess, minArg1, minArg2);
    builder->CreateRet(minRes);
    
    // max(i32, i32) -> i32
    llvm::FunctionType* maxType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        {llvm::Type::getInt32Ty(*context), llvm::Type::getInt32Ty(*context)},
        false);
    llvm::Function* maxFunc = llvm::Function::Create(
        maxType, llvm::Function::ExternalLinkage, "max", module.get());
    llvm::BasicBlock* maxBlock = llvm::BasicBlock::Create(*context, "entry", maxFunc);
    builder->SetInsertPoint(maxBlock);
    llvm::Value* maxArg1 = maxFunc->getArg(0);
    llvm::Value* maxArg2 = maxFunc->getArg(1);
    llvm::Value* maxGreater = builder->CreateICmpSGT(maxArg1, maxArg2);
    llvm::Value* maxRes = builder->CreateSelect(maxGreater, maxArg1, maxArg2);
    builder->CreateRet(maxRes);
    
    // sqrt(f64) -> f64
    llvm::FunctionType* sqrtType = llvm::FunctionType::get(
        llvm::Type::getDoubleTy(*context),
        {llvm::Type::getDoubleTy(*context)},
        false);
    llvm::Function* sqrtWrapperFunc = llvm::Function::Create(
        sqrtType, llvm::Function::ExternalLinkage, "sqrt", module.get());
    llvm::BasicBlock* sqrtWrapperBlock = llvm::BasicBlock::Create(*context, "entry", sqrtWrapperFunc);
    builder->SetInsertPoint(sqrtWrapperBlock);
    llvm::Value* sqrtWrapperInput = sqrtWrapperFunc->getArg(0);
    llvm::Function* sqrtWrapperIntrinsic = llvm::Intrinsic::getDeclaration(
        module.get(), llvm::Intrinsic::sqrt, {llvm::Type::getDoubleTy(*context)});
    llvm::Value* sqrtWrapperResult = builder->CreateCall(sqrtWrapperIntrinsic, {sqrtWrapperInput});
    builder->CreateRet(sqrtWrapperResult);
    
    // ========================================
    // Input Functions (using scanf)
    // ========================================
    
    // Declare scanf for input
    std::vector<llvm::Type*> scanfArgs = {llvm::PointerType::get(*context, 0)};
    llvm::FunctionType* scanfType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context), scanfArgs, true);
    llvm::Function* scanfFunc = llvm::Function::Create(
        scanfType, llvm::Function::ExternalLinkage, "scanf", module.get());
    
    // read_i32() -> i32 (reads an integer from stdin)
    llvm::FunctionType* readI32Type = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context), {}, false);
    llvm::Function* readI32Func = llvm::Function::Create(
        readI32Type, llvm::Function::ExternalLinkage, "read_i32", module.get());
    llvm::BasicBlock* readI32Block = llvm::BasicBlock::Create(*context, "entry", readI32Func);
    builder->SetInsertPoint(readI32Block);
    llvm::Value* intFormatRead = builder->CreateGlobalStringPtr("%d");
    llvm::AllocaInst* intInputPtr = builder->CreateAlloca(llvm::Type::getInt32Ty(*context));
    builder->CreateCall(scanfFunc, {intFormatRead, intInputPtr});
    llvm::Value* readIntValue = builder->CreateLoad(llvm::Type::getInt32Ty(*context), intInputPtr);
    builder->CreateRet(readIntValue);
    
    // read_f64() -> f64 (reads a float from stdin)
    llvm::FunctionType* readF64Type = llvm::FunctionType::get(
        llvm::Type::getDoubleTy(*context), {}, false);
    llvm::Function* readF64Func = llvm::Function::Create(
        readF64Type, llvm::Function::ExternalLinkage, "read_f64", module.get());
    llvm::BasicBlock* readF64Block = llvm::BasicBlock::Create(*context, "entry", readF64Func);
    builder->SetInsertPoint(readF64Block);
    llvm::Value* floatFormatRead = builder->CreateGlobalStringPtr("%lf");
    llvm::AllocaInst* floatInputPtr = builder->CreateAlloca(llvm::Type::getDoubleTy(*context));
    builder->CreateCall(scanfFunc, {floatFormatRead, floatInputPtr});
    llvm::Value* readFloatValue = builder->CreateLoad(llvm::Type::getDoubleTy(*context), floatInputPtr);
    builder->CreateRet(readFloatValue);
    
    // ========================================
    // System Functions
    // ========================================
    
    // Declare C library functions we'll use
    llvm::FunctionType* cExitType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*context), {llvm::Type::getInt32Ty(*context)}, false);
    llvm::Function::Create(cExitType, llvm::Function::ExternalLinkage, "exit", module.get());
    
    llvm::FunctionType* cTimeType = llvm::FunctionType::get(
        llvm::Type::getInt64Ty(*context), {llvm::PointerType::get(*context, 0)}, false);
    llvm::Function::Create(cTimeType, llvm::Function::ExternalLinkage, "time", module.get());
    
    llvm::FunctionType* cClockType = llvm::FunctionType::get(
        llvm::Type::getInt64Ty(*context), {}, false);
    llvm::Function::Create(cClockType, llvm::Function::ExternalLinkage, "clock", module.get());
    
    llvm::FunctionType* cRandType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context), {}, false);
    llvm::Function::Create(cRandType, llvm::Function::ExternalLinkage, "rand", module.get());
    
    llvm::FunctionType* cSrandType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*context), {llvm::Type::getInt32Ty(*context)}, false);
    llvm::Function::Create(cSrandType, llvm::Function::ExternalLinkage, "srand", module.get());
    
    // hash_time() -> i64 - Get current Unix timestamp
    llvm::FunctionType* hashTimeType = llvm::FunctionType::get(
        llvm::Type::getInt64Ty(*context), {}, false);
    llvm::Function* hashTimeFunc = llvm::Function::Create(
        hashTimeType, llvm::Function::ExternalLinkage, "hash_time", module.get());
    llvm::BasicBlock* timeBlock = llvm::BasicBlock::Create(*context, "entry", hashTimeFunc);
    builder->SetInsertPoint(timeBlock);
    llvm::Function* cTimeFunc = module->getFunction("time");
    llvm::Value* nullPtr = llvm::ConstantPointerNull::get(llvm::PointerType::get(*context, 0));
    llvm::Value* timestamp = builder->CreateCall(cTimeFunc, {nullPtr});
    builder->CreateRet(timestamp);
    
    // hash_clock() -> f64 - Get program execution time in seconds
    llvm::FunctionType* hashClockType = llvm::FunctionType::get(
        llvm::Type::getDoubleTy(*context), {}, false);
    llvm::Function* hashClockFunc = llvm::Function::Create(
        hashClockType, llvm::Function::ExternalLinkage, "hash_clock", module.get());
    llvm::BasicBlock* clockBlock = llvm::BasicBlock::Create(*context, "entry", hashClockFunc);
    builder->SetInsertPoint(clockBlock);
    llvm::Function* cClockFunc = module->getFunction("clock");
    llvm::Value* clockTicks = builder->CreateCall(cClockFunc, {});
    // Convert to seconds: clock_ticks / CLOCKS_PER_SEC
    llvm::Value* clocksPerSec = llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), 1000000);
    llvm::Value* clockTicksF64 = builder->CreateSIToFP(clockTicks, llvm::Type::getDoubleTy(*context));
    llvm::Value* clocksPerSecF64 = builder->CreateSIToFP(clocksPerSec, llvm::Type::getDoubleTy(*context));
    llvm::Value* seconds = builder->CreateFDiv(clockTicksF64, clocksPerSecF64);
    builder->CreateRet(seconds);
    
    // random() -> f64 - Random float between 0.0 and 1.0
    llvm::FunctionType* randomType = llvm::FunctionType::get(
        llvm::Type::getDoubleTy(*context), {}, false);
    llvm::Function* randomFunc = llvm::Function::Create(
        randomType, llvm::Function::ExternalLinkage, "random", module.get());
    llvm::BasicBlock* randomBlock = llvm::BasicBlock::Create(*context, "entry", randomFunc);
    builder->SetInsertPoint(randomBlock);
    llvm::Function* cRandFunc = module->getFunction("rand");
    llvm::Value* randValue = builder->CreateCall(cRandFunc, {});
    // Convert to 0.0 - 1.0: rand() / RAND_MAX
    llvm::Value* randMax = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 32767); // RAND_MAX
    llvm::Value* randF64 = builder->CreateSIToFP(randValue, llvm::Type::getDoubleTy(*context));
    llvm::Value* randMaxF64 = builder->CreateSIToFP(randMax, llvm::Type::getDoubleTy(*context));
    llvm::Value* randomResult = builder->CreateFDiv(randF64, randMaxF64);
    builder->CreateRet(randomResult);
    
    // seed_random(i32) -> void - Set random seed
    std::vector<llvm::Type*> seedArgs = {llvm::Type::getInt32Ty(*context)};
    llvm::FunctionType* seedType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*context), seedArgs, false);
    llvm::Function* seedFunc = llvm::Function::Create(
        seedType, llvm::Function::ExternalLinkage, "seed_random", module.get());
    llvm::BasicBlock* seedBlock = llvm::BasicBlock::Create(*context, "entry", seedFunc);
    builder->SetInsertPoint(seedBlock);
    llvm::Value* seed = seedFunc->getArg(0);
    llvm::Function* cSrandFunc = module->getFunction("srand");
    builder->CreateCall(cSrandFunc, {seed});
    builder->CreateRetVoid();
    
    // random_range(i32, i32) -> i32 - Random integer in range [min, max]
    std::vector<llvm::Type*> rangeArgs = {llvm::Type::getInt32Ty(*context), llvm::Type::getInt32Ty(*context)};
    llvm::FunctionType* rangeType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context), rangeArgs, false);
    llvm::Function* rangeFunc = llvm::Function::Create(
        rangeType, llvm::Function::ExternalLinkage, "random_range", module.get());
    llvm::BasicBlock* rangeBlock = llvm::BasicBlock::Create(*context, "entry", rangeFunc);
    builder->SetInsertPoint(rangeBlock);
    llvm::Value* minVal = rangeFunc->getArg(0);
    llvm::Value* maxVal = rangeFunc->getArg(1);
    llvm::Value* randVal = builder->CreateCall(cRandFunc, {});
    // Calculate: min + (rand() % (max - min + 1))
    llvm::Value* diff = builder->CreateSub(maxVal, minVal);
    llvm::Value* diffPlusOne = builder->CreateAdd(diff, llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 1));
    llvm::Value* modulo = builder->CreateSRem(randVal, diffPlusOne);
    llvm::Value* rangeResult = builder->CreateAdd(minVal, modulo);
    builder->CreateRet(rangeResult);
    
    // ========================================
    // String Manipulation Functions
    // ========================================
    
    // Declare C library string functions
    llvm::FunctionType* cStrlenType = llvm::FunctionType::get(
        llvm::Type::getInt64Ty(*context),
        {llvm::PointerType::get(*context, 0)},
        false);
    llvm::Function::Create(cStrlenType, llvm::Function::ExternalLinkage, "strlen", module.get());
    
    llvm::FunctionType* cStrcmpType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        {llvm::PointerType::get(*context, 0), llvm::PointerType::get(*context, 0)},
        false);
    llvm::Function::Create(cStrcmpType, llvm::Function::ExternalLinkage, "strcmp", module.get());
    
    llvm::FunctionType* cStrcatType = llvm::FunctionType::get(
        llvm::PointerType::get(*context, 0),
        {llvm::PointerType::get(*context, 0), llvm::PointerType::get(*context, 0)},
        false);
    llvm::Function::Create(cStrcatType, llvm::Function::ExternalLinkage, "strcat", module.get());
    
    llvm::FunctionType* cStrcpyType = llvm::FunctionType::get(
        llvm::PointerType::get(*context, 0),
        {llvm::PointerType::get(*context, 0), llvm::PointerType::get(*context, 0)},
        false);
    llvm::Function::Create(cStrcpyType, llvm::Function::ExternalLinkage, "strcpy", module.get());
    
    llvm::FunctionType* cMallocType = llvm::FunctionType::get(
        llvm::PointerType::get(*context, 0),
        {llvm::Type::getInt64Ty(*context)},
        false);
    llvm::Function::Create(cMallocType, llvm::Function::ExternalLinkage, "malloc", module.get());
    
    llvm::FunctionType* cToupperType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        {llvm::Type::getInt32Ty(*context)},
        false);
    llvm::Function::Create(cToupperType, llvm::Function::ExternalLinkage, "toupper", module.get());
    
    llvm::FunctionType* cTolowerType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        {llvm::Type::getInt32Ty(*context)},
        false);
    llvm::Function::Create(cTolowerType, llvm::Function::ExternalLinkage, "tolower", module.get());
    
    // len(str) -> i32 - Python-style string length!
    llvm::FunctionType* lenType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        {llvm::PointerType::get(*context, 0)},
        false);
    llvm::Function* lenFunc = llvm::Function::Create(
        lenType, llvm::Function::ExternalLinkage, "len", module.get());
    llvm::BasicBlock* lenBlock = llvm::BasicBlock::Create(*context, "entry", lenFunc);
    builder->SetInsertPoint(lenBlock);
    llvm::Value* strPtr = lenFunc->getArg(0);
    llvm::Function* cStrlenFunc = module->getFunction("strlen");
    llvm::Value* len64 = builder->CreateCall(cStrlenFunc, {strPtr});
    llvm::Value* len32 = builder->CreateTrunc(len64, llvm::Type::getInt32Ty(*context));
    builder->CreateRet(len32);
    
    // str_concat(str, str) -> str - Concatenate two strings
    llvm::FunctionType* concatType = llvm::FunctionType::get(
        llvm::PointerType::get(*context, 0),
        {llvm::PointerType::get(*context, 0), llvm::PointerType::get(*context, 0)},
        false);
    llvm::Function* concatFunc = llvm::Function::Create(
        concatType, llvm::Function::ExternalLinkage, "str_concat", module.get());
    llvm::BasicBlock* concatBlock = llvm::BasicBlock::Create(*context, "entry", concatFunc);
    builder->SetInsertPoint(concatBlock);
    llvm::Value* str1 = concatFunc->getArg(0);
    llvm::Value* str2 = concatFunc->getArg(1);
    // Calculate total length
    llvm::Value* len1_64 = builder->CreateCall(cStrlenFunc, {str1});
    llvm::Value* len2_64 = builder->CreateCall(cStrlenFunc, {str2});
    llvm::Value* totalLen = builder->CreateAdd(len1_64, len2_64);
    llvm::Value* allocSize = builder->CreateAdd(totalLen, llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), 1));
    // Allocate memory
    llvm::Function* cMallocFunc = module->getFunction("malloc");
    llvm::Value* resultPtr = builder->CreateCall(cMallocFunc, {allocSize});
    // Copy strings
    llvm::Function* cStrcpyFunc = module->getFunction("strcpy");
    llvm::Function* cStrcatFunc = module->getFunction("strcat");
    builder->CreateCall(cStrcpyFunc, {resultPtr, str1});
    builder->CreateCall(cStrcatFunc, {resultPtr, str2});
    builder->CreateRet(resultPtr);
    
    // str_eq(str, str) -> bool - String equality comparison
    llvm::FunctionType* eqType = llvm::FunctionType::get(
        llvm::Type::getInt1Ty(*context),
        {llvm::PointerType::get(*context, 0), llvm::PointerType::get(*context, 0)},
        false);
    llvm::Function* eqFunc = llvm::Function::Create(
        eqType, llvm::Function::ExternalLinkage, "str_eq", module.get());
    llvm::BasicBlock* eqBlock = llvm::BasicBlock::Create(*context, "entry", eqFunc);
    builder->SetInsertPoint(eqBlock);
    llvm::Value* eqStr1 = eqFunc->getArg(0);
    llvm::Value* eqStr2 = eqFunc->getArg(1);
    llvm::Function* cStrcmpFunc = module->getFunction("strcmp");
    llvm::Value* cmpResult = builder->CreateCall(cStrcmpFunc, {eqStr1, eqStr2});
    llvm::Value* isEqual = builder->CreateICmpEQ(cmpResult, llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 0));
    builder->CreateRet(isEqual);
    
    // upper(str) -> str - Convert to uppercase (Python-style!)
    llvm::FunctionType* upperType = llvm::FunctionType::get(
        llvm::PointerType::get(*context, 0),
        {llvm::PointerType::get(*context, 0)},
        false);
    llvm::Function* upperFunc = llvm::Function::Create(
        upperType, llvm::Function::ExternalLinkage, "upper", module.get());
    llvm::BasicBlock* upperBlock = llvm::BasicBlock::Create(*context, "entry", upperFunc);
    builder->SetInsertPoint(upperBlock);
    llvm::Value* upperStr = upperFunc->getArg(0);
    llvm::Value* upperLen64 = builder->CreateCall(cStrlenFunc, {upperStr});
    llvm::Value* upperAllocSize = builder->CreateAdd(upperLen64, llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), 1));
    llvm::Value* upperResult = builder->CreateCall(cMallocFunc, {upperAllocSize});
    // Copy and convert
    llvm::Value* upperIdx = builder->CreateAlloca(llvm::Type::getInt64Ty(*context));
    builder->CreateStore(llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), 0), upperIdx);
    llvm::BasicBlock* upperLoopCond = llvm::BasicBlock::Create(*context, "loop.cond", upperFunc);
    llvm::BasicBlock* upperLoopBody = llvm::BasicBlock::Create(*context, "loop.body", upperFunc);
    llvm::BasicBlock* upperLoopEnd = llvm::BasicBlock::Create(*context, "loop.end", upperFunc);
    builder->CreateBr(upperLoopCond);
    // Loop condition
    builder->SetInsertPoint(upperLoopCond);
    llvm::Value* upperIdxVal = builder->CreateLoad(llvm::Type::getInt64Ty(*context), upperIdx);
    llvm::Value* upperContinue = builder->CreateICmpULT(upperIdxVal, upperLen64);
    builder->CreateCondBr(upperContinue, upperLoopBody, upperLoopEnd);
    // Loop body
    builder->SetInsertPoint(upperLoopBody);
    llvm::Value* upperSrcPtr = builder->CreateGEP(llvm::Type::getInt8Ty(*context), upperStr, upperIdxVal);
    llvm::Value* upperChar = builder->CreateLoad(llvm::Type::getInt8Ty(*context), upperSrcPtr);
    llvm::Value* upperCharExt = builder->CreateSExt(upperChar, llvm::Type::getInt32Ty(*context));
    llvm::Function* cToupperFunc = module->getFunction("toupper");
    llvm::Value* upperCharConv = builder->CreateCall(cToupperFunc, {upperCharExt});
    llvm::Value* upperCharTrunc = builder->CreateTrunc(upperCharConv, llvm::Type::getInt8Ty(*context));
    llvm::Value* upperDstPtr = builder->CreateGEP(llvm::Type::getInt8Ty(*context), upperResult, upperIdxVal);
    builder->CreateStore(upperCharTrunc, upperDstPtr);
    llvm::Value* upperNextIdx = builder->CreateAdd(upperIdxVal, llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), 1));
    builder->CreateStore(upperNextIdx, upperIdx);
    builder->CreateBr(upperLoopCond);
    // Loop end - add null terminator
    builder->SetInsertPoint(upperLoopEnd);
    llvm::Value* upperNullPtr = builder->CreateGEP(llvm::Type::getInt8Ty(*context), upperResult, upperLen64);
    builder->CreateStore(llvm::ConstantInt::get(llvm::Type::getInt8Ty(*context), 0), upperNullPtr);
    builder->CreateRet(upperResult);
    
    // lower(str) -> str - Convert to lowercase (Python-style!)
    llvm::FunctionType* lowerType = llvm::FunctionType::get(
        llvm::PointerType::get(*context, 0),
        {llvm::PointerType::get(*context, 0)},
        false);
    llvm::Function* lowerFunc = llvm::Function::Create(
        lowerType, llvm::Function::ExternalLinkage, "lower", module.get());
    llvm::BasicBlock* lowerBlock = llvm::BasicBlock::Create(*context, "entry", lowerFunc);
    builder->SetInsertPoint(lowerBlock);
    llvm::Value* lowerStr = lowerFunc->getArg(0);
    llvm::Value* lowerLen64 = builder->CreateCall(cStrlenFunc, {lowerStr});
    llvm::Value* lowerAllocSize = builder->CreateAdd(lowerLen64, llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), 1));
    llvm::Value* lowerResult = builder->CreateCall(cMallocFunc, {lowerAllocSize});
    // Copy and convert
    llvm::Value* lowerIdx = builder->CreateAlloca(llvm::Type::getInt64Ty(*context));
    builder->CreateStore(llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), 0), lowerIdx);
    llvm::BasicBlock* lowerLoopCond = llvm::BasicBlock::Create(*context, "loop.cond", lowerFunc);
    llvm::BasicBlock* lowerLoopBody = llvm::BasicBlock::Create(*context, "loop.body", lowerFunc);
    llvm::BasicBlock* lowerLoopEnd = llvm::BasicBlock::Create(*context, "loop.end", lowerFunc);
    builder->CreateBr(lowerLoopCond);
    // Loop condition
    builder->SetInsertPoint(lowerLoopCond);
    llvm::Value* lowerIdxVal = builder->CreateLoad(llvm::Type::getInt64Ty(*context), lowerIdx);
    llvm::Value* lowerContinue = builder->CreateICmpULT(lowerIdxVal, lowerLen64);
    builder->CreateCondBr(lowerContinue, lowerLoopBody, lowerLoopEnd);
    // Loop body
    builder->SetInsertPoint(lowerLoopBody);
    llvm::Value* lowerSrcPtr = builder->CreateGEP(llvm::Type::getInt8Ty(*context), lowerStr, lowerIdxVal);
    llvm::Value* lowerChar = builder->CreateLoad(llvm::Type::getInt8Ty(*context), lowerSrcPtr);
    llvm::Value* lowerCharExt = builder->CreateSExt(lowerChar, llvm::Type::getInt32Ty(*context));
    llvm::Function* cTolowerFunc = module->getFunction("tolower");
    llvm::Value* lowerCharConv = builder->CreateCall(cTolowerFunc, {lowerCharExt});
    llvm::Value* lowerCharTrunc = builder->CreateTrunc(lowerCharConv, llvm::Type::getInt8Ty(*context));
    llvm::Value* lowerDstPtr = builder->CreateGEP(llvm::Type::getInt8Ty(*context), lowerResult, lowerIdxVal);
    builder->CreateStore(lowerCharTrunc, lowerDstPtr);
    llvm::Value* lowerNextIdx = builder->CreateAdd(lowerIdxVal, llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), 1));
    builder->CreateStore(lowerNextIdx, lowerIdx);
    builder->CreateBr(lowerLoopCond);
    // Loop end - add null terminator
    builder->SetInsertPoint(lowerLoopEnd);
    llvm::Value* lowerNullPtr = builder->CreateGEP(llvm::Type::getInt8Ty(*context), lowerResult, lowerLen64);
    builder->CreateStore(llvm::ConstantInt::get(llvm::Type::getInt8Ty(*context), 0), lowerNullPtr);
    builder->CreateRet(lowerResult);
    
    // ========================================
    // File I/O Functions
    // ========================================
    
    // Declare C library file functions
    llvm::FunctionType* cFopenType = llvm::FunctionType::get(
        llvm::PointerType::get(*context, 0),
        {llvm::PointerType::get(*context, 0), llvm::PointerType::get(*context, 0)},
        false);
    llvm::Function::Create(cFopenType, llvm::Function::ExternalLinkage, "fopen", module.get());
    
    llvm::FunctionType* cFcloseType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        {llvm::PointerType::get(*context, 0)},
        false);
    llvm::Function::Create(cFcloseType, llvm::Function::ExternalLinkage, "fclose", module.get());
    
    llvm::FunctionType* cFseekType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        {llvm::PointerType::get(*context, 0), llvm::Type::getInt64Ty(*context), llvm::Type::getInt32Ty(*context)},
        false);
    llvm::Function::Create(cFseekType, llvm::Function::ExternalLinkage, "fseek", module.get());
    
    llvm::FunctionType* cFtellType = llvm::FunctionType::get(
        llvm::Type::getInt64Ty(*context),
        {llvm::PointerType::get(*context, 0)},
        false);
    llvm::Function::Create(cFtellType, llvm::Function::ExternalLinkage, "ftell", module.get());
    
    llvm::FunctionType* cFreadType = llvm::FunctionType::get(
        llvm::Type::getInt64Ty(*context),
        {llvm::PointerType::get(*context, 0), llvm::Type::getInt64Ty(*context), 
         llvm::Type::getInt64Ty(*context), llvm::PointerType::get(*context, 0)},
        false);
    llvm::Function::Create(cFreadType, llvm::Function::ExternalLinkage, "fread", module.get());
    
    llvm::FunctionType* cFwriteType = llvm::FunctionType::get(
        llvm::Type::getInt64Ty(*context),
        {llvm::PointerType::get(*context, 0), llvm::Type::getInt64Ty(*context),
         llvm::Type::getInt64Ty(*context), llvm::PointerType::get(*context, 0)},
        false);
    llvm::Function::Create(cFwriteType, llvm::Function::ExternalLinkage, "fwrite", module.get());
    
    llvm::FunctionType* cRemoveType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        {llvm::PointerType::get(*context, 0)},
        false);
    llvm::Function::Create(cRemoveType, llvm::Function::ExternalLinkage, "remove", module.get());
    
    llvm::FunctionType* cAccessType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        {llvm::PointerType::get(*context, 0), llvm::Type::getInt32Ty(*context)},
        false);
    llvm::Function::Create(cAccessType, llvm::Function::ExternalLinkage, "_access", module.get());
    
    // file_read(str) -> str - Read entire file as string
    llvm::FunctionType* fileReadType = llvm::FunctionType::get(
        llvm::PointerType::get(*context, 0),
        {llvm::PointerType::get(*context, 0)},
        false);
    llvm::Function* fileReadFunc = llvm::Function::Create(
        fileReadType, llvm::Function::ExternalLinkage, "file_read", module.get());
    llvm::BasicBlock* fileReadBlock = llvm::BasicBlock::Create(*context, "entry", fileReadFunc);
    builder->SetInsertPoint(fileReadBlock);
    llvm::Value* filename = fileReadFunc->getArg(0);
    
    // Open file for reading
    llvm::Value* modeR = builder->CreateGlobalStringPtr("rb");
    llvm::Function* cFopenFunc = module->getFunction("fopen");
    llvm::Value* filePtr = builder->CreateCall(cFopenFunc, {filename, modeR});
    
    // Check if file opened successfully
    llvm::Value* fileIsNull = builder->CreateIsNull(filePtr);
    llvm::BasicBlock* fileOpenedBlock = llvm::BasicBlock::Create(*context, "file.opened", fileReadFunc);
    llvm::BasicBlock* fileFailBlock = llvm::BasicBlock::Create(*context, "file.fail", fileReadFunc);
    builder->CreateCondBr(fileIsNull, fileFailBlock, fileOpenedBlock);
    
    // File failed to open - return empty string
    builder->SetInsertPoint(fileFailBlock);
    llvm::Value* emptyStr = builder->CreateGlobalStringPtr("");
    builder->CreateRet(emptyStr);
    
    // File opened successfully
    builder->SetInsertPoint(fileOpenedBlock);
    
    // Get file size
    llvm::Function* cFseekFunc = module->getFunction("fseek");
    llvm::Function* cFtellFunc = module->getFunction("ftell");
    llvm::Value* seekEnd = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 2); // SEEK_END
    llvm::Value* seekSet = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 0); // SEEK_SET
    builder->CreateCall(cFseekFunc, {filePtr, llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), 0), seekEnd});
    llvm::Value* fileSize = builder->CreateCall(cFtellFunc, {filePtr});
    builder->CreateCall(cFseekFunc, {filePtr, llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), 0), seekSet});
    
    // Allocate buffer
    llvm::Value* bufferSize = builder->CreateAdd(fileSize, llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), 1));
    llvm::Value* buffer = builder->CreateCall(cMallocFunc, {bufferSize});
    
    // Read file
    llvm::Function* cFreadFunc = module->getFunction("fread");
    builder->CreateCall(cFreadFunc, {buffer, llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), 1), fileSize, filePtr});
    
    // Null terminate
    llvm::Value* nullPos = builder->CreateGEP(llvm::Type::getInt8Ty(*context), buffer, fileSize);
    builder->CreateStore(llvm::ConstantInt::get(llvm::Type::getInt8Ty(*context), 0), nullPos);
    
    // Close file
    llvm::Function* cFcloseFunc = module->getFunction("fclose");
    builder->CreateCall(cFcloseFunc, {filePtr});
    
    builder->CreateRet(buffer);
    
    // file_write(filename: str, content: str) -> bool - Write string to file
    llvm::FunctionType* fileWriteType = llvm::FunctionType::get(
        llvm::Type::getInt1Ty(*context),
        {llvm::PointerType::get(*context, 0), llvm::PointerType::get(*context, 0)},
        false);
    llvm::Function* fileWriteFunc = llvm::Function::Create(
        fileWriteType, llvm::Function::ExternalLinkage, "file_write", module.get());
    llvm::BasicBlock* fileWriteBlock = llvm::BasicBlock::Create(*context, "entry", fileWriteFunc);
    builder->SetInsertPoint(fileWriteBlock);
    llvm::Value* writeFilename = fileWriteFunc->getArg(0);
    llvm::Value* writeContent = fileWriteFunc->getArg(1);
    
    // Open file for writing
    llvm::Value* modeW = builder->CreateGlobalStringPtr("wb");
    llvm::Value* writeFilePtr = builder->CreateCall(cFopenFunc, {writeFilename, modeW});
    
    // Check if file opened
    llvm::Value* writeFileIsNull = builder->CreateIsNull(writeFilePtr);
    llvm::BasicBlock* writeOpenedBlock = llvm::BasicBlock::Create(*context, "write.opened", fileWriteFunc);
    llvm::BasicBlock* writeFailBlock = llvm::BasicBlock::Create(*context, "write.fail", fileWriteFunc);
    builder->CreateCondBr(writeFileIsNull, writeFailBlock, writeOpenedBlock);
    
    // Failed to open
    builder->SetInsertPoint(writeFailBlock);
    builder->CreateRet(llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context), 0)); // false
    
    // Opened successfully
    builder->SetInsertPoint(writeOpenedBlock);
    
    // Get content length
    llvm::Value* contentLen = builder->CreateCall(cStrlenFunc, {writeContent});
    
    // Write content
    llvm::Function* cFwriteFunc = module->getFunction("fwrite");
    builder->CreateCall(cFwriteFunc, {writeContent, llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), 1), contentLen, writeFilePtr});
    
    // Close file
    builder->CreateCall(cFcloseFunc, {writeFilePtr});
    
    builder->CreateRet(llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context), 1)); // true
    
    // file_exists(str) -> bool - Check if file exists
    llvm::FunctionType* fileExistsType = llvm::FunctionType::get(
        llvm::Type::getInt1Ty(*context),
        {llvm::PointerType::get(*context, 0)},
        false);
    llvm::Function* fileExistsFunc = llvm::Function::Create(
        fileExistsType, llvm::Function::ExternalLinkage, "file_exists", module.get());
    llvm::BasicBlock* fileExistsBlock = llvm::BasicBlock::Create(*context, "entry", fileExistsFunc);
    builder->SetInsertPoint(fileExistsBlock);
    llvm::Value* existsFilename = fileExistsFunc->getArg(0);
    
    // Use _access to check file existence (0 = F_OK on Windows)
    llvm::Function* cAccessFunc = module->getFunction("_access");
    llvm::Value* accessResult = builder->CreateCall(cAccessFunc, {existsFilename, llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 0)});
    
    // _access returns 0 if file exists, -1 if not
    llvm::Value* fileExists = builder->CreateICmpEQ(accessResult, llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 0));
    builder->CreateRet(fileExists);
    
    // file_delete(str) -> bool - Delete file
    llvm::FunctionType* fileDeleteType = llvm::FunctionType::get(
        llvm::Type::getInt1Ty(*context),
        {llvm::PointerType::get(*context, 0)},
        false);
    llvm::Function* fileDeleteFunc = llvm::Function::Create(
        fileDeleteType, llvm::Function::ExternalLinkage, "file_delete", module.get());
    llvm::BasicBlock* fileDeleteBlock = llvm::BasicBlock::Create(*context, "entry", fileDeleteFunc);
    builder->SetInsertPoint(fileDeleteBlock);
    llvm::Value* deleteFilename = fileDeleteFunc->getArg(0);
    
    // Use remove() to delete file
    llvm::Function* cRemoveFunc = module->getFunction("remove");
    llvm::Value* removeResult = builder->CreateCall(cRemoveFunc, {deleteFilename});
    
    // remove() returns 0 on success, non-zero on failure
    llvm::Value* deleteSuccess = builder->CreateICmpEQ(removeResult, llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 0));
    builder->CreateRet(deleteSuccess);
    
    // Generate global variables
    for (auto& global : node.globals) {
        global->accept(*this);
    }
    
    // Generate functions
    for (auto& func : node.functions) {
        func->accept(*this);
    }
}

void CodeGenerator::visit(FunctionDecl& node) {
    // Build parameter types
    std::vector<llvm::Type*> paramTypes;
    for (auto& param : node.parameters) {
        paramTypes.push_back(getLLVMType(param.type));
    }
    
    // Create function type
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        getLLVMType(node.returnType), paramTypes, false);
    
    // Create function
    llvm::Function* function = llvm::Function::Create(
        funcType, llvm::Function::ExternalLinkage, node.name, module.get());
    
    functions[node.name] = function;
    currentFunction = function;
    
    // Set parameter names
    unsigned idx = 0;
    for (auto& arg : function->args()) {
        arg.setName(node.parameters[idx++].name);
    }
    
    // Create entry block
    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(*context, "entry", function);
    builder->SetInsertPoint(entryBlock);
    
    // Allocate space for parameters and store their values
    namedValues.clear();
    idx = 0;
    for (auto& arg : function->args()) {
        llvm::AllocaInst* alloca = createEntryBlockAlloca(
            function, arg.getName().str(), arg.getType());
        builder->CreateStore(&arg, alloca);
        namedValues[arg.getName().str()] = alloca;
        idx++;
    }
    
    // Generate function body
    for (auto& stmt : node.body) {
        stmt->accept(*this);
    }
    
    // Add return if missing (for void functions)
    if (node.returnType->kind == Type::Kind::VOID) {
        if (!builder->GetInsertBlock()->getTerminator()) {
            builder->CreateRetVoid();
        }
    }
    
    // Verify function
    std::string errorStr;
    llvm::raw_string_ostream errorStream(errorStr);
    if (llvm::verifyFunction(*function, &errorStream)) {
        std::cerr << "Function verification failed for '" << node.name << "':\n" 
                  << errorStr << std::endl;
    }
    
    currentFunction = nullptr;
}

void CodeGenerator::visit(VariableDecl& node) {
    llvm::Type* type = getLLVMType(node.varType);
    
    if (currentFunction) {
        // Local variable
        llvm::AllocaInst* alloca = createEntryBlockAlloca(currentFunction, node.name, type);
        namedValues[node.name] = alloca;
        
        if (node.initializer) {
            node.initializer->accept(*this);
            builder->CreateStore(currentValue, alloca);
        }
    } else {
        // Global variable
        llvm::Constant* initializer = nullptr;
        if (node.initializer) {
            node.initializer->accept(*this);
            if (auto* constant = llvm::dyn_cast<llvm::Constant>(currentValue)) {
                initializer = constant;
            } else {
                initializer = llvm::Constant::getNullValue(type);
            }
        } else {
            initializer = llvm::Constant::getNullValue(type);
        }
        
        module->getOrInsertGlobal(node.name, type);
        llvm::GlobalVariable* gVar = module->getNamedGlobal(node.name);
        gVar->setInitializer(initializer);
    }
}

void CodeGenerator::visit(Assignment& node) {
    auto it = namedValues.find(node.name);
    if (it == namedValues.end()) {
        std::cerr << "Unknown variable name: " << node.name << std::endl;
        return;
    }
    
    node.value->accept(*this);
    builder->CreateStore(currentValue, it->second);
}

void CodeGenerator::visit(ReturnStmt& node) {
    if (node.value) {
        node.value->accept(*this);
        builder->CreateRet(currentValue);
    } else {
        builder->CreateRetVoid();
    }
}

void CodeGenerator::visit(IfStmt& node) {
    node.condition->accept(*this);
    llvm::Value* condValue = currentValue;
    
    llvm::Function* function = builder->GetInsertBlock()->getParent();
    llvm::BasicBlock* thenBlock = llvm::BasicBlock::Create(*context, "then", function);
    llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(*context, "else");
    llvm::BasicBlock* mergeBlock = llvm::BasicBlock::Create(*context, "merge");
    
    if (!node.elseBody.empty()) {
        builder->CreateCondBr(condValue, thenBlock, elseBlock);
    } else {
        builder->CreateCondBr(condValue, thenBlock, mergeBlock);
    }
    
    // Then block
    builder->SetInsertPoint(thenBlock);
    for (auto& stmt : node.thenBody) {
        stmt->accept(*this);
    }
    bool thenTerminated = builder->GetInsertBlock()->getTerminator() != nullptr;
    if (!thenTerminated) {
        builder->CreateBr(mergeBlock);
    }
    
    // Else block
    bool elseTerminated = false;
    if (!node.elseBody.empty()) {
        elseBlock->insertInto(function);
        builder->SetInsertPoint(elseBlock);
        for (auto& stmt : node.elseBody) {
            stmt->accept(*this);
        }
        elseTerminated = builder->GetInsertBlock()->getTerminator() != nullptr;
        if (!elseTerminated) {
            builder->CreateBr(mergeBlock);
        }
    }
    
    // Merge block
    mergeBlock->insertInto(function);
    builder->SetInsertPoint(mergeBlock);
    
    // If BOTH branches exist and BOTH terminated, merge block is unreachable
    if (!node.elseBody.empty() && thenTerminated && elseTerminated) {
        builder->CreateUnreachable();
    }
}

void CodeGenerator::visit(WhileStmt& node) {
    llvm::Function* function = builder->GetInsertBlock()->getParent();
    llvm::BasicBlock* condBlock = llvm::BasicBlock::Create(*context, "while.cond", function);
    llvm::BasicBlock* bodyBlock = llvm::BasicBlock::Create(*context, "while.body");
    llvm::BasicBlock* afterBlock = llvm::BasicBlock::Create(*context, "while.after");
    
    builder->CreateBr(condBlock);
    
    // Condition block
    builder->SetInsertPoint(condBlock);
    node.condition->accept(*this);
    llvm::Value* condValue = currentValue;
    builder->CreateCondBr(condValue, bodyBlock, afterBlock);
    
    // Body block
    bodyBlock->insertInto(function);
    builder->SetInsertPoint(bodyBlock);
    for (auto& stmt : node.body) {
        stmt->accept(*this);
    }
    if (!builder->GetInsertBlock()->getTerminator()) {
        builder->CreateBr(condBlock);
    }
    
    // After block
    afterBlock->insertInto(function);
    builder->SetInsertPoint(afterBlock);
}

void CodeGenerator::visit(ExprStmt& node) {
    node.expression->accept(*this);
}

void CodeGenerator::visit(IntegerLiteral& node) {
    // Use the type from the AST node to determine bit width
    unsigned bitWidth = 32; // default
    if (node.type) {
        switch (node.type->kind) {
            case Type::Kind::I8:
            case Type::Kind::U8:
                bitWidth = 8;
                break;
            case Type::Kind::I16:
            case Type::Kind::U16:
                bitWidth = 16;
                break;
            case Type::Kind::I32:
            case Type::Kind::U32:
                bitWidth = 32;
                break;
            case Type::Kind::I64:
            case Type::Kind::U64:
                bitWidth = 64;
                break;
            default:
                bitWidth = 32;
        }
    }
    currentValue = llvm::ConstantInt::get(*context, llvm::APInt(bitWidth, node.value, true));
}

void CodeGenerator::visit(FloatLiteral& node) {
    currentValue = llvm::ConstantFP::get(*context, llvm::APFloat(node.value));
}

void CodeGenerator::visit(StringLiteral& node) {
    currentValue = builder->CreateGlobalStringPtr(node.value);
}

void CodeGenerator::visit(BoolLiteral& node) {
    currentValue = llvm::ConstantInt::get(*context, llvm::APInt(1, node.value ? 1 : 0));
}

void CodeGenerator::visit(Identifier& node) {
    auto it = namedValues.find(node.name);
    if (it == namedValues.end()) {
        // Try global variable
        llvm::GlobalVariable* gVar = module->getNamedGlobal(node.name);
        if (gVar) {
            currentValue = builder->CreateLoad(gVar->getValueType(), gVar, node.name);
        } else {
            std::cerr << "Unknown variable name: " << node.name << std::endl;
            currentValue = nullptr;
        }
        return;
    }
    
    currentValue = builder->CreateLoad(it->second->getAllocatedType(), it->second, node.name);
}

void CodeGenerator::visit(BinaryOp& node) {
    node.left->accept(*this);
    llvm::Value* left = currentValue;
    
    node.right->accept(*this);
    llvm::Value* right = currentValue;
    
    if (!left || !right) {
        currentValue = nullptr;
        return;
    }
    
    bool isFloat = left->getType()->isFloatingPointTy() || right->getType()->isFloatingPointTy();
    
    switch (node.op) {
        case BinaryOp::Op::ADD:
            currentValue = isFloat ? builder->CreateFAdd(left, right, "addtmp") 
                                    : builder->CreateAdd(left, right, "addtmp");
            break;
        case BinaryOp::Op::SUB:
            currentValue = isFloat ? builder->CreateFSub(left, right, "subtmp") 
                                    : builder->CreateSub(left, right, "subtmp");
            break;
        case BinaryOp::Op::MUL:
            currentValue = isFloat ? builder->CreateFMul(left, right, "multmp") 
                                    : builder->CreateMul(left, right, "multmp");
            break;
        case BinaryOp::Op::DIV:
            currentValue = isFloat ? builder->CreateFDiv(left, right, "divtmp") 
                                    : builder->CreateSDiv(left, right, "divtmp");
            break;
        case BinaryOp::Op::MOD:
            currentValue = isFloat ? builder->CreateFRem(left, right, "modtmp") 
                                    : builder->CreateSRem(left, right, "modtmp");
            break;
        case BinaryOp::Op::EQ:
            currentValue = isFloat ? builder->CreateFCmpOEQ(left, right, "eqtmp") 
                                    : builder->CreateICmpEQ(left, right, "eqtmp");
            break;
        case BinaryOp::Op::NE:
            currentValue = isFloat ? builder->CreateFCmpONE(left, right, "netmp") 
                                    : builder->CreateICmpNE(left, right, "netmp");
            break;
        case BinaryOp::Op::LT:
            currentValue = isFloat ? builder->CreateFCmpOLT(left, right, "lttmp") 
                                    : builder->CreateICmpSLT(left, right, "lttmp");
            break;
        case BinaryOp::Op::LE:
            currentValue = isFloat ? builder->CreateFCmpOLE(left, right, "letmp") 
                                    : builder->CreateICmpSLE(left, right, "letmp");
            break;
        case BinaryOp::Op::GT:
            currentValue = isFloat ? builder->CreateFCmpOGT(left, right, "gttmp") 
                                    : builder->CreateICmpSGT(left, right, "gttmp");
            break;
        case BinaryOp::Op::GE:
            currentValue = isFloat ? builder->CreateFCmpOGE(left, right, "getmp") 
                                    : builder->CreateICmpSGE(left, right, "getmp");
            break;
        case BinaryOp::Op::AND:
            currentValue = builder->CreateAnd(left, right, "andtmp");
            break;
        case BinaryOp::Op::OR:
            currentValue = builder->CreateOr(left, right, "ortmp");
            break;
        case BinaryOp::Op::BIT_AND:
            currentValue = builder->CreateAnd(left, right, "bitandtmp");
            break;
        case BinaryOp::Op::BIT_OR:
            currentValue = builder->CreateOr(left, right, "bitortmp");
            break;
        case BinaryOp::Op::BIT_XOR:
            currentValue = builder->CreateXor(left, right, "bitxortmp");
            break;
        case BinaryOp::Op::SHL:
            currentValue = builder->CreateShl(left, right, "shltmp");
            break;
        case BinaryOp::Op::SHR:
            currentValue = builder->CreateAShr(left, right, "shrtmp");
            break;
    }
}

void CodeGenerator::visit(UnaryOp& node) {
    node.operand->accept(*this);
    llvm::Value* operand = currentValue;
    
    if (!operand) {
        currentValue = nullptr;
        return;
    }
    
    switch (node.op) {
        case UnaryOp::Op::NEG:
            if (operand->getType()->isFloatingPointTy()) {
                currentValue = builder->CreateFNeg(operand, "negtmp");
            } else {
                currentValue = builder->CreateNeg(operand, "negtmp");
            }
            break;
        case UnaryOp::Op::NOT:
            currentValue = builder->CreateNot(operand, "nottmp");
            break;
        case UnaryOp::Op::BIT_NOT:
            currentValue = builder->CreateNot(operand, "bitnottmp");
            break;
    }
}

void CodeGenerator::visit(CallExpr& node) {
    // Special handling for LLVM intrinsic-based functions
    if (node.functionName == "pow" || node.functionName == "floor" || 
        node.functionName == "ceil" || node.functionName == "round" ||
        node.functionName == "sin" || node.functionName == "cos" || 
        node.functionName == "tan" || node.functionName == "sqrt" ||
        node.functionName == "asin" || node.functionName == "acos" || 
        node.functionName == "atan" || node.functionName == "exp" ||
        node.functionName == "log" || node.functionName == "log2" || 
        node.functionName == "log10") {
        
        // Evaluate arguments first
        std::vector<llvm::Value*> args;
        for (auto& arg : node.arguments) {
            arg->accept(*this);
            if (!currentValue) {
                std::cerr << "Error evaluating argument" << std::endl;
                currentValue = nullptr;
                return;
            }
            args.push_back(currentValue);
        }
        
        // Get the appropriate LLVM intrinsic
        llvm::Intrinsic::ID intrinsicID;
        if (node.functionName == "pow") {
            intrinsicID = llvm::Intrinsic::pow;
        } else if (node.functionName == "floor") {
            intrinsicID = llvm::Intrinsic::floor;
        } else if (node.functionName == "ceil") {
            intrinsicID = llvm::Intrinsic::ceil;
        } else if (node.functionName == "round") {
            intrinsicID = llvm::Intrinsic::round;
        } else if (node.functionName == "sqrt") {
            intrinsicID = llvm::Intrinsic::sqrt;
        } else if (node.functionName == "sin") {
            intrinsicID = llvm::Intrinsic::sin;
        } else if (node.functionName == "cos") {
            intrinsicID = llvm::Intrinsic::cos;
        } else if (node.functionName == "tan") {
            // Note: tan is not an LLVM intrinsic, we'll handle it differently
            // tan(x) = sin(x) / cos(x)
            llvm::Function* sinFunc = llvm::Intrinsic::getDeclaration(
                module.get(), llvm::Intrinsic::sin, {llvm::Type::getDoubleTy(*context)});
            llvm::Function* cosFunc = llvm::Intrinsic::getDeclaration(
                module.get(), llvm::Intrinsic::cos, {llvm::Type::getDoubleTy(*context)});
            llvm::Value* sinVal = builder->CreateCall(sinFunc, args);
            llvm::Value* cosVal = builder->CreateCall(cosFunc, args);
            currentValue = builder->CreateFDiv(sinVal, cosVal, "tanval");
            return;
        } else if (node.functionName == "exp") {
            intrinsicID = llvm::Intrinsic::exp;
        } else if (node.functionName == "log") {
            intrinsicID = llvm::Intrinsic::log;
        } else if (node.functionName == "log2") {
            intrinsicID = llvm::Intrinsic::log2;
        } else if (node.functionName == "log10") {
            intrinsicID = llvm::Intrinsic::log10;
        } else {
            // For asin, acos, atan - these are not LLVM intrinsics
            // We need to declare them as external C math library functions
            std::vector<llvm::Type*> paramTypes = {llvm::Type::getDoubleTy(*context)};
            llvm::FunctionType* mathFuncType = llvm::FunctionType::get(
                llvm::Type::getDoubleTy(*context), paramTypes, false);
            llvm::Function* mathFunc = module->getFunction(node.functionName);
            if (!mathFunc) {
                mathFunc = llvm::Function::Create(
                    mathFuncType, llvm::Function::ExternalLinkage, node.functionName, module.get());
            }
            currentValue = builder->CreateCall(mathFunc, args, "mathcall");
            return;
        }
        
        llvm::Function* intrinsic = llvm::Intrinsic::getDeclaration(
            module.get(), intrinsicID, {llvm::Type::getDoubleTy(*context)});
        currentValue = builder->CreateCall(intrinsic, args, "mathcall");
        return;
    }
    
    llvm::Function* callee = module->getFunction(node.functionName);
    if (!callee) {
        std::cerr << "Unknown function referenced: " << node.functionName << std::endl;
        currentValue = nullptr;
        return;
    }
    
    std::vector<llvm::Value*> args;
    for (auto& arg : node.arguments) {
        arg->accept(*this);
        if (!currentValue) {
            std::cerr << "Error evaluating argument" << std::endl;
            currentValue = nullptr;
            return;
        }
        args.push_back(currentValue);
    }
    
    // Check if function returns void
    if (callee->getReturnType()->isVoidTy()) {
        builder->CreateCall(callee, args);
        currentValue = nullptr; // Void functions don't produce a value
    } else {
        currentValue = builder->CreateCall(callee, args, "calltmp");
    }
}

} // namespace hash
