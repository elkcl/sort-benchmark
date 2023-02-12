cmake_minimum_required(VERSION 3.14)
project(sort-benchmark)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wpointer-arith -Wtype-limits -Wempty-body -Wno-logical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -DONPC -Wshift-overflow=2 -fsanitize=address,undefined -Wmaybe-uninitialized")
link_libraries(m)
add_executable(main main.c)