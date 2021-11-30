cmake_minimum_required(VERSION 3.2)

project(allocator_test)

# Locate GTest
find_package(GTest REQUIRED)
find_package(Threads)

add_executable(gtest_allocator
    main_test.cpp  
    version_test.cpp       
)

set_target_properties(gtest_allocator PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

 target_include_directories(
    gtest_allocator
    PRIVATE
    ${GTEST_INCLUDE_DIRS}
    ${TEST_SOURCE_DIR}
)

target_link_libraries(gtest_allocator
    ${GTEST_LIBRARIES} 
    ${CMAKE_THREAD_LIBS_INIT}
    allocator_lib
)

if (MSVC)
    target_compile_options(gtest_allocator PRIVATE
        /W4
    )
else ()
    target_compile_options(gtest_allocator PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

add_test(gtest_allocator gtest_allocator)