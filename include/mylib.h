// mylib.h
#include <Eigen/Dense>
#include <cmath>
#include <thread>
#include <iostream>
#include <omp.h>

using Eigen::Matrix, Eigen::Dynamic;
typedef Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> myMatrix;


void my_thread_func()
{
    std::cout<<"hello"<<std::endl;
}

class MyClass {

    int N;
    double a;
    double b;

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



    void run() 
    { 
        std::thread t(my_thread_func);
        // std::thread t(&MyClass::my_thread_func, this); // using this to refer to the member function

        v_data = Eigen::VectorXd::LinSpaced(N, a, b); 

        auto gammafunc = [](double it) { return std::tgamma(it); };
        v_gamma = v_data.unaryExpr(gammafunc);
        t.join();

        // openMP Hello world
        #pragma omp parallel
        {
            printf("Hello World... from thread = %d\n", 
                omp_get_thread_num());
        }  

    }


};