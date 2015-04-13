cmake_minimum_required(VERSION 2.8)

find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)	

if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_executable( test_color_naming main.cpp  )
add_library( color_name color_name.h color_name.cpp)

target_link_libraries( color_name ${OpenCV_LIBS} )
target_link_libraries(  test_color_naming color_name ${OpenCV_LIBS} )

