#ifndef ODBC_DATABASEMETADATA_H_INCLUDED
#define ODBC_DATABASEMETADATA_H_INCLUDED
//------------------------------------------------------------------------------
#include <string>
#include <odbc/Config.h>
#include <odbc/Forwards.h>
#include <odbc/Types.h>
//------------------------------------------------------------------------------
namespace odbc {
//------------------------------------------------------------------------------
/**
 * Provides information about the database.
 *
 * A DatabaseMetaData object can be created using
 * Connection::getDatabaseMetadata().
 *
 * Most functions provide only a rudementary description of the data that is
 * returned. Refer to the ODBC documentation and your ODBC driver documentation
 * for further details.
 */
class ODBC_EXPORT DatabaseMetaData : public RefCounted
{
    friend class Connection;

private:
    DatabaseMetaData(Connection* parent);

public:
    /**
     * Retrieves a list of columns in specified tables.
     *
     * The list of columns is returned as a ResultSet object, in which each
     * returned row has the following columns:
     *     1. Catalog name
     *     2. Schema name
     *     3. Table name
     *     4. Column name
     *     5. Data type
     *     6. Type name
     *     7. Column size
     *     8. Buffer length
     *     9. Decimal digits
     *     10. Numeric radix
     *     11. Nullability
     *     12. Remarks
     *     13. Default value (ODBC 3.0)
     *     14. SQL data type (ODBC 3.0)
     *     15. Subtype code for date, time and interval data types (ODBC 3.0)
     *     16. Maximum length of bytes in a character or binary column type
     *         (ODBC 3.0)
     *     17. The ordinal position of the column in the table (1-based)
     *         (ODBC 3.0)
     *     18. Nullability as "NO" or "YES" string (ODBC 3.0)
     *
     * This function uses the ODBC function SQLColumns. Refer to its
     * documentation for further details on the data in the ResultSet object.
     *
     * @param catalogName  A string indicating the catalog name.
     * @param schemaName   A string search pattern for schema names.
     * @param tableName    A string search pattern for table names.
     * @param columnName   A string search pattern for column names.
     * @return             Returns a ResultSet object containing the requested
     *                     table description.
     */
    ResultSetRef getColumns(
        const char* catalogName,
        const char* schemaName,
        const char* tableName,
        const char* columnName);

    /**
     * Retrieves a list of columns and associated privileges for the specified
     * table.
     *
     * The list of columns is returned as a ResultSet object, in which each
     * returned row has the following columns:
     *     1. Catalog name
     *     2. Schema name
     *     3. Table name
     *     4. Column name
     *     5. Grantor
     *     6. Grantee
     *     7. Privilege
     *     8. Grantable
     *
     * This function uses the ODBC function SQLColumnPrivileges. Refer to its
     * documentation for further details on the data in the ResultSet object.
     *
     * @param catalogName  A string indicating the catalog name.
     * @param schemaName   A string indicating the schema name.
     * @param tableName    A string indicating the table name.
     * @param columnName   A string search pattern for column names.
     * @return             Returns a ResultSet object containing the requested
     *                     information about privileges.
     */
    ResultSetRef getColumnPrivileges(
        const char* catalogName,
        const char* schemaName,
        const char* tableName,
        const char* columnName);

    /**
     * Retrieves a list of primary keys in the specified table.
     *
     * The list of primary keys is returned as a ResultSet object, in which
     * each returned row has the following columns:
     *     1. Table catalog name
     *     2. Table schema name
     *     3. Table name
     *     4. Primary key column name
     *     5. Primary column sequence number in key (1-based)
     *     6. Primary key name
     *
     * This functions uses the ODBC function SQLPrimaryKeys. Refer to its
     * documentation for further details on the data in the ResultSet object.
     *
     * @param catalogName  A string indicating the catalog name.
     * @param schemaName   A string indicating the schema name.
     * @param tableName    A string indicating the table name.
     * @return             Returns a ResultSet object containing the requested
     *                     table description.
     */
    ResultSetRef getPrimaryKeys(
        const char* catalogName,
        const char* schemaName,
        const char* tableName);

    /**
     * Retrieves a description of the tables that are available in the connected
     * database.
     *
     * The list of tables is returned as a ResultSet object, in which each
     * returned row has the following columns:
     *     1. Catalog name
     *     2. Schema name
     *     3. Table name
     *     4. Table type
     *     5. Remarks
     *
     * This function uses the ODBC function SQLTables. Refer to its
     * documentation for further details on the data in the ResultSet object.
     *
     * @param catalogName  A string indicating the catalog name.
     * @param schemaName   A string search pattern for schema names.
     * @param tableName    A string search pattern for table names.
     * @param tableType    A list of table types to be searched. The list must
     *                     be empty or must contain a list of a comma-separated
     *                     values. These values are "TABLE", "VIEW",
     *                     "SYSTEM TABLE", "GLOBAL TEMPORARY",
     *                     "LOCAL TEMPORARY", "ALIAS", "SYNONYM", or a data
     *                     source-specific type name.
     * @return             Returns a ResultSet object containing the requested
     *                     table description.
     */
    ResultSetRef getTables(
        const char* catalogName,
        const char* schemaName,
        const char* tableName,
        const char* tableType);

