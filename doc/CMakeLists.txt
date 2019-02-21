FIND_PACKAGE(Doxygen)
IF (DOXYGEN_FOUND)

    GET_PROPERTY(ph TARGET odbccpp PROPERTY public_headers)
    LIST(TRANSFORM ph PREPEND src/odbc/)
    
    SET(DOXYGEN_STRIP_FROM_PATH "src")
    SET(DOXYGEN_STRIP_FROM_INC_PATH "src")
    DOXYGEN_ADD_DOCS(doc
        doc/overview.dox
        ${ph}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
ENDIF()
