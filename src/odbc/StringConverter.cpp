#include <odbc/Exception.h>
#include <odbc/StringConverter.h>
#include <odbc/internal/Macros.h>
#include <odbc/internal/charset/Utf16.h>
#include <odbc/internal/charset/Utf8.h>
#if defined(__GLIBC__)
#include <endian.h>
#elif defined(__APPLE__)
#include <machine/endian.h>
#endif
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
NS_ODBC_START
//------------------------------------------------------------------------------
namespace  {
// TODO(C++20): Replace by std::endian.
enum class endian
{
#ifdef BYTE_ORDER
    little = LITTLE_ENDIAN,
    big = BIG_ENDIAN,
    native = BYTE_ORDER
#elif defined(_M_IX86) || defined(_M_AMD64)
    little = 0,
    big = 1,
    native = little
#else
#error "Cannot determine endianness"
#endif
};
//------------------------------------------------------------------------------
} // anonymous namespace
//------------------------------------------------------------------------------
// StringConverter class
//------------------------------------------------------------------------------
u16string StringConverter::utf8ToUtf16(const char* src, size_t srcLength)
{
    if (src == nullptr)
        ODBC_FAIL("Input string cannot be nullptr.");

    if (srcLength == 0)
        return u16string();

    const char* srcEnd = src + srcLength;
    size_t dstLength = utf8ToUtf16Length(src, srcLength);
    u16string str(dstLength/sizeof(char16_t), 0);
    char* dst = reinterpret_cast<char*>(const_cast<char16_t*>(str.data()));

    while (src < srcEnd)
    {
        pair<int, char32_t> cp = utf8ToCodePoint(src, srcEnd);
        src += cp.first;
        if (endian::native == endian::big)
            dst += utf16::encodeBE(cp.second, dst);
        else
            dst += utf16::encodeLE(cp.second, dst);
    }

    return str;
}
//------------------------------------------------------------------------------
pair<int, char32_t> StringConverter::utf8ToCodePoint(
    const char* curr, const char* end)
{
    ODBC_ASSERT_0(curr != nullptr && end != nullptr);

    int len = utf8::getSequenceLength(*curr);
    if (len == 1)
    {
        // Short-cut for the easy and common case.
        return pair<int, char32_t>(len, *curr);
    }
    if (len == -1)
    {
        ODBC_FAIL("The byte-sequence '" <<
                  string(curr, 1) << "' has invalid length.");
    }

    // We have to check that we don't exceed the end of the string.
    if ((curr + len) > end)
    {
        ODBC_FAIL("The byte-sequence '" <<
                  string(curr, end - curr) << "' is incomplete.");
    }

    if (!utf8::isValidSequence(len, curr))
    {
        ODBC_FAIL("The byte-sequence '" <<
                  string(curr, len) << "' is incomplete.");
    }
    return pair<int, char32_t>(len, utf8::decode(len, curr));
}
//------------------------------------------------------------------------------
size_t StringConverter::utf8ToUtf16Length(const char* src, size_t srcLength)
{
    size_t len = 0;
    const char* srcEnd = src + srcLength;

    while (src < srcEnd)
    {
        pair<int, char32_t> cp = utf8ToCodePoint(src, srcEnd);
        src += cp.first;
        len += utf16::needsSurrogatePair(cp.second) ? 4 : 2;
    }

    return len;
}
//------------------------------------------------------------------------------
NS_ODBC_END
