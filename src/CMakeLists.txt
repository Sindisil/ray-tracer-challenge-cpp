cmake_minimum_required(VERSION 3.18)
project(raytrace VERSION 0.1.0 LANGUAGES CXX)

add_library(libraytrace STATIC)
target_sources(libraytrace PRIVATE canvas.cpp primitives.cpp ray.cpp sphere.cpp color.cpp)
target_include_directories(libraytrace PUBLIC ../include)
set_target_properties(libraytrace PROPERTIES OUTPUT_NAME "raytrace")
target_compile_features(libraytrace PRIVATE cxx_std_17)

if (MSVC)
    # warning level 4 plus extra warnings
    target_compile_options(libraytrace PRIVATE /W4 /w44388 /w44287)
else()
    # lots of warnings
    target_compile_options(libraytrace PRIVATE -Wall -Wextra -pedantic)
endif()

