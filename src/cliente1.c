/* Programar una aplicación cliente en lenguaje C que se conecte a un servidor Mysql que está escuchando en el puerto 3306 en la máquina virtual del CIDETIC.

En el SGBD Mysql que esta escuchando en el puerto 3306, ya existe  una base de datos llamada PRIMER_EVALUACION, hay que tener en cuenta las letras mayúsculas, ya que Mysql corriendo en la MV Linux del CIDETIC es sensible a las mayúsculas y minúsculas.

La aplicación cliente llamara a ejecución un procedimiento que esta en la base de datos PRIMER_EVALUACION ya listo para ser ejecutado.

El nombre del Procedimiento es PRO_AGREGAR_CONSULTAR_ESTUDIANTE y tiene los siguientes argumentos de entrada: 
Primer Argumento    : Legajo del estudiante, tipo de dato int.
Segundo Argumento : Apellido del estudiante, tipo de dato varchar(32).
Tercer Argumento     : Nombre del estudiante, tipo de dato varchar(32).
Cuarto Argumento    : Opción, tipo de dato char, si la opción es 'A' se agregan los datos, si la opción es 'C' se consultan los datos, en este caso solo se le pasa el Legajo como argumento y el resto de los argumentos están en blanco. Por ejemplo: call PRO_AGREGAR_CONSULTAR_ESTUDIANTE(111111 , ' ' , ' ' , 'C');

El nombre de usuario de MySql es tú legajo. La password del usuario es "MundoCruel".

Objetivo de la Actividad
El Objetivo es registrarse en la base de datos PRIMER_EVALUACION con su legajo su apellido y su nombre. 

Entrega de la Actividad
Se debe enviar un mail a JRomer  por la plataforma UNLU con los siguientes archivos adjuntos, un archivo Actividad-1.rar que contenga un archivo .pdf 
con la explicación general del trabajo y una carátula con los miembros del equipo de trabajo, máximo tres estudiantes, y los archivos .h, .c y el script de compilación de la aplicación cliente.

*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../include/cliente1.h"
#include "../include/AccesoMySQL.h"

#define PROCEDURE "call PRO_AGREGAR_CONSULTAR_ESTUDIANTE(%s, '%s', '%s', '%c');"
#define MAX_CHAR 1024

int main() {
    char user[MAX_CHAR];
    char password[MAX_CHAR];

    printf("Ingrese el usuario de MySQL: ");
    fgets(user, sizeof(user), stdin);
    user[strcspn(user, "\n")] = '\0';

    printf("Ingrese la contraseña del usuario MySQL: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    MYSQL *conn = conectarMySQL(REMOTEHOST, REMOTEPORT, user, password, "PRIMER_EVALUACION");

    if (conn == NULL) {
        return -1;
    }

    char opcion[8];
    while (1) {
        printf("\n\tMenú:\n");
        printf("A - Alta de alumno\n");
        printf("C - Consultar alumno\n");
        printf("0 - Salir\n");
        printf("Ingrese una opción: ");
        fgets(opcion, sizeof(opcion), stdin);
        opcion[strcspn(opcion, "\n")] = '\0';
        fflush(stdin);

        if ((strcmp(opcion, "A") != 0) &&
            (strcmp(opcion, "C") != 0) &&
            (strcmp(opcion, "0") != 0)) {
            printf("Opción inválida.\n");
        } else if (strcmp(opcion, "0") == 0) {
            cerrarSesionSQL(conn);
            break;
        } else {
            char legajo[20];
            char apellido[20];
            char nombre[20];

            printf("Ingrese el legajo: ");
            fgets(legajo, sizeof(legajo), stdin);
            legajo[strcspn(legajo, "\n")] = '\0';
            fflush(stdin);
            printf("Ingrese el apellido: ");
            fgets(apellido, sizeof(apellido), stdin);
            apellido[strcspn(apellido, "\n")] = '\0';
            fflush(stdin);
            printf("Ingrese el nombre: ");
            fgets(nombre, sizeof(nombre), stdin);
            nombre[strcspn(nombre, "\n")] = '\0';
            fflush(stdin);

            char query[MAX_CHAR];
            snprintf(query, sizeof(query),
                     PROCEDURE, legajo, apellido, nombre, opcion[0]);
            querySQL(conn, query);
        }
    }
    return 0;
} 


