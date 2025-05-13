cmake_minimum_required(VERSION 3.10)
project(QuineMcCluskey)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add source files
set(SOURCES
    src/main.cpp
    src/BooleanFunction.cpp
    src/QuineMcCluskey.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES}) 