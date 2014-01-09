project(nix)
cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)
pkg_check_modules(WebKitNix WebKitNix REQUIRED)
find_package(PythonLibs 2.7)

set(nix_SRC
    nixmodule.cpp
    wkcontext.cpp
    wkpage.cpp
    nixmouseevent.cpp
    nixview.cpp
)

include_directories(
    ${WebKitNix_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
)

link_directories(
    ${WebKitNix_LIBRARY_DIRS}
)


add_library(nix MODULE ${nix_SRC})
set_property(TARGET nix PROPERTY PREFIX "")
target_link_libraries(nix
    ${WebKitNix_LIBRARIES}
    ${PYTHON_LIBRARIES}
)

