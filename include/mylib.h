// mylib.h
#include <Eigen/Dense>
// #include <torch/torch.h>
#include <ATen/ATen.h>
#include <cmath>
#include <thread>
#include <iostream>
#include <omp.h>

using Eigen::Matrix, Eigen::Dynamic;
typedef Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> myMatrix;

// Sample code using Eigen and TypeDef
myMatrix eigen_sample() {
    myMatrix mat(3, 3);
    return mat;
}

// Sample stand alone function used in Class
void my_thread_func()
{
    std::cout<<"hello"<<std::endl;
}

// Sample torch call, using `at` compiles faster than including the entire torch.
// see: https://pytorch.org/cppdocs/
void torch_sample() {
  at::Tensor tensor = at::rand({2, 3});
  std::cout << tensor << std::endl;
}

// Sample class, showing how it can be called in the PyBind11 module.
class MyClass {

    int N;
    double a;
    double b;

private:

    // Showing how pybind11/stl.h maps stl containers directly to python data types.
    std::vector<int> make_vector() {
        return {1, 2, 3, 4, 5};
    }

public:

    Eigen::VectorXd v_data;
    Eigen::VectorXd v_gamma;

    MyClass(){}
    MyClass( double a_in, double b_in, int N_in) 
    {
        N = N_in;
        a = a_in;
        b = b_in;
    }

    
    void test() {
        std::cout << "Member variable called." << std::endl;
    }

    std::vector<int> vector_getter() {
        return make_vector();
    }

    void run() 
    { 
        torch_sample();
        
        std::thread t(my_thread_func);
        test();
        std::thread x(&MyClass::test, this); // using this to refer to the member function

        v_data = Eigen::VectorXd::LinSpaced(N, a, b); 

        auto gammafunc = [](double it) { return std::tgamma(it); };
        v_gamma = v_data.unaryExpr(gammafunc);
        t.join();
        x.join();

        // openMP Hello world
        #pragma omp parallel
        {
            printf("Hello World... from thread = %d\n", 
                omp_get_thread_num());
        }  

    }


};

// Stand alone function that uses the class defined above.
std::vector<int> use_class_get_vector() {
  MyClass c = MyClass();
  std::vector<int> v = c.vector_getter();
  return v;
}