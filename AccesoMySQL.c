#include "/usr/include/mariadb/mysql.h"
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void funcionMysql(const char *remote_host, 
    const char *remote_port, 
    const char *username, 
    const char *userpassword,
    char dabasename[],
    char query[],
    char respuesta[])
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    const char *server = remote_host;
    const char *user = username;
    const char *password = userpassword;
    const char *database = databasename;

    conn = mysql_init(NULL);
    if (conn == NULL){
        printf("Error: mysql_init! \n");
    }
    else {
        if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)){
            sprintf(respuesta, "%s\n", mysql_error(conm));
        }
        else{
            if (mysql_query(conn, query)){
                printf(respuesta, "%s\n", mysql_error(conm));
            }
            else {
                res = mysql_use_result(conn);
                int num_attrib = mysql_num_fields(res);
                while ((row = mysql_fetch_row(res)) != NULL){
                    int i;
                    for (i=0; i< num_attrib, i++){
                        strcat(respuesta, row[i]);
                        strcat(respuesta, " ");
                    }
                    strcat(respuesta, "\n");
                }
                strcat(respuesta, "\0");
                mysql_free_resul(res);
            }
        }
        mysql_close(conn);
    }
}