aux_source_directory(. SRC_LIST)

include_directories(../include)

add_executable(Serv ${SRC_LIST})
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall")
TARGET_LINK_LIBRARIES(Serv hiredis)
TARGET_LINK_LIBRARIES(Serv pthread)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)