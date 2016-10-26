cmake_minimum_required (VERSION 2.6)
project (FaceRecognition)
find_package(PkgConfig)
pkg_search_module(GTK3 REQUIRED gtk+-3.0)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++14)
endif()

set(SRC
    src/classifier.cpp
    src/projector.cpp
    src/img_mgr.cpp
    src/eigenfaces.cpp
    src/fisherfaces.cpp
    src/mlp.cpp
    src/report.cpp
    src/result.cpp
    src/main_frame.cpp
    src/main_frame_base.cpp
)

add_executable(frgui ${SRC})

find_package(wxWidgets REQUIRED net gl core base adv propgrid)
include(${wxWidgets_USE_FILE})

target_link_libraries(frgui
                      ${wxWidgets_LIBRARIES})
