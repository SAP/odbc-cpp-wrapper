#include <odbc/Exception.h>
#include <odbc/Types.h>
#include <odbc/Util.h>
#include <GoogleTest.h>
#include <sstream>
//------------------------------------------------------------------------------
using namespace std;
using testing::Types;
//------------------------------------------------------------------------------
NS_ODBC_START
namespace tests {
//------------------------------------------------------------------------------
TEST(TypesTest, decimal)
{
    EXPECT_THROW(decimal("167", 0, 1), Exception);
    EXPECT_THROW(decimal("167", 0, 0), Exception);
    EXPECT_THROW(decimal("167", 39, 2), Exception);
    EXPECT_THROW(decimal("167", 10, 15), Exception);
    EXPECT_THROW(decimal("1234p", 12, 10), Exception);
    EXPECT_THROW(decimal("   ", 10, 3), Exception);
    EXPECT_THROW(decimal("-", 10, 3), Exception);
    EXPECT_THROW(decimal("+-2", 10, 3), Exception);
    EXPECT_THROW(decimal("-+2", 10, 3), Exception);
    EXPECT_THROW(decimal("+", 10, 3), Exception);
    EXPECT_THROW(decimal("++2", 10, 3), Exception);
    EXPECT_THROW(decimal("+5651", 3, 2), Exception);
    EXPECT_THROW(decimal("651", 2, 1), Exception);

    EXPECT_NO_THROW(decimal());
    EXPECT_NO_THROW(decimal("81236", 10, 3));
    EXPECT_NO_THROW(decimal("0123", 10, 3));
    EXPECT_NO_THROW(decimal("+181236", 10, 3));
    EXPECT_NO_THROW(decimal("-181236", 10, 3));
    EXPECT_NO_THROW(decimal("-0", 10, 3));

    EXPECT_EQ(3, decimal("0", 12, 3).scale());

    EXPECT_EQ(19, decimal("0", 19, 6).precision());

    EXPECT_EQ(0, decimal("-0", 12, 3).signum());
    EXPECT_EQ(1, decimal("345", 12, 3).signum());
    EXPECT_EQ(1, decimal("+167", 12, 3).signum());
    EXPECT_EQ(-1, decimal("-267", 12, 3).signum());

    EXPECT_STREQ("-312", decimal((std::int64_t)-312, 12).toString().c_str());
    EXPECT_STREQ("312", decimal((std::uint64_t)312, 12).toString().c_str());
    EXPECT_STREQ("6.72", decimal(std::string("672"), 12, 2).toString().c_str());
    EXPECT_STREQ("0.167", decimal("167", 12, 3).toString().c_str());
    EXPECT_STREQ("167", decimal("167", 12, 0).toString().c_str());
    EXPECT_STREQ("0.0167", decimal("167", 4, 4).toString().c_str());
    EXPECT_STREQ("-0.0167", decimal("-167", 4, 4).toString().c_str());
    EXPECT_STREQ("0.0056", decimal("+56", 4, 4).toString().c_str());
    EXPECT_STREQ("8730198.23623",
        decimal("873019823623", 20, 5).toString().c_str());

    EXPECT_STREQ("0", decimal("-0", 12, 3).unscaledValue());
    EXPECT_STREQ("0", decimal("0", 12, 3).unscaledValue());
    EXPECT_STREQ("0", decimal("+0000", 12, 3).unscaledValue());
    EXPECT_STREQ("0", decimal("-000000", 12, 3).unscaledValue());
    EXPECT_STREQ("0", decimal("000000", 12, 3).unscaledValue());
    EXPECT_STREQ("-2", decimal("-00002", 12, 3).unscaledValue());
    EXPECT_STREQ("10", decimal("+10", 12, 3).unscaledValue());
    EXPECT_STREQ("-567", decimal("-0567", 12, 3).unscaledValue());
    EXPECT_STREQ("167", decimal("167", 12, 3).unscaledValue());
    EXPECT_STREQ("-167", decimal("-167", 12, 3).unscaledValue());
    EXPECT_STREQ("69234234", decimal("69234234", 12, 3).unscaledValue());

    EXPECT_EQ(decimal("120000", 6, 4), decimal("12", 2, 0));
    EXPECT_EQ(decimal("0", 12, 3), decimal("0", 12, 4));
    EXPECT_EQ(decimal("-120000", 12, 4), decimal("-12", 12, 0));
    EXPECT_FALSE(decimal("120", 12, 1) == decimal("120", 12, 0));

    EXPECT_NE(decimal("120", 12, 1), decimal("120", 12, 0));
    EXPECT_FALSE(decimal("120", 12, 1) != decimal("120", 12, 1));

    EXPECT_LT(decimal("110", 12, 1), decimal("12", 12, 0));
    EXPECT_LT(decimal("-11", 12, 0), decimal("9", 12, 0));
    EXPECT_LT(decimal("-11", 12, 0), decimal("0", 12, 0));
    EXPECT_LT(decimal("0", 12, 0), decimal("1", 12, 8));
    EXPECT_FALSE(decimal("120", 12, 1) < decimal("120", 12, 2));

    EXPECT_GT(decimal("12", 12, 0), decimal("110", 12, 1));
    EXPECT_GT(decimal("9", 12, 0), decimal("-11", 12, 0));
    EXPECT_GT(decimal("0", 12, 0), decimal("-11", 12, 0));
    EXPECT_GT(decimal("1", 12, 8), decimal("0", 12, 0));
    EXPECT_FALSE(decimal("120", 12, 2) > decimal("120", 12, 1));

    EXPECT_LE(decimal("120000", 6, 4), decimal("12", 2, 0));
    EXPECT_LE(decimal("0", 12, 3), decimal("0", 12, 4));
    EXPECT_LE(decimal("-120000", 12, 4), decimal("-12", 12, 0));
    EXPECT_LE(decimal("110", 12, 1), decimal("12", 12, 0));
    EXPECT_LE(decimal("-11", 12, 0), decimal("9", 12, 0));
    EXPECT_LE(decimal("-11", 12, 0), decimal("0", 12, 0));
    EXPECT_LE(decimal("0", 12, 0), decimal("1", 12, 8));
    EXPECT_FALSE(decimal("120", 12, 1) <= decimal("120", 12, 2));

    EXPECT_GE(decimal("120000", 6, 4), decimal("12", 2, 0));
    EXPECT_GE(decimal("0", 12, 3), decimal("0", 12, 4));
    EXPECT_GE(decimal("-120000", 12, 4), decimal("-12", 12, 0));
    EXPECT_GE(decimal("12", 12, 0), decimal("110", 12, 1));
    EXPECT_GE(decimal("9", 12, 0), decimal("-11", 12, 0));
    EXPECT_GE(decimal("0", 12, 0), decimal("-11", 12, 0));
    EXPECT_GE(decimal("1", 12, 8), decimal("0", 12, 0));
    EXPECT_FALSE(decimal("120", 12, 2) >= decimal("120", 12, 1));

    ostringstream os;
    os << decimal("69234234", 12, 3);
    EXPECT_STREQ("69234.234", os.str().c_str());
}
//------------------------------------------------------------------------------
TEST(TypesTest, date)
{
    EXPECT_THROW(date(-2, 1, 1), Exception);
    EXPECT_THROW(date(1980, 0, 1), Exception);
    EXPECT_THROW(date(1980, 1, 0), Exception);
    EXPECT_THROW(date(1980, 2, 30), Exception);

    EXPECT_NO_THROW(date());
    EXPECT_NO_THROW(date(1980, 1, 1));
    EXPECT_NO_THROW(date(1980, 12, 31));
    EXPECT_NO_THROW(date(1980, 2, 28));

    EXPECT_STREQ("0000-01-01", date().toString().c_str());
    EXPECT_STREQ("1980-12-03", date(1980, 12, 3).toString().c_str());
    EXPECT_STREQ("1980-01-13", date(1980, 1, 13).toString().c_str());
    EXPECT_STREQ("0001-01-13", date(1, 1, 13).toString().c_str());

    EXPECT_EQ(date(1980, 12, 3), date(1980, 12, 3));
    EXPECT_NE(date(1980, 12, 3), date(1980, 12, 2));

    EXPECT_LT(date(1979, 12, 2), date(1980, 12, 2));
    EXPECT_LT(date(1980, 11, 2), date(1980, 12, 2));
    EXPECT_LT(date(1980, 12, 2), date(1980, 12, 3));
    EXPECT_FALSE(date(1981, 12, 2) < date(1980, 12, 2));
    EXPECT_FALSE(date(1980, 12, 2) < date(1980, 11, 2));

    EXPECT_GT(date(1981, 12, 2), date(1980, 12, 2));
    EXPECT_GT(date(1980, 12, 2), date(1980, 11, 2));
    EXPECT_GT(date(1980, 12, 3), date(1980, 12, 2));
    EXPECT_FALSE(date(1980, 12, 2) > date(1981, 12, 2));
    EXPECT_FALSE(date(1980, 11, 2) > date(1980, 12, 2));

    EXPECT_LE(date(1980, 12, 3), date(1980, 12, 3));
    EXPECT_LE(date(1980, 12, 2), date(1980, 12, 3));
    EXPECT_FALSE(date(1980, 12, 3) <= date(1980, 12, 2));

    EXPECT_GE(date(1980, 12, 3), date(1980, 12, 3));
    EXPECT_GE(date(1980, 12, 3), date(1980, 12, 2));
    EXPECT_FALSE(date(1980, 12, 2) >= date(1980, 12, 3));

    ostringstream os;
    os << date(1, 1, 13);
    EXPECT_STREQ("0001-01-13", os.str().c_str());
}
//------------------------------------------------------------------------------
TEST(TypesTest, time)
{
    EXPECT_THROW(time(-1, 1, 1), Exception);
    EXPECT_THROW(time(1, -1, 1), Exception);
    EXPECT_THROW(time(1, 1, -1), Exception);
    EXPECT_THROW(time(24, 1, 1), Exception);

    EXPECT_NO_THROW(time());
    EXPECT_NO_THROW(time(0, 0, 59));
    EXPECT_NO_THROW(time(0, 59, 59));
    EXPECT_NO_THROW(time(23, 59, 59));
    EXPECT_NO_THROW(time(23, 59, 59));

    EXPECT_STREQ("00:00:00", time().toString().c_str());
    EXPECT_STREQ("05:12:03", time(5, 12, 3).toString().c_str());
    EXPECT_STREQ("23:04:46", time(23, 4, 46).toString().c_str());
    EXPECT_STREQ("02:12:03", time(2, 12, 3).toString().c_str());

    EXPECT_EQ(time(12, 34, 56), time(12, 34, 56));

    EXPECT_NE(time(12, 34, 56), time(12, 34, 55));
    EXPECT_FALSE(time(12, 34, 55) != time(12, 34, 55));

    EXPECT_LT(time(12, 34, 55), time(13, 34, 55));
    EXPECT_LT(time(12, 34, 55), time(12, 35, 55));
    EXPECT_LT(time(12, 34, 55), time(12, 34, 56));
    EXPECT_FALSE(time(13, 34, 55) < time(12, 34, 55));
    EXPECT_FALSE(time(12, 35, 55) < time(12, 34, 55));

    EXPECT_GT(time(13, 34, 55), time(12, 34, 55));
    EXPECT_GT(time(12, 35, 55), time(12, 34, 55));
    EXPECT_GT(time(12, 34, 56), time(12, 34, 55));
    EXPECT_FALSE(time(12, 34, 55) > time(13, 34, 55));
    EXPECT_FALSE(time(12, 34, 55) > time(12, 35, 55));

    EXPECT_LE(time(12, 34, 56), time(12, 34, 56));
    EXPECT_LE(time(12, 34, 55), time(12, 34, 56));
    EXPECT_FALSE(time(13, 34, 55) <= time(12, 34, 55));
    EXPECT_FALSE(time(12, 35, 55) <= time(12, 34, 55));

    EXPECT_GE(time(12, 34, 56), time(12, 34, 56));
    EXPECT_GE(time(12, 34, 56), time(12, 34, 55));
    EXPECT_FALSE(time(12, 34, 55) >= time(13, 34, 55));
    EXPECT_FALSE(time(12, 34, 55) >= time(12, 35, 55));

    ostringstream os;
    os << odbc::time(2, 12, 3);
    EXPECT_STREQ("02:12:03", os.str().c_str());
}
//------------------------------------------------------------------------------
TEST(TypesTest, timestamp)
{
    EXPECT_THROW(timestamp(1980, 12, 13, -1, 1, 1, 0), Exception);
    EXPECT_THROW(timestamp(1980, 14, 13, 1, 1, 1, 0), Exception);
    EXPECT_THROW(timestamp(1980, 12, 13, 1, -1, 1, 0), Exception);
    EXPECT_THROW(timestamp(1980, 12, 13, 1, 1, 1, 1002), Exception);

    EXPECT_NO_THROW(timestamp());
    EXPECT_NO_THROW(timestamp(1980, 1, 1, 23, 59, 59, 0));
    EXPECT_NO_THROW(timestamp(1980, 12, 28, 0, 59, 59, 0));

    EXPECT_STREQ("1980-12-13 05:12:03.042",
        timestamp(1980, 12, 13, 5, 12, 3, 42).toString().c_str());
    EXPECT_STREQ("1980-01-02 05:12:45.998",
        timestamp(1980, 1, 2, 5, 12, 45, 998).toString().c_str());

    EXPECT_EQ(
        timestamp(1980, 12, 3, 12, 34, 56, 123),
        timestamp(1980, 12, 3, 12, 34, 56, 123));

    EXPECT_NE(
        timestamp(1980, 12, 3, 12, 34, 56, 122),
        timestamp(1980, 12, 3, 12, 34, 56, 123));
    EXPECT_FALSE(
        timestamp(1980, 12, 3, 12, 34, 56, 122) !=
        timestamp(1980, 12, 3, 12, 34, 56, 122));

    EXPECT_LT(
        timestamp(1980, 12, 3, 12, 34, 56, 122),
        timestamp(1981, 12, 3, 12, 34, 56, 122));
    EXPECT_LT(
        timestamp(1980, 12, 3, 12, 34, 56, 122),
        timestamp(1980, 12, 3, 12, 35, 56, 122));
    EXPECT_LT(
        timestamp(1980, 12, 3, 12, 34, 56, 122),
        timestamp(1980, 12, 3, 12, 34, 56, 123));

    EXPECT_GT(
        timestamp(1981, 12, 3, 12, 34, 56, 122),
        timestamp(1980, 12, 3, 12, 34, 56, 122));
    EXPECT_GT(
        timestamp(1980, 12, 3, 12, 35, 56, 122),
        timestamp(1980, 12, 3, 12, 34, 56, 122));
    EXPECT_GT(
        timestamp(1980, 12, 3, 12, 34, 56, 123),
        timestamp(1980, 12, 3, 12, 34, 56, 122));

    EXPECT_LE(
        timestamp(1980, 12, 3, 12, 34, 56, 123),
        timestamp(1980, 12, 3, 12, 34, 56, 123));
    EXPECT_LE(
        timestamp(1980, 12, 3, 12, 34, 56, 122),
        timestamp(1980, 12, 3, 12, 34, 56, 123));
    EXPECT_FALSE(
        timestamp(1981, 12, 3, 12, 34, 56, 122) <=
        timestamp(1980, 12, 3, 12, 34, 56, 122));

    EXPECT_GE(
        timestamp(1980, 12, 3, 12, 34, 56, 123),
        timestamp(1980, 12, 3, 12, 34, 56, 123));
    EXPECT_GE(
        timestamp(1980, 12, 3, 12, 34, 56, 123),
        timestamp(1980, 12, 3, 12, 34, 56, 122));
    EXPECT_FALSE(
        timestamp(1980, 12, 3, 12, 34, 56, 122) >=
        timestamp(1981, 12, 3, 12, 34, 56, 122));

    ostringstream os;
    os << timestamp(1980, 1, 2, 5, 12, 45, 998);
    EXPECT_STREQ("1980-01-02 05:12:45.998", os.str().c_str());
}
//------------------------------------------------------------------------------
TEST(TypesTest, Nullable)
{
    Int null;
    Int four(4);
    Int five(5);

    EXPECT_TRUE(null.isNull());
    EXPECT_FALSE(four.isNull());
    EXPECT_EQ(4, *Int(four));
    EXPECT_EQ(4, *Int(Int(4)));

    Int nullCopy;
    nullCopy = null;
    EXPECT_TRUE(nullCopy.isNull());

    Int fourCopy;
    fourCopy = four;
    EXPECT_FALSE(fourCopy.isNull());
    EXPECT_EQ(4, *fourCopy);

    Int nullMove;
    nullMove = Int();
    EXPECT_TRUE(nullMove.isNull());

    Int fourMove;
    fourMove = Int(4);
    EXPECT_FALSE(fourMove.isNull());
    EXPECT_EQ(4, *fourMove);

    EXPECT_EQ(null, null);
    EXPECT_EQ(five, five);

    EXPECT_NE(five, null);
    EXPECT_NE(null, five);
    EXPECT_NE(four, five);

    EXPECT_LT(five, null);
    EXPECT_LT(four, five);

    EXPECT_GT(null, five);
    EXPECT_GT(five, four);

    EXPECT_LE(null, null);
    EXPECT_LE(five, five);
    EXPECT_LE(five, null);
    EXPECT_LE(four, five);

    EXPECT_GE(null, null);
    EXPECT_GE(five, five);
    EXPECT_GE(null, five);
    EXPECT_GE(five, four);

    Date d = makeNullable<date>(2018, 4, 6);
    EXPECT_EQ(2018, d->year());
    EXPECT_EQ(4, d->month());
    EXPECT_EQ(6, d->day());

    ostringstream os;
    os << d;
    EXPECT_STREQ("2018-04-06", os.str().c_str());

    os.str("");
    os << Decimal();
    EXPECT_STREQ("<NULL>", os.str().c_str());
}
//------------------------------------------------------------------------------
} // namespace tests
NS_ODBC_END
