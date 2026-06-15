cmake_minimum_required(VERSION 3.14)
project(file-compressor VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-std=c++17)

enable_testing()
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2
  GIT_TAG        v3.15.0
)
FetchContent_MakeAvailable(Catch2)

add_subdirectory(src)
add_subdirectory(tests)

find_program(CLANG_FORMAT_EXE clang-format)

if(NOT CLANG_FORMAT_EXE)
    message(STATUS "clang-format not found. Attempting to install via pip...")
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    
    execute_process(
        COMMAND ${Python3_EXECUTABLE} -m pip install clang-format
        RESULT_VARIABLE PIP_INSTALL_RESULT
        OUTPUT_QUIET
        ERROR_QUIET
    )
    
    find_program(CLANG_FORMAT_EXE clang-format)
endif()

if(CLANG_FORMAT_EXE)
    message(STATUS "Ready using clang-format: ${CLANG_FORMAT_EXE}")
    
    file(GLOB_RECURSE FORMAT_SOURCES 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp" 
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.hpp"
    )

    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i --style=file ${FORMAT_SOURCES}
        COMMENT "Formatting source files..."
    )

    if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
        set(HOOK_FILE "${CMAKE_SOURCE_DIR}/.git/hooks/pre-commit")
        
        set(HOOK_CONTENT 
"#!/bin/bash
STAGED_FILES=$(git diff --cached --name-only --diff-filter=ACM | grep -E '\\.(h|hpp|c|cpp|cc|cxx)$')
if [ -z \"\$STAGED_FILES\" ]; then exit 0; fi

echo \"Checking code formatting with clang-format...\"
echo \"\$STAGED_FILES\" | xargs ${CLANG_FORMAT_EXE} --dry-run --Werror --style=file

if [ \$? -ne 0 ]; then
    echo \"Error: Code formatting violations found!\"
    echo \"Please run: 'cmake --build build --target format' locally to fix them, then re-stage your changes.\"
    exit 1
fi

echo \"Code formatting passes validation!\"
exit 0"
        )
        
        file(WRITE ${HOOK_FILE} "${HOOK_CONTENT}")
        
        if(NOT WIN32)
            execute_process(COMMAND chmod +x ${HOOK_FILE})
        endif()
        
        message(STATUS "Successfully installed native Git pre-commit formatting hook!")
    endif()

else()
    message(WARNING "Could not find or install clang-format automatically.")
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
