#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"


int CustomersListProducts(int number) {
    SQLHENV env = 0;
    SQLHDBC dbc = 0;
    SQLHSTMT stmt = 0;
    SQLRETURN ret = 0; /* ODBC API return status */
    SQLINTEGER total = 0;
    char productname[512] = "";


    /* CONNECT */
    ret = (SQLRETURN)odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    (void) SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    (void) SQLPrepare(stmt, (SQLCHAR*) "select productname, sum(quantityordered) as total from orders, orderdetails, products where orders.customernumber = ? and orderdetails.ordernumber=orders.ordernumber and orderdetails.productcode=products.productcode group by products.productcode", SQL_NTS);

    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &number, 0, NULL);
    
    (void) SQLExecute(stmt);

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_C_CHAR, productname, (SQLLEN)sizeof(productname), NULL);
        ret = SQLGetData(stmt, 2, SQL_C_SLONG, &total, (SQLLEN)sizeof(SQLINTEGER), NULL);
        printf("%s %d\n", productname, total);
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