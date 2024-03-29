cmake_minimum_required(VERSION 3.20)

project("CL_Single_Phase_AC_Electronic_Load_H743IIT6_Hollies" C CXX ASM)

include(cmake/st-project.cmake)

add_executable(${PROJECT_NAME})
add_st_target_properties(${PROJECT_NAME})