add_executable(projectile projectile.cpp  primatives.cpp)
target_compile_features(projectile PRIVATE cxx_std_17)
set_target_properties(projectile PROPERTIES CXX_EXTENSIONS OFF)
target_compile_definitions(projectile PRIVATE DOCTEST_CONFIG_DISABLE)

add_executable(plot_projectile plot_projectile.cpp  primatives.cpp canvas.cpp)
target_compile_features(plot_projectile PRIVATE cxx_std_17)
set_target_properties(plot_projectile PROPERTIES CXX_EXTENSIONS OFF)
target_compile_definitions(plot_projectile PRIVATE DOCTEST_CONFIG_DISABLE)

if (MSVC)
    # warning level 4
    target_compile_options(projectile PRIVATE /W4)
    target_compile_options(plot_projectile PRIVATE /W4)
else()
    # lots of warnings
    target_compile_options(projectile -Wall -Wextra -pedantic)
    target_compile_options(plot_projectile -Wall -Wextra -pedantic)
endif()

