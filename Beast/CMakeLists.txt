cmake_minimum_required (VERSION 3.10)

set(PUBLIC_INCLUDE_PATHS
    include
)

set(PRIVATE_INCLUDE_PATHS
    .
    include/Beast
)

set(SOURCES
    include/Beast/Beast.h

    Beast.cpp
    QuineMcCluskey.cpp
#    QuineMcCluskey.h
)
source_group(Sources FILES ${SOURCES})

add_library(Beast ${SOURCES})
target_link_libraries(Beast PUBLIC nlohmann_json::nlohmann_json)
target_include_directories(Beast PRIVATE ${PRIVATE_INCLUDE_PATHS} PUBLIC ${PUBLIC_INCLUDE_PATHS})
target_compile_definitions(Beast
    PRIVATE
        -DNOMINMAX
        -DWIN32_LEAN_AND_MEAN
        -DVC_EXTRALEAN
        -D_CRT_SECURE_NO_WARNINGS
        -D_SECURE_SCL=0
        -D_SCL_SECURE_NO_WARNINGS
)
target_compile_features(Beast PUBLIC cxx_std_17)
set_target_properties(Beast PROPERTIES CXX_EXTENSIONS OFF)
