//
// Created by Diego Negrín on 22-05-24.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define CAPACIDAD_PROMEDIO 10000

struct Lote {
    char *numeroLote;
    int cantidadLote;
    char *fechaCaducidad;
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

struct Transaccion {
    int id;
    char *nombre;
    char *rut;
    int cantidadProductos;
    int totalProductosDistintos;
    struct Producto **productos;
    int costoTotal;
    char estadoEnvio;
    char *fechaSolicitud;
    char *fechaLlegada;
};

struct NodoTransaccion {
    struct Transaccion *datosTransaccion;
    struct NodoTransaccion *sig;
};

struct Farmacia {
    char *id;
    char *ciudad;
    char *region;
    struct NodoProducto *inventario;
    int maxCapacidad;
    int totalProductos;
    struct NodoTransaccion *ventas, *compras;
};

struct NodoFarmacia {
    struct Farmacia *datosFarmacia;
    struct NodoFarmacia *ant, *sig;
};

struct FarmaSalud {
    struct NodoFarmacia *headFarmacias;
};


void toUpperString(char *cadena) {
    // Recibe un puntero a una cadena de caracteres, convierte todos los caracteres de la cadena a mayusculas.
    int i;
    if (cadena != NULL) {
        for (i = 0; i < strlen(cadena); i++)
            cadena[i] = toupper(cadena[i]);
    }
}

char *leerCadena() {
    // Lee una cadena de caracteres en un buffer largo 100, y la copia en un puntero con el largo exacto
    // de la cadena leida. Retorna el puntero donde se copio la cadena leida.
    char buffer[101], *cadena;
    scanf(" %100[^\n]s", buffer);
    cadena = (char *) malloc(sizeof(char)*(strlen(buffer) + 1));
    strcpy(cadena, buffer);
    toUpperString(cadena);
    return cadena;
}

struct FarmaSalud *crearFarmasalud() {
    // Función para crear y asignar memoria al sistema de FarmaSalud
    struct FarmaSalud *nuevoFarmaSalud;
    nuevoFarmaSalud = (struct FarmaSalud *) malloc(sizeof(struct FarmaSalud));
    nuevoFarmaSalud->headFarmacias = NULL;
    return nuevoFarmaSalud;
}

struct Lote *crearLote(char *numeroLote, int cantidadLote, char *fechaCaducidad) {
    // Recibe datos de un lote, asigna los datos recibidos a un struct Lote.
    // Retorna un puntero al struct Lote al que se asignaron los datos leidos.
    struct Lote *nuevoLote;
    nuevoLote = (struct Lote *) malloc(sizeof(struct Lote));
    nuevoLote->numeroLote = numeroLote;
    nuevoLote->cantidadLote = cantidadLote;
    nuevoLote->fechaCaducidad = fechaCaducidad;
    return nuevoLote;
}

struct Lote *leerDatosLote() {
    // Lee datos de un lote por la entrada del usuario.
    // Retorna un puntero al struct Lote que contiene los datos leidos.
    char *fechaCaducidad, *numeroLote;
    int cantidadLote;

    printf("Numero de lote: ");
    numeroLote = leerCadena();
    printf("Cantidad de productos del lote: ");
    scanf("%d", &cantidadLote);
    printf("Fecha de caducidad del lote (DD/MM/AAAA): ");
    fechaCaducidad = leerCadena();
    return crearLote(numeroLote, cantidadLote, fechaCaducidad);
}

struct NodoLote *crearNodoLote(struct Lote *lote) {
    // Recibe un struct Lote y lo asigna a un struct NodoLote.
    // Retorna un puntero al struct NodoLote que contiene el struct Lote recibido.
    struct NodoLote *nuevoNodo = NULL;
    if (lote != NULL) {
        nuevoNodo = (struct NodoLote *) malloc(sizeof(struct NodoLote));
        nuevoNodo->datosLote = lote;
        nuevoNodo->sig = NULL;
    }
    return nuevoNodo;
}

struct Lote *getLote(struct NodoLote *head, char *numeroLoteBuscado) {
    // Recibe una lista simplemente enlazada de struct NodoLote y un numero de lote, busca el elemento que posea dichp
    // numero de lote. Retorna un puntero a struct Lote si se encuentra en la lista, en caso contrario retorna NULL.
    struct NodoLote *rec;
    if (head != NULL) {
        rec = head;
        while (rec != NULL) {
            if (strcmp(rec->datosLote->numeroLote, numeroLoteBuscado) == 0)
                return rec->datosLote;
            rec = rec->sig;
        }
    }
    return NULL;
}

struct NodoLote *getNodoLote(struct NodoLote *head, char *numeroLoteBuscado) {
    // Recibe una lista simplemente enlazada de struct NodoLote y un numero de lote, busca el elemento que posea dicho
    // numero de lote. Retorna un puntero a struct NodoLote si se encuentra en la lista, en caso contrario retorna NULL.
    struct NodoLote *rec;
    if (head != NULL) {
        rec = head;
        while (rec != NULL) {
            if (strcmp(rec->datosLote->numeroLote, numeroLoteBuscado) == 0)
                return rec;
            rec = rec->sig;
        }
    }
    return NULL;
}

int agregarNodoLote(struct NodoLote **head, struct NodoLote *nuevoNodo) {
    // Recibe una lista simplemente enlazada de struct NodoLote y un puntero a struct NodoLote, agrega el nodo recibido
    // a la lista, en caso de ya existir en la lista suma los campos cantidadLote.
    // Retorna 1 en caso de exito, en caso contrario retorna 0.
    struct NodoLote *rec;
    if (nuevoNodo != NULL) {
        if (*head == NULL) {
            *head = nuevoNodo;
        } else if (getLote(*head, nuevoNodo->datosLote->numeroLote) == NULL) {
            rec = *head;
            while (rec->sig != NULL) {
                rec = rec->sig;
            }
            rec->sig = nuevoNodo;
        } else {
            rec = getNodoLote(*head, nuevoNodo->datosLote->numeroLote);
            rec->datosLote->cantidadLote += nuevoNodo->datosLote->cantidadLote;
        }
        return 1;
    }
    return 0;
}

int getCantidadProducto(struct NodoLote *lotesProducto) {
    // Recibe una lista simplemente enlazada de struct NodoLote.
    // Retorna la suma del campo cantidadLote de todos sus elementos.
    struct NodoLote *rec;
    int totalStock = 0;
    if (lotesProducto != NULL) {
        rec = lotesProducto;
        while (rec != NULL) {
            totalStock += rec->datosLote->cantidadLote;
            rec = rec->sig;
        }
    }
    return totalStock;
}

int getTotalStockFarmacia(struct NodoProducto *inventario) {
    // Recibe un arbol binario de busqueda de struct NodoProducto.
    // Retorna la suma del campo cantidad de todos los productos en el arbol.
    if (inventario == NULL)
        return 0;
    return getTotalStockFarmacia(inventario->izq) + getTotalStockFarmacia(inventario->der) +
           getCantidadProducto(inventario->datosProducto->lotes);
}

struct Producto *crearProducto(char *codigo, char *nombre, char *categoria, char *descripcion, char *proveedor,
                               int precio, int requiereReceta) {
    // Recibe datos de producto, asigna los datos recibidos a un struct Producto.
    // Retorna un puntero al struct Producto al que se asignaron los datos recibidos.
    struct Producto *nuevoProducto;
    nuevoProducto = (struct Producto *) malloc(sizeof(struct Producto));
    strcpy(nuevoProducto->codigo, codigo);
    nuevoProducto->nombre = nombre;
    nuevoProducto->categoria = categoria;
    nuevoProducto->descripcion = descripcion;
    nuevoProducto->proveedor = proveedor;
    nuevoProducto->precio = precio;
    nuevoProducto->requiereReceta = requiereReceta;
    nuevoProducto->lotes = NULL;
    nuevoProducto->cantidad = 0;
    return nuevoProducto;
}

struct Producto *leerDatosProducto(char *codigo, char *proveedorCompra) {
    // Recibe un codigo, lee datos de la entrada del usuario acerca del producto correspondientes al codigo recibido.
    // Retorna un puntero al struct Producto que contiene los datos leidos.
    char *nombre, *categoria, *descripcion, *proveedor, aux;
    int precio, requiereReceta;
    printf("Nombre del producto: ");
    nombre = leerCadena();
    printf("Categoria del producto: ");
    categoria = leerCadena();
    printf("Descripcion del producto: ");
    descripcion = leerCadena();
    if (proveedorCompra == NULL) {
        printf("Proveedor del producto: ");
        proveedor = leerCadena();
    } else {
        proveedor = proveedorCompra;
    }
    printf("Precio del producto: ");
    scanf("%d%c", &precio, &aux);
    printf("Producto requiere receta? (0/1): ");
    scanf("%d%c", &requiereReceta, &aux);
    return crearProducto(codigo, nombre, categoria, descripcion, proveedor, precio, requiereReceta);
}

struct NodoProducto *crearNodoProducto(struct Producto *producto) {
    // Recibe un puntero a struct Producto y lo asigna a un struct NodoProducto.
    // Retorna un puntero a struct NodoProducto que contiene el puntero a struct Producto recibido.
    struct NodoProducto *nuevoNodo = NULL;
    if (producto != NULL) {
        nuevoNodo = (struct NodoProducto *) malloc(sizeof(struct NodoProducto));
        nuevoNodo->datosProducto = producto;
        nuevoNodo->izq = NULL;
        nuevoNodo->der = NULL;
    }
    return nuevoNodo;
}

struct Producto *getProducto(struct NodoProducto *root, char *codigoBuscado) {
    // Recibe un arbol binario de busqueda de struct NodoProducto y un codigo, busca el elemento que posea dicho codigo.
    // Retorna un puntero a struct Producto si se encuentra el elemento en el arbol, en caso contrarip retorna NULL.
    if (!root)
        return NULL;
    if (strcmp(root->datosProducto->codigo, codigoBuscado) == 0)
        return root->datosProducto;
    if (strcmp(root->datosProducto->codigo, codigoBuscado) > 0)
        return getProducto(root->izq, codigoBuscado);
    return getProducto(root->der, codigoBuscado);
}

struct NodoProducto *getPadreACodigo(struct NodoProducto *root, char *codigo) {
    // Recibe un arbol binario de busqueda de struct NodoProducto y un codigo, busca al posible nodo padre de dicho
    // codigo. Retorna un puntero a struct NodoProducto si existe el posible padre, en caso contrario retorna NULL.
    if (!root || strcmp(root->datosProducto->codigo, codigo) == 0)
        return NULL;
    if (strcmp(root->datosProducto->codigo, codigo) > 0) {
        if (root->izq == NULL || strcmp(root->izq->datosProducto->codigo, codigo) == 0)
            return root;
        return getPadreACodigo(root->izq, codigo);
    }
    if (root->der == NULL || strcmp(root->der->datosProducto->codigo, codigo) == 0)
        return root;
    return getPadreACodigo(root->der, codigo);
}

int agregarNodoProducto(struct NodoProducto **root, struct NodoProducto *nuevoNodo) {
    // Recibe un arbol binario de busqueda de struct NodoProducto y un puntero a struct NodoProducto, agrega el nodo
    // recibido al arbol. Retorna 1 en caso de exito, en caso contrario retorna 0.
    struct NodoProducto *nodoPadre;
    if (nuevoNodo != NULL) {
        if (*root == NULL) {
            *root = nuevoNodo;
            return 1;
        }
        if (getProducto(*root, nuevoNodo->datosProducto->codigo) == NULL) {
            nodoPadre = getPadreACodigo(*root, nuevoNodo->datosProducto->codigo);
            if (strcmp(nodoPadre->datosProducto->codigo, nuevoNodo->datosProducto->codigo) > 0) {
                nodoPadre->izq = nuevoNodo;
            } else {
                nodoPadre->der = nuevoNodo;
            }
            return 1;
        }
    }
    return 0;
}

// TODO: Decidir cuál agregarNodoProducto
//int agregarNodoProducto(struct NodoProducto **root, struct NodoProducto *nuevoNodo) {
//    // Recibe la dirección de la raíz del inventario y un puntero a un nodo de producto
//    // Agrega el nodo recibido al inventario
//    // Retorna 1 en caso de éxito, 0 en caso contrario
//    if (nuevoNodo) {
//        if (!(*root)) {
//            *root = nuevoNodo;
//            return 1;
//        }
//        if (strcmp(nuevoNodo->datosProducto->codigo, (*root)->datosProducto->codigo) < 0)
//            return agregarNodoProducto(&(*root)->izq, nuevoNodo);
//        else if (strcmp(nuevoNodo->datosProducto->codigo, (*root)->datosProducto->codigo) > 0)
//            return agregarNodoProducto(&(*root)->der, nuevoNodo);
//    }
//    return 0;
//}

struct Producto *getMenorValor(struct NodoProducto *root) {
    // Función para buscar el valor más pequeño en un ABB
    // Recibe un puntero a un nodo de producto
    // Retorna el producto con el código más pequeño
    if (root) {
        while (root && root->izq) {
            root = root->izq;
        }
        return root->datosProducto;
    }
    return NULL;
}

struct Producto *quitarProducto(struct NodoProducto **root, char *codigo) {
    // Función para quitar un producto del inventario
    // Recibe la raíz del inventario y retorna el producto eliminado
    struct Producto *temp, *aux;
    if (!(*root)) {
        printf("No existen productos en el inventario.\n\n");
        return NULL;
    }

