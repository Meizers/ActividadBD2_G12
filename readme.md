
## Explicación Actividad 1

En esta actividad, tuvimos que codificar un programa cliente en el lenguaje C, a través del cual nos permitirá conectarnos a la base de datos ```PRIMER_EVALUACION``` alojada en la máquina virtual del CIDETIC. 

Para poder correr el cliente, primero se debe ejecutar el script de compilación build.sh, el cual identifica el path en el cual se encuentra la librería mysql.h, necesaria para realizar la conexión con la base de datos. Para poder ejecutar build.sh se le debe dar permisos de ejecución utilizando el comando ```chmod +x ./build.sh```.

Luego de la ejecución del script de compilación debería crearse el ejecutable ```cliente1```, si es que no hubo problemas en la compilación. Para ejecutar el cliente se debe ingresar el comando ```./cliente1```.

Al ejecutar el cliente, le va a pedir el usuario y contraseña para la base de datos. Una vez  conectado a la base de datos, nos permitirá hacer altas y consultas de alumnos, para hacer esto el cliente envía un mensaje a la base de datos solicitando la ejecución del stored procedure ```PRO_AGREGAR_CONSULTAR_ESTUDIANTE```, cuyos argumentos dependen de lo que el usuario desee realizar. Si el usuario quiere hacer un alta, se le pedirá el número de legajo, el nombre y el apellido del alumno. Si el usuario quiere hacer una consulta, sólo se le pedirá el número de legajo. Luego de la ejecución del procedure, el servidor devolverá un mensaje al cliente, mostrándolo por pantalla. Esta salida puede mostrar un error si se ingresaron argumentos inválidos, o si hubo algún tipo de error en la base de datos, o el resultado de la operación ejecutada.

Para la conexión a la base de datos, se utiliza el archivo AccesoMySQL.c, en el cual se definen funciones para realizar la apertura y cierre de sesión sobre la base de datos, y para la solicitud de queries.

En cliente.h se encuentran los datos que se utilizan para la conexión con la base de datos.

## Explicación Actividad 2