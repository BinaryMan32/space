cmake_minimum_required(VERSION 3.10)

# Project name and version
project(SpaceGame VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SDL2 and OpenGL
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR} ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})

# Source files
set(SOURCES
    actionbase.cpp
    actionbase.h
    DirectInput.cpp
    DirectInput.h
    DirectSound.cpp
    DirectSound.h
    glstream.cpp
    glstream.h
    glstuff.cpp
    glstuff.h
    image.cpp
    image.h
    list.cpp
    List.h
    main.cpp
    objects.cpp
    objects.h
    particles.cpp
    particles.h
    queue.cpp
    queue.h
    spritebase.cpp
    spritebase.h
    spritemanager.cpp
    spritemanager.h
    StarField.cpp
    StarField.h
    string.cpp
    string.h
    timer.cpp
    timer.h
    vector.cpp
    vector.h
    WinApp.cpp
    WinApp.h
    World.cpp
    World.h
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${OPENGL_gl_LIBRARY})

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
