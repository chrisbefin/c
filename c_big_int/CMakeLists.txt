


MESSAGE( STATUS "CMAKE_VERSION            : ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" )
MESSAGE( STATUS "CMAKE_SYSTEM             : ${CMAKE_SYSTEM}" )
MESSAGE( STATUS "CMAKE_SYSTEM_NAME        : ${CMAKE_SYSTEM_NAME}" )
MESSAGE( STATUS "CMAKE_SYSTEM_VERSION     : ${CMAKE_SYSTEM_VERSION}" )
MESSAGE( STATUS "CMAKE_SYSTEM_PROCESSOR   : ${CMAKE_SYSTEM_PROCESSOR}" )
MESSAGE( STATUS "CMAKE_PREFIX_PATH        : ${CMAKE_PREFIX_PATH}" )
MESSAGE( STATUS "CMAKE_SOURCE_DIR         : ${CMAKE_SOURCE_DIR}" )
MESSAGE( STATUS "CMAKE_CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}" )
MESSAGE( STATUS "CMAKE_CURRENT_BINARY_DIR : ${CMAKE_CURRENT_BINARY_DIR}" )
MESSAGE( STATUS "CMAKE_COMMAND            : ${CMAKE_COMMAND}")

MESSAGE( STATUS "CMAKE_CXX_COMPILER       : ${CMAKE_CXX_COMPILER}" )
MESSAGE( STATUS "CMAKE_C_COMPILER         : ${CMAKE_C_COMPILER}" )


# Make ctest.h available for include
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/ctest )

# Collect sources for big_int test
FILE( GLOB BIG_INT_TEST_SOURCES big_int.c test_big_int.c fib.c)

# Create big_int_test executable
ADD_EXECUTABLE( big_int_test ${BIG_INT_TEST_SOURCES} )
SET_TARGET_PROPERTIES( ${big_int_test} PROPERTIES OUTPUT_NAME "big_int_test" )

# Link ctest library
TARGET_LINK_LIBRARIES( big_int_test ctestlib )
