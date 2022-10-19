cmake_minimum_required(VERSION 2.8)
project(demo)


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)




include_directories(${PROJECT_SOURCE_DIR}/src/include)



aux_source_directory(src SRC_LIST)



add_executable(project ${SRC_LIST})

target_link_libraries(project pthread)




