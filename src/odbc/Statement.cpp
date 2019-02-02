#include <odbc/Exception.h>
#include <odbc/ResultSet.h>
#include <odbc/Statement.h>
#include <odbc/internal/Macros.h>
#include <odbc/internal/Odbc.h>
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
namespace odbc {
//------------------------------------------------------------------------------
Statement::Statement(Connection* parent) : StatementBase(parent)
{
}
//------------------------------------------------------------------------------
void Statement::setHandle(void* hstmt)
{
    hstmt_ = hstmt;
}
//------------------------------------------------------------------------------
void Statement::execute(const char* sql)
{
    EXEC_STMT(SQLExecDirectA, hstmt_, (SQLCHAR*)sql, SQL_NTS);
}
//------------------------------------------------------------------------------
ResultSetRef Statement::executeQuery(const char* sql)
{
    ResultSetRef ret(new ResultSet(this));
    EXEC_STMT(SQLFreeStmt, hstmt_, SQL_CLOSE);
    EXEC_STMT(SQLExecDirectA, hstmt_, (SQLCHAR*)sql, SQL_NTS);
    return ret;
}
//------------------------------------------------------------------------------
} // namespace odbc
