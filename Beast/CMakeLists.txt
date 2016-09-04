cmake_minimum_required (VERSION 3.4)

set(PUBLIC_INCLUDE_PATHS
    .
)

set(PRIVATE_INCLUDE_PATHS
    .
    ${PROJECT_BINARY_DIR}
    ${JSON_INCLUDE_DIR}
)

set(SOURCES
    Beast.cpp
    Beast.h
)

add_library(Beast ${SOURCES})
#target_compile_definitions(Beast PRIVATE ${DEFINES})
target_include_directories(Beast PRIVATE ${PRIVATE_INCLUDE_PATHS} PUBLIC ${PUBLIC_INCLUDE_PATHS})
source_group(Sources FILES ${SOURCES})
