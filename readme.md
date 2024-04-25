# Boiler Plate for Pybind11
Features Eigen to work on arrays. Working on Intel Mac so far.

## Install `pybind11`
Simply installing `pybind11` threw some error so I had to install it with:
```bash
# pip install pybind11 # failed
pip install pybind11[global]
```

## Install Eigen header
Downlad [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page) and extract to `/usr/local`, for our use case, no need to build.

Add to env: `export EIGEN3_INCLUDE_DIR=/usr/local/eigen-3.4.0`

Add to cmake
```cmake
SET( EIGEN3_INCLUDE_DIR "$ENV{EIGEN3_INCLUDE_DIR}" )
IF( NOT EIGEN3_INCLUDE_DIR )
    MESSAGE( FATAL_ERROR "Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
ENDIF()
INCLUDE_DIRECTORIES ( "${EIGEN3_INCLUDE_DIR}" )

add include directory in cmake
include_directories(${PROJECT_SOURCE_DIR}/include)
```

Remove from cmake
```cmake
# find_package(Eigen3 REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen)
```

## Build
```bash
source conf-env
cd pybind11_python
cmake -S . -B build # or cmake -Bbuild -DCMAKE_BUILD_TYPE=Release 
cd build
make
```

## Run
```bash
python ./python/run.py
```

# Reference
* [tutorial](https://www.matecdev.com/posts/cpp-call-from-python.html)
* Differences between `pybind11` and `ctypes`
* [Add](https://stackoverflow.com/questions/12715005/add-c0x-support-in-cmake) `c++0x` support for `threads` 
* [Install openMP](https://www.mathworks.com/help/coder/ug/install-openmp-library-on-macos-platform.html) for Eigen [threading](https://eigen.tuxfamily.org/dox/TopicMultiThreading.html). 
    * I have not tried this yet.
* [Modern openMP](https://cliutils.gitlab.io/modern-cmake/chapters/packages/OpenMP.html)