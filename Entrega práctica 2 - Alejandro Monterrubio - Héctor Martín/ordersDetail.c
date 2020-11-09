#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"


int OrdersDetail(int number) {
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt, stmt2;
    SQLRETURN ret; /* ODBC API return status */
    SQLINTEGER quantityordered;
    SQLDOUBLE coste, priceeach;
    SQLCHAR orderdate[512], status[512], productcode[512];


    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    SQLPrepare(stmt, (SQLCHAR*) "select orderdate, status, sum(quantityordered*priceeach) as coste from orders natural join orderdetails where ordernumber = ? group by ordernumber", SQL_NTS); /*Query*/
    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &number, 0, NULL);
    SQLExecute(stmt);

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_C_CHAR, orderdate, sizeof(orderdate), NULL);
        ret = SQLGetData(stmt, 2, SQL_C_CHAR, status, sizeof(status), NULL);
        ret = SQLGetData(stmt, 3, SQL_C_DOUBLE, &coste, sizeof(coste), NULL);
        printf("Orderdate > %s - Status > %s\n Coste > %.2f\n", orderdate, status, coste);
    }

    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt2);

    SQLPrepare(stmt2, (SQLCHAR*) "select productcode, quantityordered, priceeach from orderdetails where ordernumber = ? order by orderlinenumber", SQL_NTS); /*Query*/
    SQLBindParameter(stmt2, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &number, 0, NULL);
        
    SQLExecute(stmt2);

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt2))) {
        ret = SQLGetData(stmt2, 1, SQL_C_CHAR, productcode, sizeof(productcode), NULL);
        ret = SQLGetData(stmt2, 2, SQL_C_SLONG, &quantityordered, sizeof(SQLINTEGER), NULL);
        ret = SQLGetData(stmt2, 3, SQL_C_DOUBLE, &priceeach, sizeof(SQLINTEGER), NULL);
        printf("%s %d %.2f\n", productcode, quantityordered, priceeach);
    }

    SQLCloseCursor(stmt);
    SQLCloseCursor(stmt2);

    /* free up statement handle */
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt2);

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}