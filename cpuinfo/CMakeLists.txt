cmake_minimum_required(VERSION 2.8)
project(CPUMonitor)

find_package(PkgConfig REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
pkg_check_modules(GLIB glib-2.0 REQUIRED)
pkg_check_modules(WebKitNix WebKitNix REQUIRED)

set(CMAKE_CXX_FLAGS "-g -Wall")

set(CPUMONITOR_INJECTEDBUNDLE_DIR ${CMAKE_BINARY_DIR}/InjectedBundle/)
add_definitions(-DCPUMONITOR_INJECTEDBUNDLE_DIR="${CPUMONITOR_INJECTEDBUNDLE_DIR}")
set(CPUMONITOR_UI_FILE file:///${CMAKE_BINARY_DIR}/Ui/proc.html)
add_definitions(-DCPUMONITOR_UI_FILE="${CPUMONITOR_UI_FILE}")

include_directories(CPUMonitor_INCLUDES
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

add_executable(CPUMonitor main.cpp)
target_link_libraries(
	CPUMonitor
	${GLUT_LIBRARIES}
	${OPENGL_LIBRARIES}
	${GLIB_LIBRARIES}
	${WebKitNix_LIBRARIES}	
)

add_subdirectory(InjectedBundle)
file(COPY Ui DESTINATION ${CMAKE_BINARY_DIR})
