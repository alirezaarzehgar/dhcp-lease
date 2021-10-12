cmake_minimum_required(VERSION 3.6)

if(TARGET ${PROJECT_NAME})
    message(STATUS "this project already imported")
    return()
endif()

set(PROJECT_NAME lease)
set(LEASE_TESTS tests)

project(${PROJECT_NAME})

find_path(CUNIT_INCLUDE_DIR NAMES CUnit/CUnit.h)

find_path(SQLITE3_INCLUDE_DIR NAMES sqlite3.h)

find_library(CUNIT_LIBRARY NAMES
    unitlib
    libcunit
    cunit
)

find_library(SQLITE_LIBRARY NAMES
    sqlite3
    libsqlite3
    sqlite
)

if(NOT CUNIT_INCLUDE_DIR)
    message(FATAL_ERROR "CUnit headers not found")
endif()

if(NOT SQLITE3_INCLUDE_DIR)
    message(FATAL_ERROR "sqlite3 headers not found")
endif()


if(NOT CUNIT_LIBRARY)
    message(FATAL_ERROR "CUnit not found!")
endif()

if(NOT SQLITE_LIBRARY)
    message(FATAL_ERROR "sqlite3 not found!")
endif()

add_library(${PROJECT_NAME}
    src/lease.c
)

target_link_libraries(${PROJECT_NAME}
    cunit
    sqlite3
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

add_subdirectory(${LEASE_TESTS})