//
// Created by Diego Negrín on 22-05-24.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
        buffer[i] = caracter;
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

    do {
        printf("\nElija modo de ingreso de fecha:\n");
        printf("1. Fecha Actual             2. Fecha Personalizada\n");
        printf("Ingrese la opcion que desea realizar (1-2): ");
        scanf("%d", &opcion);
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
                scanf("%d", &dia);
                getchar();
                scanf("%d", &mes);
                getchar();
                scanf("%d", &year);
                getchar();
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

struct FarmaSalud *crearFarmasalud() {
    // Función para crear y asignar memoria al sistema de FarmaSalud
    struct FarmaSalud *nuevoFarmaSalud;
    nuevoFarmaSalud = (struct FarmaSalud *) malloc(sizeof(struct FarmaSalud));
    nuevoFarmaSalud->headFarmacias = NULL;
    return nuevoFarmaSalud;
}

struct Farmacia *crearFarmacia() {
    // Lee datos de la entrada del usuario y los asigna a un struct Farmacia.
    // Retorna un puntero a struct Farmacia que contiene los datos leidos.
    struct Farmacia *nuevaFarmacia;
    char *id, *ciudad, *region;

    region = leerCadena();
    ciudad = leerCadena();
    id = leerCadena();

    nuevaFarmacia = (struct Farmacia *) malloc(sizeof(struct Farmacia));
    nuevaFarmacia->id = id;
    nuevaFarmacia->ciudad = ciudad;
    nuevaFarmacia->region = region;
    nuevaFarmacia->maxCapacidad = CAPACIDAD_PROMEDIO;
    nuevaFarmacia->inventario = NULL;
    nuevaFarmacia->totalProductos = 0;
    nuevaFarmacia->ventas = NULL;
    nuevaFarmacia->compras = NULL;
    return nuevaFarmacia;
};

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

void freeListaCompraVenta(struct NodoCompraVenta *head) {
    // Recibe una lista simplemente enlazada de struct NodoCompraVenta, libera la memoria asignada a dicha lista.
    struct NodoCompraVenta *aux;
    int i;
    while (head != NULL) {
        aux = head;
        head = head->sig;
        free(aux->datosCompraVenta->nombre);
        for (i = 0; i < aux->datosCompraVenta->cantidadProductos; i++)
            freeProducto(aux->datosCompraVenta->productos[i]);
        free(aux->datosCompraVenta->fechaSolicitud);
        free(aux->datosCompraVenta->fechaLlegada);
        free(aux->datosCompraVenta);
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

struct Producto *crearProducto() {
    // Lee datos de la entrada del usuario y los asigna a un struct Producto.
    // Retorna un puntero al struct Producto que contiene los datos leidos.
    struct Producto *nuevoProducto;
    char *nombre, *categoria, *descripcion, *proveedor, codigo[11];
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
    scanf("%d", &precio);
    printf("Producto requiere receta (0-1): ");
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
    return 0;
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
    nuevoLote->fechaCaducidad = (struct Fecha *) malloc(sizeof(struct Fecha));
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

struct Producto **getArregloProductos(int totalProductosDistintos) {
    // Recibe un entero correspondiente al largo de un arreglo de struct Producto, lee datos de la entrada del usuario y
    // los asigna en las posiciones del arreglo. Retorna un puntero al arreglo de struct Producto.
    struct Producto **arregloProductos, *producto;
    int i;
    arregloProductos = (struct Producto **) malloc(totalProductosDistintos * sizeof(struct Producto *));
    for (i = 0; i < totalProductosDistintos; i++) {
        producto = crearProducto();
        arregloProductos[i] = producto;
    }
    return arregloProductos;
}

struct CompraVenta *crearCompraVenta() {
    // Lee datos de la entrada del usuario y los asigna en un struct CompraVenta.
    // Retorna un puntero al struct CompraVenta que contiene los datos leidos.
    struct CompraVenta *nuevaCompraVenta;
    struct Producto **productos;
    struct Fecha *fechaSolicitud;
    int totalProductosDistintos;
    char *nombre, estadoEnvio;

    scanf("%d", &totalProductosDistintos);
    scanf("%c ", &estadoEnvio);
    nombre = leerCadena();
    fechaSolicitud = leerFecha();
    productos = getArregloProductos(totalProductosDistintos);

    nuevaCompraVenta = (struct CompraVenta *) malloc(sizeof(struct CompraVenta));
    nuevaCompraVenta->nombre = nombre;
    nuevaCompraVenta->totalProductosDistintos = totalProductosDistintos;
    nuevaCompraVenta->productos = productos;
    nuevaCompraVenta->estadoEnvio = estadoEnvio;
    nuevaCompraVenta->fechaSolicitud = fechaSolicitud;
    nuevaCompraVenta->fechaLlegada = NULL;
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

int getTotalProductos(struct Producto **arregloProductos, int largoArreglo) {
    // Recibe un arreglo de struct Producto y su largo.
    // Retorna la suma del campo cantidad de todos sus elementos.
    int i, totalProductos = 0;
    for (i = 0; i < largoArreglo; i++)
        totalProductos += arregloProductos[i]->cantidad;
    return totalProductos;
}

int getCostoTotal(struct Producto **arregloProductos, int largoArreglo) {
    // Recibe un arreglo de struct Producto y su largo, calcula el costo de cada elemento.
    // Retorna la suma del costo de todos sus elementos.
    int i, costoTotal = 0;
    for (i = 0; i < largoArreglo; i++)
        costoTotal += arregloProductos[i]->cantidad * arregloProductos[i]->precio;
    return costoTotal;
}

void menuUnaFarmacia(struct Farmacia *farmacia) {

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

void menuFarmacias(struct NodoFarmacia **headFarmacias) {
    // Función para el menú de farmacias del sistema
    int opcion, flagSalir = 0;
    struct Farmacia *farmacia;

    do {
        printf("\nMenu de farmacias de FarmaSalud\n");
        printf("1. Listar farmacias\n");
        printf("2. Ingresar a una farmacia\n");
        printf("3. Agregar una farmacia\n");
        printf("4. Eliminar una farmacia\n");
        printf("5. Volver al menu principal\n");
        printf("Seleccione una opcion: ");

        scanf("%d", &opcion);

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
                flagSalir = 1;
                break;
            default:
                printf("Opcion no valida, por favor ingrese una opcion valida.\n\n");
                break;
        }
    } while (!flagSalir);
}

int confirmarSalida() {
    // Función para confirmar la salida del sistema
    // Retorna un 1 (true) si el usuario confirma, 0 (false) si no
    char opcion;
    printf("¿Esta seguro/a que desea salir del sistema? (s/n): ");
    scanf(" %c", &opcion);
    if (opcion == 's' || opcion == 'S' || opcion == 'y' || opcion == 'Y')
        return 1;
    return 0;
}

void menuFarmaSalud(struct FarmaSalud *farmaSalud) {
    // Función para el menú principal de usuario de FarmaSalud
    int opcion, flagSalir = 0;

    do {
        printf("\nMenú de FarmaSalud\n");
        printf("1. Ingresar a menú de farmacias\n");
        printf("2. Ingresar a menú de analisis de datos\n");
        printf("3. Salir\n");
        printf("Seleccione una opcion: ");

        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                menuFarmacias(&farmaSalud->headFarmacias);
                break;
            case 2:
                // TODO: menuAnalisisFarmaSalud(farmaSalud);
                break;
            case 3:
                if (confirmarSalida()) {
                    flagSalir = 1;
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
    } while (!flagSalir);
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

int main(void) {
    struct FarmaSalud *farmaSalud;
    farmaSalud = crearFarmasalud();
    menuFarmaSalud(farmaSalud);

    free(farmaSalud);
    return 0;
}