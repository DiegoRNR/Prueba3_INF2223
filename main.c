//
// Created by Diego Negrín on 22-05-24.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
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
    char *rut;
    int cantidadProductos;
    int totalProductosDistintos;
    struct Producto **productos;
    int costoTotal;
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

struct FarmaSalud {
    struct NodoFarmacia *headFarmacias;
};


char *leerCadena() {
    // Lee una cadena de caracteres en un buffer largo 100, y la copia en un puntero con el largo exacto
    // de la cadena leida. Retorna el puntero donde se copio la cadena leida.
    char buffer[100], caracter, *cadena;
    int i;
    for (i = 0; i < 100 && caracter != '\0'; i++) {
        caracter = getchar();
        if (caracter == '\n')
            caracter = '\0';
        buffer[i] = toupper(caracter);
    }
    cadena = (char *) malloc((sizeof(char))*(strlen(buffer) + 1));
    strcpy(cadena, buffer);
    return cadena;
}

struct Fecha *leerFecha() {
    // Da la opcion al usuario de obtener la fecha actual o que se ingrese una fecha personalizada.
    // Retorna un puntero a struct Fecha que contiene la fecha elegida.
    struct Fecha *fecha;
    struct tm *tiempoLocal;
    time_t tiempoActual;
    int dia, mes, year, opcion;
    char aux;

    do {
        printf("\nSeleccione modo de ingreso de fecha:\n");
        printf("1. Fecha Actual             2. Fecha Personalizada\n");
        printf("Ingrese la opcion que desea realizar (1-2): ");
        scanf("%d%c", &opcion, &aux);
        switch (opcion) {
            case 1:
                time(&tiempoActual);
                tiempoLocal = localtime(&tiempoActual);
                dia = tiempoLocal->tm_mday;
                mes = tiempoLocal->tm_mon + 1;
                year = tiempoLocal->tm_year + 1900;
                break;
            case 2:
                printf("Ingrese fecha (DD/MM/AAAA): ");
                scanf("%d%c%d%c%d%c", &dia, &aux, &mes, &aux, &year, &aux);
                break;
            default:
                printf("Opcion invalida, por favor ingrese una opcion valida.\n\n");
                break;
        }
    } while (opcion < 1 || opcion > 2);

    fecha = (struct Fecha *) malloc(sizeof(struct Fecha));
    fecha->dia = dia;
    fecha->mes = mes;
    fecha->year = year;
    return fecha;
}

void freeListaLotes(struct NodoLote *head) {
    // Recibe una lista simplemente enlazada de struct NodoLote, libera la memoria asignada a dicha lista.
    struct NodoLote *aux;
    while (head != NULL) {
        aux = head;
        head = head->sig;
        free(aux->datosLote->fechaCaducidad);
        free(aux->datosLote);
        free(aux);
    }
}

void freeProducto(struct Producto *producto) {
    // Recibe un puntero a struct Producto, libera la memoria asignada a dicho struct.
    free(producto->nombre);
    free(producto->descripcion);
    free(producto->categoria);
    free(producto->proveedor);
    freeListaLotes(producto->lotes);
    free(producto);
}

void freeCompraVenta(struct CompraVenta *compraVenta) {
    // Recibe un puntero a struct CompraVenta, libera la memoria asignada a dicho puntero.
    int i;
    free(compraVenta->nombre);
    free(compraVenta->rut);
    for (i = 0; i < compraVenta->cantidadProductos; i++)
        freeProducto(compraVenta->productos[i]);
    free(compraVenta->fechaSolicitud);
    free(compraVenta->fechaLlegada);
    free(compraVenta);
}

void freeListaCompraVenta(struct NodoCompraVenta *head) {
    // Recibe una lista simplemente enlazada de struct NodoCompraVenta, libera la memoria asignada a dicha lista.
    struct NodoCompraVenta *aux;
    while (head != NULL) {
        aux = head;
        head = head->sig;
        freeCompraVenta(aux->datosCompraVenta);
        free(aux);
    }
}

void freeArbolProductos(struct NodoProducto *root) {
    // Recibe un arbol binario de busqueda de struct NodoProducto, libera la memoria asignada a dicho arbol.
    if (root != NULL) {
        freeArbolProductos(root->izq);
        freeArbolProductos(root->der);

        freeProducto(root->datosProducto);
        free(root);
    }
}

void freeFarmacia(struct Farmacia *farmacia) {
    // Recibe un puntero a struct Farmacia, libera la memoria asignada a dicho struct.
    free(farmacia->id);
    free(farmacia->region);
    free(farmacia->ciudad);
    freeListaCompraVenta(farmacia->ventas);
    freeListaCompraVenta(farmacia->compras);
    freeArbolProductos(farmacia->inventario);
}

void freeListaFarmacias(struct NodoFarmacia *head) {
    // Recibe una lista doblemente enlazada de struct NodoFarmacia, libera la memoria asignada a dicha lista.
    struct NodoFarmacia *aux;
    if (head != NULL) {
        aux = head;
        head = head->sig;
        freeFarmacia(head->datosFarmacia);
        free(aux);
    }
}

