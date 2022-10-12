
-- borrar sequencias -> 
UPDATE sqlite_sequence SET seq = 0 WHERE name = 'Clientes';
select * from sqlite_sequence;

-----------------------
delete from TelefonosD;
delete from DireccionC;
delete from Clientes;




----------TRAER TABLA COMPLETA DE CLIENTES
select C.nombre,D.direccion,T.telefono
from (Clientes C left join DireccionC D on C.nombre=D.nombreC)
left join TelefonosD T on D.direccion=T.direccion and D.nombreC=T.nombreC;


----------TABLA DE VENTAS POR CLIENTES
select nombreC as Cliente, count(nombreC) as Cantidad_Ventas , sum(total_Venta) || '  $' as Total_Ingresado
from Ventas
group by nombreC

----------TABLA DE VENTAS POR PRODUCTOS
select nombreP as Producto, count(nombreP) as Cantidad_Ventas , sum(total_Venta) || '  $' as Total_Ingresado
from Ventas
group by nombreP


--SELECCIONAR TODAS LAS VENTAS DESDE UNA FECHA PARTICUALR por producto
select nombreP as Producto, count(nombreP) as Cantidad_Ventas ,
sum(cantidad_Producto) as Cantidad_Prod_Comprados,
sum(total_Venta) || '  $' as Total_Ingresado
from Ventas  
where substr(fecha_Venta,7) || substr(fecha_Venta,4,2) || substr (fecha_Venta,1,2) 
		between '20220913' and '20220913'
group by nombreP;

--SELECCIONAR TODAS LAS VENTAS DESDE UNA FECHA PARTICUALR por cliente
ct nombreC as Cliente, count(nombreC) as Cantidad_Ventas ,
sum(cantidad_Producto) as Cantidad_Prod_Comprados,
sum(total_Venta) || '  $' as Total_Ingresado 
from Ventas  
where substr(fecha_Venta,7) || substr(fecha_Venta,4,2) || substr (fecha_Venta,1,2) 
		between '20220913' and '20220913'
group by nombreC;


--SELECCIONAR QUIEN TIENE BIDONES
select nombreC as Cliente , nombreP as Producto,sum(cantidad_Producto) as Cantidad_Bidones
from Ventas
where (substr(fecha_Venta,7) || substr(fecha_Venta,4,2) || substr (fecha_Venta,1,2) 
		between '20221001' and '20221001')
		and
		nombreP='Bidon Agua de Mesa 20 [L]'
group by nombreC;
