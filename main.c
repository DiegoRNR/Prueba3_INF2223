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

struct FarmaSalud {
    struct NodoFarmacia *headFarmacias;
};

char *leerCadena() {
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

struct Producto *crearProducto() {
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
    if (!root)
        return NULL;
    if (strcmp(root->datosProducto->codigo, codigoBuscado) == 0)
        return root->datosProducto;
    if (compararCodigoProductos(root->datosProducto->codigo, codigoBuscado) > 0)
        return getProducto(root->izq, codigoBuscado);
    return getProducto(root->der, codigoBuscado);
}

struct NodoProducto *getPadreACodigo(struct NodoProducto *root, char *codigo) {
    if (strcmp(root->datosProducto->codigo, codigo) == 0)
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
    struct NodoLote *nuevoNodo = NULL;
    if (lote != NULL) {
        nuevoNodo = (struct NodoLote *) malloc(sizeof(struct NodoLote));
        nuevoNodo->datosLote = lote;
        nuevoNodo->sig = NULL;
    }
    return nuevoNodo;
}

struct Lote *getLote(struct NodoLote *head, int numeroLoteBuscado) {
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

int stockProducto(struct NodoLote *lotesProducto) {
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

void menuFarmacia(struct Farmacia **farmacia) {

}

int confirmarSalida() {
    // Función para confirmar la salida del sistema
    char opcion;
    printf("¿Está seguro/a que desea salir del sistema? (s/n): ");
    scanf(" %c", &opcion);
    if (opcion == 's' || opcion == 'S' || opcion == 'y' || opcion == 'Y')
        return 1;
    return 0;
}

void menuFarmaSalud(struct FarmaSalud *farmaSalud) {
    // Función para el menú principal de usuario de FarmaSalud
    int opcion;
    int flagSalir = 0;

    do {
        printf("\nMenú de FarmaSalud\n");
        printf("1. Ingresar a menú de farmacias\n");
        printf("2. Ingresar a menú de análisis de datos\n");
        printf("3. Salir\n");
        printf("Seleccione una opción: ");

        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                menuFarmacias(farmaSalud);
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
                printf("Opción no válida, por favor ingrese una opción válida.\n\n");
                break;
        }
    } while (!flagSalir);
}

int main(void) {
    struct FarmaSalud *farmaSalud;
    farmaSalud = crearFarmasalud();
    menuFarmaSalud(farmaSalud);

    free(farmaSalud);
    return 0;
}