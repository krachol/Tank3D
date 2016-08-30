cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_LEGACY_CYGWIN_WIN32 0)

project("Tanks Game")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

enable_testing()

find_package(OpenGL)
if(NOT OPENGL_FOUND)
    message(
        FATAL_ERROR 
        "Did not find OpenGL library. Please install!"
        )
endif()

find_package(GLUT REQUIRED)
if(NOT GLUT_FOUND)
    message(
        FATAL_ERROR 
        "Did not find GLUT library. Please install!"
        )
endif()

find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
    message(
        FATAL_ERROR 
        "Did not find GLEW library. Please install!"
        )
endif()

find_package(GLFW REQUIRED)
if(NOT GLFW_FOUND)
    message(
        FATAL_ERROR 
        "Did not find GLFW library. Please install!"
        )
endif()

include_directories(SYSTEM ${OPENGL_INCLUDE_DIRS})
include_directories(SYSTEM ${GLEW_INCLUDE_DIRS})
include_directories(SYSTEM ${GLUT_INCLUDE_DIRS})
include_directories(SYSTEM ${GLFW_INCLUDE_DIRS})
include_directories(glm/)
include_directories(Lib/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-c++0x-compat --std=c++11")

add_subdirectory(Lib)

add_executable(TanksGame main_file.cc)

#add_custom_command(
#TARGET Szkielet7 POST_BUILD
#COMMAND ${CMAKE_COMMAND} -E copy_directory
#${CMAKE_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:Szkielet7>)

add_custom_target(
    CopyResources
        ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Resources ${CMAKE_CURRENT_BINARY_DIR}
        VERBATIM
    COMMENT "Copying Resources to build dir"
    )

add_dependencies(TanksGame CopyResources)

target_link_libraries(
    TanksGame
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLUT_LIBRARIES}
    ${GLFW_LIBRARIES}
    GameEngine
    )

add_custom_target(
    run
    ./TanksGame
    VERBATIM
    )

add_dependencies(run TanksGame)

