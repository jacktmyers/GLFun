cmake_minimum_required(VERSION 3.26)
project(Modern)

# Create Library for cfiles
set(CFILES ./src/cFiles)
set(CFILES_HEADER_FILES ${CFILES}/camera.h ${CFILES}/pipeline.h ${CFILES}/shaderUtil.h ${CFILES}/tm.h ${CFILES}/scene.h)
add_library(CFILES_LIB ${CFILES}/camera.cpp ${CFILES}/pipeline.cpp ${CFILES}/shaderUtil.cpp ${CFILES}/tm.cpp ${CFILES}/scene.cpp
		${CFILES_HEADER_FILES})

# Create Library for primatives
set(PRIMATIVES ./src/cFiles/primatives)
set(PRIMATIVES_HEADER_FILES ${PRIMATIVES}/m33.h ${PRIMATIVES}/v3.h)
add_library(PRIMATIVES_LIB ${PRIMATIVES}/m33.cpp ${PRIMATIVES}/v3.cpp
		${PRIMATIVES_HEADER_FILES})

# Create executable
add_executable(Modern "./src/cFiles/main.cpp")

# Find SDL2, OpenGL, and GLEW on system
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# Include directories
# 3rd party
target_include_directories(Modern PUBLIC ${SL2_INCLUDE_DIRS})
target_include_directories(Modern PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(Modern PUBLIC ${GLEW_INCLUDE_DIRS})
# Custom
target_include_directories(Modern PUBLIC ${CFILES})
target_include_directories(Modern PUBLIC ${PRIMATIVES})

# Link Libraries
# 3rd party
target_link_libraries(Modern ${SDL2_LIBRARIES})
target_link_libraries(Modern ${OPENGL_LIBRARIES})
target_link_libraries(Modern ${GLEW_LIBRARIES})
# Custom
target_link_libraries(Modern CFILES_LIB)
target_link_libraries(Modern PRIMATIVES_LIB)

# Set Debug
set(CMAKE_BUILD_TYPE Debug)

