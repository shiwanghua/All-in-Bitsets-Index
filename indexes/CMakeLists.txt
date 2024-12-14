# set(HEADER_FILES include/BitsetIndex/BitsetIndex.h)

set(SOURCE_FILES)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

set(
    SOURCE_FILES
    ${SRC_FILES}
)

# set(SOURCE_FILES src/*.cpp)
add_library(indexes STATIC ${SOURCE_FILES})

# 方式1
# target_link_directories(indexes PRIVATE /opt/homebrew/Cellar/boost/1.82.0_1/lib)

# 方式2
# set(Boost_DIR /opt/homebrew/Cellar/boost/1.82.0_1)
# set_target_properties(
# indexes
# PROPERTIES
# LINK_DIRECTORIES ${Boost_DIR}
# )

# 方式3
# link_directories(/opt/homebrew/Cellar/boost/1.82.0_1/lib)
target_link_libraries(indexes PRIVATE boost_filesystem)

# target_include_directories(indexes PRIVATE include/bitsetindex)
# target_include_directories(indexes PRIVATE include/flatindex)
# target_include_directories(indexes PRIVATE include/setindex)
target_include_directories(indexes PUBLIC include)
