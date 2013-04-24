set(CPUMonitorInjectedBundle_SOURCES
    ../InjectedBundle/Bundle.cpp
)

set(CPUMonitorInjectedBundle_LIBRARIES
    ${WebKit2_LIBRARY_NAME}
)

add_library(CPUMonitorInjectedBundle SHARED ${CPUMonitorInjectedBundle_SOURCES})
target_link_libraries(CPUMonitorInjectedBundle ${CPUMonitorInjectedBundle_LIBRARIES})