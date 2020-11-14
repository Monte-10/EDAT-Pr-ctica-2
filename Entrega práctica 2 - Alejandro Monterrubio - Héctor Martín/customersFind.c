#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"


int CustomersFind(char* name) {
    SQLHENV env = 0;
    SQLHDBC dbc = 0;
    SQLHSTMT stmt = 0;
    SQLRETURN ret = 0; /* ODBC API return status */
    SQLINTEGER customernumber = 0;
    char aux[512] = "", customername[512] = "", contactfirstname[512] = "", contactlastname[512] = "";


    /* CONNECT */
    ret = (SQLRETURN)odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    (void) SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    (void) SQLPrepare(stmt, (SQLCHAR*) "select customername, contactfirstname, contactlastname, customernumber from customers where contactfirstname like ? or contactlastname like ? order by customernumber", SQL_NTS); /*Query*/
    (void) snprintf(aux, 512, "%c%s%c", '%', name, '%');

    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, aux, 0, NULL);
    (void) SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, aux, 0, NULL);

    (void) SQLExecute(stmt);

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_C_CHAR, customername, (SQLLEN)sizeof(customername), NULL);
        ret = SQLGetData(stmt, 2, SQL_C_CHAR, contactfirstname, (SQLLEN)sizeof(contactfirstname), NULL);
        ret = SQLGetData(stmt, 3, SQL_C_CHAR, contactlastname, (SQLLEN)sizeof(contactlastname), NULL);
        ret = SQLGetData(stmt, 4, SQL_C_SLONG, &customernumber, (SQLLEN)sizeof(SQLINTEGER), NULL);
        printf("%s %s %s %d\n", customername, contactfirstname, contactlastname, customernumber);
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