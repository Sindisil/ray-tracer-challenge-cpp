add_executable(tests tests.cpp test_primatives.cpp test_canvas.cpp)
target_include_directories(tests PRIVATE ../src ../extern/doctest)
target_sources(tests PRIVATE ../src/primatives.cpp ../src/canvas.cpp)
target_compile_features(tests PRIVATE cxx_std_17)
set_target_properties(tests PROPERTIES CXX_EXTENSIONS OFF)

if (MSVC)
    # warning level 4
    target_compile_options(tests PRIVATE /W4)
else()
    # lots of warnings
    target_compile_options(tests -Wall -Wextra -pedantic)
endif()
