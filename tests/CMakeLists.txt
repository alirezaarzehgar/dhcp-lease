cmake_minimum_required(VERSION 3.6)

set(PROJECT_NAME lease_test)
set(FAKE_DBS scripts/dhcpd.db)
set(FAKER_SCRIPT scripts/faker.py)

project(${PROJECT_NAME})

find_path(PYTHON_EXECUTABLE_DIR NAMES python3)

if(PYTHON_EXECUTABLE_DIR)
    set(PYTHON_EXECUTABLE ${PYTHON_EXECUTABLE_DIR}/python3)
endif()


find_path(FAKE_DATABASE_PATH NAMES
    ${FAKE_DBS}
)

if(NOT ${FAKE_DATABASE_PATH})
    message(STATUS "running faker scripts")

    execute_process(COMMAND ${PYTHON_EXECUTABLE} ${FAKER_SCRIPT}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE PYTHON_FAKER_SCRIPT_RESULT)

    if(NOT PYTHON_FAKER_SCRIPT_RESULT EQUAL "0")
        message(FATAL_ERROR "can't run script")
    endif()
endif()

file(COPY ${FAKE_DBS} DESTINATION databases)

add_executable(${PROJECT_NAME}
    lease_test.c
    main.c
)

target_link_libraries(${PROJECT_NAME}
    cunit
    lib::lease
)