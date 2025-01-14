cmake_minimum_required(VERSION 3.10...3.30)

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

# To remove cmake warning regarding boost.
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

# Under construction
# set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp")
# set(OpenMP_C_LIB_NAMES omp)
# set(OpenMP_omp_LIBRARY ${OpenMP_C_LIB_NAMES})
# set(OpenMP_C_INCLUDE_DIR "${CMAKE_PREFIX_PATH}/opt/libomp/include") # add this!

find_package(pybind11 REQUIRED)
find_package(OpenMP)
find_package(Torch REQUIRED)
# Use the filesystem component of boost.
find_package(Boost 1.87 REQUIRED COMPONENTS filesystem)

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
# Under construction
# target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_PREFIX_PATH}/opt/libomp/lib) # add this!

if(Torch_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        ${TORCH_LIBRARIES}
        ${TORCH_PYTHON_LIBRARY}
        pybind11::module
    )
    message(STATUS "PyTorch found, status:")
    message(STATUS "    libraries: ${TORCH_LIBRARIES}")
else()
    message(STATUS "PyTorch libraries not found.")
endif()

if(Boost_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${Boost_LIBRARIES}
    )
    message(STATUS "Boost found, status:")
    message(STATUS "    libraries: ${Boost_LIBRARIES}")
else()
    message(STATUS "Install boost via apt or build it first.")
endif()


set_target_properties(${PROJECT_NAME} 
    PROPERTIES POSITION_INDEPENDENT_CODE ON 
    CXX_STANDARD 17
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${PYBIND11_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
    ${EIGEN3_INCLUDE_DIR}
)

# preprocessor definitions to #define VERSION_INFO
target_compile_definitions(${PROJECT_NAME} PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})

# Eigen is a header-only library, does not need to be linked against.
# There's no binary library to link to, hence no need for a target_link_libraries command for Eigen.
# target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen)

# Link Boost libraries (if necessary), certain boost libraries are header-only
# thus, does not need to be linked
# target_link_libraries(boost_example ${Boost_LIBRARIES})