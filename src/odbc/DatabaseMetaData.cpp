#include <cstring>
#include <odbc/Connection.h>
#include <odbc/DatabaseMetaData.h>
#include <odbc/Exception.h>
#include <odbc/ResultSet.h>
#include <odbc/Statement.h>
#include <odbc/internal/Macros.h>
#include <odbc/internal/Odbc.h>
//------------------------------------------------------------------------------
namespace odbc {
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
DatabaseMetaData::DatabaseMetaData(Connection* parent) : parent_(parent, true)
{
}
//------------------------------------------------------------------------------
ResultSetRef DatabaseMetaData::getColumns(const char* catalogName,
    const char* schemaName, const char* tableName, const char* columnName)
{
    size_t catalogLen = catalogName ? strlen(catalogName) : 0;
    size_t schemaLen = schemaName ? strlen(schemaName) : 0;
    size_t tableLen = tableName ? strlen(tableName) : 0;
    size_t columnLen = columnName ? strlen(columnName) : 0;

    size_t maxLen = (1 << 8*sizeof(SQLSMALLINT)) - 1;
    if (catalogLen > maxLen)
        throw Exception("The catalog name is too long");
    if (schemaLen > maxLen)
        throw Exception("The schema name is too long");
    if (tableLen > maxLen)
        throw Exception("The table name is too long");
    if (columnLen > maxLen)
        throw Exception("The column name is too long");

    StatementRef stmt = parent_->createStatement();
    ResultSetRef ret(new ResultSet(stmt.get()));
    EXEC_STMT(SQLColumnsA, stmt->hstmt_,
        (SQLCHAR*)catalogName, (SQLSMALLINT)catalogLen,
        (SQLCHAR*)schemaName, (SQLSMALLINT)schemaLen,
        (SQLCHAR*)tableName, (SQLSMALLINT)tableLen,
        (SQLCHAR*)columnName, (SQLSMALLINT)columnLen);
    return ret;
}
//------------------------------------------------------------------------------
ResultSetRef DatabaseMetaData::getPrimaryKeys(const char* catalogName,
    const char* schemaName, const char* tableName)
{
    size_t catalogLen = catalogName ? strlen(catalogName) : 0;
    size_t schemaLen = schemaName ? strlen(schemaName) : 0;
    size_t tableLen = tableName ? strlen(tableName) : 0;

    size_t maxLen = (1 << 8*sizeof(SQLSMALLINT)) - 1;
    if (catalogLen > maxLen)
        throw Exception("The catalog name is too long");
    if (schemaLen > maxLen)
        throw Exception("The schema name is too long");
    if (tableLen > maxLen)
        throw Exception("The table name is too long");

    StatementRef stmt = parent_->createStatement();
    ResultSetRef ret(new ResultSet(stmt.get()));
    EXEC_STMT(SQLPrimaryKeysA, stmt->hstmt_,
        (SQLCHAR*)catalogName, (SQLSMALLINT)catalogLen,
        (SQLCHAR*)schemaName, (SQLSMALLINT)schemaLen,
        (SQLCHAR*)tableName, (SQLSMALLINT)tableLen);
    return ret;
}
//------------------------------------------------------------------------------
ResultSetRef DatabaseMetaData::getTables(const char* catalogName,
    const char* schemaName, const char* tableName, const char* tableType)
{
    size_t catalogLen = catalogName ? strlen(catalogName) : 0;
    size_t schemaLen = schemaName ? strlen(schemaName) : 0;
    size_t tableLen = tableName ? strlen(tableName) : 0;
    size_t tableTypeLen = tableType ? strlen(tableType) : 0;

    size_t maxLen = (1 << 8 * sizeof(SQLSMALLINT)) - 1;
    if (catalogLen > maxLen)
        throw Exception("The catalog name is too long");
    if (schemaLen > maxLen)
        throw Exception("The schema name is too long");
    if (tableLen > maxLen)
        throw Exception("The table name is too long");
    if (tableTypeLen > maxLen)
        throw Exception("The table type is too long");

    StatementRef stmt = parent_->createStatement();
    ResultSetRef ret(new ResultSet(stmt.get()));
    EXEC_STMT(SQLTablesA, stmt->hstmt_,
        (SQLCHAR*)catalogName, (SQLSMALLINT)catalogLen,
        (SQLCHAR*)schemaName, (SQLSMALLINT)schemaLen,
        (SQLCHAR*)tableName, (SQLSMALLINT)tableLen,
        (SQLCHAR*)tableType, (SQLSMALLINT)tableTypeLen);
    return ret;
}
//------------------------------------------------------------------------------
ResultSetRef DatabaseMetaData::getTypeInfo()
{
    StatementRef stmt = parent_->createStatement();
    ResultSetRef ret(new ResultSet(stmt.get()));
    EXEC_STMT(SQLGetTypeInfoA, stmt->hstmt_, SQL_ALL_TYPES);
    return ret;
}
//------------------------------------------------------------------------------
ResultSetRef DatabaseMetaData::getTypeInfo(int type)
{
    StatementRef stmt = parent_->createStatement();
    ResultSetRef ret(new ResultSet(stmt.get()));
    EXEC_STMT(SQLGetTypeInfoA, stmt->hstmt_, type);
    return ret;
}
//------------------------------------------------------------------------------
string DatabaseMetaData::getDataSourceName()
{
    return getStringTypeInfo(SQL_DATA_SOURCE_NAME);
}
//------------------------------------------------------------------------------
string DatabaseMetaData::getDatabaseName()
{
    return getStringTypeInfo(SQL_DATABASE_NAME);
}
//------------------------------------------------------------------------------
string DatabaseMetaData::getDBMSName()
{
    return getStringTypeInfo(SQL_DBMS_NAME);
}
//------------------------------------------------------------------------------
string DatabaseMetaData::getDBMSVersion()
{
    return getStringTypeInfo(SQL_DBMS_VER);
}
//------------------------------------------------------------------------------
string DatabaseMetaData::getDriverName()
{
    return getStringTypeInfo(SQL_DRIVER_NAME);
}
//------------------------------------------------------------------------------
string DatabaseMetaData::getDriverVersion()
{
    return getStringTypeInfo(SQL_DRIVER_VER);
}
//------------------------------------------------------------------------------
unsigned short DatabaseMetaData::getMaxConnections()
{
    return getUSmallIntTypeInfo(SQL_MAX_DRIVER_CONNECTIONS);
}
//------------------------------------------------------------------------------
unsigned long DatabaseMetaData::getMaxStatementLength()
{
    return getUIntTypeInfo(SQL_MAX_STATEMENT_LEN);
}
//------------------------------------------------------------------------------
string DatabaseMetaData::getServerName()
{
    return getStringTypeInfo(SQL_SERVER_NAME);
}
//------------------------------------------------------------------------------
string DatabaseMetaData::getUserName()
{
    return getStringTypeInfo(SQL_USER_NAME);
}
//------------------------------------------------------------------------------
bool DatabaseMetaData::isReadOnly()
{
    return (getStringTypeInfo(SQL_DATA_SOURCE_READ_ONLY) == "Y");
}
//------------------------------------------------------------------------------
bool DatabaseMetaData::supportsAlterTableWithAddColumn()
{
    SQLUINTEGER val = getUIntTypeInfo(SQL_ALTER_TABLE);
    return IS_FLAG_SET(val, SQL_AT_ADD_COLUMN_COLLATION) ||
           IS_FLAG_SET(val, SQL_AT_ADD_COLUMN_DEFAULT) ||
           IS_FLAG_SET(val, SQL_AT_ADD_COLUMN_SINGLE);
}
//------------------------------------------------------------------------------
bool DatabaseMetaData::supportsAlterTableWithDropColumn()
{
    SQLUINTEGER val = getUIntTypeInfo(SQL_ALTER_TABLE);
    return IS_FLAG_SET(val, SQL_AT_DROP_COLUMN_CASCADE) ||
           IS_FLAG_SET(val, SQL_AT_DROP_COLUMN_DEFAULT) ||
           IS_FLAG_SET(val, SQL_AT_DROP_COLUMN_RESTRICT);
}
//------------------------------------------------------------------------------
TransactionIsolationLevel DatabaseMetaData::getDefaultTransactionIsolation()
{
    SQLUINTEGER txn = getUIntTypeInfo(SQL_DEFAULT_TXN_ISOLATION);
    switch (txn)
    {
    case SQL_TXN_READ_COMMITTED:
        return TransactionIsolationLevel::READ_COMMITTED;
    case SQL_TXN_READ_UNCOMMITTED:
        return TransactionIsolationLevel::READ_UNCOMMITTED;
    case SQL_TXN_REPEATABLE_READ:
        return TransactionIsolationLevel::REPEATABLE_READ;
    case SQL_TXN_SERIALIZABLE:
        return TransactionIsolationLevel::SERIALIZABLE;
    case 0:
        return TransactionIsolationLevel::NONE;
    default:
        throw Exception("Unknown transaction isolation level.");
    }
}
//------------------------------------------------------------------------------
bool DatabaseMetaData::supportsTransactionIsolation(
    TransactionIsolationLevel level)
{
    SQLUINTEGER txn = getUIntTypeInfo(SQL_TXN_ISOLATION_OPTION);
    switch (level)
    {
    case TransactionIsolationLevel::READ_COMMITTED:
        return IS_FLAG_SET(txn, SQL_TXN_READ_COMMITTED);
    case TransactionIsolationLevel::READ_UNCOMMITTED:
        return IS_FLAG_SET(txn, SQL_TXN_READ_UNCOMMITTED);
    case TransactionIsolationLevel::REPEATABLE_READ:
        return IS_FLAG_SET(txn, SQL_TXN_REPEATABLE_READ);
    case TransactionIsolationLevel::SERIALIZABLE:
        return IS_FLAG_SET(txn, SQL_TXN_SERIALIZABLE);
    default:
        return false;
    }
}
//------------------------------------------------------------------------------
string DatabaseMetaData::getStringTypeInfo(unsigned short typeInfo)
{
    vector<char> buffer;
    buffer.resize(256);
    while (true)
    {
        SQLPOINTER ptr = &buffer[0];
        SQLSMALLINT bufLen = (SQLSMALLINT)buffer.size();
        SQLSMALLINT dataLen;
        EXEC_DBC(SQLGetInfoA, parent_->hdbc_, typeInfo, ptr, bufLen, &dataLen);
        if (dataLen < bufLen)
            break;
        buffer.resize(dataLen + 1);
    }
    return string(&buffer[0]);
}
//------------------------------------------------------------------------------
unsigned long DatabaseMetaData::getUIntTypeInfo(unsigned short typeInfo)
{
    SQLUINTEGER ret;
    SQLSMALLINT len;
    EXEC_DBC(SQLGetInfoA, parent_->hdbc_, typeInfo, &ret, sizeof(ret), &len);
    return ret;
}
//------------------------------------------------------------------------------
unsigned short DatabaseMetaData::getUSmallIntTypeInfo(unsigned short typeInfo)
{
    SQLUSMALLINT ret;
    SQLSMALLINT len;
    EXEC_DBC(SQLGetInfoA, parent_->hdbc_, typeInfo, &ret, sizeof(ret), &len);
    return ret;
}
//------------------------------------------------------------------------------
} // namespace odbc
