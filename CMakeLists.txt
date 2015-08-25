PROJECT(EIGEN_TESTING)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../opm-cmake/cmake/Modules" CACHE STRING " ")
SET(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build Type")

FIND_PACKAGE(Boost REQUIRED COMPONENTS chrono system)
FIND_PACKAGE(Eigen3 REQUIRED)
FIND_PACKAGE(OpenMP REQUIRED)

INCLUDE_DIRECTORIES(
	${EIGEN3_INCLUDE_DIR})

ADD_DEFINITIONS("-std=c++11 -Wall -Wextra" ${OpenMP_CXX_FLAGS})

#Create extern function
ADD_LIBRARY(extern_lib "src/extern_func.c") 

# - Create test application
ADD_EXECUTABLE(eigen_test "src/test.cpp")
TARGET_LINK_LIBRARIES(eigen_test extern_lib ${Boost_LIBRARIES})

ADD_EXECUTABLE(eigen_slow_sparse "src/eigen_slow_sparse.cpp")
