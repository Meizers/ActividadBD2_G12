#include "/usr/include/mysql/mysql.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    res = mysql_store_result(conn);
    if (res == NULL) {
        if (mysql_field_count(conn) == 0) {
            printf("Consulta ejecutada correctamente, %llu filas afectadas.\n",
                   mysql_affected_rows(conn));
        } else {
            fprintf(stderr, "Error obteniendo resultados: %s\n", mysql_error(conn));
        }
        return;
    }

    int num_attrib = mysql_num_fields(res);
    MYSQL_FIELD *fields = mysql_fetch_fields(res);

    // Calcular anchos
    unsigned long *col_widths = calloc(num_attrib, sizeof(unsigned long));
    for (int i = 0; i < num_attrib; i++) {
        col_widths[i] = strlen(fields[i].name);  // mínimo el nombre
        if (fields[i].max_length > col_widths[i]) {
            col_widths[i] = fields[i].max_length;
        }
    }

    // Encabezados
    for (int i = 0; i < num_attrib; i++) {
        printf("%-*s ", (int)col_widths[i], fields[i].name);
    }
    printf("\n");

    // Separador
    for (int i = 0; i < num_attrib; i++) {
        for (unsigned long j = 0; j < col_widths[i]; j++) printf("-");
        printf(" ");
    }
    printf("\n");

    // Filas
    MYSQL_ROW row_values;
    while ((row_values = mysql_fetch_row(res)) != NULL) {
        unsigned long *lengths = mysql_fetch_lengths(res);
        for (int i = 0; i < num_attrib; i++) {
            printf("%-*s ", (int)col_widths[i],
                   row_values[i] ? row_values[i] : "NULL");
        }
        printf("\n");
    }

    free(col_widths);
    mysql_free_result(res);
}


void cerrarSesionSQL(MYSQL *conn)
{
    mysql_close(conn);
}