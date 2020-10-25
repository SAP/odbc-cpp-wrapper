#include <vector>
#include <odbc/Environment.h>
#include <GoogleTest.h>
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
namespace odbc {
namespace tests {
//------------------------------------------------------------------------------
TEST(EnvironmentTest, create)
{
    EnvironmentRef env = Environment::create();
    EXPECT_FALSE(env.isNull());
}
//------------------------------------------------------------------------------
TEST(EnvironmentTest, getDataSources)
{
    EnvironmentRef env = Environment::create();
    vector<DataSourceInformation> dataSources = env->getDataSources();
    for (const DataSourceInformation& dsInfo : dataSources)
    {
        EXPECT_TRUE(dsInfo.name.length() > 0);
    }
}
//------------------------------------------------------------------------------
TEST(EnvironmentTest, getDrivers)
{
    EnvironmentRef env = Environment::create();
    vector<DriverInformation> drivers = env->getDrivers();
    for (const DriverInformation& drvInfo : drivers)
    {
        EXPECT_TRUE(drvInfo.description.length() > 0);
    }
}
//------------------------------------------------------------------------------
TEST(EnvironmentTest, isDriverInstalled)
{
    EnvironmentRef env = Environment::create();
    vector<DriverInformation> drivers = env->getDrivers();
    for (const DriverInformation& drvInfo : drivers)
    {
        EXPECT_TRUE(env->isDriverInstalled(drvInfo.description.c_str()));
    }
    EXPECT_FALSE(env->isDriverInstalled("DUMMY DRIVER"));
}
//------------------------------------------------------------------------------
} // namespace tests
} // namespace odbc