    // Si código es menor o mayor, buscarlo en sub-árbol correspondiente
    if (strcmp(codigo, (*root)->datosProducto->codigo) < 0)
        return quitarProducto(&(*root)->izq, codigo);
    else if (strcmp(codigo, (*root)->datosProducto->codigo) < 0)
        return quitarProducto(&(*root)->der, codigo);

    // Si nodo actual es el buscado, podemos eliminar, pero antes guardamos sus datos para retornar
    temp = (*root)->datosProducto;

    // Si no tiene hijo izquierdo o no tiene hijos en general
    if (!(*root)->izq) {
        // Asignamos hijo derecho como reemplazo del nodo
        *root = (*root)->der;
    }
    // Si tiene hijo izquiero pero no derecho
    else if (!(*root)->der) {
        // Asignamos hijo izquiero como reemplazo del nodo
        *root = (*root)->izq;
    }
    // Si tiene ambos hijos debemos buscar un reemplazo
    else {
        // Buscamos el menor valor del sub-árbol derecho, por lo tanto su sucesor por definición de ABB
        aux = getMenorValor((*root)->der);
        // Asignamos sus datos al nodo actual
        (*root)->datosProducto = aux;
        // Ahora tenemos 2 nodos con los mismos datos, así que eliminamos el nodo sucesor que encontramos antes
        quitarProducto(&(*root)->der, aux->codigo);
    }

    // Retornamos los datos
    return temp;
}

int cadenaNumerica(char *cadena) {
    // Recibe una cadena, verifica si todos sus caracteres son numericos.
    // Retorna 1 en caso de ser verdaderom en caso contraario retorna 0.
    int i;
    if (cadena != NULL) {
        for (i = 0; i < strlen(cadena); i++) {
            if (!isdigit(cadena[i]))
                return 0;
        }
    }
    return 1;
}

char *lecturaCodigo() {
    // Lee un codigo de 10 digitos de la entrada del usuario y lo copia en un puntero a char.
    // Retorna el puntero donde se copio el codigo leido.
    char *codigo;
    do {
        printf("\nIngrese codigo del producto (10 digitos): ");
        codigo = leerCadena();
        if (strlen(codigo) != 10 || !cadenaNumerica(codigo))
            printf("Codigo no válido, por favor ingrese un codigo de 10 digitos.\n");
    } while (strlen(codigo) != 10 || !cadenaNumerica(codigo));
    return codigo;
}

char lecturaReceta() {
    // Lee un caracter de la entrada del usuario, verifica si es 's' o 'n', de no serlo pide volver a ingresar un
    // caracter. Retorna el caracter leido.
    char receta, aux;
    do {
        printf("Producto requiere receta medica, ingrese si cliente posee receta (s/n): ");
        scanf("%c%c", &receta, &aux);
        if (receta != 's' && receta != 'S' && receta != 'n' && receta != 'N')
            printf("Opcion invalida, por favor ingrese 's' o 'n'.\n");
    } while (receta != 's' && receta != 'S' && receta != 'n' && receta != 'N');
    return receta;
}

int lecturaUnidades(struct Producto *producto, char tipoTransaccion) {
    // Recibe un puntero a struct Producto y un char que indica si se realiza una compra o una venta, lee un entero de la
    // entrada del usuario y verifica si es válido. Retorna el entero leido.
    int unidades, entradaValida = 0;
    char aux;
    do {
        if (tipoTransaccion == 'V' && producto->cantidad == 0) {
            printf("No hay unidades en stock, no se puede agregar a la transaccion.\n");
            return 0;
        }
        printf("\nUnidades en stock: %d\n", producto->cantidad);
        printf("Unidades de la transaccion: ");
        scanf("%d%c", &unidades, &aux);
        if (unidades < 1)
            printf("Unidades invalidas, por favor ingrese un entero positivo.\n");
        else if (tipoTransaccion == 'V' && unidades > producto->cantidad)
            printf("No hay suficientes unidades en stock, ingrese una cantidad menor o igual a las unidades en stock.\n");
        else
            entradaValida = 1;
    } while (entradaValida == 0);
    return unidades;

}

int cantidadProductoEnVentas(struct Producto *producto, struct NodoTransaccion *ventas) {
    // Lo que recibe esta funcion es un puntero a un struct producto y un puntero a un struct nodoCompraVenta (las ventas de una farmacia)
    // El retorno es la cantidad de veces que ese producto fue vendido en la farmacia
    struct NodoTransaccion *rec;
    int cantVentas = 0;
    int i;
    if (ventas != NULL) {
        rec = ventas;
        while (rec != NULL) {
            for (i = 0; i < rec->datosTransaccion->totalProductosDistintos; i++) {
                if (strcmp(producto->codigo, rec->datosTransaccion->productos[i]->codigo) == 0) {
                    cantVentas += rec->datosTransaccion->productos[i]->cantidad;
                }
            }
            rec = rec->sig;
        }
    }
    return cantVentas;
}

void menosVendidoVentas(struct NodoProducto *inventario, struct NodoTransaccion *ventas, int *cantidad,
                        struct Producto **menosVendido, int *contador) {
    // Esta funcion recibe un puntero a un struct NodoProducto (el inventario) y un puntero a un struct NodoTransaccion
    // (las ventas de una farmacia), a la vez que un puntero a un entero (cantidad), un puntero a un struct Producto y
    // un puntero a un entero. Al ser una funcion tipo void no tiene un retorno claro, ya que se dedica a actualizar las
    // variables de cantidad y producto para encontrar el producto con mas ventas dentro de la farmacia.
    int cantCandidato;
    if (inventario != NULL) {
        menosVendidoVentas(inventario->izq, ventas, cantidad, menosVendido, contador);
        cantCandidato = cantidadProductoEnVentas(inventario->datosProducto, ventas);
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
        menosVendidoVentas(inventario->der, ventas, cantidad, menosVendido, contador);
    }
}

void masVendidoVentas(struct NodoProducto *inventario, struct NodoTransaccion *ventas, int *cantidad,
                    struct Producto **masVendido) {
    // Esta funcion recibe un puntero a un struct nodoProducto (el inventario) y un puntero a un struct nodocompraventa
    // (las ventas de una farmacia) a la vez que un puntero a un entero (cantidad) y un puntero a un struct Producto
    // Al ser una funcion tipo void no tiene un retorno claro, ya que se dedica a actualizar las variables de cantidad y
    // producto para encontrar el producto con mas ventas dentro de la farmacia
    int cantCandidato;
    if (inventario != NULL) {
        masVendidoVentas(inventario->izq, ventas, cantidad, masVendido);
        cantCandidato = cantidadProductoEnVentas(inventario->datosProducto, ventas);
        if (cantCandidato > (*cantidad)) {
            *masVendido = inventario->datosProducto;
            (*cantidad) = cantCandidato;
        }
        masVendidoVentas(inventario->der, ventas, cantidad, masVendido);
    }
}

struct Producto *getProductoMasVendido(struct NodoProducto *inventario, struct NodoTransaccion *ventas) {
    // Recibe un puntero a un struct de la farmacia y a las ventas de esta misma recorriendo cada una de ellas y
    // retornando el producto mas vendido en la farmacia
    int cantidad = 0;
    struct Producto *masVendido = NULL;
    masVendidoVentas(inventario, ventas, &cantidad, &masVendido);

    return masVendido;
}

struct Producto *getProductoMenosVendido(struct NodoProducto *inventario, struct NodoTransaccion *ventas) {
    // Recibe un puntero a un struct de la farmacia y a las ventas de esta misma recorriendo cada una de ellas y
    // retornando el producto menos vendido en la farmacia
    int cantidad;
    int contador = 0;
    struct Producto *menosVendido = NULL;
    menosVendidoVentas(inventario, ventas, &cantidad, &menosVendido, &contador);

