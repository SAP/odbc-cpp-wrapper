#ifndef ODBC_INTERNAL_MACROS_H_INCLUDED
#define ODBC_INTERNAL_MACROS_H_INCLUDED
//------------------------------------------------------------------------------
#include <sstream>
//------------------------------------------------------------------------------
#define EXEC_DBC(function, handle, ...)                                        \
    do {                                                                       \
        SQLRETURN rc = function(handle, ##__VA_ARGS__);                        \
        ::NS_ODBC::Exception::checkForError(rc, SQL_HANDLE_DBC, handle);       \
    } while (false)
//------------------------------------------------------------------------------
#define EXEC_ENV(function, handle, ...)                                        \
    do {                                                                       \
        SQLRETURN rc = function(handle, ##__VA_ARGS__);                        \
        ::NS_ODBC::Exception::checkForError(rc, SQL_HANDLE_ENV, handle);       \
    } while (false)

//------------------------------------------------------------------------------
#define EXEC_STMT(function, handle, ...)                                       \
    do {                                                                       \
        SQLRETURN rc = function(handle, ##__VA_ARGS__);                        \
        ::NS_ODBC::Exception::checkForError(rc, SQL_HANDLE_STMT, handle);      \
    } while (false)
//------------------------------------------------------------------------------
#define ODBC_FAIL(msg)                                                         \
    do {                                                                       \
        ::std::ostringstream out;                                              \
        out << msg;                                                            \
        throw ::NS_ODBC::Exception(out.str());                                 \
    } while (false)
//------------------------------------------------------------------------------
#define ODBC_CHECK(condition, msg)                                             \
    do {                                                                       \
        if (!(condition)) {                                                    \
            ::std::ostringstream out;                                          \
            out << msg;                                                        \
            throw ::NS_ODBC::Exception(out.str());                             \
        }                                                                      \
    } while (false)
//------------------------------------------------------------------------------
#define ODBC_TERMINATE(msg) std::terminate()
//------------------------------------------------------------------------------
#define ODBC_TERMINATE_CHECK(cond, expr)                                       \
    do                                                                         \
    {                                                                          \
        if (!(cond))                                                           \
        {                                                                      \
            ODBC_TERMINATE(expr << "; Condition '" << #cond << "' failed.");   \
        }                                                                      \
    } while (false)
//------------------------------------------------------------------------------
#define ODBC_TERMINATE_CHECK_0(cond)                                           \
    do                                                                         \
    {                                                                          \
        if (!(cond))                                                           \
        {                                                                      \
            ODBC_TERMINATE("Condition '" << #cond << "' failed.");             \
        }                                                                      \
    } while (false)
//------------------------------------------------------------------------------
// Asserts are executed in debug mode only
#ifdef ODBC_DBG
#define ODBC_ASSERT(cond, expr) ODBC_TERMINATE_CHECK(cond, expr)
#define ODBC_ASSERT_0(cond) ODBC_TERMINATE_CHECK_0(cond)
#else
#define ODBC_ASSERT(cond, expr)
#define ODBC_ASSERT_0(cond)
#endif
//------------------------------------------------------------------------------
#if defined(__GNUC__) || defined(__clang__)
#define ODBC_BUILTIN_UNREACHABLE __builtin_unreachable()
#elif defined(_MSC_VER)
#define ODBC_BUILTIN_UNREACHABLE __assume(0)
#endif
//------------------------------------------------------------------------------
// Use these macros at code locations that should be unreachable.
#define ODBC_TERMINATE_CHECK_UNREACHABLE                                       \
    ODBC_TERMINATE("Reached unreachable code location")
//------------------------------------------------------------------------------
// Use this macro at code locations that are unreachable.
#ifdef ODBC_DBG
#define ODBC_ASSERT_UNREACHABLE ODBC_TERMINATE_CHECK_UNREACHABLE
#else
#define ODBC_ASSERT_UNREACHABLE ODBC_BUILTIN_UNREACHABLE
#endif
//------------------------------------------------------------------------------
#define IS_FLAG_SET(value, flag) (((value & flag) == flag))
//------------------------------------------------------------------------------
#endif
