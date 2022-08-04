#include <odbc/Exception.h>
#include <odbc/StringConverter.h>
#include <odbc/internal/Macros.h>
#include <odbc/internal/charset/Utf16.h>
#include <odbc/internal/charset/Utf8.h>
#include <cstring>
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
NS_ODBC_START
//------------------------------------------------------------------------------
// StringConverter class
//------------------------------------------------------------------------------
u16string StringConverter::utf8ToUtf16(const char* src)
{
    return utf8ToUtf16(src, nullptr);
}
//------------------------------------------------------------------------------
u16string StringConverter::utf8ToUtf16(const char* src, size_t srcLength)
{
    return utf8ToUtf16(src, src + srcLength);
}
//------------------------------------------------------------------------------
u16string StringConverter::utf8ToUtf16(const char* begin, const char* end)
{
    ODBC_CHECK(begin != nullptr, "Input string must not be nullptr.");

    if (end == nullptr)
        end = begin + strlen(begin);

    size_t dstLength = utf8ToUtf16Length(begin, end);
    u16string str(dstLength, 0);

    const char* curr = begin;
    size_t i = 0;

    while (curr < end)
    {
        pair<int, char32_t> cp = utf8ToCodePoint(begin, curr, end);
        curr += cp.first;

        ODBC_CHECK(utf16::isRepresentable(cp.second),
                   "Codepoint " << (uint32_t)cp.second << " is invalid");

        if (utf16::needsSurrogatePair(cp.second))
        {
            pair<char16_t, char16_t> sp = utf16::encodeSurrogatePair(cp.second);
            str[i] = static_cast<char16_t>(sp.first);
            str[i + 1] = static_cast<char16_t>(sp.second);
            i += 2;
        }
        else
        {
            str[i] = static_cast<char16_t>(cp.second);
            ++i;
        }
    }

    return str;
}
//------------------------------------------------------------------------------
pair<int, char32_t> StringConverter::utf8ToCodePoint(
    const char* begin, const char* curr, const char* end)
{
    ODBC_ASSERT_0(begin != nullptr && end != nullptr);
    ODBC_ASSERT_0(begin <= curr && curr < end);

    int len = utf8::getSequenceLength(*curr);
    if (len == 1)
    {
        // Short-cut for the easy and common case.
        return pair<int, char32_t>(len, *curr);
    }
    if (len == -1)
    {
        ODBC_FAIL("The string contains an invalid UTF-8 byte sequence at "
                  "position " << (curr - begin) << ".");
    }

    // We have to make sure that the sequence does not contain a terminating
    // zero and the following byte-sequence is valid.
    if ((curr + len) > end || !utf8::isValidSequence(len, curr))
    {
        ODBC_FAIL("The string contains an incomplete byte-sequence at "
                  "position " << (curr - begin) << ".");
    }
    return pair<int, char32_t>(len, utf8::decode(len, curr));
}
//------------------------------------------------------------------------------
size_t StringConverter::utf8ToUtf16Length(const char* begin, const char* end)
{
    ODBC_ASSERT_0(begin != nullptr && end != nullptr);

    size_t len = 0;

    const char* curr = begin;
    while (curr < end)
    {
        pair<int, char32_t> cp = utf8ToCodePoint(begin, curr, end);
        curr += cp.first;
        len += utf16::needsSurrogatePair(cp.second) ? 2 : 1;
    }

    return len;
}
//------------------------------------------------------------------------------
NS_ODBC_END
