CPMFindPackage(
    NAME Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.0.1
    EXCLUDE_FROM_ALL YES
)

# common option-propagating target to use for test suite sub-targets
add_library(tests-config INTERFACE)
target_link_libraries(
    tests-config
    INTERFACE
        codlili-compiler-options  # tests use same compiler options as main project
        codlili
        Catch2::Catch2  # unit testing framework
)

add_executable(tests)
target_sources(
    tests PRIVATE
        Container.cpp
)
target_link_libraries(
    tests PRIVATE
        codlili-compiler-options  # tests use same compiler options as main project
        codlili
        Catch2::Catch2WithMain  # unit testing framework
)

enable_testing()

# auto-discover and add Catch2 tests from unit tests program
include(CTest)
include(Catch)

# TODO: we might consider having one separate test executable per part, but not for now
catch_discover_tests(tests WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
