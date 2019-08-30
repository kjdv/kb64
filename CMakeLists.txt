cmake_minimum_required(VERSION 3.10)

project(kb64)
set(CMAKE_CXX_STANDARD 17)

include(GoogleTest)
find_package(GTest MODULE REQUIRED)
find_library(GMOCK_LIBRARIES gmock)
enable_testing()


add_subdirectory(lib)
add_subdirectory(test)

# install rules
install(TARGETS kb64
        EXPORT Kb64Config
        ARCHIVE DESTINATION lib COMPONENT lib
        PUBLIC_HEADER DESTINATION include/kb64 COMPONENT dev
)

export(TARGETS kb64 NAMESPACE Kb64:: FILE ${CMAKE_CURRENT_BINARY_DIR}/Kb64Config.cmake)
install(EXPORT Kb64Config DESTINATION share/kb64 NAMESPACE Kb64::)
