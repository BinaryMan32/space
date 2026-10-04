cmake_minimum_required(VERSION 3.16)

# Project name and version
project(SpaceGame VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SDL3, SDL3_ttf and OpenGL
find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_ttf REQUIRED CONFIG)
find_package(OpenGL REQUIRED)

# Source files
set(SOURCES
    actionbase.cpp
    actionbase.h
    App.cpp
    App.h
    glstream.cpp
    glstream.h
    glstuff.cpp
    glstuff.h
    image.cpp
    image.h
    Input.cpp
    Input.h
    List.h
    main.cpp
    objects.cpp
    objects.h
    particles.cpp
    particles.h
    Sound.cpp
    Sound.h
    spritebase.cpp
    spritebase.h
    spritemanager.cpp
    spritemanager.h
    StarField.cpp
    StarField.h
    timer.cpp
    timer.h
    vector.cpp
    vector.h
    World.cpp
    World.h
)

# list.cpp holds template definitions included by List.h
set(TEMPLATE_SOURCES
    list.cpp
)
set_source_files_properties(${TEMPLATE_SOURCES} PROPERTIES HEADER_FILE_ONLY TRUE)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${TEMPLATE_SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf OpenGL::GL OpenGL::GLU)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Images, sounds and fonts are loaded by relative path, so copy them next to the executable
file(GLOB ASSETS CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/*.tga
    ${CMAKE_SOURCE_DIR}/*.wav
    ${CMAKE_SOURCE_DIR}/*.ttf
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ASSETS} $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
