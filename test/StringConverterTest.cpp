#include <odbc/Exception.h>
#include <odbc/StringConverter.h>
#include <GoogleTest.h>
#include <string>
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
NS_ODBC_START
namespace tests {
//------------------------------------------------------------------------------
struct StringConverterTestParams
{
    const char* src;
    int srcLength;
    u16string expected;
};
//------------------------------------------------------------------------------
class StringConverterTest : public ::testing::Test,
    public ::testing::WithParamInterface<StringConverterTestParams>
{
};
//------------------------------------------------------------------------------
TEST_P(StringConverterTest, utf8ToUtf16)
{
    const auto& p = GetParam();
    u16string actual = p.srcLength >= 0 ?
                StringConverter::utf8ToUtf16(p.src, p.srcLength) :
                StringConverter::utf8ToUtf16(p.src);
    ASSERT_EQ(p.expected, actual);
}
//------------------------------------------------------------------------------
// clang-format off
static const StringConverterTestParams stringConverterTestParams[] = {
// Zero length
{
    "\x48",
    0,
    u""
},
// Zero length with null-terminator
{
    "\x00",
    -1,
    u""
},
// Digit 0
{
    "\x30",
    1,
    u"\x0030"
},
// Digit 0 with null-terminator
{
    "\x30\x00\x00",
    -1,
    u"\x0030"
},
// Surrogate pair, LINEAR B SYLLABLE B008 A' (U+10000)
{
    "\xF0\x90\x80\x80",
    4,
    u"\xD800\xDC00"
},
// Surrogate pair, LINEAR B SYLLABLE B008 A' (U+10000) with null-terminator
{
    "\xF0\x90\x80\x80",
    -1,
    u"\xD800\xDC00"
},
// Some string
{
    "\x4F\x73\x74\x73\x74\x72\x61\xC3\x9F\x65",
    10,
    u"Oststraße"
},
// Some null-terminated string
{
    "\x4F\x73\x74\x73\x74\x72\x61\xC3\x9F\x65\x00\x00\x00\x00",
    -1,
    u"Oststraße"
},
};
// clang-format on
//------------------------------------------------------------------------------
INSTANTIATE_TEST_CASE_P(
    StringConverterTestInstance,
    StringConverterTest,
    ::testing::ValuesIn(stringConverterTestParams));
//------------------------------------------------------------------------------
struct StringConverterThrowTestParams
{
    const char* src;
    int srcLength;
    const char* errorMessage;
};
//------------------------------------------------------------------------------
class StringConverterThrowTest : public ::testing::Test,
    public ::testing::WithParamInterface<StringConverterThrowTestParams>
{
};
//------------------------------------------------------------------------------
TEST_P(StringConverterThrowTest, utf8ToUtf16)
{
    const auto& p = GetParam();
    try
    {
        u16string dst = p.srcLength >= 0 ?
                    StringConverter::utf8ToUtf16(p.src, p.srcLength) :
                    StringConverter::utf8ToUtf16(p.src);
        FAIL();
    }
    catch (const Exception& e)
    {
        EXPECT_STREQ(p.errorMessage, e.what());
    }
}
//------------------------------------------------------------------------------
// clang-format off
static const StringConverterThrowTestParams stringConverterThrowTestParams[] = {
{
    nullptr,
    10,
    "Input string must not be nullptr."
},
{
    nullptr,
    -1,
    "Input string must not be nullptr."
},
{
    "\x80\x4F\x73",
    3,
    "The string contains an invalid UTF-8 byte sequence at position 0."
},
{
    "\x80\x4F\x73",
    -1,
    "The string contains an invalid UTF-8 byte sequence at position 0."
},
{
    "\xC2\xA2\xE2\x82\xAC\xF0\x90\x8D",
    8,
    "The string contains an incomplete byte sequence at position 5."
},
{
    "\x73\x74\xE5\x5D\x0D\x0A",
    6,
    "The string contains an incomplete byte sequence at position 2."
},
};
// clang-format on
//------------------------------------------------------------------------------
INSTANTIATE_TEST_CASE_P(
    StringConverterThrowTestInstance,
    StringConverterThrowTest,
    ::testing::ValuesIn(stringConverterThrowTestParams));
//------------------------------------------------------------------------------
} // namespace tests
NS_ODBC_END
