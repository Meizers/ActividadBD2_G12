#include "/usr/include/mysql/mysql.h"
#include <sys/types.h>

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
        unsigned long client_flag = CLIENT_MULTI_RESULTS; /* y opcional CLIENT_MULTI_STATEMENTS si haces multi-statement */
        if (!mysql_real_connect(conn, server, user, password, database, remote_port, NULL, client_flag)) {
            fprintf(stderr, "mysql_real_connect error: %s\n", mysql_error(conn));
            mysql_close(conn);
            return NULL;
        }
    }
    return conn;
}

void querySQL(MYSQL *conn, const char *query)
{
    if (!conn || !query) { fprintf(stderr, "Bad params\n"); return; }

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error en mysql_query: %s\n", mysql_error(conn));
        return;
    }

    /* Procesar todos los result sets devueltos por esta query / CALL */
    do {
        MYSQL_RES *res = mysql_store_result(conn);
        if (res) {
            int nf = mysql_num_fields(res);
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != NULL) {
                for (int i = 0; i < nf; ++i)
                    printf("%s%s", row[i] ? row[i] : "NULL", (i+1==nf) ? "" : "\t");
                printf("\n");
            }
            mysql_free_result(res);
        } else {
            if (mysql_field_count(conn) == 0) {
                printf("Query OK, %llu filas afectadas\n",
                       (unsigned long long) mysql_affected_rows(conn));
            } else {
                fprintf(stderr, "Error al obtener result set: %s\n", mysql_error(conn));
                /* podés breakear si preferís */
            }
        }

        /* avanzar al siguiente result set si existe */
        if (mysql_more_results(conn)) {
            int st = mysql_next_result(conn);
            if (st > 0) {
                fprintf(stderr, "mysql_next_result error: %s\n", mysql_error(conn));
                break;
            }
        } else break;

    } while (1);
}

MYSQL *asegurarConn(MYSQL *conn, const char *host, int port,
                   const char *user, const char *pass, char *db)
{
    if (!conn) return conectarMySQL(host, port, user, pass, db);
    if (mysql_ping(conn) != 0) {
        mysql_close(conn);
        return conectarMySQL(host, port, user, pass, db);
    }
    return conn;
}

void cerrarSesionSQL(MYSQL *conn)
{
    mysql_close(conn);
}