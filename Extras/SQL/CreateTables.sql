PRAGMA foreign_keys = ON;

CREATE TABLE "Clientes" (
	"nombre"	TEXT,
	PRIMARY KEY("nombre")
);

CREATE TABLE "DireccionC" (
	"nombreC"	TEXT,
	"direccion"	TEXT,
	PRIMARY KEY("direccion","nombreC"),
	FOREIGN KEY("nombreC") REFERENCES "Clientes"("nombre")
);

CREATE TABLE "TelefonosD" (
	"nombreC"	TEXT,
	"direccion"	TEXT,
	"telefono"	TEXT,
	PRIMARY KEY("nombreC","direccion","telefono"),
	FOREIGN KEY("nombreC","direccion") REFERENCES "DireccionC"("nombreC","direccion") ON UPDATE CASCADE
); 

CREATE TABLE "Productos" (
	"nombre"	TEXT,
	"precio"	REAL NOT NULL,
	"precio_mayorista"	REAL,
	"cant_Stock"	INTEGER DEFAULT 0,
	"ultima_Actualizacion"	TEXT,
	PRIMARY KEY("nombre")
);

CREATE TABLE "Ventas" (
	"id_Venta"	INTEGER,
	"cantidad_Producto"	INTEGER NOT NULL CHECK("cantidad_Producto" > 0),
	"precio_Producto"	REAL NOT NULL CHECK("precio_Producto" >= 0),
	"fecha_Venta"	TEXT NOT NULL,
	"hora_Venta"	TEXT NOT NULL,
	"nombreP"	TEXT NOT NULL,
	"nombreC"	TEXT NOT NULL,
	"total_Venta"	AS (cantidad_Producto*precio_Producto ),
	PRIMARY KEY("id_Venta" AUTOINCREMENT),
	FOREIGN KEY("nombreC") REFERENCES "Clientes"("nombre"),
	FOREIGN KEY("nombreP") REFERENCES "Productos"("nombre")
)
