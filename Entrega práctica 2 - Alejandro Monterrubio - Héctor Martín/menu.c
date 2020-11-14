#include <stdio.h>
#include <stdlib.h>

#include "productsStock.c"
#include "productsFind.c"
#include "ordersDetail.c"
#include "ordersRange.c"
#include "ordersOpen.c"
#include "customersListProducts.c"
#include "customersFind.c"
#include "customersBalance.c"


static int ShowMainMenu();
static void ShowProductsMenu();
static int ShowProductsSubMenu();
static void ShowOrdersMenu();
static int ShowOrdersSubMenu();
static void ShowCustomersMenu();
static int ShowCustomersSubMenu();

/* #endif //A2BIS_MENU_TEMPLATE_H */

/**
 * @file menu.c
 * @author Alejandro Monterrubio y Héctor Martín
 * @date 9 de Noviembre 2020
 * @brief El archivo tiene el menú y los submenús
 *
 * El programa nos deja elegir entre Products, Orders,
 * Customers o Exit y según avanzamos se abren distintos
 * submenús con los que podemos elegir lo que queremos seleccionar
 * y probar las querys.
 * 
 */


/**
 * @brief Enseña y procesa el menú principal
 *
 * La función es un bucle que se repite
 * hasta elegir un case.
 *
 * @return 0 si no hay error
 * @author Alejandro Monterrubio y Héctor Martín
 */
int main(void) {
    int nChoice = 0;

    do {
        nChoice = ShowMainMenu();
        switch (nChoice) {
            case 1: {
                ShowProductsMenu();
            }
                break;

            case 2: {
                ShowOrdersMenu();
            }
                break;

            case 3: {
                ShowCustomersMenu();
            }
                break;

            case 4: {
                printf("Bye bye\n\n");
            }
                break;
        }
    } while (nChoice != 4);
    return 0;
}


/**
 * @brief Printea el menú principal y las distintas opciones.
 *
 *
 * @return Entrada al la opción seleccionada
 * @author Alejandro Monterrubio y Héctor Martín
 */

int ShowMainMenu() {
    int nSelected = 0;
    char buf[16]; /* buffer for reading user input
                     in theory we only need 1 char
                     but a large buffer will handle
                     more gracefully wrong inputs
                     as 'qwerty' */

    do {
        printf(" (1) Products\n"
               " (2) Orders\n"
               " (3) Customers\n"
               " (4) Quit\n\n"
               "Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 4)) {
            printf("Has introducido un valor no válido. Prueba otra vez\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}

/**
 * @brief Enseña y procesa el submenú de Products.
 *
 * Es una función que crea un bucle
 * hasta que se selecciona pulsando
 * un número una de las elecciones
 * dadas.
 * @return Opción seleccionada
 * @author Alejandro Monterrubio y Héctor Martín
 */

void ShowProductsMenu() {
    int nChoice = 0;
    char productcode[512];
    char productname[512];
    char buf[16];

    do {
        nChoice = ShowProductsSubMenu();
        switch (nChoice) {

            case 1: {
                printf ("Introduce productcode > ");
                (void) scanf ("%s",productcode);
                (void) fgets(buf, 16, stdin);
                (void) ProductsStock(productcode);
            }
                break;

            case 2: {
                printf ("Introduce productname > ");
                (void) scanf ("%s",productname);
                (void) fgets(buf, 16, stdin);
                (void) ProductsFind(productname);
            }
                break;

            case 3:
                break;
        }
    } while (nChoice != 3);

}

/**
 * @brief Imprime el submenu de Products.
 *
 * @return Opción seleccionada
 * @author Alejandro Monterrubio y Héctor Martín
 */

 int ShowProductsSubMenu() {
    int nSelected = 0;
    char buf[16];

    do {

        printf(" (1) Stock\n"
               " (2) Find\n"
               " (3) Back\n");

        printf("Introduce un número que corresponda con tu elección > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");


        if ((nSelected < 1) || (nSelected > 3)) {
            printf("Has introducido un valor no válido. Prueba otra vez\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 3));

    return nSelected;
}

/**
 * @brief Enseña y procesa el menú de Orders.
 *
 * Es una función que crea un bucle
 * hasta que se selecciona pulsando
 * un número una de las elecciones
 * dadas.
 *
 * @return Opción seleccionada
 * @author Alejandro Monterrubio y Héctor Martín
 */

void ShowOrdersMenu() {
    int nChoice = 0;
    char date1[512];
    char date2[512];
    int ordernumber;
    char buf[16];
    do {
        nChoice = ShowOrdersSubMenu();
        switch (nChoice) {
            case 1: {
                (void) OrdersOpen();
            }
                break;

            case 2: {
                printf("Introduce dos fechas con el siguiente formato (YYYY-MM-DD - YYYY-MM-DD) > ");
                (void) scanf ("%s - %s", date1, date2);
                (void) fgets(buf, 16, stdin);
                (void) OrdersRange(date1,date2);
            }
                break;

            case 3: {
                printf("Introduce un ordernumber > ");
                (void) scanf ("%d", &ordernumber);
                (void) fgets(buf, 16, stdin);
                (void) OrdersDetail(ordernumber);
            }
                break;

            case 4:
                break;
        }
    } while (nChoice != 4);

}

/**
 * @brief prints the fairy menu and allows to select an option.
 *
 * @return selected option
 * @author rmarabini
 */
 int ShowOrdersSubMenu() {
    int nSelected = 0;
    char buf[16];

    do {

        printf(" (1) Open\n"
               " (2) Range\n"
               " (3) Detail\n"
               " (4) Back\n\n");

        printf("Introduce un número que corresponda con tu elección > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 4)) {
            printf("Has introducido un valor no válido. Prueba otra vez\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}

/*
 * @brief Enseña y procesa el submenú de Customers.
 *
 * Es una función que crea un bucle
 * hasta que se selecciona pulsando
 * un número una de las elecciones
 * dadas.
 * @return Opción seleccionada
 * @author Alejandro Monterrubio y Héctor Martín
 * */

void ShowCustomersMenu() {
    int nChoice = 0;
    int customernumber;
    char name[512];
    char buf[16];
    do {
        nChoice = ShowCustomersSubMenu();
        switch (nChoice) {

            case 1: {
                printf("Introduce un nombre > ");
                (void) scanf ("%s", name);
                (void) fgets(buf, 16, stdin);
                (void) CustomersFind(name);
            }
                break;

            case 2: {
                printf("Introduce customernumber > ");
                (void) scanf ("%d", &customernumber);
                (void) fgets(buf, 16, stdin);
                (void) CustomersListProducts(customernumber);
            }
                break;

            case 3: {
                printf("Introduce customernumber > ");
                (void) scanf ("%d", &customernumber);
                (void) fgets(buf, 16, stdin);
                (void) CustomersBalance(customernumber);
            }
                break;

            case 4:
                break;
        }
    } while (nChoice != 4);

}

/**
 * @brief Imprime el submenu de Customers.
 *
 * @return Opción seleccionada
 * @author Alejandro Monterrubio y Héctor Martín
 */

 int ShowCustomersSubMenu() {
    int nSelected = 0;
    char buf[16];

    do {

        printf(" (1) Find\n"
               " (2) List Products\n"
               " (3) Balance\n"
               " (4) Back\n");

        printf("Introduce un número que corresponda con tu elección > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");


        if ((nSelected < 1) || (nSelected > 4)) {
            printf("Has introducido un valor no válido. Prueba otra vez\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}
