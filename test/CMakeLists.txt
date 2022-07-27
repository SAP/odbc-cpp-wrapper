INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src)

SET(OdbcCppTest_sources
    EnvironmentTest.cpp
    StringConverterTest.cpp
    TestMain.cpp
    TypesTest.cpp
    internal/ParameterDataTest.cpp
    internal/UtilInternalTest.cpp
)

ADD_EXECUTABLE(
    OdbcCppTest
    ${OdbcCppTest_sources}
)

TARGET_LINK_LIBRARIES(
    OdbcCppTest
    odbccpp_static
    GTest::GTest
    ${ODBC_LIBRARIES}
)

TARGET_COMPILE_DEFINITIONS(OdbcCppTest
    PUBLIC
        ODBC_STATIC
)

ADD_TEST(AllTests OdbcCppTest)
