#include <pybind11/pybind11.h>
#include "mytorch.h"
#include <iostream>
#include <ATen/ATen.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

void init_adder(py::module_ &m) {
    m.def("add", &add, "A function that adds two numbers");
}

void show_zeroes(int a, int b) {
  at::Tensor t = at::zeros({a, b});
  std::cout << t << std::endl;
}

void init_show_zeroes(py::module_ &m) {
    m.def("show_zeroes", &show_zeroes, "A function that prints zeroes");
}