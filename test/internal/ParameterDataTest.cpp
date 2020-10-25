#include <odbc/internal/Odbc.h>
#include <odbc/internal/ParameterData.h>
#include <GoogleTest.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <utility>
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
namespace odbc {
namespace tests {
//------------------------------------------------------------------------------
template<typename CharT, int N>
static void generateAlphabet(CharT (&buffer)[N], CharT firstLetter = (CharT)'A')
{
    for (int i = 0; i < N; ++i)
        buffer[i] = (CharT)'A' + ((firstLetter - (CharT)'A' + i) % 26);
}
//------------------------------------------------------------------------------
TEST(ParameterDataTest, testInitialState)
{
    ParameterData pd;
    EXPECT_FALSE(pd.isInitialized());
}
//------------------------------------------------------------------------------
TEST(ParameterDataTest, testSettingValues)
{
    int32_t inplaceValue = 1234567890;
    char heapValue[ParameterData::INPLACE_BYTES + 1];
    generateAlphabet(heapValue);

    ParameterData pd;

    // Test setting a value that can be stored inplace
    pd.setValue(SQL_C_SLONG, &inplaceValue, sizeof(inplaceValue));
    EXPECT_EQ(SQL_C_SLONG, pd.getValueType());
    EXPECT_FALSE(pd.isNull());
    ASSERT_EQ(sizeof(inplaceValue), pd.getSize());
    EXPECT_EQ(0, memcmp(&inplaceValue, pd.getData(), sizeof(inplaceValue)));
    EXPECT_EQ(sizeof(inplaceValue), *(size_t*)pd.getLenIndPtr());
    EXPECT_FALSE(pd.usesHeapBuffer());
    EXPECT_FALSE(pd.ownsHeapBuffer());

    // Test setting a value that has to be stored on the heap
    pd.setValue(SQL_C_CHAR, heapValue, sizeof(heapValue));
    EXPECT_EQ(SQL_C_CHAR, pd.getValueType());
    EXPECT_FALSE(pd.isNull());
    ASSERT_EQ(sizeof(heapValue), pd.getSize());
    EXPECT_EQ(0, memcmp(heapValue, pd.getData(), sizeof(heapValue)));
    EXPECT_EQ(sizeof(heapValue), *(size_t*)pd.getLenIndPtr());
    EXPECT_TRUE(pd.usesHeapBuffer());
    EXPECT_TRUE(pd.ownsHeapBuffer());

    // Test resetting a value that can be stored inplace
    pd.setValue(SQL_C_SLONG, &inplaceValue, sizeof(inplaceValue));
    EXPECT_EQ(SQL_C_SLONG, pd.getValueType());
    EXPECT_FALSE(pd.isNull());
    ASSERT_EQ(sizeof(inplaceValue), pd.getSize());
    EXPECT_EQ(0, memcmp(&inplaceValue, pd.getData(), sizeof(inplaceValue)));
    EXPECT_EQ(sizeof(inplaceValue), *(size_t*)pd.getLenIndPtr());
    EXPECT_FALSE(pd.usesHeapBuffer());
    EXPECT_FALSE(pd.ownsHeapBuffer());
}
//------------------------------------------------------------------------------
TEST(ParameterDataTest, testSettingNull)
{
    ParameterData pd;
    pd.setNull(SQL_C_CHAR);

    EXPECT_EQ(SQL_C_CHAR, pd.getValueType());
    EXPECT_TRUE(pd.isNull());
    EXPECT_FALSE(pd.usesHeapBuffer());
}
//------------------------------------------------------------------------------
TEST(ParameterDataTest, testSettingNullAfterHeapBufferUse)
{
    char heapValue[ParameterData::INPLACE_BYTES + 1];
    generateAlphabet(heapValue);

    ParameterData pd;
    pd.setValue(SQL_C_CHAR, heapValue, sizeof(heapValue));
    EXPECT_TRUE(pd.usesHeapBuffer());
    EXPECT_TRUE(pd.ownsHeapBuffer());

    pd.setNull(SQL_C_SLONG);
    EXPECT_EQ(SQL_C_SLONG, pd.getValueType());
    EXPECT_TRUE(pd.isNull());
    EXPECT_FALSE(pd.usesHeapBuffer());
}
//------------------------------------------------------------------------------
TEST(ParameterDataTest, testVaryingHeapBufferSizes)
{
    constexpr size_t BASESIZE = 2 * ParameterData::INPLACE_BYTES;
    constexpr size_t INCSIZE = 2 * BASESIZE;
    constexpr size_t REUSESIZE =
        (size_t)(ParameterData::LOAD_FACTOR * (double)INCSIZE);
    constexpr size_t SHRINKSIZE = REUSESIZE - 1;

    char heapValue[INCSIZE];
    generateAlphabet(heapValue);

    ParameterData pd;

    // Initialize with base size
    pd.setValue(SQL_C_CHAR, heapValue, BASESIZE);
    EXPECT_EQ(BASESIZE, pd.getSize());
    EXPECT_EQ(BASESIZE, pd.getHeapBufferCapacity());
    EXPECT_EQ(0, memcmp(heapValue, pd.getData(), BASESIZE));

    // Increase the size (re-allocation required)
    pd.setValue(SQL_C_CHAR, heapValue, INCSIZE);
    EXPECT_EQ(INCSIZE, pd.getSize());
    EXPECT_EQ(INCSIZE, pd.getHeapBufferCapacity());
    EXPECT_EQ(0, memcmp(heapValue, pd.getData(), INCSIZE));

    // Set a shorter string that should re-use the existing buffer
    pd.setValue(SQL_C_CHAR, heapValue, REUSESIZE);
    EXPECT_EQ(REUSESIZE, pd.getSize());
    EXPECT_EQ(INCSIZE, pd.getHeapBufferCapacity());
    EXPECT_EQ(0, memcmp(heapValue, pd.getData(), REUSESIZE));

    // Set a shorter string that should shring the existing buffer
    pd.setValue(SQL_C_CHAR, heapValue, SHRINKSIZE);
    EXPECT_EQ(SHRINKSIZE, pd.getSize());
    EXPECT_EQ(SHRINKSIZE, pd.getHeapBufferCapacity());
    EXPECT_EQ(0, memcmp(heapValue, pd.getData(), SHRINKSIZE));
}
//------------------------------------------------------------------------------
TEST(ParameterDataTest, testBufferOwnershipRelease)
{
    char heapValue[ParameterData::INPLACE_BYTES + 1];
    generateAlphabet(heapValue);

    const void* data;
    {
        ParameterData pd;
        pd.setValue(SQL_C_CHAR, heapValue, sizeof(heapValue));
        data = pd.getData();
        pd.releaseHeapBufferOwnership();
        EXPECT_TRUE(pd.usesHeapBuffer());
        EXPECT_FALSE(pd.ownsHeapBuffer());
    }
    EXPECT_EQ(0, memcmp(heapValue, data, sizeof(heapValue)));
    free((void*)data);
}
//------------------------------------------------------------------------------
TEST(ParameterDataTest, testBufferOwnershipRestore)
{
    char heapValue[ParameterData::INPLACE_BYTES + 1];
    generateAlphabet(heapValue);

    ParameterData pd;
    pd.setValue(SQL_C_CHAR, heapValue, sizeof(heapValue));

    pd.releaseHeapBufferOwnership();
    EXPECT_TRUE(pd.usesHeapBuffer());
    EXPECT_FALSE(pd.ownsHeapBuffer());

    pd.restoreHeapBufferOwnership();
    EXPECT_TRUE(pd.usesHeapBuffer());
    EXPECT_TRUE(pd.ownsHeapBuffer());
}
//------------------------------------------------------------------------------
TEST(ParameterDataTest, testMoveConstructorInplace)
{
    int32_t value = 1234567890;

    ParameterData pd1;
    pd1.setValue(SQL_C_SLONG, &value, sizeof(value));

    ParameterData pd2(move(pd1));
    EXPECT_EQ(SQL_C_SLONG, pd2.getValueType());
    EXPECT_FALSE(pd2.isNull());
    ASSERT_EQ(sizeof(value), pd2.getSize());
    EXPECT_EQ(0, memcmp(&value, pd2.getData(), sizeof(value)));
    EXPECT_FALSE(pd2.usesHeapBuffer());
    EXPECT_FALSE(pd2.ownsHeapBuffer());
}
//------------------------------------------------------------------------------
TEST(ParameterDataTest, testMoveConstructorHeap)
{
    char value[ParameterData::INPLACE_BYTES + 1];
    generateAlphabet(value);

    ParameterData pd1;
    pd1.setValue(SQL_C_CHAR, &value, sizeof(value));

    ParameterData pd2(move(pd1));
    EXPECT_EQ(SQL_C_CHAR, pd2.getValueType());
    EXPECT_FALSE(pd2.isNull());
    ASSERT_EQ(sizeof(value), pd2.getSize());
    EXPECT_EQ(0, memcmp(value, pd2.getData(), sizeof(value)));
    EXPECT_TRUE(pd2.usesHeapBuffer());
    EXPECT_TRUE(pd2.ownsHeapBuffer());
}
//------------------------------------------------------------------------------
TEST(ParameterDataTest, testMoveAssignmentInplaceInplace)
{
    int32_t value1 = 1234567890;
    int32_t value2 = 42;

    ParameterData pd1;
    pd1.setValue(SQL_C_SLONG, &value1, sizeof(value1));
    ParameterData pd2;
    pd2.setValue(SQL_C_SLONG, &value2, sizeof(value2));

    pd2 = move(pd1);
    EXPECT_EQ(SQL_C_SLONG, pd2.getValueType());
    EXPECT_FALSE(pd2.isNull());
    ASSERT_EQ(sizeof(value1), pd2.getSize());
    EXPECT_EQ(0, memcmp(&value1, pd2.getData(), sizeof(value1)));
    EXPECT_FALSE(pd2.usesHeapBuffer());
    EXPECT_FALSE(pd2.ownsHeapBuffer());
}
//------------------------------------------------------------------------------
TEST(ParameterDataTest, testMoveAssignmentInplaceHeap)
{
    int32_t value1 = 1234567890;
    char value2[ParameterData::INPLACE_BYTES + 1];
    generateAlphabet(value2);

    ParameterData pd1;
    pd1.setValue(SQL_C_SLONG, &value1, sizeof(value1));
    ParameterData pd2;
    pd2.setValue(SQL_C_CHAR, &value2, sizeof(value2));

    pd2 = move(pd1);
    EXPECT_EQ(SQL_C_SLONG, pd2.getValueType());
    EXPECT_FALSE(pd2.isNull());
    ASSERT_EQ(sizeof(value1), pd2.getSize());
    EXPECT_EQ(0, memcmp(&value1, pd2.getData(), sizeof(value1)));
    EXPECT_FALSE(pd2.usesHeapBuffer());
    EXPECT_FALSE(pd2.ownsHeapBuffer());
}
//------------------------------------------------------------------------------
TEST(ParameterDataTest, testMoveAssignmentHeapInplace)
{
    char value1[ParameterData::INPLACE_BYTES + 1];
    generateAlphabet(value1);
    int32_t value2 = 1234567890;

    ParameterData pd1;
    pd1.setValue(SQL_C_CHAR, &value1, sizeof(value1));
    ParameterData pd2;
    pd2.setValue(SQL_C_SLONG, &value2, sizeof(value2));

    pd2 = move(pd1);
    EXPECT_EQ(SQL_C_CHAR, pd2.getValueType());
    EXPECT_FALSE(pd2.isNull());
    ASSERT_EQ(sizeof(value1), pd2.getSize());
    EXPECT_EQ(0, memcmp(value1, pd2.getData(), sizeof(value1)));
    EXPECT_TRUE(pd2.usesHeapBuffer());
    EXPECT_TRUE(pd2.ownsHeapBuffer());
}
//------------------------------------------------------------------------------
TEST(ParameterDataTest, testMoveAssignmentHeapHeap)
{
    char value1[ParameterData::INPLACE_BYTES + 1];
    generateAlphabet(value1);
    char16_t value2[ParameterData::INPLACE_BYTES + 2];
    generateAlphabet(value2, u'B');

    ParameterData pd1;
    pd1.setValue(SQL_C_CHAR, &value1, sizeof(value1));
    ParameterData pd2;
    pd2.setValue(SQL_C_WCHAR, &value2, sizeof(value2));

    pd2 = move(pd1);
    EXPECT_EQ(SQL_C_CHAR, pd2.getValueType());
    EXPECT_FALSE(pd2.isNull());
    ASSERT_EQ(sizeof(value1), pd2.getSize());
    EXPECT_EQ(0, memcmp(value1, pd2.getData(), sizeof(value1)));
    EXPECT_TRUE(pd2.usesHeapBuffer());
    EXPECT_TRUE(pd2.ownsHeapBuffer());
}
//------------------------------------------------------------------------------
} // namespace tests
} // namespace odbc
