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
    //Recibe un puntero a un arreglo de las ventas y un entero que ilustra el tamaño del arreglo
    //Muestra al usuario los prodyctos vendidos y la cantidad de cada uno
    if (Ventas != NULL) {
        int i = 0;
        for (i ; i < Tam; i++) {
            printf("Producto = %s\n", Ventas[i]->nombre);
            printf("Cantidad = %d\n\n", Ventas[i]->cantidad);
        }
    }
}

void mostrarVentas(struct NodoCompraVenta *Ventas) {
    //Recibe un puntero a un struct de NodoCompraVentas
    //Muestra por pantalla al usuario los productos de cada venta
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
    //Recibe un puntero a un struct NodoProducto, un entero que indica si estamos buscando los pproductos con o sin stock y un contador para tener registro de si hay o no hay productos con esa condicion
    //Luego de eso se dedica a imprimir el nombre de los productos que cumplan las condiciones junto a sus respectvas cantidades
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
    //Recibe un puntero a un arreglo de farmacia
    //Imprimiendo por pantalla todos los productos que tienen stock en ese momento
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
    //Recibe un puntero a un arreglo de farmacia
    //Imprimiendo por pantalla todos los productos que no tienen stock en ese momento
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
    //
    //
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
    //Recibe un puntero a un struct NodoProducto y un int llamado promedio
    //Se dedica a imprimir por pantalla todos los productos que esten por debajo del promedio
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
    //Recibe un puntero a un struck de farmacia, sacando el promedio
    //Y mostrando por pantalla si es que hay o no hay productos con bajo stock

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

int compararCodigoProductos(char *codigo1, char *codigo2) {
    // Recibe dos cadenas de caracteres codigo1 y codigo2, correspondientes a numeros de 10 digitos, compara sus
    // valores. Retorna 1 si el valor numerico de codigo1 es mayor al valor numerico de codigo2, -1 si el valor numerico
    // de codigo1 es menor al valor numerico de codigo2, o 0 en caso de tener mismo valor numerico.
    char digito1[] = "\0\0", digito2[] = "\0\0";
    int i;
    for (i = 0; i < 10; i++) {
        digito1[0] = codigo1[i];
        digito2[0] = codigo2[i];
        if (strcmp(digito1, digito2) != 0)
            return strcmp(digito1, digito2);
    }
    return 0;
}


int cantidadProductoEnVentas(struct Producto * producto, struct NodoCompraVenta * Ventas) {
    //Lo que recibe esta funcion es un puntero a un struct producto y un puntero a un struct nodoCompraVenta (las ventas de una farmacia)
    //El retorno es la cantidad de veces que ese producto fue vendido en la farmacia
    struct NodoCompraVenta * rec = Ventas;
    int cantVentas = 0;
    while (rec != NULL) {
        int i = 0;
        for (i = 0; i < rec->datosCompraVenta->totalProductosDistintos; i++) {
            if (compararCodigoProductos(producto->codigo, rec->datosCompraVenta->productos[i]->codigo) == 0) {
                cantVentas += rec->datosCompraVenta->productos[i]->cantidad;
            }
        }
        rec = rec->sig;
    }
    return cantVentas;
}




void MenosVendidoVentas(struct NodoProducto * inventario, struct NodoCompraVenta * ventas, int *cantidad, struct Producto **menosVendido, int *contador) {
    //Esta funcion recibe un puntero a un struct nodoProducto (el inventario) y un puntero a un struct nodocompraventa (las ventas de una farmacia),
    // a la vez que un puntero a un entero (cantidad), un puntero a un struct Producto y un puntero a un entero
    //Al ser una funcion tipo void no tiene un retorno claro, ya que se dedica a actualizar las variables de cantidad y producto para encontrar el producto con mas ventas dentro de la farmacia
    if (inventario != NULL) {
        int cantCandidato = buscarProductoEnVentas(inventario->datosProducto, ventas);
        if (cantCandidato != 0) {
            if (contador == 0) {
                (*menosVendido) = inventario->datosProducto;
                (*cantidad) = cantCandidato;
                (*contador)++;
            } else {
                if (cantCandidato < (*cantidad)) {
                    *menosVendido = inventario->datosProducto;
                    (*cantidad) = cantCandidato;
                }
            }
        }
        MenosVendidoVentas(inventario->izq, ventas, cantidad, menosVendido, contador);
        MenosVendidoVentas(inventario->der, ventas, cantidad, menosVendido, contador);
    }
}

void MasVendidoVentas(struct NodoProducto * inventario, struct NodoCompraVenta * ventas, int *cantidad, struct Producto **masVendido) {
    //Esta funcion recibe un puntero a un struct nodoProducto (el inventario) y un puntero a un struct nodocompraventa (las ventas de una farmacia),
    // a la vez que un puntero a un entero (cantidad) y un puntero a un struct Producto
    //Al ser una funcion tipo void no tiene un retorno claro, ya que se dedica a actualizar las variables de cantidad y producto para encontrar el producto con mas ventas dentro de la farmacia
    if (inventario != NULL) {
        int cantCandidato = buscarProductoEnVentas(inventario->datosProducto, ventas);
        if (cantCandidato > (*cantidad)) {
            *masVendido = inventario->datosProducto;
            (*cantidad) = cantCandidato;
        }
        MasVendidoVentas(inventario->izq, ventas, cantidad, masVendido);
        MasVendidoVentas(inventario->der, ventas, cantidad, masVendido);
    }
}

struct Producto * getProductoMasVendido(struct NodoProducto * inventario, struct NodoCompraVenta * ventas) {
    //Recibe un puntero a un struct de la farmacia y a las ventas de esta misma recorriendo cada una de ellas y
    // retornando el producto mas vendido en la farmacia
    int cantidad = 0;
    struct Producto *masVendido = NULL;
    MasVendidoVentas(inventario, ventas, &cantidad, &masVendido);

    return masVendido;
}


struct Producto *getProductoMenosVendido(struct NodoProducto * inventario, struct NodoCompraVenta * ventas) {
    //Recibe un puntero a un struct de la farmacia y a las ventas de esta misma recorriendo cada una de ellas y
    //retornando el producto menos vendido en la farmacia
    int cantidad;
    int contador = 0;
    struct Producto *menosVendido = NULL;
    MenosVendidoVentas(inventario, ventas, &cantidad, &menosVendido, &contador);

    return menosVendido;
}