    return menosVendido;
}

void mostrarArregloVentas(struct Producto **prodVendidos, int tam) {
    // Recibe un puntero a un arreglo de las ventas y un entero que ilustra el tamaño del arreglo
    // Muestra al usuario los productos vendidos y la cantidad de cada uno
    int i;
    if (prodVendidos != NULL) {
        for (i = 0; i < tam; i++) {
            printf("%s, codigo: %s\n", prodVendidos[i]->nombre, prodVendidos[i]->codigo);
            printf("Cantidad: %d\n\n", prodVendidos[i]->cantidad);
        }
    }
}

void mostrarProductosRecetaVendidos(struct Producto **prodVendidos, int tam) {
    // TODO: Comentario
    int i;
    if (prodVendidos != NULL) {
        for (i = 0; i < tam; i++) {
            if (prodVendidos[i]->requiereReceta) {
                printf("%s, codigo: %s\n", prodVendidos[i]->nombre, prodVendidos[i]->codigo);
                printf("Cantidad: %d\n\n", prodVendidos[i]->cantidad);
            }
        }
    }
}

int hayProductoConReceta(struct Transaccion *transaccion) {
    // TODO: Comentario
    int i;
    for (i = 0; i < transaccion->totalProductosDistintos; i++)
        if (transaccion->productos[i]->requiereReceta)
            return 1;
    return 0;
}

void mostrarVentasReceta(struct NodoTransaccion *ventas) {
    // TODO: Comentario
    struct NodoTransaccion *rec;
    if (ventas != NULL) {
        rec = ventas;
        while (rec != NULL) {
            if (hayProductoConReceta(rec->datosTransaccion)) {
                printf("Venta %d\n", rec->datosTransaccion->id);
                mostrarProductosRecetaVendidos(rec->datosTransaccion->productos,
                                               rec->datosTransaccion->totalProductosDistintos);
            }
            rec = rec->sig;
        }
    } else {
        printf("No existen ventas en el sistema.\n\n");
    }
}

void mostrarVentas(struct NodoTransaccion *ventas) {
    // Recibe un puntero a un struct de NodoTransaccion
    // Muestra por pantalla al usuario los productos de cada venta
    struct NodoTransaccion *rec;
    int tam;
    if (ventas != NULL) {
        rec = ventas;
        while (rec != NULL) {
            printf("Venta %d\n", rec->datosTransaccion->id);
            printf("Cantidad de productos: %d\n", rec->datosTransaccion->cantidadProductos);
            tam = rec->datosTransaccion->totalProductosDistintos;
            mostrarArregloVentas(rec->datosTransaccion->productos, tam);
            rec = rec->sig;
        }
    }
    else {
        printf("No hay ventas registradas\n\n");
    }
}

void recorrerInventario(struct NodoProducto *inventario, int condicion, int *contador) {
    // Recibe un puntero a un struct NodoProducto, un entero que indica si estamos buscando los productos con o sin stock
    // y un contador para tener registro de si hay o no hay productos con esa condicion. Luego de eso se dedica a
    // imprimir el nombre de los productos que cumplan las condiciones junto a sus respectvas cantidades.
    if (inventario != NULL) {
        recorrerInventario(inventario->izq, condicion, contador);
        if (condicion == 1) { // Productos con stock
            if (inventario->datosProducto->cantidad > 0) {
                printf("%s, codigo: %s\n", inventario->datosProducto->nombre, inventario->datosProducto->codigo);
                printf("Cantidad: %d\n\n", inventario->datosProducto->cantidad);
                (*contador)++;
            }
        } else { // Productos sin stock
            if (inventario->datosProducto->cantidad == 0) {
                printf("%s, codigo: %s\n", inventario->datosProducto->nombre, inventario->datosProducto->codigo);
                (*contador)++;
            }
        }
        recorrerInventario(inventario->der, condicion, contador);
    }
}

void mostrarProductosConStock(struct NodoProducto *inventario) {
    // Recibe un puntero a un arreglo de farmacia
    // Imprimiendo por pantalla todos los productos que tienen stock en ese momento
    int contador;
    if (inventario != NULL) {
        printf("Productos con stock en la farmacia\n");
        recorrerInventario(inventario, 1, &contador);
        if (contador == 0){
            printf("No se encuentran productos con stock.\n");
        }
    }
    else {
        printf("No hay productos en el inventario.\n");
    }
}

void mostrarProductosSinStock(struct NodoProducto *inventario) {
    // Recibe un puntero a un arreglo de farmacia
    // Imprimiendo por pantalla todos los productos que no tienen stock en ese momento
    int contador;
    if (inventario != NULL) {
        printf("Productos sin stock en la farmacia\n");
        recorrerInventario(inventario, 0, &contador);
        if (contador == 0){
            printf("No se encuentran productos sin stock.\n");
        }
        printf("\n");
    }
    else {
        printf("No hay productos en el inventario.\n");
    }
}

void contarProductoEnVentas(struct Producto *producto, struct Transaccion *venta, int tam, int *contador,
                            int *cantidad) {
    // TODO: Comentario
    // Recibe un puntero a un arreglo de las ventas y un entero que ilustra el tamaño del arreglo
    // Muestra al usuario los productos vendidos y la cantidad de cada uno
    int i;
    if (venta != NULL) {
        for (i = 0; i < tam; i++) {
                if (strcmp(producto->codigo, venta->productos[i]->codigo) == 0
                    && venta->productos[i]->cantidad != 0) {
                    (*contador)++;
                    (*cantidad) += venta->productos[i]->cantidad;
            }
        }
    }
}

int calcularPromedioStock(struct Producto *producto, struct NodoTransaccion *ventas) {
    //Recibe un puntero a un struct de un producto y a las ventas de la farmacia recorriendo cada una de ellas y
    //retornando el promedio de stock del producto
    int contador = 0;
    int cantidad = 0;
    struct NodoTransaccion * rec;
    if (ventas != NULL) {
        rec = ventas;
        while(rec != NULL) {
            contarProductoEnVentas(producto, rec->datosTransaccion, rec->datosTransaccion->totalProductosDistintos,
                                   &contador, &cantidad);
            rec = rec->sig;
        }
    }
    if (contador != 0) {
        return (cantidad / contador);
    }
    else return 0;
}

int tieneBajoStock(struct Producto *producto, struct NodoTransaccion *ventas) {
    //Recibe un puntero a un struct de un producto y a las ventas de la farmacia recorriendo cada una de ellas y
    //retornando 1 si el producto tiene bajo stock y 0 si no lo tiene
    int promedio;
    promedio = calcularPromedioStock(producto, ventas);
    if (promedio > producto->cantidad) {
        return 1;
    }
    else return 0;
}

void mostrarProductosBajoStockAux(struct NodoProducto *inventario, struct NodoTransaccion *ventas) {
    //Recibe un puntero al inventario de la farmacia y a las ventas de la farmacia
    //Se dedica a imprimir por pantalla todos los productos que esten por debajo del promedio de su stock
    if (inventario != NULL && ventas != NULL) {

        mostrarProductosBajoStockAux(inventario->izq, ventas);

        if (tieneBajoStock(inventario->datosProducto, ventas) == 1) {
            printf("%s, código: %s\n", inventario->datosProducto->nombre, inventario->datosProducto->codigo);
            printf("Cantidad: %d\n\n", inventario->datosProducto->cantidad);
        }

        mostrarProductosBajoStockAux(inventario->der, ventas);
    }
}

struct NodoProducto *getProductosTransaccion(struct NodoProducto *inventario, char tipoTransaccion,
                                            char *proveedorCompra, int *totalProductosDistintos) {
    // Recibe un arbol binario de busqueda de struct NodoProducto, un char que indica si se realiza una compra o una
    // venta, y un puntero a un entero. Lee datos de la entrada del usuario, los asigna en un arbol binario de busqueda
    // de struct NodoProducto y aumenta en 1 el entero. Retorna un puntero al arbol creado.
    struct NodoProducto *productosTransaccion = NULL;
    struct Producto *producto, *productoTransaccion;
    char *codigoProducto, poseeReceta, opcion, aux;
    int unidades;
    do {
        do {
            codigoProducto = lecturaCodigo();
            producto = getProducto(inventario, codigoProducto);
            if (tipoTransaccion == 'C' && producto == NULL)
                producto = leerDatosProducto(codigoProducto, proveedorCompra);
            if (tipoTransaccion == 'V' && producto == NULL)
                printf("Producto no encontrado, ingrese codigo de un producto en inventario.\n");
        } while (producto == NULL);
        if (tipoTransaccion == 'C' && strcmp(producto->proveedor, proveedorCompra) != 0) {
            printf("El producto que desea agregar no es del proveedor de esta compra.\n");
            producto = NULL;
        }
        if (tipoTransaccion == 'V' && producto->requiereReceta) {
            poseeReceta = lecturaReceta();
            if (poseeReceta == 'n' || poseeReceta == 'N') {
                printf("Producto requiere receta medica, no se puede agregar a la transaccion.\n");
                producto = NULL;
            }
        }
        if (producto != NULL) {
            unidades = lecturaUnidades(producto, tipoTransaccion);
            productoTransaccion = crearProducto(producto->codigo, producto->nombre, producto->categoria,
                                  producto->descripcion, producto->proveedor, producto->precio, producto->requiereReceta);
            productoTransaccion->cantidad = unidades;
            agregarNodoProducto(&productosTransaccion, crearNodoProducto(productoTransaccion));
            (*totalProductosDistintos)++;
        }
        printf("\nDesea agregar mas productos a la transaccion? (s/n): ");
        scanf("%c%c", &opcion, &aux);
    } while (opcion == 's' || opcion == 'S');

    return productosTransaccion;
}

void asignarProductoAArreglo(struct NodoProducto *root, struct Producto **arregloProductos, int *i) {
    // Recibe un arbol binario de busqueda de struct NodoProducto, un arreglo de punteros a struct Producto y un puntero
    // a un entero. Asigna los elementos del arbol al arreglo, y aumenta el valor del entero en 1.
    if (root != NULL) {
        asignarProductoAArreglo(root->izq, arregloProductos, i);
        arregloProductos[*i] = root->datosProducto;
        (*i)++;
        asignarProductoAArreglo(root->der, arregloProductos, i);
    }
}

struct Producto **getArregloProductos(struct NodoProducto *productosTransaccion, int totalProductosDistintos) {
    // Recibe un arbol binario de busqueda de struct NodoProducto y un entero, crea un arreglo de punteros a struct
    // Producto y asigna los elementos del arbol al arreglo. Retorna un puntero al arreglo creado.
    struct Producto **arregloProductos = NULL;
    int i = 0;
    if (totalProductosDistintos > 0) {
        arregloProductos = (struct Producto **) malloc(sizeof(struct Producto *) * totalProductosDistintos);
        if (arregloProductos != NULL) {
            asignarProductoAArreglo(productosTransaccion, arregloProductos, &i);
        }
    }
    return arregloProductos;
}

int getTotalProductos(struct Producto **arregloProductos, int largoArreglo) {
    // Recibe un arreglo de struct Producto y su largo.
    // Retorna la suma del campo cantidad de todos sus elementos.
    int i, totalProductos = 0;
    if (arregloProductos != NULL) {
        for (i = 0; i < largoArreglo; i++)
            totalProductos += arregloProductos[i]->cantidad;
    }
    return totalProductos;
}

int getCostoTotal(struct Producto **arregloProductos, int largoArreglo) {
    // Recibe un arreglo de struct Producto y su largo, calcula el costo de cada elemento.
    // Retorna la suma del costo de todos sus elementos.
    int i, costoTotal = 0;
    if (arregloProductos != NULL) {
        for (i = 0; i < largoArreglo; i++)
            costoTotal += arregloProductos[i]->cantidad * arregloProductos[i]->precio;
    }
    return costoTotal;
}

struct Transaccion *crearTransaccion(int id, char *nombre, char *rut, int totalProductosDistintos,
                            struct Producto **productos, char estadoEnvio, char *fechaSolicitud, char *fechaLlegada) {
    // Recibe datos de una transaccion, asigna los datos recibidos en un struct Transaccion.
    // Retorna un puntero al struct Transaccion que contiene los datos recibidos.
    struct Transaccion *nuevaTransaccion = NULL;

    nuevaTransaccion = (struct Transaccion *) malloc(sizeof(struct Transaccion));
    nuevaTransaccion->id = id;
    nuevaTransaccion->nombre = nombre;
    nuevaTransaccion->rut = rut;
    nuevaTransaccion->totalProductosDistintos = totalProductosDistintos;
    nuevaTransaccion->productos = productos;
    nuevaTransaccion->cantidadProductos = getTotalProductos(productos, totalProductosDistintos);
    nuevaTransaccion->costoTotal = getCostoTotal(productos, totalProductosDistintos);
    nuevaTransaccion->estadoEnvio = estadoEnvio;
    nuevaTransaccion->fechaSolicitud = fechaSolicitud;
    nuevaTransaccion->fechaLlegada = fechaLlegada;

    return nuevaTransaccion;
}

struct Transaccion *leerDatosTransaccion(struct NodoProducto *inventario, char tipoTransaccion) {
    // Recibe el inventario y un char que indica si se crea una compra o una venta, lee datos de la entrada del usuario.
    // Retorna un puntero a un struct Transaccion que contiene los datos leidos.
    struct NodoProducto *productosTransaccion;
    struct Producto **productos;
    int totalProductosDistintos = 0, id;
    char *nombre, *rut, *fechaSolicitud, *fechaLlegada = NULL, estadoEnvio = 'X', aux;

