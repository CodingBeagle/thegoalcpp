# Specify the minimum version for CMake
# This means that if the running version of CMake on the computer is of a lower version,
# CMake will stop processing the project and report an error.
cmake_minimum_required(VERSION 3.0)

# Project's name
# project command will set the name of the project and store it in the variable PROJECT_NAME
project(thegoal)

# This command will add an executable target with a given name.
# This is an EXE file that should be build from a given number of source files.
# The given name must be globally unique within a project.
add_executable(thegoal)

# The target_sources command can be used to give the source files which the exe is based upon
target_sources(thegoal PRIVATE src/main.cpp src/Shader.cpp ${thegoal_SOURCE_DIR}/libs/glad/src/glad.c)

# Specify where to search for library files
target_link_directories(thegoal PRIVATE ${thegoal_SOURCE_DIR}/libs/glfw/bin)

# Specify which libraries to link to
target_link_libraries(thegoal glfw3.lib opengl32.lib)

# Include directories
# This command will add the given directories to those that the compiler uses to search
# for include files. Relative paths are relative to the current source 
target_include_directories(thegoal PRIVATE ${thegoal_SOURCE_DIR}/libs/glfw/include)
target_include_directories(thegoal PRIVATE ${thegoal_SOURCE_DIR}/libs/glad/include)
target_include_directories(thegoal PRIVATE ${thegoal_SOURCE_DIR}/libs/glm/include)
target_include_directories(thegoal PRIVATE ${thegoal_SOURCE_DIR}/include)

set_target_properties(thegoal PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${thegoal_SOURCE_DIR}/build")

# A custom build rule which will copy all shader files to the output directory
separate_arguments(xcopy_parameter_list WINDOWS_COMMAND "resources build /i /s /e /q /y")
add_custom_command(
	TARGET thegoal
	POST_BUILD
	COMMAND cd ${thegoal_SOURCE_DIR}
	COMMAND xcopy ${xcopy_parameter_list}
)