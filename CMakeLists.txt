cmake_minimum_required(VERSION 4.0)
project(testcpp CXX)
include(cmake/copy_dll.cmake)
find_package(Eigen3 REQUIRED)
find_package(Boost COMPONENTS filesystem system thread locale REQUIRED)

add_executable(testcpp main.cpp)
target_link_libraries(testcpp 
Eigen3::Eigen
Boost::system
Boost::filesystem
Boost::dynamic_linking
Boost::thread
Boost::locale
)

copy_dll(testcpp)