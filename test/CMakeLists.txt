INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src)

SET(OdbcCppTest_sources
    EnvironmentTest.cpp
    StringConverterTest.cpp
    TestMain.cpp
    TypesTest.cpp
    internal/ParameterDataTest.cpp
    internal/UtilInternalTest.cpp
    internal/charset/Utf16Test.cpp
    internal/charset/Utf8Test.cpp
)

ADD_EXECUTABLE(
    OdbcCppTest
    ${OdbcCppTest_sources}
)

TARGET_LINK_LIBRARIES(
    OdbcCppTest
    odbccpp
    GTest::GTest
)

ADD_TEST(AllTests OdbcCppTest)
