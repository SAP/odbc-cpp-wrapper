#include <odbc/Exception.h>
#include <odbc/StringConverter.h>
#include <odbc/internal/charset/Utf16.h>
#include <odbc/internal/charset/Utf8.h>
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
NS_ODBC_START
//------------------------------------------------------------------------------
// StringConverter class
//------------------------------------------------------------------------------
template<bool BigEndian>
void StringConverter<BigEndian>::convert(
    const char* src,
    std::size_t srcLength,
    Encoding srcEnc,
    char* dst,
    std::size_t dstLength,
    Encoding dstEnc)
{
    if (srcEnc == dstEnc)
        throw Exception("Source and destination encodings are the same");

    const char* srcCurr = src;
    const char* srcEnd = src + srcLength;
    const char* dstCurr = dst;
    const char* dstEnd = dst + dstLength;

    switch (srcEnc)
    {
    case Encoding::UTF8:
    {
        while (srcCurr < srcEnd && dstCurr < dstEnd)
        {
            pair<int, char32_t> cp = utf8ToCodePoint(srcCurr, srcEnd);
            srcCurr += cp.first;

            switch(dstEnc)
            {
            case Encoding::UTF16:
            {
                if (BigEndian)
                    utf16::encodeBE(cp.second, dstCurr);
                else
                    utf16::encodeLE(cp.second, dstCurr);
                dstCurr += utf16::needsSurrogatePair(cp.second) ? 4 : 2;
            }
            default:
                ODBC_ASSERT_UNREACHABLE;
            }
        }
        break;
    }
    case Encoding::UTF16:
    {
        throw Exception("Encoding not impelemented");
    }
    }
}
//------------------------------------------------------------------------------
template<bool BigEndian>
std::size_t StringConverter<BigEndian>::getLength(
    const char* src,
    std::size_t srcLength,
    Encoding srcEnc,
    Encoding dstEnc)
{
    size_t ret = 0;

    const char* srcCurr = src;
    const char* srcEnd = src + srcLength;

    switch (srcEnc)
    {
    case Encoding::UTF8:
    {
        while (srcCurr < srcEnd)
        {
            pair<int, char32_t> cp = utf8ToCodePoint(srcCurr, srcEnd);
            srcCurr += cp.first;

            switch (dstEnc)
            {
            case Encoding::UTF16:
                ret += utf16::needsSurrogatePair(cp.second) ? 4 : 2;
                break;
            default:
                ODBC_ASSERT_UNREACHABLE;
            }
        }
        break;
    }
    case Encoding::UTF16:
    {
        throw Exception("Encoding not impelemented");
    }
    }

    return ret;
}
//------------------------------------------------------------------------------
template<bool BigEndian>
pair<int, char32_t> StringConverter<BigEndian>::utf8ToCodePoint(
    const char* curr, const char* end)
{
    int len = utf8::getSequenceLength(*curr);
    if (len == 1)
    {
        // Short-cut for the easy and common case.
        return pair<int, char32_t>(len, *curr);
    }
    if (len == -1)
    {
        throw Exception("A byte-sequence has invalid sequence length.");
    }
    if (end)
    {
        // If the end of the string is known, we have to check that we don't
        // exceed the end of the string.
        if ((curr + len) > end)
        {
            throw Exception("There is an incomplete byte-sequence at the end of"
                " the string.");
        }
    }
    else
    {
        // If the string is zero-terminated, we have to make sure that the
        // sequence does not contain a terminating zero.
        for (int i = 1; i < len; ++i)
        {
            if (curr[i] == '\0')
            {
                throw Exception("There is an incomplete byte-sequence at the "
                    "end of the string.");
            }
        }
    }
    if (!utf8::isValidSequence(len, curr))
    {
        throw Exception("There is an incomplete byte-sequence at the end of the"
            " string.");
    }
    return pair<int, char32_t>(len, utf8::decode(len, curr));
}
//------------------------------------------------------------------------------
NS_ODBC_END
