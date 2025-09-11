#!/bin/bash

# Archivos fuente
SRC="AccesoMySQL.c cliente2.c"

# Nombre del ejecutable
OUT="cliente2"

MYSQLCONFIG=$(which mysql_config)

CFLAGS=$($MYSQLCONFIG --cflags)
LIBS=$($MYSQLCONFIG --libs)

echo "Compilando..."
gcc $SRC -o $OUT $CFLAGS $LIBS

if [ $? -eq 0 ]; then
    echo "Compilación exitosa. Ejecuta ./$OUT"
else
    echo "Error en la compilación."
fi