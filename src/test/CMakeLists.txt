cmake_minimum_required(VERSION 3.9)
project(cmake_test)

# Enables testing from this directory downwards.
enable_testing(true)

# Prepare "Catch" library for other executables.
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Make main executable that will be linked with other tests.
set(TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests Catch)

set(TEST_SUBDIRS
        maze
        thickmaze
        types
        )

# We build up a file to execute all the test cases.
configure_file(run_tests.sh run_tests.sh COPYONLY)
set(test_script "${CMAKE_CURRENT_BINARY_DIR}/run_tests.sh")

function(make_tests pkgname tests)
    foreach(testname ${tests})
        add_executable(${testname} "${pkgname}/${testname}.cpp" ${TEST_SOURCES})
        target_link_libraries(${testname} Catch spelunker)
        file(APPEND ${test_script} "printname ${testname} && ${CMAKE_CURRENT_BINARY_DIR}/${testname} && \\\n")
    endforeach()
endfunction()


foreach (pkgname ${TEST_SUBDIRS})
    add_subdirectory(${pkgname})
    make_tests(${pkgname} "${${pkgname}_tests}")
endforeach ()

# Terminate sequence of && with true.
# TEST: deliberately fail to see if Travis reports as failure.
file(APPEND ${test_script} "true")
file(COPY ${test_script} DESTINATION ${CMAKE_BINARY_DIR} FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
file(REMOVE ${test_script})
