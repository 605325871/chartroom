aux_source_directory(. SRC_LIST)

include_directories(../include)

add_executable(client ${SRC_LIST})
TARGET_LINK_LIBRARIES(client hiredis)
set(CMAKE_cxx_FLAGS "-O0 -Wall -g2 -ggdb")
# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)