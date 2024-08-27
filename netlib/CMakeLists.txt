cmake_minimum_required(VERSION 3.10)
project(netlib VERSION 1.0.0 LANGUAGES CXX)

add_library(netlib SHARED
        include/netlib/loop.h
        src/loop.cpp
        include/netlib/read_operation.h
        include/netlib.h
        src/read_operation.cpp
        include/netlib/write_operation.h
        src/write_operation.cpp
        include/netlib/connect_operation.h
        src/connect_operation.cpp
        include/netlib/recv_operation.h
        src/recv_operation.cpp
)


target_include_directories(netlib
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)


find_library(IO_URING_LIBRARY NAMES uring REQUIRED)


target_link_libraries(netlib PRIVATE ${IO_URING_LIBRARY})


set_target_properties(netlib PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
        PUBLIC_HEADER "include/netlib.h"
)


include(GNUInstallDirs)
install(TARGETS netlib
        EXPORT netlib-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/netlib
)

# Install the export set
install(EXPORT netlib-targets
        FILE netlibTargets.cmake
        NAMESPACE netlib::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/netlib
)


include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/netlibConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

configure_file(netlibConfig.cmake.in netlibConfig.cmake @ONLY)
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/netlibConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/netlibConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/netlib
)

add_custom_command(TARGET netlib POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:netlib> ${CMAKE_BINARY_DIR}/tests
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:netlib> ${CMAKE_BINARY_DIR}/sandbox
)
