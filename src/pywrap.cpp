// pywrap.cpp
#include <pybind11/pybind11.h>

// The pybind11/eigen.h header allows this matrix to be returned to Python as a NumPy array
//  when the function is called from Python after being exposed through pybind11.
#include <pybind11/eigen.h>
// Again for automatic conversion of stl to python data types
#include <pybind11/stl.h>
#include "mylib.h"
#include<boost/tokenizer.hpp>
namespace py = pybind11;
constexpr auto byref = py::return_value_policy::reference_internal;

void init_adder(py::module_ &);
void init_show_zeroes(py::module_ &);

// Sample of boosts uses
void boost_tokenizer() {
    std::string text = "Boost libraries are great!";
    boost::char_separator<char> sep(" "); // Space is used as a delimiter
    boost::tokenizer<boost::char_separator<char>> tokens(text, sep);

    std::cout << "Tokens:" << std::endl;
    for (const auto& token : tokens) {
        std::cout << token << std::endl;
    }
}

size_t free_function() {
    return 42;
}

namespace module_a {
      size_t func() {
            return 97;
      }
}

// Different methods of adding modules/submodules.
PYBIND11_MODULE(MyLib, m) {
    m.doc() = "optional module docstring";

    m.def("free_function", &free_function);
    m.def("eigen_sample", &eigen_sample);
    m.def("use_class_get_vector", &use_class_get_vector);
    m.def("boost_tokenizer", &boost_tokenizer);

    py::class_<MyClass>(m, "MyClass")
    .def(py::init<double, double, int>())  
    .def("run", &MyClass::run, py::call_guard<py::gil_scoped_release>())
    // Below is not needed since i dont call it in python
    // .def("my_thread_func", &MyClass::my_thread_func, py::call_guard<py::gil_scoped_release>())
    .def_readonly("v_data", &MyClass::v_data, byref)
    .def_readonly("v_gamma", &MyClass::v_gamma, byref)
    .def("vector_getter", &MyClass::vector_getter, "A public func, calling a private func.")
    ;

    auto m_a = m.def_submodule("module_a", "This is A.");
    m_a.def("func", &module_a::func);

    auto adder = m.def_submodule("adder", "A submodule for addition operations");
    init_adder(adder);

    auto zeroes = m.def_submodule("zeroes", "A submodule for addition operations");
    init_show_zeroes(zeroes);
}