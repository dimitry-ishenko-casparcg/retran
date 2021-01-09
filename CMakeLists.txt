cmake_minimum_required(VERSION 3.1)
project(retran VERSION 0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-DASIO_STANDALONE)
add_definitions(-DVERSION=${PROJECT_VERSION})

set(SOURCES
    src/args.cpp      src/args.hpp
    src/endpoints.cpp src/endpoints.hpp
    src/main.cpp
    src/server.cpp    src/server.hpp
    src/util.cpp      src/util.hpp
)

include(GNUInstallDirs)

########################
# executable
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} pthread)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES LICENSE.md README.md DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(FILES systemd/retran.service DESTINATION ${CMAKE_INSTALL_LIBDIR}/systemd/user/)

########################
# package
set(CPACK_STRIP_FILES ON)
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "OSC Re-transmission Gateway")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Dimitry Ishenko <dimitry (dot) ishenko (at) (gee) mail (dot) com>")

set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_DEBIAN_PACKAGE_DEPENDS )
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
