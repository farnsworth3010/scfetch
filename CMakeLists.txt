project(scfetch)
cmake_minimum_required(VERSION 3.14)

add_executable(scfetch ./main.cpp)
file(COPY ./logos DESTINATION $ENV{HOME}/.config/scfetch)
install(TARGETS scfetch DESTINATION bin)