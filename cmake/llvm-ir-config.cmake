# LLVM IR Configuration Module
#
# This file provides configuration for LLVM IR generation
# It is designed to be included in the main CMakeLists.txt without modifying it

# Include the LLVM IR module
include(${CMAKE_CURRENT_LIST_DIR}/common/llvm_ir.cmake)

# If LLVM IR generation is enabled, set up the targets
if(LLVM_IR_GENERATION)
    message(STATUS "LLVM IR generation enabled")
    message(STATUS "LLVM IR output directory: ${LLVM_IR_OUTPUT_DIR}")
    message(STATUS "LLVM IR optimization level: ${LLVM_IR_OPT_LEVEL}")
    
    # Add LLVM IR generation for the library target
    add_llvm_ir_target(sstring_lib)
    
    # Optionally add LLVM IR generation for the test target if it exists
    if(TARGET sstring_tests)
        add_llvm_ir_target(sstring_tests)
    endif()
endif()
