cmake_minimum_required (VERSION 3.9)

find_package(OpenMP REQUIRED)
if(OPENMP_CXX_FOUND)
message("OPENMP FOUND")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

find_package(Eigen3 3.1.0 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

include_directories(
    ./include
)

aux_source_directory(
    ./src SRC
)


add_executable(main 
    ./examples/main.cpp 
    ${SRC}    
)
target_link_libraries(main
PUBLIC
OpenMP::OpenMP_CXX
)