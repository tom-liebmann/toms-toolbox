cmake_minimum_required( VERSION 3.10 )

project( "test_app" NONE )

list( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../cmake" )

include( ttb_project )

ttb_init_project( "${CMAKE_CURRENT_LIST_DIR}/src" )