    /**
     * Retrieves information about all data types.
     *
     * The information is returned as a ResultSet object, in which each returned
     * row has the following columns:
     *     1. Type name
     *     2. Data type
     *     3. Maximum column size for the data type.
     *     4. Characters used to prefix a literal of that data type.
     *     5. Characters used to suffix a literal of that data type.
     *     6. A list of keywords, separated by commas, corresponding to each
     *        parameter that the application may specify in parentheses when
     *        using the name that is returned in field 1.
     *     7. Nullability of the type.
     *     8. Case-sensitiveness.
     *     9. Searchability.
     *     10. Unsignedness.
     *     11. Flag indicating if the type has a predefined fixed precision and
     *         scale.
     *     12. Auto-incrementing flag.
     *     13. Localized type name.
     *     14. Minimum scale.
     *     15. Maximum scale.
     *     16. SQL data type code.
     *     17. Date/time subcode.
     *     18. The radix used by a numeric type.
     *     19. Interval leading precision.
     *
     * This function uses the ODBC function SQLGetTypeInfo. Refer to its
     * documentation for further details on the data in the ResultSet object.
     *
     * @return      Returns a ResultSet object containing the requested data
     *              type information.
     */
    ResultSetRef getTypeInfo();

    /**
     * Retrieves information about a specific data type.
     *
     * See the documentation of getTypeInfo() for further details.
     *
     * @param type  The data type to retrieve the type information of.
     * @return      Returns a ResultSet object containing the requested data
     *              type information.
     */
    ResultSetRef getTypeInfo(int type);

    /**
     * Retrieves the name of the data source.
     *
     * @return  Returns the name of the data source.
     */
    std::string getDataSourceName();

    /**
     * Retrieves the current database in use.
     *
     * @return  Returns the current database in use.
     */
    std::string getDatabaseName();

    /**
     * Retrieves the name of the DBMS system.
     *
     * @return  Returns the name of the DBMS system.
     */
    std::string getDBMSName();

    /**
     * Retrieves the version of the DBMS system.
     *
     * @return  Returns the version of the DBMS system.
     */
    std::string getDBMSVersion();

    /**
     * Retrieves the name of the ODBC driver.
     *
     * @return  Returns the name of the ODBC driver.
     */
    std::string getDriverName();

    /**
     * Retrieves the version of the ODBC driver.
     *
     * @return  Returns the version of the ODBC driver.
     */
    std::string getDriverVersion();

    /**
     * Retrieves the maximum number of active connections the driver can
     * support.
     *
     * @return  Returns the maximum number of active connections the driver can
     *          support. If there is no limit or if the limit is not known,
     *          the function returns 0.
     */
    unsigned short getMaxConnections();

    /**
     * Retrieves the maximum length of a statement string.
     *
     * @return  Returns the maximum length of a statement string. If there is no
     *          limit or if the limit is not known, the function returns 0.
     */
    unsigned long getMaxStatementLength();

    /**
     * Retrieves the server name.
     *
     * @return  Returns the server name.
     */
    std::string getServerName();

    /**
     * Retrieves the name used in the database.
     *
     * @return  Returns the name used in the database.
     */
    std::string getUserName();

    /**
     * Checks whether the database is in read-only mode.
     *
     * @return  Returns true if the database is in read-only mode, false
     *          otherwise.
     */
    bool isReadOnly();

    /**
     * Checks whether the database supports adding columns to existing tables.
     *
     * @returns  Returns true if the database supports adding columns to
     *           existing tables, false otherwise.
     */
    bool supportsAlterTableWithAddColumn();

    /**
     * Checks whether the database supports dropping columns from existing
     * tables.
     *
     * @returns  Returns true if the database supports dropping columns to
     *           existing tables, false otherwise.
     */
    bool supportsAlterTableWithDropColumn();

    /**
     * Retrieves the default transaction isolation level of the database.
     *
     * @return   Returns the default transaction isolation level.
     */
    TransactionIsolationLevel getDefaultTransactionIsolation();

    /**
     * Checks whether the database supports the given transaction isolation
     * level or not.
     *
     * @return   Returns true if the given transaction isolation level is
     *           supported, otherwise false.
     */
    bool supportsTransactionIsolation(TransactionIsolationLevel level);

private:
    std::string getStringTypeInfo(unsigned short typeInfo);
    unsigned long getUIntTypeInfo(unsigned short typeInfo);
    unsigned short getUSmallIntTypeInfo(unsigned short typeInfo);

private:
    ConnectionRef parent_;
};
//------------------------------------------------------------------------------
} // namespace odbc
//------------------------------------------------------------------------------
#endif
