#include <odbc/internal/Odbc.h>
#include <odbc/internal/UtilInternal.h>
#include <GoogleTest.h>
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
namespace odbc {
namespace tests {
//------------------------------------------------------------------------------
struct NumericTestParams
{
    SQL_NUMERIC_STRUCT num;
    string str;
};
//------------------------------------------------------------------------------
static SQL_NUMERIC_STRUCT createNumeric(SQLCHAR sign,
    initializer_list<SQLCHAR> values, SQLCHAR precision, SQLSCHAR scale)
{
    SQL_NUMERIC_STRUCT num;
    num.precision = precision;
    num.scale = scale;
    num.sign = sign;
    memset(num.val, 0, SQL_MAX_NUMERIC_LEN);

    int k = 0;
    for (auto v : values) {
        num.val[k] = v;
        ++k;
        if (k >= SQL_MAX_NUMERIC_LEN)
            break;
    }
    return num;
}
//------------------------------------------------------------------------------
class NumericTestParameterized
    : public ::testing::TestWithParam<NumericTestParams>
{
};
//------------------------------------------------------------------------------
TEST_P(NumericTestParameterized, decimalToNumeric)
{
    const NumericTestParams& p = GetParam();
    const SQL_NUMERIC_STRUCT& numExpected = p.num;

    SQL_NUMERIC_STRUCT numActual;
    UtilInternal::decimalToNumeric(decimal(p.str, numExpected.precision,
        numExpected.scale), numActual);

    EXPECT_EQ(numExpected.precision, numActual.precision);
    EXPECT_EQ(numExpected.scale, numActual.scale);
    EXPECT_EQ(numExpected.sign, numActual.sign);
    EXPECT_THAT(numActual.val, ::testing::ElementsAreArray(numExpected.val));
}
//------------------------------------------------------------------------------
TEST_P(NumericTestParameterized, numericToString)
{
    const NumericTestParams& p = GetParam();

    char str[64];
    UtilInternal::numericToString(p.num, str);
    EXPECT_STREQ(p.str.c_str(), str);
}
//------------------------------------------------------------------------------
NumericTestParams numericTestParams[] = {
    {createNumeric(1,{ 124, 98 }, 12, 0), "25212"},
    {createNumeric(1,{ 207, 62, 194, 106, 220, 101, 74, 222,
        223, 159, 26, 149,  15 }, 34, 4), "1234567891234567891234567896783" },
    { createNumeric(1,{ 88, 196, 234, 69, 1 }, 34, 0), "5467980888" },
    { createNumeric(0,{ 226, 255, 255, 255, 255, 255, 255, 255, 9 }, 34, 0),
        "-184467440737095516130" },
    { createNumeric(1,{ 0 }, 34, 0), "0" },
    { createNumeric(0,{ 1 }, 5, 0), "-1" }
};
//------------------------------------------------------------------------------
INSTANTIATE_TEST_SUITE_P(
    NumericTestParameterizedInstance,
    NumericTestParameterized,
    ::testing::ValuesIn(numericTestParams));
//------------------------------------------------------------------------------
} // namespace tests
} // namespace odbc
