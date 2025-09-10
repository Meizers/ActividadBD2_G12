-- Crear los procedimientos necesarios para insertar, modificar, borrar y consultar clientes, productos, facturas, detalles y pagos.

-- CLIENTE
DELIMITER //
CREATE PROCEDURE ABMS_CLIENTE (
    IN u_accion CHAR(1),     -- A: Alta, B: Baja, M: Modificar, S: Seleccionar
    IN u_nombre VARCHAR(50),
    IN u_apellido VARCHAR(50)
)
BEGIN
    CASE u_accion
        WHEN 'A' THEN  -- ALTA
            INSERT INTO cliente(nombre, apellido)
            VALUES (u_nombre, u_apellido);

        WHEN 'B' THEN  -- BAJA
            DELETE FROM cliente WHERE id = u_id;

        WHEN 'M' THEN  -- MODIFICACION parcial
            UPDATE cliente
            SET 
                nombre   = COALESCE(u_nombre, nombre),
                apellido = COALESCE(u_apellido, apellido)
            WHERE id = u_id;

        WHEN 'S' THEN  -- SELECCION
            IF u_id IS NULL THEN
                SELECT * FROM cliente;
            ELSE
                SELECT * FROM cliente WHERE id = u_id;
            END IF;
    END CASE;
END //
DELIMITER ;


-- FACTURA
DELIMITER //
CREATE PROCEDURE ABMS_FACTURA (
    IN u_accion CHAR(1),
    IN u_nro INT,
    IN u_idcliente INT,
    IN u_fecha DATE
)
BEGIN
    CASE u_accion
        WHEN 'A' THEN
            INSERT INTO factura(id_cliente, fecha, monto_total, saldo)
            VALUES (u_idcliente, u_fecha, 0, 0); -- monto y saldo los manejarán triggers

        WHEN 'B' THEN
            DELETE FROM factura WHERE nro = u_nro;

        WHEN 'M' THEN
            UPDATE factura
            SET 
                id_cliente = COALESCE(u_idcliente, id_cliente),
                fecha      = COALESCE(u_fecha, fecha)
            WHERE nro = u_nro;

        WHEN 'S' THEN
            IF u_nro IS NULL THEN
                SELECT * FROM factura;
            ELSE
                SELECT * FROM factura WHERE nro = u_nro;
            END IF;
    END CASE;
END //
DELIMITER ;


-- PRODUCTO
DELIMITER //
CREATE PROCEDURE ABMS_PRODUCTO (
    IN u_accion CHAR(1),
    IN u_id INT,
    IN u_descripcion VARCHAR(50),
    IN u_precio_base DECIMAL,
    IN u_stock INT
)
BEGIN
    CASE u_accion
        WHEN 'A' THEN
            INSERT INTO producto(descripcion, precio_base)
            VALUES (u_descripcion, u_precio_base);

        WHEN 'B' THEN
            DELETE FROM producto WHERE id = u_id;

        WHEN 'M' THEN
            UPDATE producto
            SET 
                descripcion = COALESCE(u_descripcion, descripcion),
                precio_base = COALESCE(u_precio_base, precio_base),
                stock = COALESCE(u_stock, stock)
            WHERE id = u_id;

        WHEN 'S' THEN
            IF u_id IS NULL THEN
                SELECT * FROM producto;
            ELSE
                SELECT * FROM producto WHERE id = u_id;
            END IF;
    END CASE;
END //
DELIMITER ;


-- DETALLE
DELIMITER //
CREATE PROCEDURE ABMS_DETALLE (
    IN u_accion CHAR(1),
    IN u_id_factura INT,
    IN u_id_producto INT,
    IN u_cantidad INT,
    IN u_precio_venta DECIMAL
)
BEGIN
    CASE u_accion
        WHEN 'A' THEN
            INSERT INTO detalle(id_factura, id_producto, cantidad, precio_venta)
            VALUES (u_id_factura, u_id_producto, u_cantidad, u_precio_venta);

        WHEN 'B' THEN
            DELETE FROM detalle WHERE id_factura = u_id_factura AND id_producto = u_id_producto;

        WHEN 'M' THEN
            UPDATE detalle
            SET 
                cantidad     = COALESCE(u_cantidad, cantidad),
                precio_venta = COALESCE(u_precio_venta, precio_venta)
            WHERE id_factura = u_id_factura AND id_producto = u_id_producto;

        WHEN 'S' THEN
            IF u_id_factura IS NULL THEN
                SELECT * FROM detalle;
            ELSE
                SELECT * FROM detalle WHERE id_factura = u_id_factura;
            END IF;
    END CASE;
END //
DELIMITER ;


-- PAGO
DELIMITER //
CREATE PROCEDURE ABMS_PAGO (
    IN u_accion CHAR(1),
    IN u_nro INT,
    IN u_id_factura INT,
    IN u_monto DECIMAL(10,2),
    IN u_fecha DATE
)
BEGIN
    CASE u_accion
        WHEN 'A' THEN
            INSERT INTO pago(id_factura, monto, fecha)
            VALUES (u_id_factura, u_monto, u_fecha);

        WHEN 'B' THEN
            DELETE FROM pago WHERE nro = u_nro AND id_factura = u_id_factura;

        WHEN 'M' THEN
            UPDATE pago
            SET 
                monto = COALESCE(u_monto, monto),
                fecha = COALESCE(u_fecha, fecha)
            WHERE nro = u_nro AND id_factura = u_id_factura;

        WHEN 'S' THEN
            IF u_id_factura IS NULL THEN
                SELECT * FROM pago;
            ELSE
                SELECT * FROM pago WHERE id_factura = u_id_factura;
            END IF;
    END CASE;
END //
DELIMITER ;
