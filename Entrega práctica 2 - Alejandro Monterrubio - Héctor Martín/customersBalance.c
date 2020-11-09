#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"


int CustomersBalance(int number) {
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt, stmt2;
    SQLRETURN ret; /* ODBC API return status */
    SQLINTEGER saldo;
    SQLDOUBLE sum;


    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    SQLPrepare(stmt, (SQLCHAR) "select sum(amount) from payments where payments.customernumber = ?", SQL_NTS);

    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &number, 0, NULL);

    SQLExecute(stmt);

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_C_SLONG, &number, sizeof(SQLINTEGER), NULL);
        ret = SQLGetData(stmt, 2, SQL_C_SLONG, &saldo, sizeof(SQLINTEGER), NULL);
        printf("%d %d\n", number, saldo);
    }

    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt2);

    SQLPrepare(stmt2, (SQLCHAR) "select sum(priceeach*quantityordered) as sum from orders, orderdetails where orders.customernumber= ? and orders.ordernumber=orderdetails.ordernumber", SQL_NTS);

    SQLBindParameter(stmt2, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &number, 0, NULL);

    SQLExecute(stmt2);

    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_C_DOUBLE, &sum, sizeof(sum), NULL);
        printf("%.2f\n", sum);
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