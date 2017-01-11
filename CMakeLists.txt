cmake_minimum_required(VERSION 3.0)

# This is treated a little differently since it
# is written in strict C

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

install(TARGETS memwatch LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
configure_file("memwatchConfig.cmake.in" "memwatchConfig.cmake" @ONLY)
install(FILES memwatch.h DESTINATION include/memwatch)
install(FILES ${CMAKE_BINARY_DIR}/memwatchConfig.cmake DESTINATION share/cmake/memwatch)
