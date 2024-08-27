add_executable(sandbox src/main.cpp)

target_include_directories(sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(netlib REQUIRED PATHS $ENV{HOME}/netlib-install/lib/cmake/netlib)

target_link_libraries(sandbox PRIVATE netlib uring)

add_dependencies(sandbox netlib)
