#ifndef ACCESO_MYSQL_H
#define ACCESO_MYSQL_H

#include "/opt/homebrew/opt/mysql-client/include/mysql/mysql.h"

MYSQL *conectarMySQL(const char *remote_host,
                     int remote_port,
                     const char *username,
                     const char *userpassword,
                     char databasename[]);
void cerrarSesionSQL(MYSQL* conn);
void querySQL(MYSQL* conn, const char* query);

#endif