#include <odbc/internal/charset/Utf8.h>
#include <GoogleTest.h>
//------------------------------------------------------------------------------
using namespace std;
using namespace NS_ODBC::utf8;
//------------------------------------------------------------------------------
NS_ODBC_START
namespace tests {
//------------------------------------------------------------------------------
TEST(Utf8Test, testGetSequenceLength)
{
    EXPECT_EQ(1, getSequenceLength('\x24'));
    EXPECT_EQ(2, getSequenceLength('\xC2'));
    EXPECT_EQ(3, getSequenceLength('\xE2'));
    EXPECT_EQ(4, getSequenceLength('\xF0'));
    EXPECT_EQ(-1, getSequenceLength('\x80'));
}
//------------------------------------------------------------------------------
TEST(Utf8Test, testIsValidSequence)
{
    char sequence[4];

    // Test sequences of length 1
    sequence[0] = '\x24';
    EXPECT_TRUE(isValidSequence(1, sequence));
    sequence[0] = '\x8F';
    EXPECT_FALSE(isValidSequence(1, sequence));

    // Test sequences of length 2
    sequence[0] = '\xC2';
    sequence[1] = '\xA2';
    EXPECT_TRUE(isValidSequence(2, sequence));
    sequence[0] = '\xE2'; // first byte wrong
    sequence[1] = '\xA2';
    EXPECT_FALSE(isValidSequence(2, sequence));
    sequence[0] = '\xC2';
    sequence[1] = '\xC2'; // second byte wrong
    EXPECT_FALSE(isValidSequence(2, sequence));

    // Test sequences of length 3
    sequence[0] = '\xE2';
    sequence[1] = '\x82';
    sequence[2] = '\xAC';
    EXPECT_TRUE(isValidSequence(3, sequence));
    sequence[0] = '\xF2'; // first byte wrong
    sequence[1] = '\x82';
    sequence[2] = '\xAC';
    EXPECT_FALSE(isValidSequence(3, sequence));
    sequence[0] = '\xE2';
    sequence[1] = '\xC2'; // seond byte wrong
    sequence[2] = '\xAC';
    EXPECT_FALSE(isValidSequence(3, sequence));
    sequence[0] = '\xE2';
    sequence[1] = '\x82';
    sequence[2] = '\x0C'; // third byte wrong
    EXPECT_FALSE(isValidSequence(3, sequence));

    // Test sequences of length 4
    sequence[0] = '\xF0';
    sequence[1] = '\x90';
    sequence[2] = '\x8D';
    sequence[3] = '\x88';
    EXPECT_TRUE(isValidSequence(4, sequence));
    sequence[0] = '\xF8'; // first byte wrong
    sequence[1] = '\x90';
    sequence[2] = '\x8D';
    sequence[3] = '\x88';
    EXPECT_FALSE(isValidSequence(4, sequence));
    sequence[0] = '\xF0';
    sequence[1] = '\x10'; // second byte wrong
    sequence[2] = '\x8D';
    sequence[3] = '\x88';
    EXPECT_FALSE(isValidSequence(4, sequence));
    sequence[0] = '\xF0';
    sequence[1] = '\x90';
    sequence[2] = '\xCD'; // third byte wrong
    sequence[3] = '\x88';
    EXPECT_FALSE(isValidSequence(4, sequence));
    sequence[0] = '\xF0';
    sequence[1] = '\x90';
    sequence[2] = '\x8D';
    sequence[3] = '\x08'; // fourth byte wrong
    EXPECT_FALSE(isValidSequence(4, sequence));
}
//------------------------------------------------------------------------------
TEST(Utf8Test, testDecodeSequence)
{
    char sequence[4];

    sequence[0] = '\x24';
    EXPECT_EQ(U'$', decode(1, sequence));

    sequence[0] = '\xC2';
    sequence[1] = '\xA2';
    EXPECT_EQ(U'\u00A2', decode(2, sequence));

    sequence[0] = '\xE2';
    sequence[1] = '\x82';
    sequence[2] = '\xAC';
    EXPECT_EQ(U'\u20AC', decode(3, sequence));

    sequence[0] = '\xF0';
    sequence[1] = '\x90';
    sequence[2] = '\x8D';
    sequence[3] = '\x88';
    EXPECT_EQ(U'\U00010348', decode(4, sequence));
}
//------------------------------------------------------------------------------
} // namespace tests
NS_ODBC_END
