cmake_minimum_required(VERSION 3.2)

if(WIN32 OR WIN64)
    message(STATUS "My Windows detected")
    project(bulk VERSION 0.0.1000)
    set(BOOST_ROOT "C:/Boost")
else()
    project(bulk VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
endif()

if(WIN32 OR WIN64)
    add_executable(bulk main.cpp bulk.cpp bulk.h)
else()
    add_executable(bulk main.cpp bulk.cpp)
endif()

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic")
endif()


install(TARGETS bulk RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)