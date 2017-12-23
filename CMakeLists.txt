cmake_minimum_required(VERSION 3.2)

project(allocator VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

configure_file(version.h.in version.h)

add_executable(allocator main.cpp)

#add_library(allocator_lib allocator.h allocator.cpp)

#add_executable(allocator_tests allocator_tests.cpp)

set_target_properties(allocator #allocator_lib allocator_tests
        PROPERTIES
                CXX_STANDARD 14
                CXX_STANDARD_REQUIRED ON
                CMAKE_CXX_EXTENSIONS OFF
                COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

#set_target_properties(allocator_tests
#        PROPERTIES
#                COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
#                INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
#)

#target_link_libraries(allocator
#        allocator_lib
#)

#target_link_libraries(allocator_tests
#        ${Boost_LIBRARIES}
#        allocator_lib
#)

install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT edkolotilov@gmail.com)

include (CPack)

#enable_testing()

#add_test(check_split allocator_tests)
#add_test(check_ip_to_int allocator_tests)
#add_test(check_ip_to_str allocator_tests)
#add_test(check_filter allocator_tests)
