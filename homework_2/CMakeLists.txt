cmake_minimum_required(VERSION 3.16)
project(ip_filter LANGUAGES CXX)

if(NOT DEFINED BUILD_NUMBER)
  if(DEFINED ENV{GITHUB_RUN_NUMBER})
    set(BUILD_NUMBER $ENV{GITHUB_RUN_NUMBER})
  else()
    set(BUILD_NUMBER 0)
  endif()
endif()

add_executable(ip_filter src/main.cpp)
target_compile_features(ip_filter PRIVATE cxx_std_17)
target_link_options(ip_filter PRIVATE -static-libstdc++ -static-libgcc)

include(GNUInstallDirs)
install(TARGETS ip_filter RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CTest)
enable_testing()
add_test(NAME ip_filter_md5
  COMMAND bash -lc "cat '${CMAKE_CURRENT_LIST_DIR}/test_data/ip_filter.tsv' | '$<TARGET_FILE:ip_filter>' | md5sum | awk '{print $1}'")
set_tests_properties(ip_filter_md5 PROPERTIES
  PASS_REGULAR_EXPRESSION "24e7a7b2270daee89c64d3ca5fb3da1a")

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "ip_filter")
set(CPACK_PACKAGE_VERSION "0.0.${BUILD_NUMBER}")
set(CPACK_PACKAGE_CONTACT "Matthew Naumenko <naumenko33301@gmail.com>")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Matthew Naumenko")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Efficient IPv4 log sorter/filter with reverse lexicographic order")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-Linux")
include(CPack)
