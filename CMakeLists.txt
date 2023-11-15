cmake_minimum_required(VERSION 3.16)
project(ROSBOT_TESTING)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_FILES 
    include/
)

add_subdirectory(submodules/googletest)

set(SRC_FILES
    test/test_packet_serializer.cpp
    src/comms/packet_serializer.cpp
    test/test_math.cpp
    src/utility/math.cpp
    test/test_main.cpp
    src/data-structures/circular_queue.cpp
    test/test_circular_queue.cpp
    test/mock-objects/SerialConnection.cpp
)


add_executable(${PROJECT_NAME} 
    ${SRC_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC gtest gmock)