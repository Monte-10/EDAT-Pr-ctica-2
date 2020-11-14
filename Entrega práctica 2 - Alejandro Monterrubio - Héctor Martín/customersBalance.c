#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"


int CustomersBalance(int number) {
    SQLHENV env = 0;
    SQLHDBC dbc = 0;
    SQLHSTMT stmt = 0, stmt2 = 0;
    SQLRETURN ret = 0; /* ODBC API return status */
    double saldo = 0, sum = 0;


    /* CONNECT */
    ret = (SQLRETURN)odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    (void) SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    (void) SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt2);


    (void) SQLPrepare(stmt, (SQLCHAR*) "select sum(amount) as saldo from payments where payments.customernumber = ?", SQL_NTS);
    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &number, 0, NULL);
    (void) SQLExecute(stmt);
    
    (void) SQLPrepare(stmt2, (SQLCHAR*) "select sum(priceeach*quantityordered) as sum from orders, orderdetails where orders.customernumber= ? and orders.ordernumber=orderdetails.ordernumber", SQL_NTS);
    (void) SQLBindParameter(stmt2, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &number, 0, NULL);
    (void) SQLExecute(stmt2);

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt)) && SQL_SUCCEEDED(ret = SQLFetch(stmt2))) {
        ret = SQLGetData(stmt, 1, SQL_C_DOUBLE, &saldo, (SQLLEN)sizeof(saldo), NULL);
        ret = SQLGetData(stmt2, 1, SQL_C_DOUBLE, &sum, (SQLLEN)sizeof(sum), NULL);
        printf("Balance = %.2f\n", saldo-sum);
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