
aux_source_directory(. SRC_LIST_SAMPLE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/sample")

set(BIN_NAME "${PROJECT_LIB_LINK}_sample")
add_executable(${BIN_NAME} ${SRC_LIST_SAMPLE})
    
target_link_libraries(${BIN_NAME} ${PROJECT_LIB_LINK} ${COMPILER_OPTION_EXTERN_CXX_LIBS})

add_test(NAME test COMMAND ${BIN_NAME})