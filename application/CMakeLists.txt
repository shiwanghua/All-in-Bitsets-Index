set(SOURCE_FILES src/main.cpp)

add_executable(AiBI ${SOURCE_FILES})

target_link_libraries(AiBI LINK_PRIVATE indexes)
