if(CONFIG_CUSTOM_DISPLAY)
    zephyr_library_named(custom_display)
    zephyr_library_sources(src/custom_display.c)
    zephyr_library_sources(src/demo_cycle.c)
    zephyr_library_include_directories(${CMAKE_CURRENT_LIST_DIR}/src)
    zephyr_library_include_directories(${CMAKE_CURRENT_LIST_DIR}/../resources/pet)
    zephyr_library_include_directories(${CMAKE_CURRENT_LIST_DIR}/../resources/icons)
    zephyr_library_include_directories(${CMAKE_CURRENT_LIST_DIR}/../resources/fonts)
    zephyr_library_include_directories(${CMAKE_CURRENT_LIST_DIR}/../resources/demos)
    # ZMK app headers (behavior.h etc.) are PRIVATE to the app target,
    # so external modules must add the path explicitly.
    zephyr_library_include_directories(${APPLICATION_SOURCE_DIR}/include)
endif()
