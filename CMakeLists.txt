cmake_minimum_required(VERSION 3.16)
project(lstm)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
include_directories(src)
add_executable(lstm
        src/global.cpp
        src/global.h
        src/lstm.cpp
        src/lstm.h
        src/matrix.cpp
        src/matrix.h
        src/mulgate.cpp
        src/mulgate.h
        src/network.cpp
        src/network.h
        src/triples.cpp
        src/triples.h
        src/share.cpp
        src/share.h
        src/triplescheck.h
        )

#add_executable(main
#        src/dataReadin/main.cpp
#        src/dataReadin/data.h
#        )

# GMP
find_path(GMP_INCLUDE_DIR NAMES gmp.h)
find_library(GMP_LIBRARIES NAMES gmp libgmp)
find_library(GMPXX_LIBRARIES NAMES gmpxx libgmpxx)

target_link_libraries(lstm gmp gmpxx)
#target_link_libraries(main gmp gmpxx)
