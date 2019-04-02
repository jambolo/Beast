cmake_minimum_required (VERSION 3.10)
project(Beast VERSION 0.1.0 LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build libraries as DLLs" FALSE)

#########################################################################
# Build                                                                 #
#########################################################################

find_package(nlohmann_json REQUIRED)

set(PRIVATE_INCLUDE_PATHS
    .
)

set(SOURCES
    main.cpp
)
source_group(Sources FILES ${SOURCES})

add_executable(main ${SOURCES})
target_link_libraries(main PRIVATE Beast)
set_target_properties(main PROPERTIES OUTPUT_NAME beast)
target_include_directories(main PRIVATE ${PRIVATE_INCLUDE_PATHS})
target_compile_definitions(main
    PRIVATE
        -DNOMINMAX
        -DWIN32_LEAN_AND_MEAN
        -DVC_EXTRALEAN
        -D_CRT_SECURE_NO_WARNINGS
        -D_SECURE_SCL=0
        -D_SCL_SECURE_NO_WARNINGS
)
target_compile_features(main PUBLIC cxx_std_17)
set_target_properties(main PROPERTIES CXX_EXTENSIONS OFF)

#configure_file("${PROJECT_SOURCE_DIR}/Version.h.in" "${PROJECT_BINARY_DIR}/Version.h")

add_subdirectory(Beast)             # Beast library
