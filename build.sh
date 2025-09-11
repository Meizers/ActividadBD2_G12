#!/bin/bash

# Archivos fuente
SRC="AccesoMySQL.c cliente1.c"

# Nombre del ejecutable
OUT="cliente1"

CFLAGS=$(mysql_config --cflags)
LIBS=$(mysql_config --libs)

echo "Compilando..."
gcc $SRC -o $OUT $CFLAGS $LIBS

if [ $? -eq 0 ]; then
    echo "Compilación exitosa. Ejecuta ./$OUT"
else
    echo "Error en la compilación."
fi