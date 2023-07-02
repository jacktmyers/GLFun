cmake_minimum_required(VERSION 3.26)
project(Modern)

add_subdirectory(src/cFiles)

# Create executable
add_executable(Modern "./src/cFiles/main.cpp")

# Find SDL2, OpenGL, and GLEW on system
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

target_include_directories(Modern PUBLIC ${SL2_INCLUDE_DIRS})
target_include_directories(Modern PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(Modern PUBLIC ${GLEW_INCLUDE_DIRS})
target_link_libraries(Modern ${SDL2_LIBRARIES})
target_link_libraries(Modern ${OPENGL_LIBRARIES})
target_link_libraries(Modern ${GLEW_LIBRARIES})

target_link_libraries(Modern cFiles)
target_include_directories(Modern PUBLIC ${PROJECT_BINARY_DIR} "./src/cFiles")



