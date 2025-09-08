cmake_minimum_required(VERSION 3.16)
project(ActividadBD2_G12 C)

set(CMAKE_C_STANDARD 11)

# Directorios de headers propios y de mysql
include_directories(include)
include_directories(/opt/homebrew/opt/mysql-client/include)

# Buscar todos los .c en src/ automáticamente
file(GLOB SOURCES src/*.c)

# Crear el ejecutable con todos los .c encontrados
add_executable(${PROJECT_NAME} ${SOURCES})

# Buscar la librería mysqlclient instalada por Homebrew
find_library(MYSQLCLIENT_LIB
        NAMES mysqlclient
        HINTS /opt/homebrew/opt/mysql-client/lib
)

if(NOT MYSQLCLIENT_LIB)
    message(FATAL_ERROR "❌ No se encontró libmysqlclient. Ejecutá: brew install mysql-client")
endif()

# Linkear el ejecutable con mysqlclient
target_link_libraries(${PROJECT_NAME} ${MYSQLCLIENT_LIB})
