#include <vector>
#include <odbc/Exception.h>
#include <odbc/ResultSetMetaData.h>
#include <odbc/Statement.h>
#include <odbc/internal/Macros.h>
#include <odbc/internal/Odbc.h>
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
namespace odbc {
//------------------------------------------------------------------------------
ResultSetMetaData::ResultSetMetaData(StatementBase* stmt)
    : stmt_(stmt, true)
{
}
//------------------------------------------------------------------------------
unsigned short ResultSetMetaData::getColumnCount()
{
    SQLSMALLINT ret;
    EXEC_STMT(SQLNumResultCols, stmt_->hstmt_, &ret);
    return (unsigned short)ret;
}
//------------------------------------------------------------------------------
string ResultSetMetaData::getCatalogName(unsigned short columnIndex)
{
  return getStringColAttribute(columnIndex, SQL_DESC_CATALOG_NAME);
}
//------------------------------------------------------------------------------
string ResultSetMetaData::getSchemaName(unsigned short columnIndex)
{
    return getStringColAttribute(columnIndex, SQL_DESC_SCHEMA_NAME);
}
//------------------------------------------------------------------------------
string ResultSetMetaData::getTableName(unsigned short columnIndex)
{
    return getStringColAttribute(columnIndex, SQL_DESC_TABLE_NAME);
}
//------------------------------------------------------------------------------
string ResultSetMetaData::getColumnLabel(unsigned short columnIndex)
{
  return getStringColAttribute(columnIndex, SQL_DESC_LABEL);
}
//------------------------------------------------------------------------------
string ResultSetMetaData::getColumnName(unsigned short columnIndex)
{
    return getStringColAttribute(columnIndex, SQL_DESC_NAME);
}
//------------------------------------------------------------------------------
short ResultSetMetaData::getColumnType(unsigned short columnIndex)
{
    SQLLEN ret;
    EXEC_STMT(SQLColAttributeA, stmt_->hstmt_, columnIndex,
        SQL_DESC_TYPE, NULL, 0, NULL, &ret);
    return (short)ret;
}
//------------------------------------------------------------------------------
string ResultSetMetaData::getColumnTypeName(unsigned short columnIndex)
{
    return getStringColAttribute(columnIndex, SQL_DESC_TYPE_NAME);
}
//------------------------------------------------------------------------------
size_t ResultSetMetaData::getColumnLength(unsigned short columnIndex)
{
    SQLLEN ret;
    EXEC_STMT(SQLColAttributeA, stmt_->hstmt_, columnIndex,
        SQL_DESC_LENGTH, NULL, 0, NULL, &ret);
    return (size_t)ret;
}
//------------------------------------------------------------------------------
size_t ResultSetMetaData::getColumnOctetLength(unsigned short columnIndex)
{
    SQLLEN ret;
    EXEC_STMT(SQLColAttributeA, stmt_->hstmt_, columnIndex,
        SQL_DESC_OCTET_LENGTH, NULL, 0, NULL, &ret);
    return (size_t)ret;
}
//------------------------------------------------------------------------------
size_t ResultSetMetaData::getColumnDisplaySize(unsigned short columnIndex)
{
    SQLLEN ret;
    EXEC_STMT(SQLColAttributeA, stmt_->hstmt_, columnIndex,
        SQL_DESC_DISPLAY_SIZE, NULL, 0, NULL, &ret);
    return (size_t)ret;
}
//------------------------------------------------------------------------------
unsigned short ResultSetMetaData::getPrecision(unsigned short columnIndex)
{
    SQLLEN ret;
    EXEC_STMT(SQLColAttributeA, stmt_->hstmt_, columnIndex,
        SQL_DESC_PRECISION, NULL, 0, NULL, &ret);
    return (unsigned short)ret;
}
//------------------------------------------------------------------------------
unsigned short ResultSetMetaData::getScale(unsigned short columnIndex)
{
    SQLLEN ret;
    EXEC_STMT(SQLColAttributeA, stmt_->hstmt_, columnIndex,
        SQL_DESC_SCALE, NULL, 0, NULL, &ret);
    return (unsigned short)ret;
}
//------------------------------------------------------------------------------
bool ResultSetMetaData::isAutoIncrement(unsigned short columnIndex)
{
    SQLLEN ret;
    EXEC_STMT(SQLColAttributeA, stmt_->hstmt_, columnIndex,
        SQL_DESC_AUTO_UNIQUE_VALUE, NULL, 0, NULL, &ret);
    return ret == SQL_TRUE;
}
//------------------------------------------------------------------------------
bool ResultSetMetaData::isCaseSensitive(unsigned short columnIndex)
{
    SQLLEN ret;
    EXEC_STMT(SQLColAttributeA, stmt_->hstmt_, columnIndex,
        SQL_DESC_CASE_SENSITIVE, NULL, 0, NULL, &ret);
    return ret == SQL_TRUE;
}
//------------------------------------------------------------------------------
bool ResultSetMetaData::isNamed(unsigned int columnIndex)
{
  SQLLEN ret;
  EXEC_STMT(SQLColAttributeA, stmt_->hstmt_, columnIndex,
    SQL_DESC_UNNAMED, NULL, 0, NULL, &ret);
  return ret == SQL_NAMED;
}
//------------------------------------------------------------------------------
bool ResultSetMetaData::isNullable(unsigned short columnIndex)
{
    SQLLEN ret;
    EXEC_STMT(SQLColAttributeA, stmt_->hstmt_, columnIndex,
        SQL_DESC_NULLABLE, NULL, 0, NULL, &ret);
    return ret == SQL_NULLABLE;
}
//------------------------------------------------------------------------------
bool ResultSetMetaData::isReadOnly(unsigned int columnIndex)
{
  SQLLEN ret;
  EXEC_STMT(SQLColAttributeA, stmt_->hstmt_, columnIndex,
    SQL_DESC_UPDATABLE, NULL, 0, NULL, &ret);
  return ret == SQL_ATTR_READONLY;
}
//------------------------------------------------------------------------------
bool ResultSetMetaData::isSearchable(unsigned short columnIndex)
{
    SQLLEN ret;
    EXEC_STMT(SQLColAttributeA, stmt_->hstmt_, columnIndex,
        SQL_DESC_SEARCHABLE, NULL, 0, NULL, &ret);
    return ret == SQL_PRED_SEARCHABLE;
}
//------------------------------------------------------------------------------
bool ResultSetMetaData::isSigned(unsigned int columnIndex)
{
    SQLLEN ret;
    EXEC_STMT(SQLColAttributeA, stmt_->hstmt_, columnIndex,
        SQL_DESC_UNSIGNED, NULL, 0, NULL, &ret);
    return ret == SQL_FALSE;
}
//------------------------------------------------------------------------------
string ResultSetMetaData::getStringColAttribute(unsigned short columnIndex,
    unsigned short field)
{
    vector<char> buffer;
    buffer.resize(256);
    while (true)
    {
        SQLPOINTER ptr = &buffer[0];
        SQLSMALLINT bufLen = (SQLSMALLINT)buffer.size();
        SQLSMALLINT dataLen;
        EXEC_STMT(SQLColAttributeA, stmt_->hstmt_, columnIndex, field, ptr,
            bufLen, &dataLen, NULL);
        if (dataLen < bufLen)
            break;
        buffer.resize(dataLen + 1);
    }
    return string(&buffer[0]);
}
//------------------------------------------------------------------------------
} // namespace odbc
