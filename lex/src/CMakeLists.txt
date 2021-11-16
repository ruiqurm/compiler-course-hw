set(CMAKE_C_FLAGS,"-werror")
aux_source_directory(. DIR_SRCS)

add_library(simplelex STATIC simplelex.cpp trie.cpp)
add_custom_command(TARGET simplelex POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/test/test_data ${CMAKE_BINARY_DIR}/test/test_data)