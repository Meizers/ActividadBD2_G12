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
            MYSQL_ROW row;

            while ((row = mysql_fetch_row(res)) != NULL) {
                for (int i = 0; i < num_attrib; i++) {
                    printf("%s ", row[i] ? row[i] : "NULL");
                }
                printf("\n");
            }
            mysql_free_result(res);
        } else {
            if (mysql_field_count(conn) > 0) {
                fprintf(stderr, "Error recuperando resultado: %s\n", mysql_error(conn));
                return;
            }
        }
    } while (mysql_next_result(conn) == 0); // Avanzar al siguiente resultset si existe
}

void cerrarSesionSQL(MYSQL *conn)
{
    mysql_close(conn);
}