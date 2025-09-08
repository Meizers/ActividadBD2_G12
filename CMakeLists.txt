cmake_minimum_required(VERSION 3.10)   # ojo: no existe 4.0 todavía
project(ActividadBD2_G12 C)

set(CMAKE_C_STANDARD 11)

# Incluí tus directorios de headers
include_directories(.)
include_directories(Actividad_2)
include_directories(/opt/homebrew/opt/mysql-client/include)

# Archivos fuente
add_executable(ActividadBD2_G12
        AccesoMySQL.c
        cliente1.c)

# Buscar la librería mysqlclient en la instalación de Homebrew
find_library(MYSQLCLIENT_LIB
        NAMES mysqlclient
        HINTS /opt/homebrew/opt/mysql-client/lib
)

if(NOT MYSQLCLIENT_LIB)
    message(FATAL_ERROR "❌ No se encontró libmysqlclient. Ejecutá: brew install mysql-client")
endif()

# Linkear el ejecutable con la librería mysqlclient
target_link_libraries(ActividadBD2_G12 ${MYSQLCLIENT_LIB})

