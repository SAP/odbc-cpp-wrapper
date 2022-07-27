#ifndef ODBC_INTERNAL_CHARSET_UTF16_H_INCLUDED
#define ODBC_INTERNAL_CHARSET_UTF16_H_INCLUDED
//------------------------------------------------------------------------------
#include <utility>
#include <odbc/internal/Macros.h>
//------------------------------------------------------------------------------
NS_ODBC_START
namespace utf16 {
//------------------------------------------------------------------------------
/**
 * Checks if a 16-bit code unit is a high surrogate (starting a surrogate pair).
 *
 * @param c  The 16-bit code unit to check.
 * @return   True if the code unit is a high surrogate, false otherwise.
 */
inline bool isHighSurrogate(char16_t c)
{
    return (c >= 0xD800 && c <= 0xDBFF);
}
//------------------------------------------------------------------------------
/**
 * Checks if a 16-bit code unit is a low surrogate (ending a surrogate pair).
 *
 * @param c  The 16-bit code unit to check.
 * @return   True if the code unit is a high surrogate, false otherwise.
 */
inline bool isLowSurrogate(char16_t c)
{
    return (c >= 0xDC00 && c <= 0xDFFF);
}
//------------------------------------------------------------------------------
/**
 * Checks if the 16-bit code unit is either a high or low surrogate.
 *
 * @param c  The 16-bit code unit to check
 * @return   True if the code unit is a low or high surrogate, false otherwise.
 */
inline bool isSurrogatePart(char16_t c)
{
    return (c >= 0xD800 && c <= 0xDFFF);
}
//------------------------------------------------------------------------------
/**
 * Checks if a code point is representable in UTF-16, i.e. it is a code-point
 * less or equal to U+10FFFF and not a surrogate part.
 *
 * @param c  The code point to check.
 * @return   True if the code point is representable in UTF-16, false otherwise.
 */
inline bool isRepresentable(char32_t c)
{
    return c <= 0x10FFFF && !(c >= 0xD800 && c <= 0xDFFF);
}
//------------------------------------------------------------------------------
/**
 * Checks if a code point must be represented by a surrogate pair.
 *
 * @param c  The code point to check.
 * @return   True if a surrogate pair is needed, false otherwise.
 */
inline bool needsSurrogatePair(char32_t c)
{
    return c >= 0x10000;
}
//------------------------------------------------------------------------------
/**
 * Encodes a 16-bit code unit as little endian.
 *
 * @param c       The 16-bit code unit to encode.
 * @param target  The target buffer. buffer[0] and buffer[1] must writable.
 */
inline void encodeSingleLE(char16_t c, char* target)
{
    target[0] = (char)(c & 0xFF);
    target[1] = (char)(c >> 8);
}
//------------------------------------------------------------------------------
/**
 * Encodes a 16-bit code unit as big endian.
 *
 * @param c       The 16-bit code unit to encode.
 * @param target  The target buffer. buffer[0] and buffer[1] must writable.
 */
inline void encodeSingleBE(char16_t c, char* target)
{
    target[0] = (char)(c >> 8);
    target[1] = (char)(c & 0xFF);
}
//------------------------------------------------------------------------------
/**
 * Encodes a code point from the supplementary planes as a surrogate pair.
 *
 * @param c  Character from the supplementary planes, i.e. from U+10000 to
 *           U+10FFFF.
 * @return   A pair containing the high surrogate as first and the low surrogate
 *           as second.
 */
inline std::pair<char16_t, char16_t> encodeSurrogatePair(char32_t c)
{
    ODBC_ASSERT(
        (c >= 0x10000) && (c <= 0x10FFFF),
        "Codepoint " << (uint32_t)c << " is not from the supplementary planes");
    c -= 0x10000;
    return std::pair<char16_t, char16_t>{
        (char16_t)(0xD800 | (c >> 10)), (char16_t)(0xDC00 | (c & 0x3FF))};
}
//------------------------------------------------------------------------------
/**
 * Encodes a code point as UTF-16 little endian.
 *
 * This method automatically encodes as single 16-bit code point or a surrogate
 * pair depending on the code point. Therefore the first 4 bytes of the target
 * buffer must be accessible.
 *
 * @param c       The code point to encode.
 * @param target  The target buffer. The first four bytes must be accessible.
 * @return        The number of bytes written to the target buffer.
 */
inline int encodeLE(char32_t c, char* target)
{
    ODBC_ASSERT(
        isRepresentable(c), "Codepoint " << (uint32_t)c << " is invalid");
    if (!needsSurrogatePair(c))
    {
        encodeSingleLE((char16_t)c, target);
        return 2;
    }
    else
    {
        std::pair<char16_t, char16_t> sp = encodeSurrogatePair(c);
        encodeSingleLE(sp.first, target);
        encodeSingleLE(sp.second, target + 2);
        return 4;
    }
}
//------------------------------------------------------------------------------
/**
 * Encodes a code point as UTF-16 big endian.
 *
 * This method automatically encodes as single 16-bit code point or a surrogate
 * pair depending on the code point. Therefore the first 4 bytes of the target
 * buffer must be accessible.
 *
 * @param c       The code point to encode.
 * @param target  The target buffer. The first four bytes must be accessible.
 * @return        The number of bytes written to the target buffer.
 */
inline int encodeBE(char32_t c, char* target)
{
    ODBC_ASSERT(
        isRepresentable(c), "Codepoint " << (uint32_t)c << " is invalid");
    if (!needsSurrogatePair(c))
    {
        encodeSingleBE((char16_t)c, target);
        return 2;
    }
    else
    {
        std::pair<char16_t, char16_t> sp = encodeSurrogatePair(c);
        encodeSingleBE(sp.first, target);
        encodeSingleBE(sp.second, target + 2);
        return 4;
    }
}
//------------------------------------------------------------------------------
} // namespace utf16
NS_ODBC_END
//------------------------------------------------------------------------------
#endif
