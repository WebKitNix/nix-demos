cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)

pkg_check_modules(WebKitNix WebKitNix REQUIRED)
pkg_check_modules(GLIB glib-2.0 REQUIRED)
pkg_check_modules(CAIRO cairo REQUIRED)
#pkg_check_modules(LIBPROFILER libprofiler)

include_directories(Sample_INCLUDES
    /opt/vc/include
    /opt/vc/include/interface/vmcs_host
    /opt/vc/include/interface/vmcs_host/linux
    /opt/vc/include/interface/vcos/pthreads
    ${WebKitNix_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${CAIRO_INCLUDE_DIRS}
)

link_directories(
    /opt/vc/lib
    ${WebKitNix_LIBRARY_DIRS}
    ${GLIB_LIBRARY_DIRS}
    ${CAIRO_LIBRARY_DIRS}
)

add_executable(Sample main.cpp)
target_link_libraries(Sample
    ${WebKitNix_LIBRARIES}
    ${RASPBERRYPI_LIBRARIES}
    GLESv2
    EGL
    bcm_host
    vcos
    vchiq_arm
#    ${LIBPROFILER_LIBRARIES}
)
