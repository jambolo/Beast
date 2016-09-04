set(VSCPPUNIT_DIR "D:/Program Files (x86)/VC/UnitTest")
set(VSCPPUNIT_INCLUDE_DIR ${VSCPPUNIT_DIR}/include)
set(VSCPPUNIT_LIB_DIR ${VSCPPUNIT_DIR}/lib)

file(GLOB TESTS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)
source_group(tests FILES ${TESTS})

set(SOURCES
    targetver.h
    ${TESTS}
)

set(INCLUDE_PATHS
    ${JSON_INCLUDE_DIR}
    ${VSCPPUNIT_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}
)

link_directories(${VSCPPUNIT_LIB_DIR})

add_library(unit-test SHARED ${SOURCES})

target_include_directories(unit-test PRIVATE ${INCLUDE_PATHS})
target_link_libraries(unit-test Beast )
