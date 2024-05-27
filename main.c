//
// Created by Diego Negrín on 22-05-24.
//
#include <stdio.h>
#include <stdlib.h>
#define CAPACIDAD_PROMEDIO 10000

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

struct NodoLote {
    struct Lote *datosLote;
    struct NodoLote *sig;
};

struct Producto {
    char codigo[11];
    char *nombre;
    char *categoria;
    int precio;
    char *descripcion;
    char *proveedor;
    int cantidad;
    int requiereReceta;
    struct NodoLote *lotes;
};

struct NodoProducto {
    struct Producto *producto;
    struct NodoProducto *izq, *der;
};

struct CompraVenta {
    char *nombre;
    int cantidadProductos;
    struct Producto **productos;
    char estadoEnvio;
    struct Fecha *fechaSolicitud, *fechaLlegada;
};

struct NodoCompraVenta {
    struct CompraVenta *producto;
    struct NodoCompraVenta *sig;
};

struct Farmacia {
    char *id;
    char *ciudad;
    char *region;
    struct NodoProducto *inventario;
    int maxCapacidad;
    int totalProductos;
    struct NodoCompraVenta *ventas, *compras;
};

struct NodoFarmacia {
    struct Farmacia *datosFarmacia;
    struct NodoFarmacia *ant, *sig;
};

char *leerCadena() {

}

struct Farmacia *crearFarmacia() {
    struct Farmacia *nuevaFarmacia;
    char *id, *ciudad, *region;
    int totalProductos;
    region = leerCadena();
    ciudad = leerCadena();
    id = leerCadena();
    nuevaFarmacia = (struct Farmacia *) malloc(sizeof(struct Farmacia));
    nuevaFarmacia->id = id;
    nuevaFarmacia->ciudad = ciudad;
    nuevaFarmacia->region = region;
    nuevaFarmacia->maxCapacidad = CAPACIDAD_PROMEDIO;
    nuevaFarmacia->ventas = NULL;
    nuevaFarmacia->compras = NULL;
    return nuevaFarmacia;
};

int buscarProducto() {

}

char *categoriaMasVendida() {

}

struct Producto *getProductoMasVendido() {

}

struct Producto *getProductoMenosVendido() {

}

void mostrarVentas() {

}

void mostrarVentasConReceta() {

}

void mostrarCompras() {

}

void mostrarProductosEnStock() {

}

void mostrarProductoSinStock() {

}

void mostrarProductosConPocoStock() {

}

void mostrarInfoProducto() {

}

void analisisFarmacia() {

}

int main(void) {

    return 0;
}