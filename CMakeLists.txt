cmake_minimum_required(VERSION 4.0)
project(GNR CXX)
include(cmake/copy_dll.cmake)
find_package(Eigen3 REQUIRED)
find_package(Boost COMPONENTS filesystem system thread locale REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} 
Eigen3::Eigen
Boost::system
Boost::filesystem
Boost::dynamic_linking
Boost::thread
Boost::locale
)

copy_dll(${PROJECT_NAME})