include(GNUInstallDirs)

add_library(codlili INTERFACE)
# ALIAS target to export a namespaced target even when building in-tree
add_library(Codlili::codlili ALIAS codlili)
# library public header files location
target_include_directories(
    codlili INTERFACE
    # different include directory path depending on if using the local or installed version of library
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
# set up compatible interface properties
set_target_properties(
    codlili PROPERTIES
    INTERFACE_Codlili_MAJOR_VERSION ${PROJECT_VERSION_MAJOR}
    INTERFACE_Codlili_MINOR_VERSION ${PROJECT_VERSION_MINOR}
)
set_property(
    TARGET codlili
    APPEND PROPERTY COMPATIBLE_INTERFACE_STRING "${Codlili_MAJOR_VERSION}.${Codlili_MINOR_VERSION}"
)

# install if we're not being built as a sub-project
if (NOT CODLILI_SUBPROJECT)
    # library
    install(
        TARGETS codlili
        EXPORT CodliliTargets
        # when a static library
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        # when a shared library on UNIX
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        # when a DLL on Windows
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    # public headers
    install(
        DIRECTORY "include/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    # CMake package
    include(CMakePackageConfigHelpers)
    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/CodliliConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Codlili"
    )
    # CMake export
    install(
        EXPORT CodliliTargets
        FILE CodliliTargets.cmake
        NAMESPACE Codlili::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Codlili"
    )
    # generate the version file for the config file
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/CodliliConfigVersion.cmake"
        VERSION "${version}"
        COMPATIBILITY SameMinorVersion
    )
    install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/CodliliConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/CodliliConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Codlili"
    )
endif()
