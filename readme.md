# Boilerplate for Pybind11
Features Eigen to work on arrays and includes building PyTorch in c++.
> Might not work on ARM due to the Pytorch inclusion.

## Setup

```bash
git clone git@github.com:jptalusan/pybind11_boilerplate.git
cd pybind11_boilerplate
python3 -m venv venv
source venv/bin/activate
pip install -r requirements
mkdir extlib
```
> For Mac/ARM devices, you need to install `pybind11[global]`. Otherwise, `pybind11` will be enough.

## Download Eigen, PyTorch and Boost
[PyTorch](https://pytorch.org/cppdocs/installing.html): Build the C++ distribution of PyTorch. This allows us to use some PyTorch operations within C++. While it might not be faster than Python, everything else (non-torch) should be.

For Linux/Ubuntu18.04+
```bash
wget https://download.pytorch.org/libtorch/nightly/cpu/libtorch-shared-with-deps-latest.zip
unzip libtorch-shared-with-deps-latest.zip -d extlib
export TORCH_LIB_DIR=/absolute/path/to/libtorch
```

For Mac Silicon (ARM64)
```bash
wget https://download.pytorch.org/libtorch/nightly/cpu/libtorch-macos-arm64-latest.zip
unzip libtorch-macos-arm64-latest.zip -d extlib
export TORCH_LIB_DIR=/absolute/path/to/libtorch
```

[Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page): Eigen is a high-performance C++ library for linear algebra, including matrices, vectors, numerical solvers, and related algorithms. It is header-only, making it easy to include in projects, and it's known for its API ease-of-use and compile-time optimization.
```bash
wget https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
tar -xzvf eigen-3.4.0.tar.gz -C extlib
export EIGEN3_INCLUDE_DIR=/absolute/path/to/eigen-3.4.0
```

[Boost](https://www.boost.org/doc/libs/1_85_0/more/getting_started/unix-variants.html), you can install via `apt` or via building the code (see link). 

For Linux/Ubuntu18.04+
```bash
sudo apt install libboost-all-dev
```

For Mac
```bash
brew install boost
```

## Adding Libraries/Headers to CMakeLists.txt
Use `include_directories` to add header files.
```cmake
# Adding Eigen3.
set( EIGEN3_INCLUDE_DIR "$ENV{EIGEN3_INCLUDE_DIR}" )
if( NOT EIGEN3_INCLUDE_DIR )
    MESSAGE( FATAL_ERROR "Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
endif()
include_directories ( "${EIGEN3_INCLUDE_DIR}" )

# Adding a project header.
include_directories(${PROJECT_SOURCE_DIR}/include)
```

Use `target_link_libraries` to add libraries like PyTorch:
```cmake
# Find PyTorch (libtorch)
find_package(Torch REQUIRED)
# Link PyTorch (libtorch)
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
```

Add boost similarly, see here for using boost in [cmake](https://cmake.org/cmake/help/latest/module/FindBoost.html).


## Build
Because the Python script and embedding codes are [not in the same directory](https://github.com/pybind/pybind11/issues/1719), we need to make sure the `${PYTHONPATH}` is specified.
```bash
# For pythonpath
source ./set_env.sh
cd pybind11_python
cmake -S . -B build 
# or cmake -Bbuild -DCMAKE_BUILD_TYPE=Release ..
(cd build && make)
python ./python/run.py
```
> for some reason, on my mac, i can't get the python run.py to see my library, until I copy/paste the .so in the same directory as run.py.

# Folder Structure
The folder structure should be like this. Outside of this, you should have the `Eigen` and `PyTorch` folders.
```.
├── build
├── CMakeLists.txt
├── include
│   ├── mylib.h
│   └── mytorch.h
├── python
│   └── run.py
├── readme.md
├── requirements.txt
├── set_env.sh
└── src
    ├── pywrap.cpp
    └── trytorch.cpp
```

* `src`: contains the main C++ codes that python will be using. `pywrap.cpp` contains the main pybind11 module. I think you can only have 1 main module, and have submodules inside it. 
    ```c++
    // Different methods of adding modules/submodules.
    PYBIND11_MODULE(MyLib, m) {
        m.doc() = "optional module docstring";
        m.def("free_function", &free_function);

        auto m_a = m.def_submodule("module_a", "This is A.");
        m_a.def("func", &module_a::func);
    }
    ```
    `pywrap.cpp` contains `#include<pybind11/stl.h>` which allow for automatic conversion of stl containers to python data types.
* `include`: contains function headers (`mytorch.h`) and some function definitions (`mylib.h`), used by `pywrap.cpp`. This should be included in the `CMakeLists.txt`.
* `python`: contains the python code that imports the module named `MyLib` to be able to call the functions defined in the module, which in turn call C++ code.
    ```python
    import build.MyLib as m
    m.free_function()
    ```

# Reference
Collection of notes
* [tutorial](https://www.matecdev.com/posts/cpp-call-from-python.html): Discussing the differences between `pybind11` and `ctypes`.
* [Add](https://stackoverflow.com/questions/12715005/add-c0x-support-in-cmake) `c++0x` support for `threads` 
* [Install openMP](https://www.mathworks.com/help/coder/ug/install-openmp-library-on-macos-platform.html) for Eigen [threading](https://eigen.tuxfamily.org/dox/TopicMultiThreading.html). 
    * I have not tried this yet.
* [Modern openMP](https://cliutils.gitlab.io/modern-cmake/chapters/packages/OpenMP.html)
* [OpenMP](https://askubuntu.com/questions/144352/how-can-i-install-openmp-in-ubuntu): What is OpenMP and how is it turned on in the CMakeLists.txt?
* [Torch](https://pytorch.org/cppdocs/)
* [Boost](https://stackoverflow.com/questions/125580/what-are-the-advantages-of-using-the-c-boost-libraries): Boost is a colelction of libraries that can fill then eeds of many C++ projects.
* Why we don't have an `add_executable` in the `CMakeLists.txt`:
    * The CMakeLists.txt provided does not need an add_executable command because it is not intended to build an executable program. Instead, it is designed to build a Python extension module using pybind11, which is why the pybind11_add_module command is used.
    * The choice between add_executable and pybind11_add_module (or add_library for non-pybind11 libraries) depends on what you are trying to build. If you are building a standalone application, you would use add_executable. If you are building a library, whether it's a regular C++ library or a Python extension module, you would use add_library or pybind11_add_module, respectively.
* Use `-DCMAKE_PREFIX_PATH` in cmake to lead to locations of built libraries.