cmake_minimum_required(VERSION 3.15)
project(image_processor CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.0
)
FetchContent_MakeAvailable(Catch2)

set(SRC_COMMON
    src/filter.cpp
    src/image.cpp
    src/bmp_io.cpp
    src/matrix_filter.cpp
    src/pipeline.cpp
    src/filters/negative_filter.cpp
    src/filters/grayscale_filter.cpp
    src/filters/crop_filter.cpp
    src/filters/sharpen_filter.cpp
    src/filters/edge_filter.cpp
    src/filters/blur_filter.cpp
    src/filters/crystallize_filter.cpp
    src/filters/glass_distortion_filter.cpp
)

add_executable(image_processor
    src/main.cpp
    ${SRC_COMMON}
)
target_include_directories(image_processor PRIVATE include)
target_compile_options(image_processor PRIVATE -Wall -Wextra -Wpedantic)

add_executable(test_image
    tests/test_color.cpp
    tests/test_image.cpp
    tests/test_bmp_io.cpp
    tests/test_filters.cpp
    ${SRC_COMMON}
)
target_include_directories(test_image PRIVATE include)
target_link_libraries(test_image PRIVATE Catch2::Catch2WithMain)
target_compile_options(test_image PRIVATE -Wall -Wextra -Wpedantic)

include(Catch)
catch_discover_tests(test_image)