    printf("Ingrese ID de la transaccion: ");
    scanf("%d%c", &id, &aux);
    printf("Ingrese nombre: ");
    nombre = leerCadena();
    printf("Ingrese rut: ");
    rut = leerCadena();
    if (tipoTransaccion == 'C') {
        productosTransaccion = getProductosTransaccion(inventario, tipoTransaccion, nombre, &totalProductosDistintos);
    } else {
        productosTransaccion = getProductosTransaccion(inventario, tipoTransaccion, NULL, &totalProductosDistintos);
    }
    productos = getArregloProductos(productosTransaccion, totalProductosDistintos);
    if (productos != NULL) {
        printf("Ingrese fecha de realizacion (DD/MM/AAAA): ");
        fechaSolicitud = leerCadena();
        if (tipoTransaccion == 'C') {
            printf("Ingrese estado de envio de la compra (R:Recibido/P:Pendiente): ");
            scanf(" %c%c", &estadoEnvio, &aux);
            if (estadoEnvio == 'R' || estadoEnvio == 'r') {
                printf("Fecha de llegada de la compra: ");
                fechaLlegada = leerCadena();
            }
        }
    } else {
        printf("No se pudo crear la transaccion.\n");
        return NULL;
    }
    return crearTransaccion(id, nombre, rut, totalProductosDistintos, productos, estadoEnvio, fechaSolicitud, fechaLlegada);
}

struct NodoTransaccion *crearNodoTransaccion(struct Transaccion *transaccion) {
    // Recibe un puntero a struct Transaccion y lo asigna a un struct NodoTransaccion.
    // Retorna un puntero al struct NodoTransaccion que contiene el struct Transaccion recibido.
    struct NodoTransaccion *nuevoNodo = NULL;
    if (transaccion != NULL) {
        nuevoNodo = (struct NodoTransaccion *) malloc(sizeof(struct NodoTransaccion));
        nuevoNodo->datosTransaccion = transaccion;
        nuevoNodo->sig = NULL;
    }
    return nuevoNodo;
}

struct Transaccion *getTransaccion(struct NodoTransaccion *head, int idBuscado) {
    // Recibe una lista simplemente enlazada de struct NodoTransaccion y un id, busca el elemento que posea dicho id.
    // Retorna un puntero a struct Transaccion si se encuentra en la lista, en caso contrrio retorna NULL.
    struct NodoTransaccion *rec;
    if (head != NULL) {
        rec = head;
        while (rec != NULL) {
            if (rec->datosTransaccion->id == idBuscado)
                return rec->datosTransaccion;
            rec = rec->sig;
        }
    }
    return NULL;
}

int agregarNodoTransaccion(struct NodoTransaccion **head, struct NodoTransaccion *nuevoNodo) {
    // Recibe una lista simplemente enlazada de struct Transaccion y un puntero a struct NodoTransaccion, agrega el
    // nodo recibido a la lista. Retorna 1 en caso de exito, en caso contrario retorna 0.
    struct NodoTransaccion *rec;
    if (*head == NULL) {
        *head = nuevoNodo;
        return 1;
    }
    if (nuevoNodo != NULL) {
        if (getTransaccion(*head, nuevoNodo->datosTransaccion->id) == NULL) {
            rec = *head;
            while (rec->sig != NULL) {
                rec = rec->sig;
            }
            rec->sig = nuevoNodo;
            return 1;
        }
    }
    return 0;
}

struct Farmacia *crearFarmacia(char *id, char *region, char *ciudad) {
    // Recibe datos de una farmacia, asigna los datos recibidos a un struct Farmacia.
    // Retorna un puntero al struct Farmacia que se le asignaron los datos leidos.
    struct Farmacia *nuevaFarmacia;

