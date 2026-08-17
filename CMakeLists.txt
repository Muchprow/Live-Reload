cmake_minimum_required(VERSION 3.16)
project(live_reloader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(live_reloader_lib STATIC src/file_watcher.cpp)
set_target_properties(live_reloader_lib PROPERTIES OUTPUT_NAME "live_reloader")
target_include_directories(live_reloader_lib PUBLIC include)

add_library(test_plugin SHARED src/test_plugin.cpp)
set_target_properties(test_plugin PROPERTIES PREFIX "")

add_executable(live_reloader src/main.cpp)
target_link_libraries(live_reloader PRIVATE live_reloader_lib)

add_custom_command(TARGET test_plugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:test_plugin>
        $<TARGET_FILE_DIR:live_reloader>/test_plugin.dll
)