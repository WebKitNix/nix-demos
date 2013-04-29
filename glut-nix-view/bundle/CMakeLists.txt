
set(Bundle_SOURCES
  	bundle.c
)

set(Bundle_LIBRARIES
  	${WebKitNix_LIBRARIES}
)

add_library(Bundle SHARED ${Bundle_SOURCES})
target_link_libraries(Bundle ${Bundle_LIBRARIES})