    nuevaFarmacia = (struct Farmacia *) malloc(sizeof(struct Farmacia));
    nuevaFarmacia->id = id;
    nuevaFarmacia->region = region;
    nuevaFarmacia->ciudad = ciudad;
    nuevaFarmacia->maxCapacidad = CAPACIDAD_PROMEDIO;
    nuevaFarmacia->inventario = NULL;
    nuevaFarmacia->totalProductos = 0;
    nuevaFarmacia->ventas = NULL;
    nuevaFarmacia->compras = NULL;
    return nuevaFarmacia;
}

struct Farmacia *leerDatosFarmacia() {
    // Lee datos de una farmacia de la entrada del usuario.
    // Retorna un puntero a un struct Farmacia que contiene los datos leidos.
    char *id, *ciudad, *region;
    printf("Ingrese ID de la nueva farmacia: ");
    id = leerCadena();
    printf("Ingrese region de la nueva farmacia: ");
    region = leerCadena();
    printf("Ingrese ciudad de la nueva farmacia: ");
    ciudad = leerCadena();
    return crearFarmacia(id, region, ciudad);
}

struct NodoFarmacia *crearNodoFarmacia(struct Farmacia *farmacia) {
    // Recibe un puntero a struct Farmacia y lo asigna a un struct NodoFarmacia.
    // Retorna un puntero a struct NodoFarmacia que contiene el puntero a struct Farmacia recibido.
    struct NodoFarmacia *nuevaFarmacia = NULL;
    if (farmacia != NULL) {
        nuevaFarmacia = (struct NodoFarmacia *) malloc(sizeof(struct NodoFarmacia));
        nuevaFarmacia->datosFarmacia = farmacia;
        nuevaFarmacia->ant = NULL;
        nuevaFarmacia->sig = NULL;
    }
    return nuevaFarmacia;
}

struct Farmacia *getFarmacia(struct NodoFarmacia *head, char *idBuscado) {
    // Recibe una lista doblemente enlazada de struct NodoFarmacia y un id, busca el elemento que posea dicho id.
    // Retorna un puntero a struct Farmacia si se encuentra el elemento en la lista, en caso contrario retorna NULL.
    struct NodoFarmacia *rec;
    if (head != NULL) {
        rec = head;
        do {
            if (strcmp(rec->datosFarmacia->id, idBuscado) == 0)
                return rec->datosFarmacia;
            rec = rec->sig;
        } while (rec != head);
    }
    return NULL;
}

int agregarNodoFarmacia(struct NodoFarmacia **head, struct NodoFarmacia *nuevoNodo) {
    // Recibe una lista doblemente enlazada de struct NodoFarmacia y un struct NodoFarmacia, agrega el nodo recibido
    // a la lista. Retorna 1 en caso de exito, en caso contrario retorna 0.
    if (*head == NULL) {
        *head = nuevoNodo;
        (*head)->ant = nuevoNodo;
        (*head)->sig = nuevoNodo;
        return 1;
    }
    if (nuevoNodo != NULL) {
        if (getFarmacia(*head, nuevoNodo->datosFarmacia->id) == NULL) {
            nuevoNodo->sig = *head;
            nuevoNodo->ant = (*head)->ant;
            (*head)->ant->sig = nuevoNodo;
            (*head)->ant = nuevoNodo;
            return 1;
        }
    }
    return 0;
}

struct NodoFarmacia *getNodoFarmacia(struct NodoFarmacia *head, char *idBuscado) {
    // Recibe una lista doblemente enlazada de struct NodoFarmacia y un id, busca el elemento que posea dicho id.
    // Retorna el nodo del elemento buscado si se encuentra en la lista, en caso contrario retorna NULL.
    struct NodoFarmacia *rec;
    if (head != NULL && idBuscado != NULL) {
        rec = head;
        do {
            if (strcmp(rec->datosFarmacia->id, idBuscado) == 0)
                return rec;
            rec = rec->sig;
        } while (rec != head);
    }
    return NULL;
}

int eliminarFarmacia(struct NodoFarmacia **head, char *idAEliminar) {
    // Recibe una lista doblemente enlazada de struct NodoFarmacia y un id, elimina y libera la memoria asignada
    // al elemento que posea dicho id. Retorna 1 en caso de exito, en caso contrario retorna 0.
    struct NodoFarmacia *nodoAEliminar;
    if (*head != NULL && idAEliminar != NULL) {
        nodoAEliminar = getNodoFarmacia(*head, idAEliminar);
        if (nodoAEliminar != NULL) {
            if (nodoAEliminar == nodoAEliminar->sig) {
                *head = NULL;
            } else {
                if (nodoAEliminar == *head)
                    *head = (*head)->sig;
                nodoAEliminar->ant->sig = nodoAEliminar->sig;
                nodoAEliminar->sig->ant = nodoAEliminar->ant;
            }
            return 1;
        }
    }
    return 0;
}

int loteACaducarEnFecha(struct Lote *lote, char *fecha) {
    // Recibe un puntero a struct Lote y una fecha, verifica si el lote caduca en la fecha recibida.
    // Retorna 1 en caso de caducar en dicha fecha, en caso contrario retorna 0.
    if (strcmp(lote->fechaCaducidad, fecha) == 0)
        return 1;
    return 0;
}

int hayLoteACaducarEnFecha(struct NodoLote *head, char *fecha) {
    // Recibe una lista simplemente enlazada de struct NodoLote y una fecha, verifica si hay lotes a caducar en dicha fecha.
    // Retorna 1 en caso de encontrar alguno, en caso contrario retorna 0.
    struct NodoLote *rec;
    if (head != NULL) {
        rec = head;
        while (rec != NULL) {
            if (loteACaducarEnFecha(rec->datosLote, fecha))
                return 1;
            rec = rec->sig;
        }
    }
    return 0;
}

int totalTransaccionesDeRut(struct NodoTransaccion *transacciones, char *rut) {
    // Recibe una lista simplemente enlazada de struct NodoTransaccion y un rut, busca las transacciones asignadas al
    // rut recibido. Retorna el total de transacciones asignadas al rut.
    struct NodoTransaccion *rec;
    int totalTransacciones = 0;
    if (transacciones != NULL && rut != NULL) {
        rec = transacciones;
        while (rec != NULL) {
            if (strcmp(rec->datosTransaccion->rut, rut) == 0)
                totalTransacciones++;
            rec = rec->sig;
        }
    }
    return totalTransacciones;
}

void mostrarVentasARut(struct NodoTransaccion *transacciones, char *rut) {
    // Recibe una lista simplemente enlazada de struct NodoTransaccion y un rut, muestra las transacciones asignadas al
    // rut recibido.
    struct NodoTransaccion *rec;
    if (transacciones != NULL) {
        if (totalTransaccionesDeRut(transacciones, rut) > 0) {
            printf("Total ventas a rut: %s\n", rut);
            rec = transacciones;
            while (rec != NULL) {
                if (strcmp(rec->datosTransaccion->rut, rut) == 0) {
                    printf("ID: %d\n", rec->datosTransaccion->id);
                    printf("Costo total: $%d\n", rec->datosTransaccion->costoTotal);
                    printf("Total de productos distintos: %d\n", rec->datosTransaccion->totalProductosDistintos);
                    printf("Total de productos comprados: %d\n", rec->datosTransaccion->cantidadProductos);
                }
                rec = rec->sig;
            }
        } else {
            printf("No hay ventas asignadas al rut: %s\n", rut);
        }
    }
}

int agregarCompraAInventario(struct NodoProducto **inventario, struct Transaccion *compra) {
    // Recibe un arbol binario de busqueda de struct NodoProducto y un puntero a struct Transaccion, agrega los
    // productos de la compra recibida al inventario. Retorna 1 si la compra es valida, en caso contrario retorna 0.
    struct Producto *producto;
    struct NodoLote *rec;
    int i, totalProducto;
    if (compra != NULL && compra->fechaLlegada != NULL) {
        for (i = 0; i < compra->totalProductosDistintos; i++) {
            printf("Producto: %s\n", compra->productos[i]->nombre);
            totalProducto = compra->productos[i]->cantidad;
            while (totalProducto > 0) {
                agregarNodoLote(&compra->productos[i]->lotes, crearNodoLote(leerDatosLote()));
                totalProducto -= getCantidadProducto(compra->productos[i]->lotes);
            }
            producto = getProducto(*inventario, compra->productos[i]->codigo);
            if (producto == NULL) {
                agregarNodoProducto(inventario, crearNodoProducto(compra->productos[i]));
            } else {
                rec = compra->productos[i]->lotes;
                while (rec != NULL) {
                    agregarNodoLote(&producto->lotes, rec);
                    rec = rec->sig;
                }
            }
        }
        return 1;
    }
    return 0;
}

int restarALote(struct Lote *lote, int *cantidad) {
    // Recibe un puntero a struct Lote y un entero correspondiente a las unidades a restar de una venta.
    // Retorna 1 en caso de restar exitosamente, en caso contrario retorna 0.
    if (lote != NULL && *cantidad > 0) {
        if (*cantidad > lote->cantidadLote) {
            *cantidad -= lote->cantidadLote;
            lote->cantidadLote = 0;
        } else {
            lote->cantidadLote -= *cantidad;
            *cantidad = 0;
        }
        return 1;
    }
    return 0;
}

int quitarUnidadesVendidas(struct NodoLote *lotesProducto, int cantidad, struct NodoLote **lotesVenta) {
    // Recibe una lista simplemente enlazada de struct NodoLote, las unidades vendidas de un producto y un puntero a una
    // lista simplemente enlazada de struct NodoLote, resta las unidades vendidas a lotesProducto y las asigna
    // a lotesVenta. Retorna 1 en caso de restar y asignar exitosamente, en caso contrario retorna 0.
    struct NodoLote *rec;
    struct Lote *loteVendido;
    if (lotesProducto != NULL) {
        rec = lotesProducto;
        loteVendido = (struct Lote *) malloc(sizeof(struct Lote));
        while (rec != NULL && cantidad > 0) {
            loteVendido->numeroLote = rec->datosLote->numeroLote;
            loteVendido->cantidadLote = rec->datosLote->cantidadLote;
            restarALote(rec->datosLote, &cantidad);
            loteVendido->cantidadLote -= cantidad;
            agregarNodoLote(lotesVenta, crearNodoLote(loteVendido));
            rec = rec->sig;
        }
        return 1;
    }
    return 0;
}

int quitarVentaAInventario(struct NodoProducto *inventario, struct Transaccion *venta) {
    // Recibe un arbol binario de busqueda de struct NodoProducto y un puntero a struct Transaccion, quita los productos
    // vendidos. Retorna 1 en caso de que la venta sea valida, en caso contrario retorna 0.
    struct Producto *producto;
    int i;
    if (venta != NULL && venta->productos != NULL) {
        for (i = 0; i < venta->totalProductosDistintos && venta->productos[i]; i++) {
            producto = getProducto(inventario, venta->productos[i]->codigo);
            if (producto != NULL && venta->productos[i]->cantidad <= producto->cantidad) {
                quitarUnidadesVendidas(producto->lotes, venta->productos[i]->cantidad,
                                       &venta->productos[i]->lotes);
            }
        }
        return 1;
    }
    return 0;
}

void mostrarLotesACaducar(struct NodoLote *head, char *fecha) {
    // Recibe una lista simplemente enlazada de struct NodoLote y una fecha, muestra los lotes de la lista que caducan
    // en la fecha recibida.
    struct NodoLote *rec;
    if (head != NULL) {
        rec = head;
        while (rec != NULL) {
            if (loteACaducarEnFecha(rec->datosLote, fecha)) {
                printf("Numero Lote: %s\n", rec->datosLote->numeroLote);
                printf("Cantidad Lote: %d\n", rec->datosLote->cantidadLote);
            }
            rec = rec->sig;
        }
    }
}

void mostrarProductosACaducar(struct NodoProducto *root, char *fecha) {
    // Recibe un arbol binario de busqueda de struct NodoProducto y una fecha, muestra los productos del inventario a 
    // caducar en la fecha recibida.
    if (root != NULL) {
        mostrarProductosACaducar(root->izq, fecha);
        if (hayLoteACaducarEnFecha(root->datosProducto->lotes, fecha)) {
            printf("%s, codigo: %s\n", root->datosProducto->nombre, root->datosProducto->codigo);
            mostrarLotesACaducar(root->datosProducto->lotes, fecha);
            printf("==============================\n");
        }
        mostrarProductosACaducar(root->der, fecha);
    }
}

int totalTransaccionesDeCategoria(struct NodoTransaccion *ventas, char *categoria) {
    // Recibe una lista simplemente enlazada de struct NodoTransaccion y una categoria, busca las transacciones asignadas
    // a la categoria recibida. Retorna el total de transacciones asignadas a la categoria.
    struct NodoTransaccion *rec;
    int i, totalTransacciones = 0;
    if (ventas != NULL) {
        rec = ventas;
        while (rec != NULL) {
            for (i = 0; i < rec->datosTransaccion->totalProductosDistintos; i++) {
                if (strcmp(rec->datosTransaccion->productos[i]->categoria, categoria) == 0)
                    totalTransacciones++;
            }
            rec = rec->sig;
        }
    }
    return totalTransacciones;
}

char *getCategoriaMasVendida(struct NodoTransaccion *ventas) {
    // Recibe una lista simplemente enlazada de struct NodoTransaccion, busca la categoria mas vendida.
    // Retorna un puntero a char con el nombre de la categoria mas vendida.
    struct NodoTransaccion *rec;
    char *categoriaMasVendida = NULL;
    int i, maxVentas;
    if (ventas != NULL) {
        rec = ventas;
        while (rec != NULL) {
            for (i = 0; i < rec->datosTransaccion->totalProductosDistintos; i++) {
                if (categoriaMasVendida == NULL || (categoriaMasVendida != rec->datosTransaccion->productos[i]->categoria
                    && totalTransaccionesDeCategoria(ventas, rec->datosTransaccion->productos[i]->categoria) > maxVentas)) {
                    maxVentas = totalTransaccionesDeCategoria(ventas, rec->datosTransaccion->productos[i]->categoria);
                    categoriaMasVendida = rec->datosTransaccion->productos[i]->categoria;
                }
            }
            rec = rec->sig;
        }
    }
    return categoriaMasVendida;
}

int strToNum(const char *str) {
    // Recibe un puntero a char, convierte el string a un entero.
    // Retorna el entero correspondiente al string recibido.
    int i, num = 0;
    for (i = 0; str[i] != '\0'; i++) {
        num = num * 10 + str[i] - '0';
    }
    return num;
}

char *getCategoriaMasVendidaEstacion(struct NodoTransaccion *ventas, char estacion) {
    // Recibe una lista simplemente enlazada de struct NodoTransaccion y una estacion, busca la categoria mas vendida
    // en la estacion recibida. Retorna un puntero a char con el nombre de la categoria mas vendida.
    struct NodoTransaccion *rec;
    char *categoriaMasVendida = NULL, mesFecha[3] = "\0\0\0";
    int i, maxVentas, condicion, numeroMes;
    if (ventas != NULL) {
        rec = ventas;
        while (rec != NULL) {
            mesFecha[0] = rec->datosTransaccion->fechaSolicitud[3];
            mesFecha[1] = rec->datosTransaccion->fechaSolicitud[4];
            numeroMes = strToNum(mesFecha);
            switch (estacion) {
                case 'P':
                    condicion = numeroMes > 2 && numeroMes < 6;
                    break;
                case 'I':
                    condicion = numeroMes > 5 && numeroMes < 9;
                    break;
                case 'O':
                    condicion = numeroMes > 8 && numeroMes < 12;
                    break;
                case 'V':
                    condicion = numeroMes < 3 || numeroMes == 12;
                    break;
                default:
                    condicion = 0;
                    break;
            }
            if (condicion) {
                for (i = 0; i < rec->datosTransaccion->totalProductosDistintos; i++) {
                    if (categoriaMasVendida == NULL || (categoriaMasVendida != rec->datosTransaccion->productos[i]->categoria
                        && totalTransaccionesDeCategoria(ventas, rec->datosTransaccion->productos[i]->categoria) > maxVentas)) {
                        maxVentas = totalTransaccionesDeCategoria(ventas, rec->datosTransaccion->productos[i]->categoria);
                        categoriaMasVendida = rec->datosTransaccion->productos[i]->categoria;
                    }
                }
            }
            rec = rec->sig;
        }
    }
    return categoriaMasVendida;
}

int totalVentasFarmacia(struct NodoTransaccion *ventas) {
    // Recibe una lista simplemente enlazada de struct NodoTransaccion, calcula el total de ventas de la farmacia.
    // Retorna un int con el numero de ventas.
    struct NodoTransaccion *rec;
    int totalVentas = 0;
    if (ventas != NULL) {
        rec = ventas;
        while (rec != NULL) {
            totalVentas++;
            rec = rec->sig;
        }
    }
    return totalVentas;
}

struct Farmacia *getFarmaciaConMasVentas(struct NodoFarmacia *head) {
    // Recibe una lista doblemente enlazada de struct NodoFarmacia, busca la farmacia con mas ventas.
    // Retorna un puntero a struct Farmacia si existe la farmacia con mas ventas, en caso contrario retorna NULL.
    struct NodoFarmacia *rec;
    struct Farmacia *farmaciaMasVentas = NULL;
    int maxVentas;
    if (head != NULL) {
        rec = head;
        do {
            if (farmaciaMasVentas == NULL || totalVentasFarmacia(rec->datosFarmacia->ventas) > maxVentas) {
                maxVentas = totalVentasFarmacia(rec->datosFarmacia->ventas);
                farmaciaMasVentas = rec->datosFarmacia;
            }
            rec = rec->sig;
        } while (rec != head);
    }
    return farmaciaMasVentas;
}

int totalIngresosFarmacia(struct NodoTransaccion *ventas) {
    // Recibe una lista simplemente enlazada de struct NodoTransaccion, calcula el total de ingresos de la farmacia.
    // Retorna un int con el total de ingresos.
    struct NodoTransaccion *rec;
    int totalIngresos = 0;
    if (ventas != NULL) {
        rec = ventas;
        while (rec != NULL) {
            totalIngresos += rec->datosTransaccion->costoTotal;
            rec = rec->sig;
        }
    }
    return totalIngresos;
}

struct Farmacia *getFarmaciaConMasIngresos(struct NodoFarmacia *head) {
    // Recibe una lista doblemente enlazada de struct NodoFarmacia, busca la farmacia con mas ingresos.
    // Retorna un puntero a struct Farmacia si existe la farmacia con mas ingresos, en caso contrario retorna NULL.
    struct NodoFarmacia *rec;
    struct Farmacia *farmaciaMasIngresos = NULL;
    int maxIngresos;
    if (head != NULL) {
        rec = head;
        do {
            if (farmaciaMasIngresos == NULL || totalIngresosFarmacia(rec->datosFarmacia->ventas) > maxIngresos) {
                maxIngresos = totalIngresosFarmacia(rec->datosFarmacia->ventas);
                farmaciaMasIngresos = rec->datosFarmacia;
            }
            rec = rec->sig;
        } while (rec != head);
    }
    return farmaciaMasIngresos;
}

int contarProductosDistintos(struct NodoProducto *rootProductos) {
    // Función para contar los productos distintos dentro del inventario, usando recursividad.
    // Retorna un int con el total de productos distintos.
    if (!rootProductos)
        return 0;
    return 1 + contarProductosDistintos(rootProductos->izq) + contarProductosDistintos(rootProductos->der);
}

char *getRutConMasTransacciones(struct NodoTransaccion *headTransaccion, char **nombre) {
    // Recibe una lista simplemente enlazada de struct Transaccion y un puntero a char, busca el rut con mas transacciones
    // en la lista y asigna el nombre correspondiente al rut con mas transacciones al puntero a char recibido.
    // Retorna el rut con más transacciones si existen ventas, en caso contrario retorna NULL.
    struct NodoTransaccion *rec;
    char *rutMasTransacciones = NULL;
    int maxTransacciones, totalTransacciones;
    if (headTransaccion != NULL) {
        rec = headTransaccion;
        while (rec != NULL) {
            totalTransacciones = totalTransaccionesDeRut(headTransaccion, rec->datosTransaccion->rut);
            if (rutMasTransacciones == NULL || (strcmp(rec->datosTransaccion->rut, rutMasTransacciones) != 0
                                                && totalTransacciones > maxTransacciones)) {
                (*nombre) = rec->datosTransaccion->nombre;
                rutMasTransacciones = rec->datosTransaccion->rut;
                maxTransacciones = totalTransacciones;
            }
            rec = rec->sig;
        }
    }
    return rutMasTransacciones;
}

void mostrarRutConMasTransacciones(struct NodoTransaccion *headTransaccion) {
    // Recibe una lista simplemente enlazada de struct Transaccion, .
    // Muestra datos del rut con mas transacciones en dicha lista.
    char *rutMasTransacciones, *nombre;
    int totalTransaccionesRut;
    rutMasTransacciones = getRutConMasTransacciones(headTransaccion, &nombre);
    totalTransaccionesRut = totalTransaccionesDeRut(headTransaccion, rutMasTransacciones);
    if (rutMasTransacciones != NULL) {
        printf("Cliente con más transacciones: \n");
        printf("Nombre: %s\n", nombre);
        printf("RUT: %s\n", rutMasTransacciones);
        printf("Compras realizadas: %d\n", totalTransaccionesRut);
    } else {
        printf("No existen transacciones.\n");
    }
}

void mostrarDetalleFarmacia(struct Farmacia *farmacia) {
    // Función para mostrar los detalles de la farmacia
    printf("Detalle de la farmacia\n");
    printf("ID: %s\n", farmacia->id);
    printf("Ciudad: %s\n", farmacia->ciudad);
    printf("Region: %s\n", farmacia->region);
    printf("Total de productos distintos: %d\n", contarProductosDistintos(farmacia->inventario));
    printf("Total de productos en bodega: %d\n", farmacia->totalProductos);
    printf("Capacidad maxima de almacenaje: %d\n", farmacia->maxCapacidad);
}

void mostrarDetalleProducto(struct Producto *producto) {
    // Función para mostrar los detalles de un producto
    printf("Detalle del producto\n");
    printf("Nombre: %s\n", producto->nombre);
    printf("Codigo: %s\n", producto->codigo);
    printf("Descripcion: %s\n", producto->descripcion);
    printf("Categoria: %s\n", producto->categoria);
    printf("Precio: $%d\n", producto->precio);
    printf("Stock: %d\n", producto->cantidad);
    printf("Requiere receta: ");
    if (producto->requiereReceta)
        printf("Si\n");
    else
        printf("No\n");
    printf("Proveedor: %s\n", producto->proveedor);
}

void mostrarLotesProducto(struct Producto *producto) {
    // Función para mostrar los lotes de un producto en el sistema
    // Se imprimen datos para cada lote y en caso de no haber, muestra mensaje correspondiente
    struct NodoLote *rec;
    printf("Lotes de producto %s, codigo %s\n", producto->nombre, producto->codigo);
    if (producto->lotes) {
        rec = producto -> lotes;
        while (rec) {
            printf("Numero de lote: %s\n", rec->datosLote->numeroLote);
            printf("Cantidad de productos: %d\n", rec->datosLote->cantidadLote);
            printf("Fecha de caducidad: %s\n\n", rec->datosLote->fechaCaducidad);
            rec = rec->sig;
        }
    }
    else {
        printf("No hay lotes para mostrar\n");
    }
}

void menuProducto(struct Producto *producto) {
    int opcion;
    char aux;

    do {
        printf("Menu de opciones de producto ID: %s\n", producto->codigo);
        printf("1. Ver detalle del producto\n");
        printf("2. Mostrar lotes del producto\n");
        printf("3. Volver al menu anterior\n");
        printf("Ingrese una opcion: ");

        scanf("%d%c", &opcion, &aux);

        switch (opcion) {
            case 1:
                mostrarDetalleProducto(producto);
                break;
            case 2:
                mostrarLotesProducto(producto);
                break;
            case 3:
                printf("Volviendo al menu anterior...\n");
                break;
            default:
                printf("Opcion no valida, por favor ingrese una opcion valida.\n\n");
                break;

        }
    } while (opcion != 3);
}

struct Producto *seleccionarProducto(struct NodoProducto *root) {
    // Función para seleccionar un producto según ID
    // Retorna un puntero al producto seleccionado
    // Imprime mensajes según errores
    struct Producto *producto;
    char id[10], aux;

