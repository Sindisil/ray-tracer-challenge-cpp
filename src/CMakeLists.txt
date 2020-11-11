add_executable(tests tests.cpp primatives.cpp canvas.cpp)
target_compile_features(tests PRIVATE cxx_std_17)
set_target_properties(tests PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(tests PRIVATE ../extern/doctest)

if (MSVC)
    # warning level 4 and all warnings as errors
    target_compile_options(tests PRIVATE /W4 /WX)
else()
    # lots of warnings and all warnings as errors
    target_compile_options(tests -Wall -Wextra -pedantic)
endif()

