cmake_minimum_required(VERSION 3.4...3.18)
project(collision_checker)

# First we have to find our dependencies:
FIND_PACKAGE(icl_core REQUIRED )
FIND_PACKAGE(gpu_voxels REQUIRED)
FIND_PACKAGE(Boost COMPONENTS system REQUIRED)
FIND_PACKAGE(CUDA REQUIRED)
FIND_PACKAGE(ompl REQUIRED)

# This is a quirk and should be removed in upcoming versions
# If you built GPU Voxels without ROS support, remove this.
FIND_PACKAGE(orocos_kdl REQUIRED)

# A little debug info:
MESSAGE(STATUS "GVL include dirs are: ${gpu_voxels_INCLUDE_DIRS}")
MESSAGE(STATUS "OMPL include dirs are: ${OMPL_INCLUDE_DIRS}")

# Also we have to inherit some Environment definitions required for our base libs:
add_definitions(
  ${icl_core_DEFINITIONS}
  ${gpu_voxels_DEFINITIONS}
)

add_subdirectory(pybind11)
pybind11_add_module(collision_checker src/collision_checker.cpp)

target_include_directories (collision_checker
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${gpu_voxels_INCLUDE_DIRS}
    PUBLIC ${icl_core_INCLUDE_DIRS}
    PUBLIC ${CUDA_INCLUDE_DIRS}
    PUBLIC ${OMPL_INCLUDE_DIRS}
    PUBLIC ${orocos_kdl_INCLUDE_DIRS} # this should be removed in upcoming versions.
)

# Link the executable to the library.
# We currently also have to link against Boost and icl_core...
target_link_libraries (collision_checker
    PRIVATE ${Boost_SYSTEM_LIBRARY}
    PRIVATE ${icl_core_LIBRARIES}
    PRIVATE ${gpu_voxels_LIBRARIES}
    PRIVATE ${OMPL_LIBRARIES}
    PRIVATE ${CUDA_LIBRARIES}
)

# EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(collision_checker
                           PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