    if (!root) {
        printf("No existen productos en el sistema.\n");
        return NULL;
    }

    printf("Ingrese el codigo del producto que desea seleccionar (10 caracteres): ");
    scanf("%s%c", id, &aux);
    producto = getProducto(root, id);
    if (!producto) {
        printf("Producto no encontrado / codigo no válido\n");
        return NULL;
    }
    return producto;
}

void mostrarInventarioAux(struct NodoProducto *root) {
    // Función para imprimir los productos del inventario de forma recursiva
    // Imprime nombre, código y stock de cada producto
    if (!root)
        return;
    mostrarInventarioAux(root->izq);
    printf("%s, codigo: %s, stock: %d\n\n", root->datosProducto->nombre,
           root->datosProducto->codigo, root->datosProducto->cantidad);
    mostrarInventarioAux(root->der);
}

void mostrarInventario(struct NodoProducto *root) {
    // Función para mostrar el inventario de productos
    // Imprime un mensaje si no hay productos en el sistema
    // Llama a una función auxiliar para mostrar los productos
    if (!root) {
        printf("No existen productos en el sistema.\n");
        return;
    }
    printf("Inventario de productos\n");
    mostrarInventarioAux(root);
}

// TODO: Decidir cuál de las dos funciones de mostrar se va a usar

//void mostrarProductosSinStockAux(struct NodoProducto *root) {
//    // Función para imprimir los productos sin stock de forma recursiva
//    // Imprime nombre y código de cada producto
//    if (!root)
//        return;
//    mostrarProductosSinStockAux(root->izq);
//    if (root->datosProducto->cantidad == 0)
//        printf("%s, codigo: %s\n\n", root->datosProducto->nombre, root->datosProducto->codigo);
//    mostrarProductosSinStockAux(root->der);
//}

//void mostrarProductosSinStock(struct NodoProducto *root) {
//    // Función para mostrar los productos sin stock
//    // Imprime un mensaje si no hay productos en el sistema
//    // Llama a una función auxiliar para mostrar los productos sin stock
//    if (!root) {
//        printf("No existen productos en el sistema.\n");
//        return;
//    }
//    printf("Productos sin stock (Codigo, Nombre)\n");
//    mostrarProductosSinStockAux(root);
//}

void productosACaducarEnFecha(struct NodoProducto *root) {
    // Función para mostrar los productos a caducar en cierta fecha
    // Recibe la raíz del árbol de productos y lee la fecha a revisar
    // Imprime un mensaje si no hay productos en el sistema
    char fecha[11], aux;
    if (!root) {
        printf("No existen productos en el sistema.\n\n");
        return;
    }
    printf("Ingrese la fecha a revisar (dd/mm/aaaa): ");
    scanf("%s%c", fecha, &aux);
    mostrarProductosACaducar(root, fecha);
}

void mostrarProductosBajoStock(struct NodoProducto *inventario, struct NodoTransaccion *ventas) {
    // Función para mostrar los productos con bajo stock
    // Recibe la raíz del árbol de productos y la lista de ventas
    // Imprime un mensaje si no hay productos en el sistema o si no hay ventas
    // Llama a una función auxiliar para mostrar los productos con bajo stock
    if (!inventario) {
        printf("No existen productos en el sistema.\n\n");
        return;
    }
    if (!ventas) {
        printf("No existen ventas en el sistema.\n");
        printf("Productos con bajo stock se basa en promedio de ventas.\n\n");
        return;
    }
    printf("Productos con bajo stock\n");
    mostrarProductosBajoStockAux(inventario, ventas);
}

int confirmacionEliminar(struct Producto *producto) {
    // Función para confirmar la eliminación de un producto
    // Recibe un puntero al producto
    // Retorna 1 (true) o 0 (false)
    char opcion, aux;
    printf("Está seguro que desea eliminar %s, codigo %s del sistema? (s/n): ", producto->nombre, producto->codigo);
    scanf("%c%c", &opcion, &aux);
    if (opcion == 's' || opcion == 'S')
        return 1;
    return 0;
}

void menuEliminar(struct NodoProducto *inventario) {
    // Función con menú de usuario para eliminar un producto del inventario
    // Recibe la raíz del inventario
    // Imprime un mensaje si no hay productos en el sistema
    // Pregunta por confirmación para eliminar el producto
    struct Producto *producto;
    if (!inventario) {
        printf("No existen productos en el sistema.\n\n");
        return;
    }
    producto = seleccionarProducto(inventario);
    if (producto) {
        if (confirmacionEliminar(producto)) {
            if (quitarProducto(&inventario, producto->codigo))
                printf("Producto eliminado exitosamente.\n");
            else
                printf("Error al eliminar el producto.\n");
        }
    }
}

void menuInventario(struct Farmacia *farmacia) {
    // Función para el menú con opciones relacionadas al inventario de productos
    int opcion;
    char aux;
    struct Producto *producto;

    do {
        printf("\nMenu de inventario\n");
        printf("1. Ver inventario\n");
        printf("2. Ver productos a caducar en cierta fecha\n");
        printf("3. Ver productos con bajo stock\n");
        printf("4. Ver productos sin stock\n");
        printf("5. Ver productos con stock\n");
        printf("6. Opciones de un producto\n");
        printf("7. Eliminar un producto\n");
        printf("8. Volver al menu anterior\n");
        printf("Seleccione una opcion: ");

        scanf("%d%c", &opcion, &aux);

        switch (opcion) {
            case 1:
                mostrarInventario(farmacia->inventario);
                break;
            case 2:
                productosACaducarEnFecha(farmacia->inventario);
                break;
            case 3:
                mostrarProductosBajoStock(farmacia->inventario, farmacia->ventas);
                break;
            case 4:
                mostrarProductosSinStock(farmacia->inventario);
                break;
            case 5:
                mostrarProductosConStock(farmacia->inventario);
                break;
            case 6:
                producto = seleccionarProducto(farmacia->inventario);
                if (producto)
                    menuProducto(producto);
                else
                    opcion = 0;
                break;
            case 7:
                menuEliminar(farmacia->inventario);
                break;
            case 8:
                printf("Volviendo al menu anterior...\n");
                break;
            default:
                printf("Opcion no valida, por favor ingrese una opcion valida.\n\n");
                break;
        }
    } while (opcion != 8);
}

struct Transaccion *seleccionarTransaccion(struct NodoTransaccion *headTransaccion, char tipoTransaccion) {
    // Función para seleccionar una orden de compra o venta según ID
    // Retorna un puntero a la orden de compra o venta seleccionada
    // Según el tipo de transacción recibido por parámetro hace un print distinto
    struct Transaccion *transaccion;
    int id;
    char aux;

    if (tipoTransaccion == 'C') {
        printf("Ingrese el ID de la orden de compra que desea seleccionar: ");
    } else {
        printf("Ingrese el ID de la venta que desea seleccionar: ");
    }

