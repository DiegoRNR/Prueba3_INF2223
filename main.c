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



void mostrarArregloVentas(struct Producto **Ventas, int Tam) {
    if (Ventas != NULL) {
        int i = 0;
        for (i ; i < Tam; i++) {
            printf("Producto = %s\n", Ventas[i]->nombre);
            printf("Cantidad = %d\n\n", Ventas[i]->cantidad);
        }
    }
}

void mostrarVentas(struct NodoCompraVenta *Ventas) {
    if (Ventas != NULL) {
        struct NodoCompraVenta *rec = Ventas;
        printf("Cantidad de Productos Totales = %d\n", Ventas->datosCompraVenta->cantidadProductos);

        while (rec != NULL) {
            mostrarArregloVentas(rec->datosCompraVenta->productos, rec->datosCompraVenta->totalProductosDistintos);
            rec = rec->sig;
        }
    }
}


void recorrerInventario(struct NodoProducto* nodo, int condicion, int* contador) {
    if (nodo != NULL) {
        if (condicion == 1) { // Productos con stock
            if (nodo->datosProducto->cantidad > 0) {
                printf("Nombre = %s\n", nodo->datosProducto->nombre);
                printf("Cantidad = %d\n\n", nodo->datosProducto->cantidad);
                (*contador)++;
            }
        } else { // Productos sin stock
            if (nodo->datosProducto->cantidad == 0) {
                printf("Nombre = %s\n", nodo->datosProducto->nombre);
                (*contador)++;
            }
        }
        recorrerInventario(nodo->izq, condicion, contador);
        recorrerInventario(nodo->der, condicion, contador);
    }
}

void mostrarProductosEnStock(struct Farmacia *farmacia) {
    if (farmacia != NULL) {
        int contador;
        printf("Productos con Stock en Farmacia Seleccionada:\n\n");
        recorrerInventario(farmacia->inventario, 1, &contador);
        if (contador == 0){
            printf("No se encontraron Productos con Stock\n");
        }
    }
}

void mostrarProductoSinStock(struct Farmacia *farmacia) {
    if (farmacia != NULL) {
        int contador;
        printf("Productos sin Stock en Farmacia Seleccionada\n\n");
        recorrerInventario(farmacia->inventario, 0, &contador);
        if (contador == 0){
            printf("No se encuentran Productos sin Stock\n");
        }
    }
}

int calcularPromedioStock(struct NodoProducto* inventario) {

    int contador = 0;
    int cantidad = 0;
    if (inventario != NULL) {

        if (inventario->datosProducto->cantidad != 0) {
            contador++;
            cantidad += inventario->datosProducto->cantidad;
        }
        calcularPromedioStock(inventario->izq);
        calcularPromedioStock(inventario->der);
    }
    if (contador != 0) {
        return (cantidad / contador);
    }
    else return 0;
}

void recorrerArbol(struct NodoProducto* inventario, int promedio) {
    if (inventario != NULL) {

        if (promedio > inventario->datosProducto->cantidad){
            printf("Nombre = %s\n", inventario->datosProducto->nombre);
            printf("Cantidad = %d\n\n", inventario->datosProducto->cantidad);
        }
        recorrerArbol(inventario->izq, promedio);
        recorrerArbol(inventario->der, promedio);
    }
}

void mostrarProductosConPocoStock(struct Farmacia * farmacia) {

    if (farmacia != NULL) {
        int promedio = calcularPromedioStock(farmacia->inventario);
        if (promedio > 0){
            printf("Productos con Stock Bajo en Farmacia Seleccionada\n\n");
            recorrerArbol(farmacia->inventario, promedio);
        }
        else {
            printf("No se encuentran productos con Bajo Stock");
        }

    }

}
