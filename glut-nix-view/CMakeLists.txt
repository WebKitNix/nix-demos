cmake_minimum_required(VERSION 2.8)
project(GlutView)

find_package(PkgConfig REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
pkg_check_modules(GLIB glib-2.0 REQUIRED)
pkg_check_modules(WebKitNix WebKitNix REQUIRED)

set(CMAKE_CXX_FLAGS "-g -Wall")
set(GlutView_INJECTEDBUNDLE_DIR ${CMAKE_BINARY_DIR}/bundle/)
add_definitions(-DINJECTEDBUNDLE_DIR="${GlutView_INJECTEDBUNDLE_DIR}")

include_directories(GlutView_INCLUDES
	${GLIB_INCLUDE_DIRS}
	${GLUT_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS}
	${WebKitNix_INCLUDE_DIRS}
)

link_directories(
	${GLIB_LIBRARY_DIRS}
	${GLUT_LIBRARY_DIRS}
	${OPENGL_LIBRARY_DIRS}
	${WebKitNix_LIBRARY_DIRS}
)

add_executable(GlutView main.cpp)
target_link_libraries(
	GlutView
	${GLUT_LIBRARIES}
	${OPENGL_LIBRARIES}
	${GLIB_LIBRARIES}
	${WebKitNix_LIBRARIES}
)

add_subdirectory(bundle)
