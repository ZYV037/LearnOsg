cmake_minimum_required(VERSION 2.8)

project(3_build_geometry_models)
add_executable(${PROJECT_NAME} "main.cpp")

find_package(OpenThreads)
find_package(osg)
find_package(osgDB)
find_package(osgUtil)
find_package(osgViewer)
find_package(osgUtil)
include_directories( ${${OSG_ROOT}/include} )

macro( config_project PROJNAME LIBNAME)
    target_link_libraries( ${PROJNAME} ${${LIBNAME}_LIBRARY} )
endmacro()

config_project(3_build_geometry_models OPENTHREADS )
config_project(3_build_geometry_models OSG)
config_project(3_build_geometry_models OSGDB)
config_project(3_build_geometry_models OSGUTYIL)
config_project(3_build_geometry_models OSGVIEWER)
config_project(3_build_geometry_models OSGUTIL)
