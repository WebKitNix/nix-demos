cmake_minimum_required(VERSION 2.8)
project(QTGLView)

find_package(PkgConfig REQUIRED)
find_package(Qt4 REQUIRED)
find_package(OpenGL REQUIRED)
pkg_check_modules(WebKitNix WebKitNix REQUIRED)

set(QT_USE_QTOPENGL TRUE)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

include_directories(
    ${WebKitNix_INCLUDE_DIRS}
)

link_directories(
    ${WebKitNix_LIBRARY_DIRS}
)

set(QTGLViewSources
    main.cpp
    qglview.cpp
)

qt4_wrap_cpp(QTGLView_Headers_MOC
    qglview.h
)

add_executable(qtglview ${QTGLViewSources} ${QTGLView_Headers_MOC})
target_link_libraries(qtglview ${QT_LIBRARIES} ${OPENGL_LIBRARIES} ${WebKitNix_LIBRARIES})
