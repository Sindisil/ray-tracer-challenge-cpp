cmake_minimum_required(VERSION 3.18)
project(raytracer VERSION 0.1.0 LANGUAGES CXX)

add_executable(projectile projectile.cpp  primitives.cpp)
target_compile_features(projectile PRIVATE cxx_std_17)
set_target_properties(projectile PROPERTIES CXX_EXTENSIONS OFF)
target_compile_definitions(projectile PRIVATE DOCTEST_CONFIG_DISABLE)

add_executable(plot_projectile plot_projectile.cpp  primitives.cpp canvas.cpp)
target_compile_features(plot_projectile PRIVATE cxx_std_17)
set_target_properties(plot_projectile PROPERTIES CXX_EXTENSIONS OFF)
target_compile_definitions(plot_projectile PRIVATE DOCTEST_CONFIG_DISABLE)

add_executable(clock clock.cpp  primitives.cpp canvas.cpp)
target_compile_features(clock PRIVATE cxx_std_17)
set_target_properties(clock PROPERTIES CXX_EXTENSIONS OFF)
target_compile_definitions(clock PRIVATE DOCTEST_CONFIG_DISABLE)

if (MSVC)
    # warning level 4 plus extra warnings
    target_compile_options(projectile PRIVATE /W4 /w44388 /w44287)
    target_compile_options(plot_projectile PRIVATE /W4 /w44388 /w44287)
    target_compile_options(clock PRIVATE /W4 /w44388 /w44287)
else()
    # lots of warnings
    target_compile_options(projectile PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(plot_projectile PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(clock PRIVATE -Wall -Wextra -pedantic)
endif()

