cmake_minimum_required(VERSION 3.15)
project(HashLang VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Point CMake to the LLVM installation
set(CMAKE_PREFIX_PATH "C:/LLVM-Dev/lib/cmake/llvm;C:/LLVM-Dev/lib/cmake/clang" CACHE PATH "CMake prefix path for LLVM")

# Try to find LLVM, fallback to manual configuration
find_package(LLVM CONFIG QUIET)

# Fix DIA SDK path for Visual Studio 2022 (needed before creating executable)
set(DIA_SDK_DIR "C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/DIA SDK")
if(EXISTS "${DIA_SDK_DIR}/lib/amd64/diaguids.lib")
    link_directories("${DIA_SDK_DIR}/lib/amd64")
endif()

if(LLVM_FOUND)
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
    include_directories(${LLVM_INCLUDE_DIRS})
    separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
    add_definitions(${LLVM_DEFINITIONS_LIST})
else()
    # Manual LLVM configuration for installations without CMake config files
    message(STATUS "LLVM CMake config not found, using manual configuration")
    
    # Set LLVM paths manually
    set(LLVM_ROOT "C:/LLVM-Dev" CACHE PATH "LLVM installation directory")
    
    if(EXISTS "${LLVM_ROOT}/include")
        message(STATUS "Using LLVM from: ${LLVM_ROOT}")
        include_directories("${LLVM_ROOT}/include")
        link_directories("${LLVM_ROOT}/lib")
        
        # Add LLVM definitions
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
        add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    else()
        message(FATAL_ERROR "LLVM not found at ${LLVM_ROOT}. Please install LLVM or set LLVM_ROOT correctly.")
    endif()
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/lexer.cpp
    src/ast.cpp
    src/parser.cpp
    src/semantic.cpp
    src/codegen.cpp
    src/error_reporter.cpp
)

# Executable
add_executable(hashc ${SOURCES})

# Workaround for LLVM's hardcoded VS2019 DIA SDK path
if(MSVC AND EXISTS "${DIA_SDK_DIR}/lib/amd64/diaguids.lib")
    set_target_properties(hashc PROPERTIES LINK_FLAGS "/LIBPATH:\"${DIA_SDK_DIR}/lib/amd64\"")
endif()

# Link against LLVM libraries
if(LLVM_FOUND)
    llvm_map_components_to_libnames(llvm_libs
        core
        support
        irreader
        native
        orcjit
        mc
        mcparser
        option
        target
        analysis
        x86asmparser
        x86codegen
        x86desc
        x86info
        aarch64asmparser
        aarch64codegen
        aarch64desc
        aarch64info
        armasmparser
        armcodegen
        armdesc
        arminfo
        bpfasmparser
        bpfcodegen
        bpfdesc
        bpfinfo
        riscvasmparser
        riscvcodegen
        riscvdesc
        riscvinfo
        webassemblyasmparser
        webassemblycodegen
        webassemblydesc
        webassemblyinfo
        nvptxcodegen
        nvptxdesc
        nvptxinfo
    )
    target_link_libraries(hashc ${llvm_libs})
    
    # Add DIA SDK library for Visual Studio 2022
    if(EXISTS "${DIA_SDK_DIR}/lib/amd64/diaguids.lib")
        target_link_libraries(hashc "${DIA_SDK_DIR}/lib/amd64/diaguids.lib")
    endif()
else()
    # Manual linking for LLVM libraries
    target_link_libraries(hashc 
        LLVM-C.lib
        LTO.lib
    )
endif()

# Installation
install(TARGETS hashc DESTINATION bin)

# Examples
file(GLOB EXAMPLE_FILES "examples/*.hash")
install(FILES ${EXAMPLE_FILES} DESTINATION share/hash/examples)
