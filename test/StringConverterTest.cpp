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
    size_t srcLength;
    const char* dst;
    size_t dstLength;
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
    u16string dst = StringConverter::utf8ToUtf16(p.src, p.srcLength);
    ASSERT_EQ(p.dstLength, dst.length()*sizeof(char16_t));
    const char* pDst = reinterpret_cast<const char*>(dst.c_str());
    for (size_t i = 0; i < p.dstLength; ++i)
        EXPECT_EQ(p.dst[i], pDst[i]);
}
//------------------------------------------------------------------------------
// clang-format off
static const StringConverterTestParams stringConverterTestParams[] = {
// Zero length
{
    "\x48",
    0,
    "",
    0
},
// Digit 0
{
    "\x30",
    1,
    "\x30\x00",
    2
},
// Surrogate pair, LINEAR B SYLLABLE B008 A' (U+10000)
{
    "\xF0\x90\x80\x80",
    4,
    "\x00\xD8\x00\xDC",
    4
},
// Oststraße
{
    "\x4F\x73\x74\x73\x74\x72\x61\xC3\x9F\x65",
    10,
    "\x4F\x00\x73\x00\x74\x00\x73\x00\x74\x00\x72\x00\x61\x00\xDF\x00\x65\x00",
    18
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
    size_t srcLength;
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
        u16string dst = StringConverter::utf8ToUtf16(p.src, p.srcLength);
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
    "Input string cannot be nullptr."
},
{
    "\x80\x4F\x73",
    3,
    "The byte-sequence '\x80' has invalid length."
},
{
    "\xC2\xA2\xE2\x82\xAC\xF0\x90\x8D",
    8,
    "The byte-sequence '\xF0\x90\x8D' is incomplete."
},
{
    "\x73\x74\xE5\x5D\x0D\x0A",
    6,
    "The byte-sequence '\xE5]\r' is incomplete."
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
