// Crear una aplicación cliente en lenguaje C en Linux para ejecutar los procedimientos de inserción, modificación, borrado y consulta de las tablas. 
// (Puede reutilizar el código de la Actividad-1).
#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
#include "cliente2.h"
#include "AccesoMySQL.h" 

// Helper: devuelve NULL si vacío, o 'valor' si texto
const char* AdaptarSQL(const char *input, int esString) {
    static char bufs[8][256];
    static int idx = 0;
    char *buffer = bufs[(idx++) & 7];
    if (input == NULL || input[0] == '\0') return "NULL";
    if (esString)
        snprintf(buffer, sizeof(bufs[0]), "'%s'", input);
    else
        snprintf(buffer, sizeof(bufs[0]), "%s", input);
    return buffer;
}

void menuEntidad(MYSQL *conn) {
    char entidad[10], accion[3];
    char query[1024];

    while (1) {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1 - Cliente\n");
        printf("2 - Factura\n");
        printf("3 - Producto\n");
        printf("4 - Detalle\n");
        printf("5 - Pago\n");
        printf("0 - Salir\n");
        printf("Elija una entidad: ");
        fgets(entidad, sizeof(entidad), stdin);
        entidad[strcspn(entidad, "\n")] = '\0';
        fflush(stdin);

        if (strcmp(entidad, "0") == 0) break;

        printf("Accion (A=Alta, B=Baja, M=Modificacion, S=Seleccion): ");
        fgets(accion, sizeof(accion), stdin);
        accion[strcspn(accion, "\n")] = '\0';
        fflush(stdin);

        // <<<<<<< CLIENTE >>>>>>>
        if (strcmp(entidad, "1") == 0) {
            char id[20] = "";
            char nombre[50] = "";
            char apellido[50] = "";

            if (strcmp(accion, "A") == 0) { // ALTA
                printf("Nombre: "); 
                fgets(nombre, sizeof(nombre), stdin); 
                nombre[strcspn(nombre, "\n")] = '\0';
                fflush(stdin);
                printf("Apellido: "); 
                fgets(apellido, sizeof(apellido), stdin); 
                apellido[strcspn(apellido, "\n")] = '\0';
                fflush(stdin);
                snprintf(query, sizeof(query),
                    "CALL ABMS_CLIENTE('%s', NULL, %s, %s);",
                    accion,
                    AdaptarSQL(nombre, 1),
                    AdaptarSQL(apellido, 1));
            }
            else if (strcmp(accion, "B") == 0) { // BAJA
                printf("ID: "); 
                fgets(id, sizeof(id), stdin); 
                id[strcspn(id, "\n")] = '\0';
                snprintf(query, sizeof(query),
                    "CALL ABMS_CLIENTE('%s', %s, NULL, NULL);",
                    accion,
                    AdaptarSQL(id, 0));
            }
            else if (strcmp(accion, "M") == 0) { // MODIFICAR
                printf("ID: "); 
                fgets(id, sizeof(id), stdin); 
                id[strcspn(id, "\n")] = '\0';
                fflush(stdin);
                printf("Nombre (presione enter para saltear): "); 
                fgets(nombre, sizeof(nombre), stdin); 
                nombre[strcspn(nombre, "\n")] = '\0';
                fflush(stdin);
                printf("Apellido (presione enter para saltear): "); 
                fgets(apellido, sizeof(apellido), stdin); 
                apellido[strcspn(apellido, "\n")] = '\0';
                fflush(stdin);
                snprintf(query, sizeof(query),
                    "CALL ABMS_CLIENTE('%s', %s, %s, %s);",
                    accion,
                    AdaptarSQL(id, 0),
                    AdaptarSQL(nombre, 1),
                    AdaptarSQL(apellido, 1));
            }
            else if (strcmp(accion, "S") == 0) { // SELECIONAR
                printf("ID (vacío = todos): "); 
                fgets(id, sizeof(id), stdin); 
                id[strcspn(id, "\n")] = '\0';
                fflush(stdin);
                snprintf(query, sizeof(query),
                    "CALL ABMS_CLIENTE('%s', %s, NULL, NULL);",
                    accion,
                    AdaptarSQL(id, 0));
            }
            else {
                printf("Accion invalida.\n");
                continue;
            }

            printf("\nEjecutando: %s\n", query);
            querySQL(conn, query);
        }


        // <<<<<<< FACTURA  >>>>>>>
        if (strcmp(entidad, "2") == 0) {
            char nro[20] = "";
            char id_cliente[50] = "";
            char fecha[50] = "";

            if (strcmp(accion, "A") == 0) { // ALTA
                printf("id_cliente: "); 
                fgets(id_cliente, sizeof(id_cliente), stdin); 
                id_cliente[strcspn(id_cliente, "\n")] = '\0';
                fflush(stdin);
                printf("u_fecha: "); fgets(fecha, sizeof(fecha), stdin); 
                fecha[strcspn(fecha, "\n")] = '\0';
                fflush(stdin);
                snprintf(query, sizeof(query),
                    "CALL ABMS_FACTURA('%s', NULL, %s, %s);",
                    accion,
                    AdaptarSQL(id_cliente, 0),
                    AdaptarSQL(fecha, 0));
            }
            else if (strcmp(accion, "B") == 0) { // BAJA
                printf("nro: "); 
                fgets(nro, sizeof(nro), stdin); 
                nro[strcspn(nro, "\n")] = '\0';
                fflush(stdin);
                snprintf(query, sizeof(query),
                    "CALL ABMS_FACTURA('%s', %s, NULL, NULL);",
                    accion,
                    AdaptarSQL(nro, 0));
            }
            else if (strcmp(accion, "M") == 0) { // MODIFICAR
                printf("numero de factura a modificar: "); 
                fgets(nro, sizeof(nro), stdin); 
                nro[strcspn(nro, "\n")] = '\0';
                fflush(stdin);
                printf("id_cliente nuevo(presione enter para saltear): "); 
                fgets(id_cliente, sizeof(id_cliente), stdin); 
                id_cliente[strcspn(id_cliente, "\n")] = '\0';
                fflush(stdin);
                printf("fecha nueva(presione enter para saltear): "); 
                fgets(fecha, sizeof(fecha), stdin); 
                fecha[strcspn(fecha, "\n")] = '\0';
                fflush(stdin);
                snprintf(query, sizeof(query),
                    "CALL ABMS_FACTURA('%s', %s, %s, %s);",
                    accion,
                    AdaptarSQL(nro, 0),
                    AdaptarSQL(id_cliente, 0),
                    AdaptarSQL(fecha, 0));
            }
            else if (strcmp(accion, "S") == 0) { // SELECCIONAR
                printf("nro (vacío = todos): "); 
                fgets(nro, sizeof(nro), stdin); 
                nro[strcspn(nro, "\n")] = '\0';
                fflush(stdin);
                snprintf(query, sizeof(query),
                    "CALL ABMS_FACTURA('%s', %s, NULL, NULL);",
                    accion,
                    AdaptarSQL(nro, 0));
            }
            else {
                printf("Accion invalida.\n");
                continue;
            }

            printf("\nEjecutando: %s\n", query);
            querySQL(conn, query);
        }


        // <<<<<<< PRODUCTO  >>>>>>>
        if (strcmp(entidad, "3") == 0) {
            char id[20] = "";
            char descripcion[50] = "";
            char precio_base[50] = "";
            char stock[50] = "";

            if (strcmp(accion, "A") == 0) { // ALTA
                printf("descripcion: "); 
                fgets(descripcion, sizeof(descripcion), stdin); 
                descripcion[strcspn(descripcion, "\n")] = '\0';
                fflush(stdin);
                printf("precio_base: "); 
                fgets(precio_base, sizeof(precio_base), stdin); 
                precio_base[strcspn(precio_base, "\n")] = '\0';
                fflush(stdin);
                snprintf(query, sizeof(query),
                    "CALL ABMS_PRODUCTO('%s', NULL, %s, %s, 0);",
                    accion,
                    AdaptarSQL(descripcion, 1),
                    AdaptarSQL(precio_base, 0));
            }
            else if (strcmp(accion, "B") == 0) { // BAJA
                printf("id: "); 
                fgets(id, sizeof(id), stdin); 
                id[strcspn(id, "\n")] = '\0';
                fflush(stdin);
                snprintf(query, sizeof(query),
                    "CALL ABMS_PRODUCTO('%s', %s, NULL, NULL, NULL);",
                    accion,
                    AdaptarSQL(id, 0));
            }
            else if (strcmp(accion, "M") == 0) { // MODIFICAR
                printf("id: "); 
                fgets(id, sizeof(id), stdin); 
                id[strcspn(id, "\n")] = '\0';
                fflush(stdin);
                printf("descripcion (presione enter para saltear): "); 
                fgets(descripcion, sizeof(descripcion), stdin); 
                descripcion[strcspn(descripcion, "\n")] = '\0';
                fflush(stdin);
                printf("precio_base (presione enter para saltear): "); 
                fgets(precio_base, sizeof(precio_base), stdin); 
                precio_base[strcspn(precio_base, "\n")] = '\0';
                fflush(stdin);
                printf("Stock (presione enter para saltear): "); 
                fgets(stock, sizeof(stock), stdin); 
                stock[strcspn(stock, "\n")] = '\0';
                fflush(stdin);
                snprintf(query, sizeof(query),
                    "CALL ABMS_PRODUCTO('%s', %s, %s, %s, %s);",
                    accion,
                    AdaptarSQL(id, 0),
                    AdaptarSQL(descripcion, 1),
                    AdaptarSQL(precio_base, 0),
                    AdaptarSQL(stock, 0));
            }
            else if (strcmp(accion, "S") == 0) { // SELECCIONAR
                printf("id (vacío = todos): "); 
                fgets(id, sizeof(id), stdin); 
                id[strcspn(id, "\n")] = '\0';
                fflush(stdin);
                snprintf(query, sizeof(query),
                    "CALL ABMS_PRODUCTO('%s', %s, NULL, NULL, NULL);",
                    accion,
                    AdaptarSQL(id, 0));
            }
            else {
                printf("Accion invalida.\n");
                continue;
            }

            printf("\nEjecutando: %s\n", query);
            querySQL(conn, query);
        }

        
        // <<<<<<< DETALLE  >>>>>>>
        if (strcmp(entidad, "4") == 0) {
            char id_factura[20] = "";
            char id_producto[20] = "";
            char cantidad[50] = "";
            char precio_venta[50] = "";

            if (strcmp(accion, "A") == 0) { // ALTA
                printf("id_factura: "); 
                fgets(id_factura, sizeof(id_factura), stdin); 
                id_factura[strcspn(id_factura, "\n")] = '\0';
                fflush(stdin);
                printf("id_producto: "); 
                fgets(id_producto, sizeof(id_producto), stdin); 
                id_producto[strcspn(id_producto, "\n")] = '\0';
                fflush(stdin);
                printf("cantidad: "); 
                fgets(cantidad, sizeof(cantidad), stdin); 
                cantidad[strcspn(cantidad, "\n")] = '\0';
                fflush(stdin);
                printf("precio_venta: "); 
                fgets(precio_venta, sizeof(precio_venta), stdin); 
                precio_venta[strcspn(precio_venta, "\n")] = '\0';
                fflush(stdin);
                snprintf(query, sizeof(query),
                    "CALL ABMS_DETALLE('%s', %s, %s, %s, %s);",
                    accion,
                    AdaptarSQL(id_factura, 0),
                    AdaptarSQL(id_producto, 0),
                    AdaptarSQL(cantidad, 0),
                    AdaptarSQL(precio_venta, 0));
            }
            else if (strcmp(accion, "B") == 0) { // BAJA
                printf("id_factura: "); 
                fgets(id_factura, sizeof(id_factura), stdin); 
                id_factura[strcspn(id_factura, "\n")] = '\0';
                fflush(stdin);
                printf("id_producto: "); 
                fgets(id_producto, sizeof(id_producto), stdin); 
                id_producto[strcspn(id_producto, "\n")] = '\0';
                fflush(stdin);
                snprintf(query, sizeof(query),
                    "CALL ABMS_DETALLE('%s', %s, %s, NULL, NULL);",
                    accion,
                    AdaptarSQL(id_factura, 0),
                    AdaptarSQL(id_producto, 0));
            }
            else if (strcmp(accion, "M") == 0) { // MODIFICAR
                printf("id_factura: "); 
                fgets(id_factura, sizeof(id_factura), stdin); 
                id_factura[strcspn(id_factura, "\n")] = '\0';
                fflush(stdin);
                printf("id_producto: "); 
                fgets(id_producto, sizeof(id_producto), stdin); 
                id_producto[strcspn(id_producto, "\n")] = '\0';
                fflush(stdin);
                printf("cantidad: "); 
                fgets(cantidad, sizeof(cantidad), stdin); 
                cantidad[strcspn(cantidad, "\n")] = '\0';
                fflush(stdin);
                printf("precio_venta: "); 
                fgets(precio_venta, sizeof(precio_venta), stdin); 
                precio_venta[strcspn(precio_venta, "\n")] = '\0';
                fflush(stdin);
                snprintf(query, sizeof(query),
                    "CALL ABMS_DETALLE('%s', %s, %s, %s, %s);",
                    accion,
                    AdaptarSQL(id_factura, 0),
                    AdaptarSQL(id_producto, 0),
                    AdaptarSQL(cantidad, 0),
                    AdaptarSQL(precio_venta, 0));
            }
            else if (strcmp(accion, "S") == 0) { // SELECCIONAR
                printf("id_factura (vacío = todos): "); 
                fgets(id_factura, sizeof(id_factura), stdin); 
                id_factura[strcspn(id_factura, "\n")] = '\0';
                fflush(stdin);
                snprintf(query, sizeof(query),
                    "CALL ABMS_DETALLE('%s', %s, NULL, NULL, NULL);",
                    accion,
                    AdaptarSQL(id_factura, 0));
            }
            else {
                printf("Accion invalida.\n");
                continue;
            }

            printf("\nEjecutando: %s\n", query);
            querySQL(conn, query);
        }


        // <<<<<<< PAGO  >>>>>>>
        if (strcmp(entidad, "5") == 0) {
            char nro[20] = "";
            char id_factura[20] = "";
            char monto[50] = "";
            char fecha[50] = "";

            if (strcmp(accion, "A") == 0) { // ALTA
                printf("nro: "); 
                fgets(nro, sizeof(nro), stdin); 
                nro[strcspn(nro, "\n")] = '\0';
                fflush(stdin);
                printf("id_factura: "); 
                fgets(id_factura, sizeof(id_factura), stdin); 
                id_factura[strcspn(id_factura, "\n")] = '\0';
                fflush(stdin);
                printf("monto: "); 
                fgets(monto, sizeof(monto), stdin); 
                monto[strcspn(monto, "\n")] = '\0';
                fflush(stdin);
                printf("fecha: "); 
                fgets(fecha, sizeof(fecha), stdin); 
                fecha[strcspn(fecha, "\n")] = '\0';
                fflush(stdin);
                snprintf(query, sizeof(query),
                    "CALL ABMS_PAGO('%s', %s, %s, %s, %s);",
                    accion,
                    AdaptarSQL(nro, 0),
                    AdaptarSQL(id_factura, 0),
                    AdaptarSQL(monto, 0),
                    AdaptarSQL(fecha, 0));
            }
            else if (strcmp(accion, "B") == 0) { // BAJA
                printf("nro: "); 
                fgets(nro, sizeof(nro), stdin); 
                nro[strcspn(nro, "\n")] = '\0';
                fflush(stdin);
                printf("id_factura: "); 
                fgets(id_factura, sizeof(id_factura), stdin); 
                id_factura[strcspn(id_factura, "\n")] = '\0';
                fflush(stdin);
                snprintf(query, sizeof(query),
                    "CALL ABMS_PAGO('%s', %s, %s, NULL, NULL);",
                    accion,
                    AdaptarSQL(nro, 0),
                    AdaptarSQL(id_factura, 0));
            }
            else if (strcmp(accion, "M") == 0) { // MODIFICAR
                printf("nro: "); 
                fgets(nro, sizeof(nro), stdin); 
                nro[strcspn(nro, "\n")] = '\0';
                fflush(stdin);
                printf("id_factura: "); 
                fgets(id_factura, sizeof(id_factura), stdin); 
                id_factura[strcspn(id_factura, "\n")] = '\0';
                fflush(stdin);
                printf("monto: "); 
                fgets(monto, sizeof(monto), stdin);
                monto[strcspn(monto, "\n")] = '\0';
                fflush(stdin);
                printf("fecha: "); 
                fgets(fecha, sizeof(fecha), stdin); 
                fecha[strcspn(fecha, "\n")] = '\0';
                fflush(stdin);
                snprintf(query, sizeof(query),
                    "CALL ABMS_PAGO('%s', %s, %s, %s, %s);",
                    accion,
                    AdaptarSQL(nro, 0),
                    AdaptarSQL(id_factura, 0),
                    AdaptarSQL(monto, 0),
                    AdaptarSQL(fecha, 0));
            }
            else if (strcmp(accion, "S") == 0) { // SELECCIONAR
                printf("id_factura (vacío = todos): "); 
                fgets(id_factura, sizeof(id_factura), stdin); 
                id_factura[strcspn(id_factura, "\n")] = '\0';
                fflush(stdin);
                snprintf(query, sizeof(query),
                    "CALL ABMS_PAGO('%s', NULL, %s, NULL, NULL);",
                    accion,
                    AdaptarSQL(id_factura, 0));
            }
            else {
                printf("Accion invalida.\n");
                continue;
            }

            printf("\nEjecutando: %s\n", query);
            querySQL(conn, query);
        }
        
    }
}

int main() {
    char user[100], password[100];
    printf("Usuario: ");
    fgets(user, sizeof(user), stdin);
    user[strcspn(user, "\n")] = '\0';
    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    MYSQL *conn = conectarMySQL(REMOTEHOST, REMOTEPORT, user, password, "SEGUNDA_EVALUACION12");
    if (!conn) return -1;

    menuEntidad(conn); 

    cerrarSesionSQL(conn);
    return 0;
}