# LLVM IR Generation Module
#
# This module adds support for generating LLVM IR files from C++ source files
# It creates a custom target 'llvm_ir' that generates .ll files for all source files

# Options
option(LLVM_IR_GENERATION "Enable LLVM IR generation" OFF)
set(LLVM_IR_OUTPUT_DIR "${CMAKE_BINARY_DIR}/llvm-ir" CACHE PATH "Output directory for LLVM IR files")
set(LLVM_IR_OPT_LEVEL "0" CACHE STRING "Optimization level for LLVM IR generation (0-3)")

# Only proceed if LLVM IR generation is enabled
if(LLVM_IR_GENERATION)
    # Check if we're using Clang
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(WARNING "LLVM IR generation requires Clang compiler. Current compiler: ${CMAKE_CXX_COMPILER_ID}")
        message(WARNING "LLVM IR generation will be disabled")
        set(LLVM_IR_GENERATION OFF)
        return()
    endif()

    # Create output directory
    file(MAKE_DIRECTORY ${LLVM_IR_OUTPUT_DIR})

    # Function to add LLVM IR generation for a target
    function(add_llvm_ir_target TARGET)
        # Get source files from the target
        get_target_property(TARGET_SOURCES ${TARGET} SOURCES)
        get_target_property(TARGET_SOURCE_DIR ${TARGET} SOURCE_DIR)
        get_target_property(TARGET_INCLUDE_DIRS ${TARGET} INCLUDE_DIRECTORIES)
        
        # Convert include directories to compiler flags
        set(INCLUDE_FLAGS "")
        foreach(DIR ${TARGET_INCLUDE_DIRS})
            list(APPEND INCLUDE_FLAGS "-I${DIR}")
        endforeach()
        
        # Create custom commands for each source file
        set(IR_FILES "")
        foreach(SOURCE_FILE ${TARGET_SOURCES})
            # Only process C++ source files
            if(SOURCE_FILE MATCHES "\\.(cpp|cc|cxx)$")
                # Get the full path to the source file
                if(IS_ABSOLUTE ${SOURCE_FILE})
                    set(SOURCE_PATH ${SOURCE_FILE})
                else()
                    set(SOURCE_PATH ${TARGET_SOURCE_DIR}/${SOURCE_FILE})
                endif()
                
                # Determine output file path
                get_filename_component(SOURCE_NAME ${SOURCE_FILE} NAME_WE)
                set(IR_FILE "${LLVM_IR_OUTPUT_DIR}/${SOURCE_NAME}.ll")
                list(APPEND IR_FILES ${IR_FILE})
                
                # Add custom command to generate IR
                add_custom_command(
                    OUTPUT ${IR_FILE}
                    COMMAND ${CMAKE_CXX_COMPILER} -S -emit-llvm 
                            -O${LLVM_IR_OPT_LEVEL}
                            -std=c++${CMAKE_CXX_STANDARD}
                            ${INCLUDE_FLAGS}
                            -o ${IR_FILE}
                            ${SOURCE_PATH}
                    DEPENDS ${SOURCE_PATH}
                    COMMENT "Generating LLVM IR for ${SOURCE_FILE}"
                    VERBATIM
                )
            endif()
        endforeach()
        
        # Create custom target to generate all IR files
        add_custom_target(${TARGET}_llvm_ir DEPENDS ${IR_FILES})
        
        # Add this target as a dependency to the main llvm_ir target
        if(NOT TARGET llvm_ir)
            add_custom_target(llvm_ir)
        endif()
        add_dependencies(llvm_ir ${TARGET}_llvm_ir)
        
        # Output message
        message(STATUS "Added LLVM IR generation for target: ${TARGET}")
    endfunction()
else()
    # Dummy function when LLVM IR generation is disabled
    function(add_llvm_ir_target)
        # Do nothing
    endfunction()
endif()
