cmake_minimum_required(VERSION 3.10...3.18)

# Or call cmake with -DCMAKE_PREFIX_PATH=/absolute/path/to/libtorch
list(APPEND CMAKE_PREFIX_PATH "$ENV{TORCH_LIB_DIR}")

project(MyLib)

set(CMAKE_CXX_STANDARD 20)
set(PYBIND11_PYTHON_VERSION 3.12)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -fPIC -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto=${N}")
    message(STATUS "Set LTO flags to ${N}")
endif()

set(PYBIND11_FINDPYTHON ON) # Use the FindPython module

find_package(pybind11 REQUIRED)
find_package(OpenMP)
find_package(Torch REQUIRED)
# Use the filesystem component of boost.
find_package(Boost 1.65 REQUIRED COMPONENTS filesystem)


set( EIGEN3_INCLUDE_DIR "$ENV{EIGEN3_INCLUDE_DIR}" )
if( NOT EIGEN3_INCLUDE_DIR )
    message( FATAL_ERROR "Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
endif()


# Add multiple source files to the pybind11 module
pybind11_add_module(${PROJECT_NAME} 
    ./src/pywrap.cpp
    ./src/trytorch.cpp  # Add your additional source file here
    # Add more source files as needed
)

# Linking libraries
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
    message(STATUS "OpenMP found, status:")
    message(STATUS "    libraries: OpenMP_CXX")
endif()

if(Torch_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        ${TORCH_LIBRARIES}
        pybind11::module
    )
    message(STATUS "PyTorch found, status:")
    message(STATUS "    libraries: ${TORCH_LIBRARIES}")
else()
    message(STATUS "PyTorch libraries not found.")
endif()

set_target_properties(${PROJECT_NAME} 
    PROPERTIES POSITION_INDEPENDENT_CODE ON 
    CXX_STANDARD 17
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${PYBIND11_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
    ${EIGEN3_INCLUDE_DIR}
    ${Boost_LIBRARIES}
)

# preprocessor definitions to #define VERSION_INFO
target_compile_definitions(${PROJECT_NAME} PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})

# Eigen is a header-only library, does not need to be linked against.
# There's no binary library to link to, hence no need for a target_link_libraries command for Eigen.
# target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen)