ADD_EXECUTABLE(test_literial literial.cpp)
target_link_libraries(test_literial simplelex)

# 拷贝文件(注释掉是因为放到src里面去了)
# add_custom_command(TARGET test_literial POST_BUILD
#                    COMMAND ${CMAKE_COMMAND} -E copy_directory
#                        ${CMAKE_SOURCE_DIR}/test/test_data $<TARGET_FILE_DIR:test_literial>/test_data)