    scanf("%d%c", &id, &aux);
    transaccion = getTransaccion(headTransaccion, id);
    return transaccion;
}

void mostrarProductosTransaccion(struct Transaccion *transaccion) {
    int i;
    for (i = 0; i < transaccion->totalProductosDistintos; i++)
        printf("%d. %s\n", i+1, transaccion->productos[i]->nombre);
}

void mostrarDetalleVenta(struct NodoTransaccion *headVentas) {
    // Función para mostrar el detalle de una venta específica
    // Imprime un mensaje si no hay ventas en el sistema o si no fue encontrada la venta
    struct Transaccion *venta;

    if (!headVentas) {
        printf("No existen ventas en el sistema.\n");
        return;
    }

    venta = seleccionarTransaccion(headVentas, 'V');

    if (!venta) {
        printf("Venta no encontrada.\n");
        return;
    }
    printf("Detalle de la Venta:\n");
    printf("ID: %d\n", venta->id);
    printf("Nombre del cliente: %s\n", venta->nombre);
    printf("Rut del cliente: %s\n", venta->rut);
    printf("Costo total: $%d\n", venta->costoTotal);
    printf("Total de productos distintos: %d\n", venta->totalProductosDistintos);
    printf("Total de productos comprados: %d\n", venta->cantidadProductos);
    mostrarProductosTransaccion(venta);
    printf("Estado de envio: ");
    if (venta->estadoEnvio == 'R' || venta->estadoEnvio == 'r')
        printf("Recibido\n");
    else
        printf("Pendiente\n");
    printf("Fecha de solicitud: %s\n", venta->fechaSolicitud);
    if (venta->estadoEnvio == 'R' || venta->estadoEnvio == 'r')
        printf("Fecha de llegada: %s\n", venta->fechaLlegada);
}

void registrarVenta(struct Farmacia *farmacia) {
    // Función para registrar una venta en el sistema
    // Crea la venta, la agrega a la lista de ventas y reduce el stock del inventario
    // Imprime mensajes según errores o éxito
    struct Transaccion *venta;
    struct NodoTransaccion *nodoVenta;

    if (!farmacia->inventario) {
        printf("No existen productos en el sistema.\n\n");
        return;
    }
    venta = leerDatosTransaccion(farmacia->inventario, 'V');
    if (!venta)
        return;
    nodoVenta = crearNodoTransaccion(venta);
    if (agregarNodoTransaccion(&farmacia->ventas, nodoVenta)) {
        quitarVentaAInventario(farmacia->inventario, venta);
        printf("Venta registrada con exito.\n\n");
    }
    else {
        printf("No se pudo registrar la venta.\n\n");
    }
}

void actualizarInventarioFarmacia(struct Farmacia *farmacia) {
    // Función para actualizar el inventario de la farmacia
    // Recibe la farmacia y actualiza el total de productos en bodega
    farmacia->totalProductos = getTotalStockFarmacia(farmacia->inventario);
}

void menuVentas(struct Farmacia *farmacia) {
    // Función para el menú con opciones relacionadas a ventas de la farmacia
    int opcion;
    char aux;

    do {
        printf("\nMenu de ventas de la farmacia\n");
        printf("1. Registrar venta\n");
        printf("2. Ver ventas\n");
        printf("3. Ver ventas de productos con receta\n");
        printf("4. Ver detalle de una venta\n");
        printf("5. Volver al menu anterior\n");
        printf("Seleccione una opcion: ");

        scanf("%d%c", &opcion, &aux);

        switch (opcion) {
            case 1:
                registrarVenta(farmacia);
                actualizarInventarioFarmacia(farmacia);
                break;
            case 2:
                mostrarVentas(farmacia->ventas);
                break;
            case 3:
                mostrarVentasReceta(farmacia->ventas);
                break;
            case 4:
                mostrarDetalleVenta(farmacia->ventas);
                break;
            case 5:
                printf("Volviendo al menu anterior...\n");
                break;
            default:
                printf("Opcion no valida, por favor ingrese una opcion valida.\n\n");
                break;
        }
    } while (opcion != 5);
}

void mostrarOrdenesCompra(struct NodoTransaccion *headCompras) {
    // Función para listar las órdenes de compra
    // Imprime un mensaje si no hay órdenes de compra en el sistema
    // Imprime id, nombre, costo total y el estado de la orden de compra
    if (!headCompras) {
        printf("No existen ordenes de compra en el sistema. \n");
        return;
    }
    printf("Ordenes de compra (Id, Nombre, Costo total, Estado):\n");
    while (headCompras) {
        printf("ID: %d, Nombre: %s, Costo total: %d, Estado: \n", headCompras->datosTransaccion->id,
               headCompras->datosTransaccion->nombre, headCompras->datosTransaccion->costoTotal);
        if (headCompras->datosTransaccion->estadoEnvio == 'R' || headCompras->datosTransaccion->estadoEnvio == 'r')
            printf("Recibido\n");
        else
            printf("Pendiente\n");
        headCompras = headCompras->sig;
    }
}

void mostrarDetalleOrdenCompra(struct NodoTransaccion *headCompras) {
    // Función para mostrar el detalle de una orden de compra específica
    // Imprime un mensaje si no hay órdenes de compra o si no fue encontrada la orden de compra
    struct Transaccion *ordenCompra;
    if (!headCompras) {
        printf("No existen ordenes de compra en el sistema. \n");
        return;
    }

    ordenCompra = seleccionarTransaccion(headCompras, 'C');

    if (!ordenCompra) {
        printf("Orden de compra no encontrada.\n");
        return;
    }
    printf("Detalle de la orden de compra:\n");
    printf("ID: %d\n", ordenCompra->id);
    printf("Nombre del proveedor: %s\n", ordenCompra->nombre);
    printf("Rut del proveedor: %s\n", ordenCompra->rut);
    printf("Costo total: $%d\n", ordenCompra->costoTotal);
    printf("Total de productos distintos: %d\n", ordenCompra->totalProductosDistintos);
    printf("Total de productos comprados: %d\n", ordenCompra->cantidadProductos);
    printf("Productos comprados:\n");
    mostrarProductosTransaccion(ordenCompra);
    printf("Estado de envio: ");
    if (ordenCompra->estadoEnvio == 'R' || ordenCompra->estadoEnvio == 'r')
        printf("Recibido\n");
    else
        printf("Pendiente\n");
    printf("Fecha de solicitud: %s\n", ordenCompra->fechaSolicitud);
    if (ordenCompra->estadoEnvio == 'R' || ordenCompra->estadoEnvio == 'r')
        printf("Fecha de llegada: %s\n", ordenCompra->fechaLlegada);
}

void registrarOrdenCompra(struct Farmacia *farmacia) {
    // Función para registrar una orden de compra en el sistema
    // Crea la orden de compra, la agrega a la lista de compras y agrega los productos al inventario
    // Imprime mensajes según errores o éxito
    struct Transaccion *compra;
    struct NodoTransaccion *nodoCompra;

    compra = leerDatosTransaccion(farmacia->inventario, 'C');
    if (!compra)
        return;
    if (compra->estadoEnvio == 'R' || compra->estadoEnvio == 'r')
        agregarCompraAInventario(&farmacia->inventario, compra);
    nodoCompra = crearNodoTransaccion(compra);
    if (agregarNodoTransaccion(&farmacia->compras, nodoCompra)) {
        printf("Orden de compra registrada con exito.\n\n");
    }
    else {
        printf("No se pudo registrar la orden de compra.\n\n");
    }
}

void actualizarEstadoOrdenCompra(struct NodoProducto *inventario, struct NodoTransaccion *headTransaccion) {
    struct Transaccion *ordenCompra;
    ordenCompra = seleccionarTransaccion(headTransaccion, 'C');
    if (!ordenCompra) {
        printf("Orden de compra no encontrada.\n");
    } else {
        ordenCompra->estadoEnvio = 'R';
        printf("Ingrese fecha de recepcion (DD/MM/AAAA): ");
        ordenCompra->fechaLlegada = leerCadena();
        agregarCompraAInventario(&inventario, ordenCompra);
    }
}

void menuCompras(struct Farmacia *farmacia) {
    // Función para el menú con opciones relacionadas a ordenes de compra de la farmacia
    int opcion;
    char aux;

    do {
        printf("\nMenu de ordenes de compra de la farmacia\n");
        printf("1. Registrar orden de compra\n");
        printf("2. Ver ordenes de compra\n");
        printf("3. Actualizar estado de orden de compra\n");
        printf("4. Ver detalle de una orden de compra\n");
        printf("5. Volver al menu anterior\n");
        printf("Seleccione una opcion: ");

        scanf("%d%c", &opcion, &aux);

        switch(opcion) {
            case 1:
                registrarOrdenCompra(farmacia);
                actualizarInventarioFarmacia(farmacia);
                break;
            case 2:
                mostrarOrdenesCompra(farmacia->compras);
                break;
            case 3:
                actualizarEstadoOrdenCompra(farmacia->inventario, farmacia->compras);
                actualizarInventarioFarmacia(farmacia);
                break;
            case 4:
                mostrarDetalleOrdenCompra(farmacia->compras);
                break;
            case 5:
                printf("Volviendo al menu anterior...\n");
                break;
            default:
                printf("Opcion no valida, por favor ingrese una opcion valida.\n\n");
                break;
        }
    } while (opcion != 5);
}

void mostrarCategoriaMasVendida(struct NodoTransaccion *headVentas) {
    // Función para mostrar la categoría más vendida en la farmacia
    // Imprime un mensaje si no hay ventas en el sistema o si no se pudo determinar la categoría
    // Recibe la lista de ventas y llama a una función auxiliar
    char *categoriaMasVendida;
    if (!headVentas) {
        printf("No existen ventas en el sistema.\n\n");
        return;
    }

    categoriaMasVendida = getCategoriaMasVendida(headVentas);
    if (!categoriaMasVendida) {
        printf("No se pudo determinar la categoria mas vendida.\n\n");
        return;
    }
    printf("La categoria mas vendida es: %s\n\n", categoriaMasVendida);
}

void mostrarCategoriaMasVendidaEstacion(struct NodoTransaccion *headVentas) {
    // Función para mostrar la categoría más vendida en una estación del año
    // Imprime un mensaje si no hay ventas en el sistema o si no se pudo determinar la categoría
    // Recibe la lista de ventas y llama a una función auxiliar
    char estacion, aux, *categoriaMasVendida;
    if (!headVentas) {
        printf("No existen ventas en el sistema.\n\n");
        return;
    }

    printf("Ingrese la estacion del año (P: Primavera, V: Verano, O: Otoño, I: Invierno): ");
    scanf("%c%c", &estacion, &aux);

    categoriaMasVendida = getCategoriaMasVendidaEstacion(headVentas, estacion);
    if (!categoriaMasVendida) {
        printf("No se pudo determinar la categoria mas vendida en la estacion.\n\n");
        return;
    }
    printf("La categoria mas vendida en la estacion es: %s\n\n", categoriaMasVendida);
}

void mostrarProductoMasVendido(struct Farmacia *farmacia) {
    // Función para mostrar el producto más vendido en la farmacia
    // Imprime un mensaje si no hay ventas o productos en el sistema o si no se pudo determinar el producto
    // Recibe la farmacia y llama a una función auxiliar
    struct Producto *productoMasVendido;
    if (!farmacia->ventas) {
        printf("No existen ventas en el sistema.\n\n");
        return;
    }
    if (!farmacia->inventario) {
        printf("No existen productos en el sistema.\n\n");
        return;
    }
    productoMasVendido = getProductoMasVendido(farmacia->inventario, farmacia->ventas);
    if (!productoMasVendido) {
        printf("No se pudo determinar el producto mas vendido.\n\n");
        return;
    }
    printf("El producto mas vendido es: %s, codigo: %s\n\n", productoMasVendido->nombre,
           productoMasVendido->codigo);
}

void mostrarProductoMenosVendido(struct Farmacia *farmacia) {
    // Función para mostrar el producto menos vendido en la farmacia
    // Imprime un mensaje si no hay ventas o productos en el sistema o si no se pudo determinar el producto
    // Recibe la farmacia y llama a una función auxiliar
    struct Producto *productoMenosVendido;
    if (!farmacia->ventas) {
        printf("No existen ventas en el sistema.\n\n");
        return;
    }
    if (!farmacia->inventario) {
        printf("No existen productos en el sistema.\n\n");
        return;
    }
    productoMenosVendido = getProductoMenosVendido(farmacia->inventario, farmacia->ventas);
    if (!productoMenosVendido) {
        printf("No se pudo determinar el producto menos vendido.\n\n");
        return;
    }
    printf("El producto mas vendido es: %s, codigo: %s\n\n", productoMenosVendido->nombre,
           productoMenosVendido->codigo);
}

void mostrarIngresosFarmacia(struct NodoTransaccion *headVentas) {
    // Función para mostrar los ingresos de la farmacia
    // Recibe la lista de ventas y llama a una función auxiliar
    int totalIngresos;

    totalIngresos = totalIngresosFarmacia(headVentas);
    printf("Los ingresos de la farmacia son: $%d\n\n", totalIngresos);
}

void mostrarVentasFarmacia(struct NodoTransaccion *headVentas) {
    // Función para mostrar el número de ventas de la farmacia
    // Recibe la lista de ventas y llama a una función auxiliar
    int totalVentas;

    totalVentas = totalVentasFarmacia(headVentas);
    printf("El numero de ventas de la farmacia es: %d\n\n", totalVentas);
}

void menuAnalisisDatosFarmacia(struct Farmacia *farmacia) {
    int opcion;
    char aux;

    do {
        printf("\nMenu de analisis de datos de la farmacia\n");
        printf("1. Categoria mas vendida\n");
        printf("2. Categoria mas vendida por estacion\n");
        printf("3. Producto mas vendido\n");
        printf("4. Producto menos vendido\n");
        printf("5. Ingresos de la farmacia\n");
        printf("6. Número de ventas de la farmacia\n");
        printf("7. Volver al menu anterior\n");
        printf("Seleccione una opcion: ");

        scanf("%d%c", &opcion, &aux);

        switch (opcion) {
            case 1:
                mostrarCategoriaMasVendida(farmacia->ventas);
                break;
            case 2:
                mostrarCategoriaMasVendidaEstacion(farmacia->ventas);
                break;
            case 3:
                mostrarProductoMasVendido(farmacia);
                break;
            case 4:
                mostrarProductoMenosVendido(farmacia);
                break;
            case 5:
                mostrarIngresosFarmacia(farmacia->ventas);
                break;
            case 6:
                mostrarVentasFarmacia(farmacia->ventas);
                break;
            case 7:
                printf("Volviendo al menu anterior...\n");
                break;
            default:
                printf("Opcion no valida, por favor ingrese una opcion valida.\n\n");
                break;
        }
    } while (opcion != 7);
}

void menuUnaFarmacia(struct Farmacia *farmacia) {
    // Función para el menú principal de una farmacia específica
    int opcion;
    char aux;

    do {
        printf("\nMenu de la farmacia\n");
        printf("1. Detalle de la farmacia\n");
        printf("2. Opciones de inventario/productos\n");
        printf("3. Opciones de venta\n");
        printf("4. Opciones de ordenes de compra\n");
        printf("5. Opciones de analisis de datos\n");
        printf("6. Volver al menu anterior\n");
        printf("Seleccione una opcion: ");

        scanf("%d%c", &opcion, &aux);

        switch (opcion) {
            case 1:
                mostrarDetalleFarmacia(farmacia);
                break;
            case 2:
                menuInventario(farmacia);
                break;
            case 3:
                menuVentas(farmacia);
                break;
            case 4:
                menuCompras(farmacia);
                break;
            case 5:
                menuAnalisisDatosFarmacia(farmacia);
                break;
            case 6:
                printf("Volviendo al menu anterior...\n");
                break;
            default:
                printf("Opcion no valida, por favor ingrese una opcion valida.\n\n");
                break;
        }
    } while (opcion != 6);
}

void mostrarFarmacias(struct NodoFarmacia *headFarmacias) {
    // Función para mostrar las farmacias registradas
    struct NodoFarmacia *curr;
    if (!headFarmacias) {
        printf("No existen farmacias en el sistema. \n");
        return;
    }
    curr = headFarmacias;
    printf("Farmacias (ID, Ciudad, Region):\n");
    do {
        printf("%s, %s, %s\n", curr->datosFarmacia->id, curr->datosFarmacia->ciudad, curr->datosFarmacia->region);
        curr = curr->sig;
    } while (curr != headFarmacias);
}

struct Farmacia *seleccionarFarmacia(struct NodoFarmacia *headFarmacias) {
    // Función para que usuario seleccione una farmacia según ID
    // Retorna un puntero a la farmacia seleccionada
    // Imprime mensaje según errores
    struct Farmacia *farmacia;
    char *idBuscado;

