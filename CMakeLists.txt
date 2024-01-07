project(scfetch)
cmake_minimum_required(VERSION 3.14)

add_executable(scfetch ./main.cpp)
# file(COPY ./logos DESTINATION $ENV{HOME}/.config/scfetch)
install(TARGETS scfetch DESTINATION bin)
add_custom_command(
        TARGET scfetch POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/logos $ENV{HOME}/.config/scfetch/logos)