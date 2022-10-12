delete from TelefonosD;
delete from DireccionC;
delete from Clientes;

UPDATE sqlite_sequence SET seq = 0 WHERE name = 'Clientes';

INSERT INTO Clientes (nombre)
VALUES ('Particular');



INSERT INTO Clientes (nombre)
VALUES ('Empresa Jullier');

INSERT INTO DireccionC ( nombreC , direccion)
VALUES ( 'Empresa Jullier' , 'Direccion 1');

INSERT INTO DireccionC ( nombreC , direccion)
VALUES ( 'Empresa Jullier' , 'Direccion 1.2');

INSERT INTO TelefonosD ( nombreC , direccion , telefono)
VALUES ( 'Empresa Jullier' , 'Direccion 1', '123456789');

INSERT INTO TelefonosD ( nombreC , direccion , telefono)
VALUES ( 'Empresa Jullier' , 'Direccion 1' , '987654321');



INSERT INTO Clientes (nombre)
VALUES ('Scalzo S.A.');

INSERT INTO DireccionC ( nombreC , direccion)
VALUES ( 'Scalzo S.A.' , 'Direccion 2');

INSERT INTO TelefonosD (nombreC , direccion , telefono)
VALUES ( 'Scalzo S.A.' ,'Direccion 2', '567891234');




