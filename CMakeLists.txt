cmake_minimum_required(VERSION 3.0.0)
project(victoria VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
include(InstallRequiredSystemLibraries)

set(OpenCV_STATIC OFF)
find_package(OpenCV 3.0.0 REQUIRED core imgproc highgui calib3d objdetect video videoio)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
set_target_properties(${OpenCV_LIBS} PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)

add_executable(victoria victoria.cpp victoria.h)
target_link_libraries(victoria ${OpenCV_LIBS})

include(FeatureSummary)
feature_summary(WHAT ALL)
