# This command will add an executable target with a given name.
# This is an EXE file that should be build from a given number of source files.
# The given name must be globally unique within a project.
add_executable(thegoal)

set_target_properties(thegoal PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${thegoal_SOURCE_DIR}/build")

target_sources(thegoal PRIVATE main.cpp Shader.cpp ${thegoal_SOURCE_DIR}/libs/glad/src/glad.c)

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

# A custom build rule which will copy all shader files to the output directory
separate_arguments(xcopy_parameter_list WINDOWS_COMMAND "resources build /i /s /e /q /y")
add_custom_command(
	TARGET thegoal
	POST_BUILD
	COMMAND cd ${thegoal_SOURCE_DIR}
	COMMAND xcopy ${xcopy_parameter_list}
)