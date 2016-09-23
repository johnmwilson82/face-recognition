cmake_minimum_required (VERSION 2.6)
project (FaceRecognition)
find_package(PkgConfig)
pkg_search_module(GTK3 REQUIRED gtk+-3.0)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++14)
endif()

set(SRC
    src/img_mgr.cpp
    src/eigenfaces.cpp
    src/main_frame.cpp
    src/main_frame_base.cpp
)

add_executable(frgui ${SRC})

find_package(wxWidgets REQUIRED net gl core base)
include(${wxWidgets_USE_FILE})

target_link_libraries(frgui
                      ${wxWidgets_LIBRARIES})
