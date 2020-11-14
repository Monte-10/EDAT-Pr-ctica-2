#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"


int OrdersDetail(int number) {
    SQLHENV env = 0;
    SQLHDBC dbc = 0;
    SQLHSTMT stmt = 0, stmt2 = 0;
    SQLRETURN ret = 0; /* ODBC API return status */
    SQLINTEGER quantityordered = 0;
    SQLDOUBLE coste = 0, priceeach = 0;
    char orderdate[512] = "", status[512] = "", productcode[512] = "";


    /* CONNECT */
    ret = (SQLRETURN)odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    (void) SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    (void) SQLPrepare(stmt, (SQLCHAR*) "select orderdate, status, sum(quantityordered*priceeach) as coste from orders natural join orderdetails where ordernumber = ? group by ordernumber", SQL_NTS); /*Query*/
    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &number, 0, NULL);
    (void) SQLExecute(stmt);

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_C_CHAR, orderdate, (SQLLEN)sizeof(orderdate), NULL);
        ret = SQLGetData(stmt, 2, SQL_C_CHAR, status, (SQLLEN)sizeof(status), NULL);
        ret = SQLGetData(stmt, 3, SQL_C_DOUBLE, &coste, (SQLLEN)sizeof(coste), NULL);
        printf("Orderdate > %s - Status > %s\n Coste > %.2f\n", orderdate, status, coste);
    }

    (void) SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt2);

    (void) SQLPrepare(stmt2, (SQLCHAR*) "select productcode, quantityordered, priceeach from orderdetails where ordernumber = ? order by orderlinenumber", SQL_NTS); /*Query*/
    (void) SQLBindParameter(stmt2, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &number, 0, NULL);
        
    (void) SQLExecute(stmt2);

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt2))) {
        ret = SQLGetData(stmt2, 1, SQL_C_CHAR, productcode, (SQLLEN)sizeof(productcode), NULL);
        ret = SQLGetData(stmt2, 2, SQL_C_SLONG, &quantityordered, (SQLLEN)sizeof(SQLINTEGER), NULL);
        ret = SQLGetData(stmt2, 3, SQL_C_DOUBLE, &priceeach, (SQLLEN)sizeof(SQLINTEGER), NULL);
        printf("%s %d %.2f\n", productcode, quantityordered, priceeach);
    }

    (void) SQLCloseCursor(stmt);
    (void) SQLCloseCursor(stmt2);

    /* free up statement handle */
    (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt2);

    /* DISCONNECT */
    ret = (SQLRETURN)odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}