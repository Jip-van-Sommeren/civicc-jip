cmake_minimum_required(VERSION 3.13)

project(civicc VERSION 1.0 LANGUAGES C)

set(TEST_DIR "${CMAKE_CURRENT_LIST_DIR}/test")
set(COMPILER "${CMAKE_CURRENT_BINARY_DIR}/civicc")

option(DISABLE_ASAN "Disable address sanitizer")

# These will only work after you received the testing framework from us.
enable_testing()
add_test(NAME "basic" COMMAND "${TEST_COMMAND}/run.bash" "${COMPILER}" basic WORKING_DIRECTORY "${TEST_DIR}")
add_test(NAME "nested_funs" COMMAND "${TEST_COMMAND}/run.bash" "${COMPILER}" nested_funs WORKING_DIRECTORY "${TEST_DIR}")
add_test(NAME "arrays" COMMAND "${TEST_COMMAND}/run.bash" "${COMPILER}" arrays WORKING_DIRECTORY "${TEST_DIR}")


find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

bison_target(CivicParser src/scanparse/parser.y "${CMAKE_CURRENT_BINARY_DIR}/parser.c"
    DEFINES_FILE "${CMAKE_CURRENT_BINARY_DIR}/parser.h" VERBOSE)

flex_target(CivicLexer src/scanparse/lexer.l "${CMAKE_CURRENT_BINARY_DIR}/lexer.c"
    DEFINES_FILE "${CMAKE_CURRENT_BINARY_DIR}/lexer.h")
ADD_FLEX_BISON_DEPENDENCY(CivicLexer CivicParser)


include(./coconut/coconut.cmake)

# Whenever you add a file, add it here too.
add_executable(civicc ${FLEX_CivicLexer_OUTPUTS} ${BISON_CivicParser_OUTPUTS}
    src/main.c src/print/print.c src/scanparse/scanParse.c
    src/global/globals.c src/global/globals.h src/vartypetostring.c

    src/symboltable/symboltable.c src/symboltable/semanticanalysis.c  src/symboltable/sahelper.c 
    src/arrayparam/arrayparam.c src/arrayparam/flattenarrayindex.c  src/arrayparam/arrayhelpers.c 
    src/booldisjunctionconjunction/booldisjunctionconjunction.c
    src/booldisjunctionconjunction/booleancast.c src/symboltable/putinitatstart.c
    # Youre free to remove the demo directory and these entries.
    src/varinitregassignment/vardef.c  src/varinitregassignment/vardef2.c src/varinitregassignment/vardefhelper.c 
    src/optimization/forlooptowhilehelper.c  src/optimization/forlooptowhile.c src/optimization/forlooptowhileloop2.c
     src/codegen/constanttable.c   src/codegen/makeassembly.c   src/codegen/makeassemblyhelper.c
 src/codegen/makeassemblycombine.c)

target_compile_options(civicc PRIVATE
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Wno-unused-function>
)

# Enable address sanitizer
if(NOT DISABLE_ASAN)
    target_compile_options(civicc PRIVATE
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:$<$<CONFIG:Debug>:-fsanitize=address>>
    )

    target_link_options(civicc PRIVATE
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:$<$<CONFIG:Debug>:-fsanitize=address>>
    )
endif()

coconut_target_generate(civicc "${CMAKE_CURRENT_LIST_DIR}/src/main.ccn" dynamic)
target_include_directories(civicc
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src"
)

add_custom_target(dot
    dot -Tpng ccngen/ast.dot > ast.png
    COMMENT "Generate a png of your ast based on the generated dot diagram."
)
