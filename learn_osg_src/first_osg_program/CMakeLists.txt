cmake_minimum_required( VERSION 2.6 )
project( MyFirstOsgProgram )

find_package( OpenThreads )
find_package( osg )
find_package( osgDB )
find_package( osgUtil )
find_package( osgViewer )

macro( config_project PROJNAME LIBNAME )
	include_directories( ${${OSG_ROOT}_INCLUDE_DIR} )
	target_link_libraries( ${PROJNAME} ${${LIBNAME}_LIBRARY} )
endmacro()

add_executable( MyFirstOsgProgram main.cpp )

config_project( MyFirstOsgProgram OPENTHREADS )
config_project( MyFirstOsgProgram OSG )
config_project( MyFirstOsgProgram OSGDB )
config_project( MyFirstOsgProgram OSGUTIL )
config_project( MyFirstOsgProgram OSGVIEWER )