cmake_minimum_required(VERSION 3.16)
project(ROSBOT_TESTING)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_FILES 
    include/
    test/include/
)

add_subdirectory(submodules/googletest)

set(TEST_SRC_FILES
    # test/test_packet_serializer.cpp
    test/test_main.cpp
    src/comms/packet_serializer.cpp
    test/test_math.cpp
    src/utility/math.cpp
    src/data-structures/circular_queue.cpp
    test/test_circular_queue.cpp
    test/test_comms.cpp
    src/comms/cli/turtle_shell.cpp
)

# SET(CLI_SRC_FILES
#     src/comms/cli/turtle_shell.cpp
#     src/data-structures/circular_queue.cpp
#     src/cli_main.cpp
#     src/utility/math.cpp
# )


add_executable(${PROJECT_NAME} 
    ${TEST_SRC_FILES}
)

# add_executable(${PROJECT_NAME}_CLI
#     ${CLI_SRC_FILES}
# )

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_FILES})
# target_include_directories(${PROJECT_NAME}_CLI PUBLIC ${INCLUDE_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC gtest gmock)