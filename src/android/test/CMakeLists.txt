cmake_minimum_required( VERSION 3.10 )

project( "test_app" NONE )

include( "${CMAKE_CURRENT_LIST_DIR}/../cmake_tools.cmake" )

ttb_init_project( "${CMAKE_CURRENT_LIST_DIR}/src" )
