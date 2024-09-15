cmake_minimum_required(VERSION 3.10)

project(AEAC_core)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(OpenCV REQUIRED)


include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/libs/liblogger/include
    ${OpenCV_INCLUDE_DIRS}  
)


file(GLOB_RECURSE SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/**/*.cpp
    ${PROJECT_SOURCE_DIR}/libs/liblogger/src/**/*.cpp
)


add_executable(${PROJECT_NAME} ${SRC_FILES} ${PROJECT_SOURCE_DIR}/main.cpp)


target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} pthread)
