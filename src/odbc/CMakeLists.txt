# Public Header Files
SET(public_headers
    Config.h
    Connection.h
    DatabaseMetaData.h
    DatabaseMetaDataBase.h
    DatabaseMetaDataUnicode.h
    Environment.h
    Exception.h
    Forwards.h
    ParameterMetaData.h
    PreparedStatement.h
    RefCounted.h
    ResultSet.h
    ResultSetMetaData.h
    ResultSetMetaDataBase.h
    ResultSetMetaDataUnicode.h
    Statement.h
    StatementBase.h
    StringConverter.h
    Types.h
    Util.h
)

# Sources
SET(odbccpp_sources
    Connection.cpp
    DatabaseMetaData.cpp
    DatabaseMetaDataBase.cpp
    DatabaseMetaDataUnicode.cpp
    Environment.cpp
    Exception.cpp
    ParameterMetaData.cpp
    PreparedStatement.cpp
    RefCounted.cpp
    ResultSet.cpp
    ResultSetMetaData.cpp
    ResultSetMetaDataBase.cpp
    ResultSetMetaDataUnicode.cpp
    Statement.cpp
    StatementBase.cpp
    StringConverter.cpp
    Types.cpp
    Util.cpp
    internal/Batch.cpp
    internal/ParameterData.cpp
    internal/UtilInternal.cpp
)

ADD_LIBRARY(odbccpp ${odbccpp_sources})

TARGET_LINK_LIBRARIES(odbccpp
    PUBLIC
        ODBC::ODBC
)

IF(BUILD_SHARED_LIBS)
    TARGET_COMPILE_DEFINITIONS(odbccpp
        PRIVATE
            ODBC_EXPORTS
    )
ELSE()
    TARGET_COMPILE_DEFINITIONS(odbccpp
        PUBLIC
            ODBC_STATIC
    )
    SET_PROPERTY(TARGET odbccpp PROPERTY POSITION_INDEPENDENT_CODE ON)
ENDIF()

SET_PROPERTY(TARGET odbccpp PROPERTY public_headers ${public_headers})

# Installation
INSTALL(TARGETS odbccpp)
INSTALL(FILES ${public_headers} DESTINATION include/odbc)
