-- Crear los triggers necesarios para mantener actualizado el saldo del cliente, el stock del producto, el total y el saldo de la factura.

DELIMITER $$

CREATE TRIGGER BI_PROCESAR_PAGO
    BEFORE INSERT ON pago
    FOR EACH ROW
BEGIN

    DECLARE saldo_actual_factura DECIMAL(10, 2);

    SELECT f.saldo
    INTO saldo_actual_factura
    FROM factura f
    WHERE f.nro = NEW.nro_factura;

    IF NEW.monto > saldo_actual_factura THEN
        SIGNAL SQLSTATE '45001'
        SET MESSAGE_TEXT = 'El pago no puede ser mayor al saldo pendiente de la factura';
    END IF;
END$$


CREATE TRIGGER AI_PROCESAR_PAGO
    AFTER INSERT ON pago
    FOR EACH ROW
BEGIN

    DECLARE p_id_cliente INT;

    SELECT f.id_cliente
    INTO p_id_cliente
    FROM factura f
    WHERE f.nro = NEW.nro_factura;

    -- 1. Actualizar saldo de la factura
    UPDATE factura
    SET saldo = saldo - NEW.monto
    WHERE nro = NEW.nro_factura;

    -- 2. Actualizar saldo del cliente
    UPDATE cliente
    SET saldo = saldo - NEW.monto
    WHERE id = p_id_cliente;

END$$


CREATE TRIGGER AD_PROCESAR_PAGO
    AFTER DELETE ON pago
    FOR EACH ROW
BEGIN

    DECLARE p_id_cliente INT;

    SELECT f.id_cliente
    INTO p_id_cliente
    FROM factura f
    WHERE f.nro = OLD.nro_factura;

    -- 1. Actualizar saldo de la factura
    UPDATE factura
    SET saldo = saldo + OLD.monto
    WHERE nro = OLD.nro_factura;

    -- 2. Actualizar saldo del cliente
    UPDATE cliente
    SET saldo = saldo + OLD.monto
    WHERE id = p_id_cliente;
    END$$


CREATE TRIGGER BI_VENTA_PRODUCTO
    BEFORE INSERT ON detalle
    FOR EACH ROW
BEGIN

    DECLARE p_stock_disponible INT;
    DECLARE p_precio_base DECIMAL(10, 2);

	-- Obtener stock del producto
    SELECT p.stock, p.precio_base
    INTO p_stock_disponible, p_precio_base
    FROM producto p
    WHERE p.id = NEW.id_producto;

    -- Validar que haya stock suficiente
    IF NEW.cantidad > stock_producto THEN
        SIGNAL SQLSTATE '45001'
        SET MESSAGE_TEXT = 'No hay stock suficiente para la venta del producto';
    END IF;

    -- Validar precio de venta
    IF NEW.precio_venta < p_precio_base THEN
        SIGNAL SQLSTATE '45002'
        SET MESSAGE_TEXT = 'El precio de venta debe ser mayor o igual al precio base';
    END IF;
    END$$


CREATE TRIGGER AI_VENTA_PRODUCTO
    AFTER INSERT ON detalle
    FOR EACH ROW
BEGIN
    DECLARE f_monto_parcial DECIMAL(10, 2);
    SET f_monto_parcial = NEW.cantidad * NEW.precio_venta;

    -- 1. Actualizar stock del producto
    UPDATE producto
    SET stock = stock - NEW.cantidad
    WHERE id = NEW.id_producto;

    -- 2. Actualizar monto total y saldo de la factura
    UPDATE factura
    SET monto_total = monto_total + f_monto_parcial,
        saldo = saldo + f_monto_parcial
    WHERE nro = NEW.nro_factura;

    -- 3. Actualizar saldo del cliente
    UPDATE cliente c
        JOIN factura f ON c.id = f.id_cliente
        SET c.saldo = c.saldo + f_monto_parcial
    WHERE f.nro = NEW.nro_factura;
END$$


CREATE TRIGGER BU_VENTA_PRODUCTO
    BEFORE UPDATE ON detalle
    FOR EACH ROW
BEGIN
    DECLARE p_stock_disponible INT;
    DECLARE p_precio_base DECIMAL(10, 2);

	-- Obtener stock del producto
    SELECT p.stock, p.precio_base
    INTO p_stock_disponible, p_precio_base
    FROM producto p
    WHERE p.id = OLD.id_producto;

    -- Validar que haya stock suficiente
    IF NEW.cantidad > stock_producto THEN
        SIGNAL SQLSTATE '45001'
        SET MESSAGE_TEXT = 'No hay stock suficiente para la venta del producto';
    END IF;

    -- Validar cantidad válida
    IF NEW.cantidad <= 0 THEN
        SIGNAL SQLSTATE '45002'
        SET MESSAGE_TEXT = 'La cantidad debe ser mayor a 0';
    END IF;

    -- Validar precio de venta
    IF NEW.precio_venta < p_precio_base THEN
        SIGNAL SQLSTATE '45003'
        SET MESSAGE_TEXT = 'El precio de venta debe ser mayor o igual al precio base';
    END IF;
END$$


CREATE TRIGGER AU_VENTA_PRODUCTO
    AFTER UPDATE ON detalle
    FOR EACH ROW
BEGIN
    DECLARE nuevo_monto_parcial DECIMAL(10, 2);
	DECLARE old_monto_parcial DECIMAL(10, 2);

	SET nuevo_monto_parcial = NEW.cantidad * NEW.precio_venta;
	SET old_monto_parcial = OLD.cantidad * OLD.precio_venta;

    -- 1. Actualizar stock del producto
    UPDATE producto
    SET stock = stock + OLD.cantidad - NEW.cantidad
    WHERE id = NEW.id_producto;

    -- 2. Actualizar monto total y saldo de la factura
    UPDATE factura
    SET monto_total = monto_total - old_monto_parcial + nuevo_monto_parcial,
        saldo = saldo - old_monto_parcial + nuevo_monto_parcial
    WHERE nro = NEW.nro_factura;

    -- 3. Actualizar saldo del cliente
    UPDATE cliente c
        JOIN factura f ON c.id = f.id_cliente
        SET c.saldo = c.saldo - old_monto_parcial + nuevo_monto_parcial
    WHERE f.nro = NEW.nro_factura;
    END$$


CREATE TRIGGER AD_VENTA_PRODUCTO
AFTER DELETE ON detalle
FOR EACH ROW
BEGIN
    DECLARE old_monto_parcial DECIMAL(10, 2);
    SET old_monto_parcial = OLD.cantidad * OLD.precio_venta;

    -- 1. Actualizar stock del producto
    UPDATE producto
    SET stock = stock + OLD.cantidad
    WHERE id = OLD.id_producto;

    -- 2. Actualizar monto total y saldo de la factura
    UPDATE factura
    SET monto_total = monto_total - old_monto_parcial,
        saldo = saldo - old_monto_parcial
    WHERE nro = OLD.nro_factura;

    -- 3. Actualizar saldo del cliente
    UPDATE cliente c
        JOIN factura f ON c.id = f.id_cliente
        SET c.saldo = c.saldo - old_monto_parcial
    WHERE f.nro = OLD.nro_factura;
END$$

CREATE TRIGGER BI_NRO_PAGO
    BEFORE INSERT ON pago
    FOR EACH ROW
BEGIN
    -- Calcular próximo número para esta factura
    SELECT COALESCE(MAX(nro), 0) + 1
    INTO @siguiente_nro
    FROM pago
    WHERE nro_factura = NEW.nro_factura;

    SET NEW.nro = @siguiente_nro;
END$$

DELIMITER ;