cmake_minimum_required(VERSION 3.0)
project(memwatch C)
set(memwatch_VERSION 1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS False)

# For standard install paths (CMAKE_INSTALL_*DIR)
include(GNUInstallDirs)

# CMake doesn't support Intel C standard until cmake 3.6
if("${CMAKE_C_COMPILER_ID}" MATCHES "Intel")
  if("${CMAKE_VERSION}" VERSION_LESS "3.6")
      add_compile_options(-std=c${CMAKE_C_STANDARD})
  endif()
endif()


if("${CMAKE_C_COMPILER_ID}" MATCHES "INTEL")
  list(APPEND MEMWATCH_C_FLAGS "-std=c99")
  list(APPEND MEMWATCH_C_FLAGS "-w3")
elseif("${CMAKE_C_COMPILER_ID}" MATCHES "GNU")
  list(APPEND MEMWATCH_C_FLAGS "-std=c99")
  list(APPEND MEMWATCH_C_FLAGS "-Wall;-Wextra;-pedantic")
  list(APPEND MEMWATCH_C_FLAGS "-Wshadow")
endif()

add_library(memwatch SHARED memwatch.c)
target_compile_options(memwatch PRIVATE ${MEMWATCH_C_FLAGS}) 
target_link_libraries(memwatch PUBLIC dl)

install(TARGETS memwatch
        EXPORT memwatchTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

configure_file("memwatchConfig.cmake.in" "memwatchConfig.cmake" @ONLY)
install(FILES memwatch.h DESTINATION include/memwatch)
install(FILES ${CMAKE_BINARY_DIR}/memwatchConfig.cmake DESTINATION share/cmake/memwatch)


####################################
# Exporting the CMake configuration
####################################
include(CMakePackageConfigHelpers)

configure_package_config_file(memwatchConfig.cmake.in
                              ${CMAKE_CURRENT_BINARY_DIR}/memwatchConfig.cmake
                              INSTALL_DESTINATION share/cmake/memwatch
                              NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/memwatchConfigVersion.cmake
                                 VERSION ${memwatch_VERSION}
                                 COMPATIBILITY AnyNewerVersion
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/memwatchConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/memwatchConfigVersion.cmake
        DESTINATION share/cmake/memwatch
)

install(EXPORT memwatchTargets
        NAMESPACE memwatch::
        DESTINATION share/cmake/memwatch
)
