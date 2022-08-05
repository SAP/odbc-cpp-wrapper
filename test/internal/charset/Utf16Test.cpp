#include <odbc/internal/charset/Utf16.h>
#include <GoogleTest.h>
//------------------------------------------------------------------------------
using namespace std;
using namespace NS_ODBC::utf16;
//------------------------------------------------------------------------------
NS_ODBC_START
namespace tests {
//------------------------------------------------------------------------------
TEST(Utf16Test, testNeedsSurrogatePair)
{
    EXPECT_FALSE(needsSurrogatePair(U'\u20AC'));
    EXPECT_TRUE(needsSurrogatePair(U'\U00010437'));
}
//------------------------------------------------------------------------------
TEST(Utf16Test, testEncodeSurrogatePair)
{
    pair<char16_t, char16_t> p = encodeSurrogatePair(U'\U00010437');
    EXPECT_EQ((uint16_t)0xD801, (uint16_t)p.first);
    EXPECT_EQ((uint16_t)0xDC37, (uint16_t)p.second);

    p = encodeSurrogatePair(U'\U00024B62');
    EXPECT_EQ((uint16_t)0xD852, (uint16_t)p.first);
    EXPECT_EQ((uint16_t)0xDF62, (uint16_t)p.second);
}
//------------------------------------------------------------------------------
} // namespace tests
NS_ODBC_END
