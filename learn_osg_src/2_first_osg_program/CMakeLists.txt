cmake_minimum_required( VERSION 2.6 )
project( 2_first_osg_program )

find_package( OpenThreads )
find_package( osg )
find_package( osgDB )
find_package( osgUtil )
find_package( osgViewer )

macro( config_project PROJNAME LIBNAME )
	include_directories( ${${OSG_ROOT}_INCLUDE_DIR} )
	target_link_libraries( ${PROJNAME} ${${LIBNAME}_LIBRARY} )
endmacro()

add_executable( 2_first_osg_program main.cpp )

config_project( 2_first_osg_program OPENTHREADS )
config_project( 2_first_osg_program OSG )
config_project( 2_first_osg_program OSGDB )
config_project( 2_first_osg_program OSGUTIL )
config_project( 2_first_osg_program OSGVIEWER )