cmake_minimum_required(VERSION 3.16)

find_package(Qt6 REQUIRED COMPONENTS Core Test Widgets)

set(TEST_SOURCES
    test_main.cpp
    test_base64.cpp
    test_number_converter.cpp
    test_guid_timestamp.cpp
    test_case_converter.cpp
    test_math_evaluator.cpp
    test_crypto_manager.cpp
    test_code_compress_pretty.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/Base64Utils.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/NumberConverter.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/GuidTimestampUtils.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/CaseConverter.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/MathEvaluator.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/CryptoManager.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/CodeCompressPretty.cpp
)

add_executable(NoteSciPadTests ${TEST_SOURCES})

target_include_directories(NoteSciPadTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/utils
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(NoteSciPadTests PRIVATE
    Qt6::Core
    Qt6::Test
    Qt6::Widgets
)

if(APPLE)
    target_link_directories(NoteSciPadTests PRIVATE "${CMAKE_BINARY_DIR}/dummy_frameworks")
    target_link_libraries(NoteSciPadTests PRIVATE "-framework Security")
endif()

add_test(NAME NoteSciPadUnitTests COMMAND NoteSciPadTests)
