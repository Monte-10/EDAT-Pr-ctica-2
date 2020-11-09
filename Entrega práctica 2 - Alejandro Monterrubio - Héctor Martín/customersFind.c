#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"


int CustomersFind(char* name) {
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret; /* ODBC API return status */
    SQLCHAR customername[512], contactfirstname[512], contactlastname[512];
    SQLINTEGER customernumber;
    char aux[512];


    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    SQLPrepare(stmt, (SQLCHAR*) "select customername, contactfirstname, contactlastname, customernumber from customers where contactfirstname like ? or contactlastname like ? order by customernumber", SQL_NTS); /*Query*/
    snprintf(aux, 512, "%c%s%c", '%', name, '%');

    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, aux, 0, NULL);
    SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, aux, 0, NULL);

    SQLExecute(stmt);

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_C_CHAR, customername, sizeof(customername), NULL);
        ret = SQLGetData(stmt, 2, SQL_C_CHAR, contactfirstname, sizeof(contactfirstname), NULL);
        ret = SQLGetData(stmt, 3, SQL_C_CHAR, contactlastname, sizeof(contactlastname), NULL);
        ret = SQLGetData(stmt, 4, SQL_C_SLONG, &customernumber, sizeof(SQLINTEGER), NULL);
        printf("%s %s %s %d\n", customername, contactfirstname, contactlastname, customernumber);
    }

    SQLCloseCursor(stmt);

    /* free up statement handle */
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}