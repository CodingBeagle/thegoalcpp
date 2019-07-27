# Specify the minimum version for CMake
# This means that if the running version of CMake on the computer is of a lower version,
# CMake will stop processing the project and report an error.
cmake_minimum_required(VERSION 3.0)

# Project's name
# project command will set the name of the project and store it in the variable PROJECT_NAME
project(thegoal)

# Add our src subdirectory to the build.
# The parameter specifies the directory in which the CMakeLists.txt file is located
add_subdirectory(src)