struct FarmaSalud *crearFarmasalud() {
    // Función para crear y asignar memoria al sistema de FarmaSalud
    struct FarmaSalud *nuevoFarmaSalud;
    nuevoFarmaSalud = (struct FarmaSalud *) malloc(sizeof(struct FarmaSalud));
    nuevoFarmaSalud->headFarmacias = NULL;
    return nuevoFarmaSalud;
}

struct Lote *crearLote() {
    // Lee datos de la entrada del usuario y los asigna a un struct Lote.
    // Retorna un puntero al struct Lote que contiene los datos leidos.
    struct Lote *nuevoLote;
    struct Fecha *fechaCaducidad;
    int numeroLote, cantidadLote;

    printf("Numero de lote: ");
    scanf("%d", &numeroLote);
    printf("Cantidad de productos del lote: ");
    scanf("%d", &cantidadLote);
    printf("Fecha de caducidad: ");
    fechaCaducidad = leerFecha();

    nuevoLote = (struct Lote *) malloc(sizeof(struct Lote));
    nuevoLote->numeroLote = numeroLote;
    nuevoLote->cantidadLote = cantidadLote;
    nuevoLote->fechaCaducidad = fechaCaducidad;
    return nuevoLote;
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

struct Lote *getLote(struct NodoLote *head, int numeroLoteBuscado) {
    // Recibe una lista simplemente enlazada de struct NodoLote y un numero de lote, busca el elemento que posea dichp
    // numero de lote. Retorna un puntero a struct Lote si se encuentra en la lista, en caso contrario retorna NULL.
    struct NodoLote *rec;
    if (head != NULL) {
        rec = head;
        while (rec != NULL) {
            if (rec->datosLote->numeroLote == numeroLoteBuscado)
                return rec->datosLote;
            rec = rec->sig;
        }
    }
    return NULL;
}

int agregarNodoLote(struct NodoLote **head, struct NodoLote *nuevoNodo) {
    // Recibe una lista simplemente enlazada de struct NodoLote y un puntero a struct NodoLote, agrega el nodo recibido
    // a la lista. Retorna 1 en caso de exito, en caso contrario retorna 0.
    struct NodoLote *rec;
    if (nuevoNodo != NULL) {
        if (*head == NULL) {
            *head = nuevoNodo;
            return 1;
        }
        if (getLote(*head, nuevoNodo->datosLote->numeroLote) == NULL) {
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

struct NodoLote *getNodoLote(struct NodoLote *head, int numeroLoteBuscado) {
    // Recibe una lista simplemente enlazada de struct NodoLote y un numero de lote, busca el elemento que posea dicho
    // numero de lote. Retorna un puntero a struct NodoLote si se encuentra en la lista, en caso contrario retorna NULL.
    struct NodoLote *rec;
    if (head != NULL) {
        rec = head;
        while (rec != NULL) {
            if (rec->datosLote->numeroLote == numeroLoteBuscado)
                return rec;
            rec = rec->sig;
        }
    }
    return NULL;
}

int eliminarLote(struct NodoLote **head, int numeroLoteAEliminar) {
    // Recibe una lista simplemente enlazada de struct NodoLote y numero de lote, elimina y libera la memoria asignada
    // al elemento que posea dicho numero de lote. Retorna 1 en caso de exito, en caso contrario retorna 0.
    struct NodoLote *rec, *nodoAEliminar;
    if (*head != NULL) {
        nodoAEliminar = getNodoLote(*head, numeroLoteAEliminar);
        if (nodoAEliminar != NULL) {
            if (nodoAEliminar == *head) {
                *head = (*head)->sig;
            } else {
                rec = *head;
                while (rec->sig != NULL && rec->sig != nodoAEliminar)
                    rec = rec->sig;
                rec->sig = rec->sig->sig;
            }
            free(nodoAEliminar->datosLote->fechaCaducidad);
            free(nodoAEliminar->datosLote);
            free(nodoAEliminar);
            return 1;
        }
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

struct Producto *crearProducto() {
    // Lee datos de la entrada del usuario y los asigna a un struct Producto.
    // Retorna un puntero al struct Producto que contiene los datos leidos.
    struct Producto *nuevoProducto;
    char *nombre, *categoria, *descripcion, *proveedor, codigo[11], aux;
    int precio, requiereReceta;

    printf("Nombre del producto: ");
    nombre = leerCadena();
    printf("Categoria del producto: ");
    categoria = leerCadena();
    printf("Descripcion del producto: ");
    descripcion = leerCadena();
    printf("Proveedor del producto: ");
    proveedor = leerCadena();
    printf("Codigo del producto (10 digitos): ");
    strcpy(codigo, leerCadena());
    printf("Precio del producto: ");
    scanf("%d%c", &precio, &aux);
    printf("Producto requiere receta (0/1): ");
    scanf("%d%c", &requiereReceta, &aux);

    nuevoProducto = (struct Producto *) malloc(sizeof(struct Producto));
    nuevoProducto->nombre = nombre;
    nuevoProducto->categoria = categoria;
    nuevoProducto->descripcion = descripcion;
    nuevoProducto->proveedor = proveedor;
    strcpy(nuevoProducto->codigo, codigo);
    nuevoProducto->precio = precio;
    nuevoProducto->requiereReceta = requiereReceta;
    nuevoProducto->lotes = NULL;
    nuevoProducto->cantidad = getCantidadProducto(nuevoProducto->lotes);
    return nuevoProducto;
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

struct Producto *getProducto(struct NodoProducto *root, char *codigoBuscado) {
    // Recibe un arbol binario de busqueda de struct NodoProducto y un codigo, busca el elemento que posea dicho codigo.
    // Retorna un puntero a struct Producto si se encuentra el elemento en el arbol, en caso contrarip retorna NULL.
    if (!root)
        return NULL;
    if (strcmp(root->datosProducto->codigo, codigoBuscado) == 0)
        return root->datosProducto;
    if (compararCodigoProductos(root->datosProducto->codigo, codigoBuscado) > 0)
        return getProducto(root->izq, codigoBuscado);
    return getProducto(root->der, codigoBuscado);
}

struct NodoProducto *getPadreACodigo(struct NodoProducto *root, char *codigo) {
    // Recibe un arbol binario de busqueda de struct NodoProducto y un codigo, busca al posible nodo padre de dicho
    // codigo. Retorna un puntero a struct NodoProducto si existe el posible padre, en caso contrario retorna NULL.
    if (!root || strcmp(root->datosProducto->codigo, codigo) == 0)
        return NULL;
    if (compararCodigoProductos(root->datosProducto->codigo, codigo) > 0) {
        if (root->izq == NULL || compararCodigoProductos(root->izq->datosProducto->codigo, codigo) == 0)
            return root;
        return getPadreACodigo(root->izq, codigo);
    }
    if (root->der == NULL || compararCodigoProductos(root->der->datosProducto->codigo, codigo) == 0)
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
            if (compararCodigoProductos(nodoPadre->datosProducto->codigo,
                                        nuevoNodo->datosProducto->codigo) > 0) {
                nodoPadre->izq = nuevoNodo;
            } else {
                nodoPadre->der = nuevoNodo;
            }
            return 1;
        }
    }
    return 0;
}

struct Producto **getArregloProductos(int totalProductosDistintos) {
    // Recibe un entero correspondiente al largo de un arreglo de struct Producto, lee datos de la entrada del usuario y
    // los asigna en las posiciones del arreglo. Retorna un puntero al arreglo de struct Producto.
    struct Producto **arregloProductos;
    int i;
    arregloProductos = (struct Producto **) malloc(totalProductosDistintos * sizeof(struct Producto *));
    for (i = 0; i < totalProductosDistintos; i++)
        arregloProductos[i] = crearProducto();
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

struct CompraVenta *crearCompraVenta(char tipoTransaccion) {
    // Recibe un char que indica si se crea una compra o una venta, lee datos de la entrada del usuario y los asigna en
    // un struct CompraVenta. Retorna un puntero al struct CompraVenta que contiene los datos leidos.
    struct CompraVenta *nuevaCompraVenta;
    struct Producto **productos;
    struct Fecha *fechaSolicitud, *fechaLlegada = NULL;
    int totalProductosDistintos;
    char *nombre, *rut, estadoEnvio = 'N', aux;

    printf("Ingrese nombre: ");
    nombre = leerCadena();
    printf("Ingrese rut: ");
    rut = leerCadena();
    printf("Ingrese el total de productos distintos: ");
    scanf("%d%c", &totalProductosDistintos, &aux);
    productos = getArregloProductos(totalProductosDistintos);
    printf("Ingrese fecha de realizacion: ");
    fechaSolicitud = leerFecha();
    if (tipoTransaccion == 'C') {
        printf("Ingrese estado de envio de la compra (R:Recibido/P:Pendiente): ");
        scanf("%c%c", &estadoEnvio, &aux);
        if (estadoEnvio == 'R' || estadoEnvio == 'r') {
            printf("Fecha de llegada de la compra: ");
            fechaLlegada = leerFecha();
        }
    }

    nuevaCompraVenta = (struct CompraVenta *) malloc(sizeof(struct CompraVenta));
    nuevaCompraVenta->nombre = nombre;
    nuevaCompraVenta->nombre = rut;
    nuevaCompraVenta->totalProductosDistintos = totalProductosDistintos;
    nuevaCompraVenta->productos = productos;
    nuevaCompraVenta->cantidadProductos = getTotalProductos(productos, totalProductosDistintos);
    nuevaCompraVenta->costoTotal = getCostoTotal(productos, totalProductosDistintos);
    nuevaCompraVenta->estadoEnvio = estadoEnvio;
    nuevaCompraVenta->fechaSolicitud = fechaSolicitud;
    nuevaCompraVenta->fechaLlegada = fechaLlegada;
    return nuevaCompraVenta;
}

struct NodoCompraVenta *crearNodoCompraVenta(struct CompraVenta *compraVenta) {
    // Recibe un puntero a struct CompraVenta y lo asigna a un struct NodoCompraVenta.
    // Retorna un puntero al struct NodoCompraVenta que contiene el struct CompraVenta recibido.
    struct NodoCompraVenta *nuevoNodo = NULL;
    if (compraVenta != NULL) {
        nuevoNodo = (struct NodoCompraVenta *) malloc(sizeof(struct NodoCompraVenta));
        nuevoNodo->datosCompraVenta = compraVenta;
        nuevoNodo->sig = NULL;
    }
    return nuevoNodo;
}

struct CompraVenta *getCompraVenta(struct NodoCompraVenta *head, char *rutBuscado) {
    // Recibe una lista simplemente enlazada de struct NodoCompraVenta y un rut, busca el elemento que posea dicho rut.
    // Retorna un puntero a struct CompraVenta si se encuentra en la lista, en caso contrrio retorna NULL.
    struct NodoCompraVenta *rec;
    if (head != NULL) {
        rec = head;
        while (rec != NULL) {
            if (strcmp(rec->datosCompraVenta->rut, rutBuscado) == 0)
                return rec->datosCompraVenta;
            rec = rec->sig;
        }
    }
    return NULL;
}

int agregarNodoCompraVenta(struct NodoCompraVenta **head, struct NodoCompraVenta *nuevoNodo) {
    // Recibe una lista simplemente enlazada de struct CompraVenta y un puntero a struct NodoCompraVenta, agrega el
    // nodo recibido a la lista. Retorna 1 en caso de exito, en caso contrario retorna 0.
    struct NodoCompraVenta *rec;
    if (*head == NULL) {
        *head = nuevoNodo;
        return 1;
    }
    if (getCompraVenta(*head, nuevoNodo->datosCompraVenta->rut) == NULL) {
        rec = *head;
        while (rec->sig != NULL) {
            rec = rec->sig;
        }
        rec->sig = nuevoNodo;
        return 1;
    }
    return 0;
}

struct NodoCompraVenta *getNodoCompraVenta(struct NodoCompraVenta *head, char *rutBuscado) {
    // Recibe una lista simplemente enlazada de struct NodoCompraVenta y un rut, busca el elemento que posea dicho rut.
    // Retorna un puntero a struct NodoCompraVenta si se encuentra en la lista, en caso contrrio retorna NULL.
    struct NodoCompraVenta *rec;
    if (head != NULL) {
        rec = head;
        while (rec != NULL) {
            if (strcmp(rec->datosCompraVenta->rut, rutBuscado) == 0)
                return rec;
            rec = rec->sig;
        }
    }
    return NULL;
}

int eliminarCompraVenta(struct NodoCompraVenta **head, char *rutAEliminar) {
    // Recibe una lista simplemente enlazada de struct NodoCompraVenta y rut, elimina y libera la memoria asignada al
    // elemento que posea dicho rut. Retorna 1 en caso de exito, en caso contrario retorna 0.
    struct NodoCompraVenta *rec, *nodoAEliminar;
    if (*head != NULL) {
        nodoAEliminar = getNodoCompraVenta(*head, rutAEliminar);
        if (nodoAEliminar != NULL) {
            if (nodoAEliminar == *head) {
                *head = (*head)->sig;
            } else {
                rec = *head;
                while (rec->sig != NULL && rec->sig != nodoAEliminar)
                    rec = rec->sig;
                rec->sig = rec->sig->sig;
            }
            freeCompraVenta(nodoAEliminar->datosCompraVenta);
            free(nodoAEliminar);
            return 1;
        }
    }
    return 0;
}

struct Farmacia *crearFarmacia() {
    // Lee datos de la entrada del usuario y los asigna a un struct Farmacia.
    // Retorna un puntero a struct Farmacia que contiene los datos leidos.
    struct Farmacia *nuevaFarmacia;
    struct NodoProducto *inventario = NULL;
    struct NodoCompraVenta *ventas = NULL, *compras = NULL;
    char *id, *ciudad, *region, opcion, aux;
    int totalProductos = 0;

    printf("Ingrese ID de la nueva farmacia: ");
    id = leerCadena();
    printf("Ingrese region de la nueva farmacia: ");
    region = leerCadena();
    printf("Ingrese ciudad de la nueva farmacia: ");
    ciudad = leerCadena();
    printf("Desea agregar productos? (s/n): ");
    scanf("%c%c", &opcion, &aux);
    while (opcion == 's' || opcion == 'S') {
        agregarNodoProducto(&inventario, crearNodoProducto(crearProducto()));
        totalProductos++;
        printf("Desea agregar mas productos? (s/n): ");
        scanf("%c%c", &opcion, &aux);
    }
    printf("Desea agregar ventas? (s/n): ");
    scanf("%c%c", &opcion, &aux);
    while (opcion == 's' || opcion == 'S') {
        agregarNodoCompraVenta(&ventas, crearNodoCompraVenta(crearCompraVenta('V')));
        printf("Desea agregar mas ventas? (s/n): ");
        scanf("%c%c", &opcion, &aux);
    }
    printf("Desea agregar compras? (s/n): ");
    scanf("%c%c", &opcion, &aux);
    while (opcion == 's' || opcion == 'S') {
        agregarNodoCompraVenta(&compras, crearNodoCompraVenta(crearCompraVenta('C')));
        printf("Desea agregar mas compras? (s/n): ");
        scanf("%c%c", &opcion, &aux);
    }


    nuevaFarmacia = (struct Farmacia *) malloc(sizeof(struct Farmacia));
    nuevaFarmacia->id = id;
    nuevaFarmacia->region = region;
    nuevaFarmacia->ciudad = ciudad;
    nuevaFarmacia->maxCapacidad = CAPACIDAD_PROMEDIO;
    nuevaFarmacia->inventario = inventario;
    nuevaFarmacia->totalProductos = totalProductos;
    nuevaFarmacia->ventas = NULL;
    nuevaFarmacia->compras = NULL;
    return nuevaFarmacia;
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
    if (getFarmacia(*head, nuevoNodo->datosFarmacia->id) == NULL) {
        nuevoNodo->sig = *head;
        nuevoNodo->ant = (*head)->ant;
        (*head)->ant->sig = nuevoNodo;
        (*head)->ant = nuevoNodo;
        return 1;
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
            freeFarmacia(nodoAEliminar->datosFarmacia);
            free(nodoAEliminar);
            return 1;
        }
    }
    return 0;
}

int loteProximoACaducar(struct Lote *lote) {
    // Recibe un puntero a struct Lote, verifica si el lote caduca en un periodo de 3 meses desde la fecha actual.
    // Retorna 1 en caso de caducar en el periodo de 3 meses, en caso contrario retorna 0.
    struct tm *fechaLimite;
    time_t tiempo;
    int yearLimite, mesLimite;
    time(&tiempo);
    fechaLimite = localtime(&tiempo);
    fechaLimite->tm_mon += 3;
    mktime(fechaLimite);
    yearLimite = fechaLimite->tm_year + 1900;
    mesLimite = fechaLimite->tm_mon + 1;
    if (lote->fechaCaducidad->year < yearLimite) {
        return 1;
    } else if (lote->fechaCaducidad->year == yearLimite) {
        if (mesLimite - lote->fechaCaducidad->mes >= 0 && mesLimite - lote->fechaCaducidad->mes < 4) {
            return 1;
        }
    }
    return 0;
}

int hayLoteACaducar(struct NodoLote *head) {
    // Recibe una lista simplemente enlazada de struct NodoLote, verifica si hay lotes proximos a caducar.
    // Retorna 1 en caso de encontrar alguno, en caso contrario retorna 0.
    struct NodoLote *rec;
    if (head != NULL) {
        rec = head;
        while (rec != NULL) {
            if (loteProximoACaducar(rec->datosLote))
                return 1;
            rec = rec->sig;
        }
    }
    return 0;
}

int totalTransaccionesDeRut(struct NodoCompraVenta *transacciones, char *rut) {
    // Recibe una lista simplemente enlazada de struct NodoCompraVenta y un rut, busca las transacciones asignadas al
    // rut recibido. Retorna el total de transacciones asignadas al rut.
    struct NodoCompraVenta *rec;
    int totalTransacciones = 0;
    if (transacciones != NULL) {
        rec = transacciones;
        while (rec != NULL) {
            if (strcmp(rec->datosCompraVenta->rut, rut) == 0)
                totalTransacciones++;
            rec = rec->sig;
        }
    }
    return totalTransacciones;
}

int agregarCompraAInventario(struct NodoProducto **inventario, struct CompraVenta *compra) {
    // Recibe un arbol binario de busqueda de struct NodoProducto y un puntero a struct CompraVenta, agrega los
    // productos de la compra recibida al inventario. Retorna 1 si la compra es valida, en caso contrario retorna 0.
    struct Producto *producto;
    struct NodoLote *rec;
    int i;
    if (compra != NULL && compra->fechaLlegada != NULL) {
        for (i = 0; i < compra->totalProductosDistintos; i++) {
            printf("Producto: %s\n", compra->productos[i]->nombre);
            agregarNodoLote(&compra->productos[i]->lotes, crearNodoLote(crearLote()));
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

int quitarUnidadesVendidas(struct NodoLote *lotes, int cantidad) {
    struct NodoLote *rec;
    if (lotes != NULL) {
        rec = lotes;
        while (rec != NULL && cantidad > 0) {
            restarALote(rec->datosLote, &cantidad);
            rec = rec->sig;
        }
        return 1;
    }
    return 0;
}

int asignarLotesAVenta(struct NodoLote *lotesProducto, struct NodoLote **lotesVenta) {
    // Recibe dos listas simplemente enlazadas de struct NodoLote correspondientes a un producto del inventario y a un
    // producto de una venta, asigna los lotes consumidos por la venta.
    // Retorna 1 en caso de existir lotes asignables, en caso contrario retorna 0.
    struct NodoLote *rec;
    if (lotesProducto != NULL) {
        rec = lotesProducto;
        while (rec == lotesProducto || rec->datosLote->cantidadLote == 0) {
            agregarNodoLote(lotesVenta, rec);
            rec = rec->sig;
        }
        return 1;
    }
    return 0;
}

int quitarVentaAInventario(struct NodoProducto *inventario, struct CompraVenta *venta) {
    // Recibe un arbol binario de busqueda de struct NodoProducto y un puntero a struct CompraVenta, quita los productos
    // vendidos. Retorna 1 en caso de que la venta sea valida, en caso contrario retorna 0.
    struct Producto *producto;
    int i;
    if (venta != NULL) {
        for (i = 0; i < venta->totalProductosDistintos; i++) {
            producto = getProducto(inventario, venta->productos[i]->codigo);
            if (producto != NULL && venta->productos[i]->cantidad <= producto->cantidad) {
                quitarUnidadesVendidas(producto->lotes, venta->productos[i]->cantidad);
                asignarLotesAVenta(producto->lotes, &venta->productos[i]->lotes);
            }
        }
        return 1;
    }
    return 0;
}

int hayLoteVacio(struct NodoLote *lotes) {
    struct NodoLote *rec;
    if (lotes != NULL) {
        rec = lotes;
        while (rec != NULL) {
            if (rec->datosLote->cantidadLote == 0)
                return 1;
            rec = rec->sig;
        }
    }
    return 0;
}

int eliminarLotesVacios(struct NodoLote **lotes) {
    struct NodoLote *rec;
    if (*lotes != NULL && hayLoteVacio(*lotes)) {
        rec = *lotes;
        while (rec != NULL) {
            if (rec->datosLote->cantidadLote == 0)
                eliminarLote(lotes, (*lotes)->datosLote->numeroLote);
            rec = rec->sig;
        }
        return 1;
    }
    return 0;
}

void agregarLoteAProducto(struct Producto *producto) {
    // Recibe un puntero a struct Producto, da la opcion al usuario de agregar lotes a dicho producto.
    struct NodoLote *nodoLote;
    char opcion, aux;
    printf("Desea agregar lote al producto? (s/n): ");
    scanf("%c%c", &opcion, &aux);
    while (opcion == 's' || opcion == 'S') {
        nodoLote = crearNodoLote(crearLote());
        if (agregarNodoLote(&producto->lotes, nodoLote)) {
            printf("Desea agregar mas lotes al producto? (s/n): ");
            scanf("%c%c", &opcion, &aux);
        } else {
            free(nodoLote->datosLote->fechaCaducidad);
            free(nodoLote->datosLote);
            free(nodoLote);
            printf("No se pudo agregar el lote.\n\n");
            printf("Desea agregar lote al producto? (s/n): ");
            scanf("%c%c", &opcion, &aux);
        }
    }
}

void mostrarLotesACaducar(struct NodoLote *head) {
    // Recibe una lista simplemente enlazada de struct NodoLote, muestra los lotes proximos a caducar en la lista.
    struct NodoLote *rec;
    if (head != NULL) {
        rec = head;
        while (rec != NULL) {
            if (loteProximoACaducar(rec->datosLote)) {
                printf("Numero Lote: %d\n", rec->datosLote->numeroLote);
                printf("Cantidad Lote: %d\n", rec->datosLote->cantidadLote);
            }
            rec = rec->sig;
        }
    }
}

void mostrarProductosACaducar(struct NodoProducto *root) {
    // Recibe un arbol binario de busqueda de struct NodoProducto, muestra los productos proximos a caducar en el arbol.
    if (root != NULL) {
        mostrarProductosACaducar(root->izq);
        if (hayLoteACaducar(root->datosProducto->lotes)) {
            printf("Producto: %s\n", root->datosProducto->nombre);
            printf("Codigo: %s\n\n", root->datosProducto->codigo);
            mostrarLotesACaducar(root->datosProducto->lotes);
            printf("==============================\n");
        }
        mostrarProductosACaducar(root->der);
    }
}

int contarProductosDistintos(struct NodoProducto *rootProductos) {
    // Función para contar los productos distintos dentro del inventario, usando recursividad.
    // Retorna un int con el total de productos distintos.
    if (!rootProductos)
        return 0;
    return 1 + contarProductosDistintos(rootProductos->izq) + contarProductosDistintos(rootProductos->der);
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

void menuProducto(struct Farmacia *farmacia, struct Producto *producto) {
    int opcion;
    char aux;

    do {
        printf("Menu de opciones de producto ID: %s\n", producto->codigo);
        printf("1. Ver detalle del producto\n");
        printf("2. Mostrar lotes del producto\n");
        printf("3. Agregar lote al producto\n");
        printf("4. Eliminar lote del producto\n");
        printf("5. Volver al menu anterior\n");

        scanf("%d%c", &opcion, &aux);

        switch (opcion) {
            case 1:
                mostrarDetalleProducto();
                break;
            case 2:
                mostrarLotesProducto();
                break;
            case 3:
                agregarLoteProducto();
                break;
            case 4:
                eliminarLoteProducto();
                break;
            case 5:
                printf("Volviendo al menu anterior...\n");
                break;
            default:
                printf("Opcion no valida, por favor ingrese una opcion valida.\n\n");
                break;

        }
    } while (opcion != 4);
}

void menuInventario(struct Farmacia *farmacia) {
    // Función para el menú con opciones relacionadas al inventario de productos
    int opcion;
    char aux;
    struct Producto *producto;

    do {
        printf("Menu de inventario\n");
        printf("1. Ver inventario\n");
        printf("2. Agregar producto manualmente\n");
        printf("3. Eliminar producto\n");
        printf("4. Ver productos proximos a caducar\n");
        printf("5. Ver productos con bajo stock\n");
        printf("6. Ver productos sin stock\n");
        printf("7. Opciones de un producto\n");
        printf("8. Volver al menu anterior\n");

        scanf("%d%c", &opcion, &aux);

        switch (opcion) {
            case 1:
                mostrarInventario();
                break;
            case 2:
                agregarProductoManualmente();
                break;
            case 3:
                eliminarProducto();
                break;
            case 4:
                mostrarProximosACaducar();
                break;
            case 5:
                mostrarProductosBajoStock();
                break;
            case 6:
                mostrarProductosSinStock();
                break;
            case 7:
                producto = seleccionarProducto();
                if (producto)
                    menuProducto(farmacia, producto);
                else
                    printf("Producto no encontrado.\n");
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

struct CompraVenta *seleccionarTransaccion(struct NodoCompraVenta *headCompraVenta, char tipoTransaccion) {
    // Función para seleccionar una orden de compra o venta según ID
    // Retorna un puntero a la orden de compra o venta seleccionada
    // Según el tipo de transacción recibido por parámetro hace un print distinto
    struct CompraVenta *compraVenta;
    int id;
    char aux;

    if (tipoTransaccion == 'C') {
        printf("Ingrese el ID de la orden de compra que desea seleccionar: ");
    } else {
        printf("Ingrese el ID de la venta que desea seleccionar: ");
    }

    scanf("%d%c", &id, &aux);
    compraVenta = getCompraVenta(headCompraVenta, id);
    return compraVenta;
}

void mostrarDetalleVenta(struct NodoCompraVenta *headVentas) {
    // Función para mostrar el detalle de una venta específica
    // Imprime un mensaje si no hay ventas en el sistema o si no fue encontrada la venta
    struct CompraVenta *venta;

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
    // TODO: Mostrar productos vendidos
    printf("Estado de envio: ");
    if (venta->estadoEnvio == 'R' || venta->estadoEnvio == 'r')
        printf("Recibido\n");
    else
        printf("Pendiente\n");
    printf("Fecha de solicitud: %d/%d/%d\n", venta->fechaSolicitud->dia, venta->fechaSolicitud->mes,
           venta->fechaSolicitud->year);
    if (venta->estadoEnvio == 'R' || venta->estadoEnvio == 'r')
        printf("Fecha de llegada: %d/%d/%d\n", venta->fechaLlegada->dia, venta->fechaLlegada->mes,
               venta->fechaLlegada->year);
    // TODO: Ver tema de despacho o compra en tienda
}

void menuVentas(struct Farmacia *farmacia) {
    // Función para el menú con opciones relacionadas a ventas de la farmacia
    int opcion;
    char aux;

    do {
        printf("Menu de ventas de la farmacia\n");
        printf("1. Registrar venta\n");
        printf("2. Ver ventas\n");
        printf("3. Ver ventas de productos con receta\n");
        printf("4. Actualizar despacho de venta\n");
        printf("5. Ver detalle de una venta\n");
        printf("6. Volver al menu anterior\n");

        scanf("%d%c", &opcion, &aux);

        switch (opcion) {
            case 1:
                registrarVenta();
                break;
            case 2:
                mostrarVentas();
                break;
            case 3:
                mostrarVentasReceta();
                break;
            case 4:
                actualizarDespachoVenta();
                break;
            case 5:
                mostrarDetalleVenta(farmacia->ventas);
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

void mostrarOrdenesCompra(struct NodoCompraVenta *headCompras) {
    // Función para listar las órdenes de compra
    // Imprime un mensaje si no hay órdenes de compra en el sistema
    // Imprime id, nombre, costo total y el estado de la orden de compra
    if (!headCompras) {
        printf("No existen ordenes de compra en el sistema. \n");
        return;
    }
    printf("Ordenes de compra (Id, Nombre, Costo total, Estado):\n");
    while (headCompras) {
        printf("ID: %d, Nombre: %s, Costo total: %d, Estado: \n", headCompras->datosCompraVenta->id,
               headCompras->datosCompraVenta->nombre, headCompras->datosCompraVenta->costoTotal);
        if (headCompras->datosCompraVenta->estadoEnvio == 'R' || headCompras->datosCompraVenta->estadoEnvio == 'r')
            printf("Recibido\n");
        else
            printf("Pendiente\n");
        headCompras = headCompras->sig;
    }
}

void mostrarDetalleOrdenCompra(struct NodoCompraVenta *headCompras) {
    // Función para mostrar el detalle de una orden de compra específica
    // Imprime un mensaje si no hay órdenes de compra o si no fue encontrada la orden de compra
    struct CompraVenta *ordenCompra;
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
    // TODO: Mostrar productos comprados
    printf("Estado de envio: ");
    if (ordenCompra->estadoEnvio == 'R' || ordenCompra->estadoEnvio == 'r')
        printf("Recibido\n");
    else
        printf("Pendiente\n");
    printf("Fecha de solicitud: %d/%d/%d\n", ordenCompra->fechaSolicitud->dia, ordenCompra->fechaSolicitud->mes,
           ordenCompra->fechaSolicitud->year);
    if (ordenCompra->estadoEnvio == 'R' || ordenCompra->estadoEnvio == 'r')
        printf("Fecha de llegada: %d/%d/%d\n", ordenCompra->fechaLlegada->dia, ordenCompra->fechaLlegada->mes,
               ordenCompra->fechaLlegada->year);
}

void menuCompras(struct Farmacia *farmacia) {
    // Función para el menú con opciones relacionadas a ordenes de compra de la farmacia
    int opcion;
    char aux;

    do {
        printf("Menu de ordenes de compra de la farmacia\n");
        printf("1. Registrar orden de compra\n");
        printf("2. Ver ordenes de compra\n");
        printf("3. Actualizar estado de orden de compra\n");
        printf("4. Ver detalle de una orden de compra\n");
        printf("5. Volver al menu anterior\n");

        scanf("%d%c", &opcion, &aux);

        switch(opcion) {
            case 1:
                registrarOrdenCompra();
                break;
            case 2:
                mostrarOrdenesCompra(farmacia->compras);
                break;
            case 3:
                actualizarEstadoOrdenCompra();
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

void menuUnaFarmacia(struct Farmacia *farmacia) {
    // Función para el menú principal de una farmacia específica
    int opcion;
    char aux;

    do {
        printf("Menu de la farmacia\n");
        printf("1. Detalle de la farmacia\n");
        printf("2. Opciones de inventario/productos\n");
        printf("3. Opciones de venta\n");
        printf("4. Opciones de ordenes de compra\n");
        printf("5. Opciones de analisis de datos\n");
        printf("6. Volver al menu anterior\n");

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
                // TODO: menuAnalisisDatosFarmacia();
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
        printf("ID: %s, %s, %s\n", curr->datosFarmacia->id, curr->datosFarmacia->ciudad, curr->datosFarmacia->region);
        curr = curr->sig;
    } while (curr != headFarmacias);
}

struct Farmacia *seleccionarFarmacia(struct NodoFarmacia *headFarmacias) {
    // Función para que usuario seleccione una farmacia según ID
    // Retorna un puntero a la farmacia seleccionada
    struct Farmacia *farmacia;
    char *idBuscado;
    printf("Ingrese el ID de la farmacia a la que desea ingresar: ");
    idBuscado = leerCadena();
    farmacia = getFarmacia(headFarmacias, idBuscado);
    return farmacia;
}

void agregarFarmaciaSistema(struct NodoFarmacia **headFarmacias) {
    // Función para agregar una nueva farmacia al sistema
    // Imprime mensaje de error si no fue posible
    struct NodoFarmacia *nuevoNodo;
    struct Farmacia *nuevaFarmacia;

    printf("Agregar una nueva farmacia al sistema\n");
    nuevaFarmacia = crearFarmacia();
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
                else {
                    printf("Farmacia no encontrada / ID no valido.\n\n");
                    opcion = 0;
                }
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
    if (opcion == 's' || opcion == 'S' || opcion == 'y' || opcion == 'Y')
        return 1;
    return 0;
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
                // TODO: menuAnalisisFarmaSalud(farmaSalud);
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
    menuFarmaSalud(farmaSalud);

    freeListaFarmacias(farmaSalud->headFarmacias);
    free(farmaSalud);
    return 0;
}