    if (!headFarmacias) {
        printf("No existen farmacias en el sistema.\n");
        return NULL;
    }

    printf("Ingrese el ID de la farmacia a la que desea ingresar: ");
    idBuscado = leerCadena();
    farmacia = getFarmacia(headFarmacias, idBuscado);
    if (!farmacia) {
        printf("Farmacia no encontrada / ID no valido.\n");
    }
    return farmacia;
}

void agregarFarmaciaSistema(struct NodoFarmacia **headFarmacias) {
    // Función para agregar una nueva farmacia al sistema
    // Imprime mensaje de error si no fue posible
    struct NodoFarmacia *nuevoNodo;
    struct Farmacia *nuevaFarmacia;

    printf("Agregar una nueva farmacia al sistema\n");
    nuevaFarmacia = leerDatosFarmacia();
    nuevoNodo = crearNodoFarmacia(nuevaFarmacia);
    if (agregarNodoFarmacia(headFarmacias, nuevoNodo))
        printf("La farmacia se agrego exitosamente.\n");
    else
        printf("La farmacia ya existe en el sistema.\n");
}

void eliminarFarmaciaSistema(struct NodoFarmacia **headFarmacias) {
    // Función para eliminar una farmacia del sistema
    // Imprime mensaje de error si no fue posible
    char *idEliminar;

    if (!*headFarmacias) {
        printf("No existen farmacias en el sistema.\n");
        return;
    }

    printf("Eliminar una farmacia del sistema\n");
    printf("Ingrese el ID de la farmacia que desea eliminar: ");
    idEliminar = leerCadena();
    if (eliminarFarmacia(headFarmacias, idEliminar))
        printf("La farmacia fue eliminada exitosamente.\n");
    else
        printf("Error al eliminar la farmacia o farmacia no encontrada.\n");
}

void menuFarmacias(struct NodoFarmacia **headFarmacias) {
    // Función para el menú de farmacias del sistema
    int opcion;
    char aux;
    struct Farmacia *farmacia;

    do {
        printf("\nMenu de farmacias de FarmaSalud\n");
        printf("1. Listar farmacias\n");
        printf("2. Ingresar a una farmacia\n");
        printf("3. Agregar una farmacia\n");
        printf("4. Eliminar una farmacia\n");
        printf("5. Volver al menu principal\n");
        printf("Seleccione una opcion: ");

        scanf("%d%c", &opcion, &aux);

        switch (opcion) {
            case 1:
                mostrarFarmacias(*headFarmacias);
                break;
            case 2:
                farmacia = seleccionarFarmacia(*headFarmacias);
                if (farmacia)
                    menuUnaFarmacia(farmacia);
                else
                    opcion = 0;
                break;
            case 3:
                agregarFarmaciaSistema(headFarmacias);
                break;
            case 4:
                eliminarFarmaciaSistema(headFarmacias);
                break;
            case 5:
                printf("Volviendo al menu principal...\n");
                break;
            default:
                printf("Opcion no valida, por favor ingrese una opcion valida.\n\n");
                break;
        }
    } while (opcion != 5);
}

int confirmarSalida() {
    // Función para confirmar la salida del sistema
    // Retorna un 1 (true) si el usuario confirma, 0 (false) si no
    char opcion, aux;
    printf("¿Esta seguro/a que desea salir del sistema? (s/n): ");
    scanf("%c%c", &opcion, &aux);
    if (opcion == 's' || opcion == 'S')
        return 1;
    return 0;
}

void mostrarFarmaciaConMasVentas(struct NodoFarmacia *headFarmacias) {
    // Función para mostrar la farmacia con más ventas en el sistema
    // Recibe la lista de farmacias y llama a una función auxiliar
    // Imprime un mensaje si no hay farmacias en el sistema o si no se pudo determinar la farmacia
    struct Farmacia *farmaciaMasVentas;

    if (!headFarmacias) {
        printf("No existen farmacias en el sistema.\n");
        return;
    }

    farmaciaMasVentas = getFarmaciaConMasVentas(headFarmacias);
    if (!farmaciaMasVentas) {
        printf("No se pudo determinar la farmacia con mas ventas.\n");
        return;
    }
    printf("La farmacia con mas ventas es:\n");
    printf("ID: %s\n", farmaciaMasVentas->id);
    printf("Ciudad: %s\n", farmaciaMasVentas->ciudad);
    printf("Region: %s\n\n", farmaciaMasVentas->region);
}

void mostrarFarmaciaConMasIngresos(struct NodoFarmacia *headFarmacias) {
    // Función para mostrar la farmacia con más ingresos en el sistema
    // Recibe la lista de farmacias y llama a una función auxiliar
    // Imprime un mensaje si no hay farmacias en el sistema o si no se pudo determinar la farmacia
    struct Farmacia *farmaciaMasIngresos;

    if (!headFarmacias) {
        printf("No existen farmacias en el sistema.\n");
        return;
    }

    farmaciaMasIngresos = getFarmaciaConMasIngresos(headFarmacias);
    if (!farmaciaMasIngresos) {
        printf("No se pudo determinar la farmacia con mas ingresos.\n");
        return;
    }
    printf("La farmacia con mas ingresos es:\n");
    printf("ID: %s\n", farmaciaMasIngresos->id);
    printf("Ciudad: %s\n", farmaciaMasIngresos->ciudad);
    printf("Region: %s\n\n", farmaciaMasIngresos->region);
}

void menuAnalisisFarmaSalud(struct FarmaSalud *farmaSalud) {
    // Función para el menú de análisis de datos de FarmaSalud
    // Recibe la estructura de FarmaSalud y llama a funciones auxiliares
    int opcion;
    char aux;

    do {
        printf("\nMenu de analisis de datos de FarmaSalud\n");
        printf("1. Farmacia con más ventas\n");
        printf("2. Farmacia con mayores ingresos\n");
        printf("3. Volver al menu anterior\n");
        printf("Seleccione una opcion: ");

        scanf("%d%c", &opcion, &aux);

        switch (opcion) {
            case 1:
                mostrarFarmaciaConMasVentas(farmaSalud->headFarmacias);
                break;
            case 2:
                mostrarFarmaciaConMasIngresos(farmaSalud->headFarmacias);
                break;
            case 3:
                printf("Volviendo al menu anterior...\n");
                break;
            default:
                printf("Opcion no valida, por favor ingrese una opcion valida.\n\n");
                break;
        }
    } while (opcion != 3);
}

void menuFarmaSalud(struct FarmaSalud *farmaSalud) {
    // Función para el menú principal de usuario de FarmaSalud
    int opcion;
    char aux;

    do {
        printf("\nMenu de FarmaSalud\n");
        printf("1. Ingresar a menu de farmacias\n");
        printf("2. Ingresar a menu de analisis de datos\n");
        printf("3. Salir\n");
        printf("Seleccione una opcion: ");

        scanf("%d%c", &opcion, &aux);

        switch (opcion) {
            case 1:
                menuFarmacias(&farmaSalud->headFarmacias);
                break;
            case 2:
                menuAnalisisFarmaSalud(farmaSalud);
                break;
            case 3:
                if (confirmarSalida()) {
                    printf("Saliendo del sistema de FarmaSalud...\n");
                    break;
                }
                else
                    opcion = 0;
                break;
            default:
                printf("Opcion no valida, por favor ingrese una opcion valida.\n\n");
                break;
        }
    } while (opcion != 3);
}

int main(void) {
    struct FarmaSalud *farmaSalud;
    farmaSalud = crearFarmasalud();
    if (farmaSalud)
        menuFarmaSalud(farmaSalud);
    return 0;
}