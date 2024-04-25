cmake_minimum_required(VERSION 3.10)

project(MyLib)
set(CMAKE_CXX_STANDARD 20)
set(PYBIND11_PYTHON_VERSION 3.11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -fPIC -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

SET( EIGEN3_INCLUDE_DIR "$ENV{EIGEN3_INCLUDE_DIR}" )
IF( NOT EIGEN3_INCLUDE_DIR )
    MESSAGE( FATAL_ERROR "Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
ENDIF()
INCLUDE_DIRECTORIES ( "${EIGEN3_INCLUDE_DIR}" )
include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(pybind11 REQUIRED)
# find_package(Eigen3 REQUIRED)


pybind11_add_module(${PROJECT_NAME} ./src/pywrap.cpp)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_compile_definitions(${PROJECT_NAME} PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
target_include_directories(${PROJECT_NAME} PRIVATE ${PYBIND11_INCLUDE_DIRS})
# target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen)
