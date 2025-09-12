#include "/usr/include/mysql/mysql.h"
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

MYSQL *conectarMySQL(const char *remote_host,
                     int remote_port,
                     const char *username,
                     const char *userpassword,
                     char databasename[])
{
    MYSQL *conn;
    const char *server = remote_host;
    const char *user = username;
    const char *password = userpassword;
    const char *database = databasename;

    conn = mysql_init(NULL);
    if (conn == NULL){
        printf("Error: mysql_init! \n");
        if (!mysql_real_connect(conn, server, user, password, database, remote_port, NULL, 0)){
            fprintf(stderr, "Error en mysql_real_connect: %s\n", mysql_error(conn));
            mysql_close(conn);
            return NULL;
        }
    }
    else {
        char respuesta[1024] = "";
        if (!mysql_real_connect(conn, server, user, password, database, remote_port, NULL, 0)){
            if (!mysql_real_connect(conn, server, user, password, database, remote_port, NULL, 0)){
                fprintf(stderr, "Error en mysql_real_connect: %s\n", mysql_error(conn));
                mysql_close(conn);
                return NULL;
            }
        }
    }
    return conn;
}

void querySQL(MYSQL *conn, const char *query)
{
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error en querySQL: %s\n", mysql_error(conn));
        return;
    }

    do {
        MYSQL_RES *res = mysql_store_result(conn);

        if (res) {
            int num_attrib = mysql_num_fields(res);
            MYSQL_FIELD *fields = mysql_fetch_fields(res);

            /* calcular anchos (inicializados al largo del nombre de la columna) */
            size_t *widths = calloc(num_attrib, sizeof(size_t));
            if (!widths) {
                fprintf(stderr, "Memoria insuficiente\n");
                mysql_free_result(res);
                return;
            }
            for (int i = 0; i < num_attrib; ++i) {
                widths[i] = strlen(fields[i].name);
            }

            /* recorrer filas para ajustar anchos */
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != NULL) {
                unsigned long *lengths = mysql_fetch_lengths(res);
                for (int i = 0; i < num_attrib; ++i) {
                    size_t len = row[i] ? (size_t) lengths[i] : 4; /* "NULL" => 4 */
                    if (len > widths[i]) widths[i] = len;
                }
            }

            /* imprimir encabezado alineado */
            for (int i = 0; i < num_attrib; ++i) {
                printf("%-*s  ", (int)widths[i], fields[i].name);
            }
            printf("\n");

            /* imprimir separador */
            for (int i = 0; i < num_attrib; ++i) {
                for (size_t j = 0; j < widths[i]; ++j) putchar('-');
                printf("  ");
            }
            printf("\n");

            /* volver al inicio del resultset y mostrar filas */
            mysql_data_seek(res, 0);
            while ((row = mysql_fetch_row(res)) != NULL) {
                unsigned long *lengths = mysql_fetch_lengths(res);
                for (int i = 0; i < num_attrib; ++i) {
                    const char *val = row[i] ? row[i] : "NULL";
                    printf("%-*s  ", (int)widths[i], val);
                }
                printf("\n");
            }

            free(widths);
            mysql_free_result(res);
        } else {
            /* res == NULL: puede ser que no haya result set (UPDATE/INSERT/...) o error */
            if (mysql_field_count(conn) > 0) {
                fprintf(stderr, "Error recuperando resultado: %s\n", mysql_error(conn));
                return;
            } else {
                /* Sin result set: mostrar filas afectadas */
                printf("Query OK, %llu filas afectadas\n",
                       (unsigned long long) mysql_affected_rows(conn));
            }
        }
        /* avanzar al siguiente resultset si existe; mysql_next_result devuelve:
           0 = OK (posible próximo resultset),
          -1 = no hay más resultsets,
          >0 = error */
    } while (mysql_next_result(conn) == 0);
}

void cerrarSesionSQL(MYSQL *conn)
{
    mysql_close(conn);
}