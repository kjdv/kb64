file(GLOB public_headers include/*.hh include/*.h)
file(GLOB sources *.cc *.cpp *.c *.hh *.h)

add_library(kb64
    ${public_headers}
    ${sources}
)

set_target_properties(kb64 PROPERTIES PUBLIC_HEADER "${public_headers}")
target_include_directories(kb64
    PUBLIC
      "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
      "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
      "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
)


