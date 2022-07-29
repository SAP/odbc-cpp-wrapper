#ifndef ODBC_STRING_CONVERTER_H_INCLUDED
#define ODBC_STRING_CONVERTER_H_INCLUDED
//------------------------------------------------------------------------------
#include <odbc/Config.h>
#include <cstddef>
#include <utility>
#include <string>
//------------------------------------------------------------------------------
NS_ODBC_START
//------------------------------------------------------------------------------
class ODBC_EXPORT StringConverter
{
public:
    StringConverter() = delete;

    static std::u16string utf8ToUtf16(const char* src, std::size_t srcLength);

private:
    static std::pair<int, char32_t> utf8ToCodePoint(
        const char* curr,
        const char* end);

    static std::size_t utf8ToUtf16Length(const char* src,
                                         std::size_t srcLength);
};
//------------------------------------------------------------------------------
NS_ODBC_END
//------------------------------------------------------------------------------
#endif
