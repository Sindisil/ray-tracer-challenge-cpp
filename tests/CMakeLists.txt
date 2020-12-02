cmake_minimum_required(VERSION 3.18)
project(raytracer VERSION 0.1.0 LANGUAGES CXX)

add_executable(tests tests.cpp test_primatives.cpp test_canvas.cpp "test_matrix.cpp" "test_ray.cpp")
target_include_directories(tests PRIVATE ../src ../extern/doctest)
target_sources(tests PRIVATE ../src/primatives.cpp ../src/canvas.cpp ../src/ray.cpp)
target_compile_features(tests PRIVATE cxx_std_17)
set_target_properties(tests PROPERTIES CXX_EXTENSIONS OFF)

if (MSVC)
    # warning level 4 plus extra warnings
    target_compile_options(tests PRIVATE /W4 /w44388 /w44287)
else()
    # lots of warnings
    target_compile_options(tests PRIVATE -Wall -Wextra -pedantic)
endif()

