#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"


int OrdersRange(char* date1, char* date2) {
    SQLHENV env = 0;
    SQLHDBC dbc = 0;
    SQLHSTMT stmt = 0;
    SQLRETURN ret = 0; /* ODBC API return status */
    SQLINTEGER ordernumber = 0;
    char orderdate[512] = "", shippeddate[512] = "";

    /* CONNECT */
    ret = (SQLRETURN)odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    (void) SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    (void) SQLPrepare(stmt, (SQLCHAR*) "select ordernumber, orderdate, shippeddate from orders where orderdate >= ? and orderdate <= ? order by ordernumber asc", SQL_NTS); /*Query*/

    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, date1, 0, NULL);
    (void) SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, date2, 0, NULL);
        
    (void) SQLExecute(stmt);

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_C_SLONG, &ordernumber, (SQLLEN)sizeof(SQLINTEGER), NULL);
        ret = SQLGetData(stmt, 2, SQL_C_CHAR, orderdate, (SQLLEN)sizeof(orderdate), NULL);
        ret = SQLGetData(stmt, 3, SQL_C_CHAR, shippeddate, (SQLLEN)sizeof(shippeddate), NULL);
        printf("%d %s %s\n", ordernumber, orderdate, shippeddate);
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