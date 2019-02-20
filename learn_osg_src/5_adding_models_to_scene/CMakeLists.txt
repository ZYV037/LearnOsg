cmake_minimum_required(VERSION 2.8)

project(5_adding_models_to_scene)
add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} "opengl32")
target_link_libraries(${PROJECT_NAME} "freeglut")

find_package(OpenThreads)
find_package(osg)
find_package(osgDB)
find_package(osgUtil)
find_package(osgViewer)
find_package(osgUtil)

macro( config_project LIBNAME)
    target_link_libraries( ${PROJECT_NAME} ${${LIBNAME}_LIBRARY} )
endmacro()

config_project( OPENTHREADS )
config_project( OSG)
config_project( OSGDB)
config_project( OSGUTYIL)
config_project( OSGVIEWER)
config_project( OSGUTIL)
