#include <iostream>
#include <Eigen/Dense>
#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#include <boost/thread.hpp>
int main() {
    Eigen::MatrixXd m(2, 2);
    m(0, 0) = 3;
    m(1, 0) = 2.5;
    m(0, 1) = -1;
    m(1, 1) = m(1, 0) + m(0, 1);

    std::cout << "Here1s is the matrix m:\n" << m << std::endl;
    boost::thread thread([]() {
        std::cout << "Hello from a thread!" << std::endl;
    });
	thread.join();
    return 0;
}