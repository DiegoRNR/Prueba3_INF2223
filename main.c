//
// Created by Diego Negrín on 22-05-24.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    struct Producto *datosProducto;
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
    struct CompraVenta *datosCompraVenta;
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
    char buffer[100], caracter, *cadena = NULL;
    int i;
    for (i = 0; i < 100 && caracter != '\0'; i++) {
        caracter = getchar();
        if (caracter == '\n')
            caracter = '\0';
        buffer[i] = caracter;
    }
    strcpy(cadena, buffer);
    return cadena;
}

struct Fecha *leerFecha() {
    struct Fecha *fecha;
    int dia, mes, year;
    scanf("%d", &dia);
    scanf("%d", &mes);
    scanf("%d", &year);
    fecha = (struct Fecha *) malloc(sizeof(struct Fecha));
    fecha->dia = dia;
    fecha->mes = mes;
    fecha->year = year;
    return fecha;
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

struct NodoFarmacia *crearNodoFarmacia(struct Farmacia *farmacia) {
    struct NodoFarmacia *nuevaFarmacia = NULL;
    if (farmacia != NULL) {
        nuevaFarmacia = (struct NodoFarmacia *) malloc(sizeof(struct NodoFarmacia));
        nuevaFarmacia->datosFarmacia = farmacia;
        nuevaFarmacia->ant = NULL;
        nuevaFarmacia->sig = NULL;
    }
    return nuevaFarmacia;
}

struct Producto *crearProducto() {
    struct Producto *nuevoProducto;
    char *nombre, *categoria, *descripcion, *proveedor, *codigo;
    int precio, requiereReceta;
    nombre = leerCadena();
    categoria = leerCadena();
    descripcion = leerCadena();
    proveedor = leerCadena();
    codigo = leerCadena();
    scanf("%d", &precio);
    scanf("%d", &requiereReceta);
    nuevoProducto = (struct Producto *) malloc(sizeof(struct Producto));
    nuevoProducto->nombre = nombre;
    nuevoProducto->categoria = categoria;
    nuevoProducto->descripcion = descripcion;
    nuevoProducto->proveedor = proveedor;
    strcpy(nuevoProducto->codigo, codigo);
    nuevoProducto->precio = precio;
    nuevoProducto->requiereReceta = requiereReceta;
    nuevoProducto->cantidad = 0;
    nuevoProducto->lotes = NULL;
    return nuevoProducto;
}

struct NodoProducto *crearNodoProducto(struct Producto *producto) {
    struct NodoProducto *nuevoNodo = NULL;
    if (producto != NULL) {
        nuevoNodo = (struct NodoProducto *) malloc(sizeof(struct NodoProducto));
        nuevoNodo->datosProducto = producto;
        nuevoNodo->izq = NULL;
        nuevoNodo->der = NULL;
    }
    return nuevoNodo;
}

struct Lote *crearLote() {
    struct Lote *nuevoLote;
    struct Fecha *fechaCaducidad;
    int numeroLote, cantidadLote;
    scanf("%d", &numeroLote);
    scanf("%d", &cantidadLote);
    fechaCaducidad = leerFecha();
    nuevoLote = (struct Lote *) malloc(sizeof(struct Lote));
    nuevoLote->numeroLote = numeroLote;
    nuevoLote->cantidadLote = cantidadLote;
    nuevoLote->fechaCaducidad = (struct Fecha *) malloc(sizeof(struct Fecha));
    nuevoLote->fechaCaducidad = fechaCaducidad;
    return nuevoLote;
}

struct NodoLote *crearNodoLote(struct Lote *lote) {
    struct NodoLote *nuevoNodo = NULL;
    if (lote != NULL) {
        nuevoNodo = (struct NodoLote *) malloc(sizeof(struct NodoLote));
        nuevoNodo->datosLote = lote;
        nuevoNodo->sig = NULL;
    }
    return nuevoNodo;
}

struct Producto **getArregloProductos(int totalProductos) {
    struct Producto **arregloProductos, *producto;
    int i;
    arregloProductos = (struct Producto **) malloc(totalProductos * sizeof(struct Producto *));
    for (i = 0; i < totalProductos; i++) {
        producto = crearProducto();
        arregloProductos[i] = producto;
    }
    return arregloProductos;
}

struct CompraVenta *crearCompraVenta() {
    struct CompraVenta *nuevaCompraVenta;
    struct Producto **productos;
    struct Fecha *fechaSolicitud;
    int cantidadProductos;
    char *nombre, estadoEnvio;
    scanf("%d", &cantidadProductos);
    scanf("%c ", &estadoEnvio);
    nombre = leerCadena();
    fechaSolicitud = leerFecha();
    productos = getArregloProductos(cantidadProductos);
    nuevaCompraVenta = (struct CompraVenta *) malloc(sizeof(struct CompraVenta));
    nuevaCompraVenta->nombre = nombre;
    nuevaCompraVenta->cantidadProductos = cantidadProductos;
    nuevaCompraVenta->productos = productos;
    nuevaCompraVenta->estadoEnvio = estadoEnvio;
    nuevaCompraVenta->fechaSolicitud = fechaSolicitud;
    nuevaCompraVenta->fechaLlegada = NULL;
    return nuevaCompraVenta;
}

struct NodoCompraVenta *crearNodoCompraVenta(struct CompraVenta *compraVenta) {
    struct NodoCompraVenta *nuevoNodo = NULL;
    if (compraVenta != NULL) {
        nuevoNodo = (struct NodoCompraVenta *) malloc(sizeof(struct NodoCompraVenta));
        nuevoNodo->datosCompraVenta = compraVenta;
        nuevoNodo->sig = NULL;
    }
    return nuevoNodo;;
}



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