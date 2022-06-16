CPMFindPackage(
    NAME Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.0.1
    EXCLUDE_FROM_ALL YES
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
catch_discover_tests(tests)
