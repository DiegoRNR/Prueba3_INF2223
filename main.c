//
// Created by Diego Negrín on 22-05-24.
//
#include <stdio.h>
#include <stdlib.h>

struct Fecha {
    int dia;
    int mes;
    int year;
};

struct Lote {
    int numeroLote;
    int cantidadLote;
    struct Fecha *fechaCaducidad;
};

struct Producto {
    int codigo;
    char *nombre;
    char *categoria;
    int precio;
    char *descripcion;
    char *proveedor;
    int cantidad;
    int requiereReceta;
    struct Lote *lotes;
};

struct NodoProducto {
    struct Producto *producto;
    struct NodoProducto *izq, *der;
};

struct CompraVenta {
    char *nombre;
    int cantidadProductos;
    struct Producto **productos;
};

struct NodoCompraVenta {
    struct CompraVenta *producto;
    struct NodoCompraVenta *sig;
};

struct Farmacia {
    char id[11];
    char *ciudad;
    char *region;
    struct NodoProducto *inventario;
    int totalProductos;
    struct NodoCompraVenta *ventas, *compras;
};

struct NodoFarmacia {
    struct Farmacia *datosFarmacia;
    struct NodoFarmacia *ant, *sig;
};

int main(void) {

    return 0;
}