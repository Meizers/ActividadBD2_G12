/* modelo relacional que permita almacenar los datos de clientes que realizan compras de distintos productos que tienen un stock, 
no se puede vender un producto que no tenga un stock suficiente para satisfacer la cantidad vendida en la venta, las compras se realizan en cuenta corriente 
y los clientes pueden saldar una factura con distintos pagos, un  pago solo  afecta a  una factura, los pagos son en efectivo y solo se factura a clientes en cuenta corriente. 
*/



-- VERSION MYSQL

DROP DATABASE IF EXISTS bd2_act_2;
CREATE DATABASE bd2_act_2;
USE bd2_act_2;

CREATE TABLE producto (
                          id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
                          descripcion VARCHAR(50),
                          precio_base DECIMAL(10, 2) NOT NULL CHECK (precio_base > 0),
                          stock INT NOT NULL DEFAULT 0
);

CREATE TABLE cliente (
                         id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
                         nombre VARCHAR(50),
                         apellido VARCHAR(50),
                         saldo DECIMAL DEFAULT 0
);

CREATE TABLE factura (
                         nro INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
                         id_cliente INT NOT NULL,
                         fecha DATE,
                         monto_total DECIMAL(10, 2) NOT NULL,
                         saldo DECIMAL(10, 2),

                         FOREIGN KEY (id_cliente) references cliente (id)
);

CREATE TABLE pago (
                      nro INT NOT NULL,
                      nro_factura INT,
                      monto DECIMAL(10, 2) CHECK (monto > 0),
                      fecha DATE,

                      PRIMARY KEY (nro, nro_factura),
                      FOREIGN KEY (nro_factura) references factura (nro)
);

CREATE TABLE detalle (
                         nro_factura INT NOT NULL,
                         id_producto INT NOT NULL,
                         cantidad INT NOT NULL CHECK (cantidad > 0),
                         precio_venta DECIMAL(10, 2) NOT NULL,

                         PRIMARY KEY (nro_factura, id_producto),
                         FOREIGN KEY (nro_factura) references factura (nro),
                         FOREIGN KEY (id_producto) references producto (id)

);

-- VERSION FIREBIRD

CREATE TABLE CLIENTE (
    ID_C INTEGER NOT NULL,
    NOMBRE VARCHAR(20) NOT NULL,
    SALDO DOUBLE PRECISION NOT NULL,
    CONSTRAINT PK_CLIENTE PRIMARY KEY (ID_C)
)

CREATE TABLE FACTURA (
    NRO_F INTEGER NOT NULL,
    ID_C INTEGER NOT NULL,
    IMPORTE DOUBLE PRECISION NOT NULL,
    SALDO DOUBLE PRECISION NOT NULL,
    CONSTRAINT PK_FACTURA PRIMARY KEY (NRO_F),
    CONSTRAINT FK_FACTURA_CLIENTE FOREIGN KEY (ID_C) REFERENCES CLIENTE
)

CREATE TABLE PAGO (
    NRO_P INTEGER NOT NULL,
    NRO_F INTEGER NOT NULL,
    MONTO DOUBLE PRECISION NOT NULL,
    CONSTRAINT PK_PAGO PRIMARY KEY (NRO_P, NRO_F),
    CONSTRAINT FK_PAGO_FACTURA FOREIGN KEY (NRO_F) REFERENCES FACTURA
)

CREATE TABLE PRODUCTO (
    ID_P INTEGER NOT NULL,
    DESCR VARCHAR(100) NOT NULL,
    STOCK INTEGER NOT NULL,
    PRECIO_BASE DOUBLE PRECISION NOT NULL,
    CONSTRAINT PK_PRODUCTO PRIMARY KEY (ID_P),
    CONSTRAINT PRODUCTO_CHECK CHECK (STOCK >= 0)
)

CREATE TABLE DETALLE (
    NRO_F INTEGER NOT NULL,
    ID_P INTEGER NOT NULL,
    CANTIDAD INTEGER NOT NULL,
    PRECIO DOUBLE PRECISION NOT NULL,
    CONSTRAINT PK_DETALLE PRIMARY KEY (NRO_F, ID_P),
    CONSTRAINT FK_DETALLE_FACTURA FOREIGN KEY (NRO_F) REFERENCES FACTURA,
    CONSTRAINT FK_DETALLE_PRODUCTO FOREIGN KEY (ID_P) REFERENCES PRODUCTO
)


