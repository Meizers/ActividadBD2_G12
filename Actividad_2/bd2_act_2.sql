DROP DATABASE IF EXISTS bd2_act_2;
CREATE DATABASE bd2_act_2;
USE bd2_act_2;

CREATE TABLE producto (
	id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
	descripcion VARCHAR(50),
	precio_base DECIMAL(10, 2) NOT NULL,
	stock INT NOT NULL DEFAULT 0
);

CREATE TABLE cliente (
	id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
	nombre VARCHAR(50),
	apellido VARCHAR(50),
	saldo DECIMAL NOT NULL DEFAULT 0
);

CREATE TABLE factura (
	nro INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
	id_cliente INT NOT NULL,
	fecha DATE,
	monto_total DECIMAL NOT NULL,
	saldo DECIMAL,
	
	FOREIGN KEY (id_cliente) references cliente (id)
);

CREATE TABLE pago (
	nro INT AUTO_INCREMENT NOT NULL,
	id_factura INT, 
	monto DECIMAL CHECK (monto > 0),
	fecha DATE,
	
	PRIMARY KEY (nro, id_factura),
	FOREIGN KEY (id_factura) references factura (id)
);

CREATE TABLE detalle (
	id_factura INT NOT NULL,
	id_producto INT NOT NULL,
	cantidad INT NOT NULL, 
	precio_venta DECIMAL NOT NULL,
	
	PRIMARY KEY (id_factura, id_producto),
	FOREIGN KEY (id_factura) references factura (nro),
	FOREIGN KEY (id_producto) references producto (id)

);


DELIMITER $$

CREATE TRIGGER BI_PROCESAR_PAGO
	BEFORE INSERT ON pago
	FOR EACH ROW
BEGIN
	
	DECLARE v_id_cliente INT;
    DECLARE v_monto_total DECIMAL;
    DECLARE v_saldo_actual_factura DECIMAL;
	
	SELECT f.id_cliente, f.monto_total, f.saldo 
    INTO v_id_cliente, v_monto_total, v_saldo_actual_factura
    FROM factura f 
    WHERE f.id = NEW.id_factura;
    
    IF NEW.monto > v_saldo_actual_factura THEN
        SIGNAL SQLSTATE '45002'
        SET MESSAGE_TEXT = 'El pago no puede ser mayor al saldo pendiente de la factura';
    END IF;
END


CREATE TRIGGER AI_PROCESAR_PAGO
    AFTER INSERT ON pago
    FOR EACH ROW
BEGIN
    
    -- 1. Actualizar saldo de la factura
    UPDATE factura 
    SET saldo = saldo - NEW.monto
    WHERE id = NEW.id_factura;
    
    -- 2. Actualizar saldo del cliente
    UPDATE cliente 
    SET saldo = saldo - NEW.monto
    WHERE id = v_id_cliente;
    
END$$

DELIMITER ;



DELIMITER

CREATE TRIGGER BI_VENTA_PRODUCTO
	BEFORE INSERT ON detalle
	FOR EACH ROW
BEGIN
	
	DECLARE p_stock_disponible INT;
    DECLARE p_precio_base DECIMAL;
	
	-- Obtener stock del producto
	SELECT p.stock, p.precio_base
	INTO p_stock_disponible, p_precio_base
	FROM producto p
	WHERE p.id = NEW.id_producto;
	
	-- Validar que haya stock suficiente
	IF NEW.cantidad > stock_producto THEN
        SIGNAL SQLSTATE '45002'
        SET MESSAGE_TEXT = 'No hay stock suficiente para la venta del producto';
    END IF;
	
	 -- Validar precio de venta
    IF NEW.precio_venta < p_precio_base THEN
        SIGNAL SQLSTATE '45003'
        SET MESSAGE_TEXT = 'El precio de venta debe ser mayor o igual al precio base';
    END IF;
END

DELIMITER ;

DELIMITER $$

CREATE TRIGGER AI_VENTA_PRODUCTO
    AFTER INSERT ON detalle
    FOR EACH ROW
BEGIN
	DECLARE f_monto_parcial DECIMAL;

	SET f_monto_parcial = NEW.cantidad * NEW.precio_venta;
    
    -- 1. Actualizar stock del producto
    UPDATE producto 
    SET stock = stock - NEW.cantidad
    WHERE id = NEW.id_producto;
    
    -- 2. Actualizar monto total y saldo de la factura
    UPDATE factura
	SET monto_total = monto_total + f_monto_parcial,
		saldo = saldo + f_monto_parcial
	WHERE id = NEW.id_factura
    
    -- 3. Actualizar saldo del cliente
    UPDATE cliente c
    JOIN factura f ON c.id = f.id_cliente
    SET c.saldo = c.saldo + f_monto_parcial
    WHERE f.id = NEW.id_factura;
    
END$$

DELIMITER ;

-- TRIGGER UPDATE Y DELETE detalle.cantidad (before para verificar cantidad válida en UPDATE)




