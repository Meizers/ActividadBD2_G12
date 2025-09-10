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
    MYSQL_RES *res;
    MYSQL_ROW row;

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error en querySQL: %s\n", mysql_error(conn));
        return;
    }

    res = mysql_store_result(conn);  // <-- mejor que use_result
    if (res == NULL) {
        fprintf(stderr, "La consulta no devolvió resultados (o error).\n");
        return;
    }

    int num_attrib = mysql_num_fields(res);

    while ((row = mysql_fetch_row(res)) != NULL) {
        for (int i = 0; i < num_attrib; i++) {
            printf("%s ", row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }

    mysql_free_result(res);
}

void cerrarSesionSQL(MYSQL *conn)
{
    mysql_close(conn);
}