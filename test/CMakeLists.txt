file(GLOB sources *.c *.cc *.cpp *.h *.hh)

add_executable(kb64_test ${sources})
target_link_libraries(kb64_test kb64 ${GMOCK_LIBRARIES} GTest::GTest GTest::Main)

gtest_discover_tests(kb64_test)
