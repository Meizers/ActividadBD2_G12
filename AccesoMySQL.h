#ifndef ACCESOMYSQL_H
#define ACCESOMYSQL_H

#include <mysql/mysql.h>

MYSQL *conectarMySQL(const char *host, unsigned int port,
                     const char *user, const char *password,
                     const char *dbname);

void querySQL(MYSQL *conn, const char *query);
void cerrarSesionSQL(MYSQL *conn);

#endif