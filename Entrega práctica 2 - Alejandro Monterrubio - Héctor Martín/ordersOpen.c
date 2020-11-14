#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"


int OrdersOpen() {
    SQLHENV env = 0;
    SQLHDBC dbc = 0;
    SQLHSTMT stmt = 0;
    SQLRETURN ret = 0; /* ODBC API return status */
    long ordernumber=0;


    /* CONNECT */
    ret = (SQLRETURN)odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    (void) SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    (void) SQLExecDirect(stmt, (SQLCHAR*) "select ordernumber from orders where shippeddate is NULL ORDER BY ordernumber asc", SQL_NTS); /*Query*/

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_C_SLONG, &ordernumber, (SQLLEN)sizeof(ordernumber), NULL);
        printf("%ld\n", ordernumber);
    }

    (void) SQLCloseCursor(stmt);

    /* free up statement handle */
    (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    /* DISCONNECT */
    ret = (SQLRETURN)odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}