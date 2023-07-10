cmake_minimum_required( VERSION 3.15 )

project( test_j2_templates )

list( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../.." )

include( ttb_utils )

set( VAR1 "var1" )
set( VAR2 "var2" )

ttb_eval_j2_template(
    TEMPLATE_FILE "${CMAKE_CURRENT_LIST_DIR}/template.j2"
    DATA_FILE "${CMAKE_CURRENT_LIST_DIR}/data.yaml.in"
    OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/output.html"
)
