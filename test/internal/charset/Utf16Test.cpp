#include <odbc/internal/charset/Utf16.h>
#include <GoogleTest.h>
//------------------------------------------------------------------------------
using namespace std;
using namespace NS_ODBC::utf16;
//------------------------------------------------------------------------------
NS_ODBC_START
namespace tests {
//------------------------------------------------------------------------------
TEST(Utf16Test, testIsHighSurrogate)
{
    EXPECT_FALSE(isHighSurrogate(u'\u20ac'));
    EXPECT_TRUE(isHighSurrogate((char16_t)0xd801));
    EXPECT_FALSE(isHighSurrogate((char16_t)0xdc37));
}
//------------------------------------------------------------------------------
TEST(Utf16Test, testIsLowSurrogate)
{
    EXPECT_FALSE(isLowSurrogate(u'\u20ac'));
    EXPECT_FALSE(isLowSurrogate((char16_t)0xd801));
    EXPECT_TRUE(isLowSurrogate((char16_t)0xdc37));
}
//------------------------------------------------------------------------------
TEST(Utf16Test, testNeedsSurrogatePair)
{
    EXPECT_FALSE(needsSurrogatePair(U'\u20AC'));
    EXPECT_TRUE(needsSurrogatePair(U'\U00010437'));
}
//------------------------------------------------------------------------------
TEST(Utf16Test, testEncodeSingleLE)
{
    char buffer[2];
    encodeSingleLE(u'\u20AC', buffer);
    EXPECT_EQ('\xAC', buffer[0]);
    EXPECT_EQ('\x20', buffer[1]);
}
//------------------------------------------------------------------------------
TEST(Utf16Test, testEncodeSingleBE)
{
    char buffer[2];
    encodeSingleBE(u'\u20AC', buffer);
    EXPECT_EQ('\x20', buffer[0]);
    EXPECT_EQ('\xAC', buffer[1]);
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
TEST(Utf16Test, testEncodeLE)
{
    char buffer[4];
    ASSERT_EQ(2, encodeLE(U'\u20AC', buffer));
    EXPECT_EQ('\xAC', buffer[0]);
    EXPECT_EQ('\x20', buffer[1]);

    ASSERT_EQ(4, encodeLE(U'\U00010437', buffer));
    EXPECT_EQ('\x01', buffer[0]);
    EXPECT_EQ('\xD8', buffer[1]);
    EXPECT_EQ('\x37', buffer[2]);
    EXPECT_EQ('\xDC', buffer[3]);
}
//------------------------------------------------------------------------------
TEST(Utf16Test, testEncodeBE)
{
    char buffer[4];
    ASSERT_EQ(2, encodeBE(U'\u20AC', buffer));
    EXPECT_EQ('\x20', buffer[0]);
    EXPECT_EQ('\xAC', buffer[1]);

    ASSERT_EQ(4, encodeBE(U'\U00010437', buffer));
    EXPECT_EQ('\xD8', buffer[0]);
    EXPECT_EQ('\x01', buffer[1]);
    EXPECT_EQ('\xDC', buffer[2]);
    EXPECT_EQ('\x37', buffer[3]);
}
//------------------------------------------------------------------------------
} // namespace tests
NS_ODBC_END
