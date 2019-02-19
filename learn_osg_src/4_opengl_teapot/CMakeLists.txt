cmake_minimum_required(VERSION 2.8)

project(4_opengl_teapot)
add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} "opengl32")
target_link_libraries(${PROJECT_NAME} "freeglut")

find_package(OpenThreads)
find_package(osg)
find_package(osgDB)
find_package(osgUtil)
find_package(osgViewer)
find_package(osgUtil)

macro( config_project PROJNAME LIBNAME)
    target_link_libraries( ${PROJNAME} ${${LIBNAME}_LIBRARY} )
endmacro()

config_project(4_opengl_teapot OPENTHREADS )
config_project(4_opengl_teapot OSG)
config_project(4_opengl_teapot OSGDB)
config_project(4_opengl_teapot OSGUTYIL)
config_project(4_opengl_teapot OSGVIEWER)
config_project(4_opengl_teapot OSGUTIL)
