#ifndef ODBC_STRING_CONVERTER_H_INCLUDED
#define ODBC_STRING_CONVERTER_H_INCLUDED
//------------------------------------------------------------------------------
#include <cstddef>
#include <utility>
#include <odbc/Config.h>
//------------------------------------------------------------------------------
NS_ODBC_START
//------------------------------------------------------------------------------
enum class Encoding
{
    UTF8,
    UTF16
};
//------------------------------------------------------------------------------
template<bool BigEndian>
class StringConverter
{
public:
    StringConverter() = delete;

    static void convert(
        const char* src,
        std::size_t srcLength,
        Encoding srcEnc,
        char* dst,
        std::size_t dstLength,
        Encoding dstEnc);

    static std::size_t getLength(
        const char* src,
        std::size_t srcLength,
        Encoding srcEnc,
        Encoding dstEnc);

private:
    static std::pair<int, char32_t> utf8ToCodePoint(
        const char* curr,
        const char* end);
};
//------------------------------------------------------------------------------
NS_ODBC_END
//------------------------------------------------------------------------------
#endif
