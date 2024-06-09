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
            cadena[i] = (char) toupper(cadena[i]);
    }
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

char *lecturaCodigo() {
    // Lee un codigo de 10 digitos de la entrada del usuario y lo copia en un puntero a char.
    // Retorna el puntero donde se copio el codigo leido.
    char *codigo;
    do {
        printf("Ingrese código del producto (10 dígitos): ");
        codigo = leerCadena();
        if (strlen(codigo) != 10 || !cadenaNumerica(codigo))
            printf("Código no válido, por favor ingrese un código de 10 dígitos.\n");
    } while (strlen(codigo) != 10 || !cadenaNumerica(codigo));
    return codigo;
}

char lecturaReceta() {
    // Lee un caracter de la entrada del usuario, verifica si es 's' o 'n', de no serlo pide volver a ingresar un
    // caracter. Retorna el caracter leido.
    char receta;
    do {
        printf("Producto requiere receta médica, ingrese si cliente posee receta (s/n): ");
        scanf(" %c", &receta);
        if (receta != 's' && receta != 'S' && receta != 'n' && receta != 'N')
            printf("Opción no válida, por favor ingrese 's' o 'n'.\n");
    } while (receta != 's' && receta != 'S' && receta != 'n' && receta != 'N');
    return receta;
}

int lecturaUnidades(struct Producto *producto, char tipoTransaccion) {
    // Recibe un puntero a struct Producto y un char que indica si se realiza una compra o una venta, lee un entero de la
    // entrada del usuario y verifica si es válido. Retorna el entero leido.
    int unidades, entradaValida = 0;
    do {
        if (tipoTransaccion == 'V' && producto->cantidad == 0) {
            printf("No hay unidades en stock, no se puede agregar a la transaccion.\n");
            return 0;
        }
        printf("\nUnidades en stock: %d\n", producto->cantidad);
        printf("Unidades de la transacción (valor numérico): ");
        scanf(" %d", &unidades);
        if (unidades < 1)
            printf("Unidades no válidas, por favor ingrese un número entero positivo.\n");
        else if (tipoTransaccion == 'V' && unidades > producto->cantidad)
            printf("No hay suficientes unidades en stock, ingrese una cantidad menor o igual a las unidades en stock.\n");
        else
            entradaValida = 1;
    } while (entradaValida == 0);
    return unidades;

}

void passVentasProducto(struct Producto *producto, struct Transaccion *venta, int tam, int *totalVentas,
                        int *unidadesVendidas) {
    // Recibe un puntero a un arreglo de las ventas y un entero que ilustra el tamaño del arreglo
    // Cuenta la cantidad de veces que un producto fue vendido en una venta y la cantidad total de ese producto
    int i;
    if (venta != NULL) {
        for (i = 0; i < tam; i++) {
            if (strcmp(producto->codigo, venta->productos[i]->codigo) == 0
                && venta->productos[i]->cantidad != 0) {
                (*totalVentas)++;
                (*unidadesVendidas) += venta->productos[i]->cantidad;
            }
        }
    }
}

int getPromedioDemanda(struct Producto *producto, struct NodoTransaccion *ventas) {
    // Recibe un puntero a un struct de un producto y a las ventas de la farmacia, recorre cada una de ellas y
    // retorna el promedio de demanda del producto
    int totalVentas = 0;
    int unidadesVendidas = 0;
    struct NodoTransaccion * rec;
    if (ventas != NULL) {
        rec = ventas;
        while(rec != NULL) {
            passVentasProducto(producto, rec->datosTransaccion, rec->datosTransaccion->totalProductosDistintos,
                               &totalVentas, &unidadesVendidas);
            rec = rec->sig;
        }
    }
    if (totalVentas != 0) {
        return (unidadesVendidas / totalVentas);
    }
    else return 0;
}

int hayProductoConReceta(struct Transaccion *transaccion) {
    // Recibe un puntero a struct Transaccion, verifica si hay un producto con receta en la transacción.
    // Retorna 1 en caso de haber un producto con receta, en caso contrario retorna 0.
    int i;
    for (i = 0; i < transaccion->totalProductosDistintos; i++)
        if (transaccion->productos[i]->requiereReceta)
            return 1;
    return 0;
}

int tieneBajoStock(struct Producto *producto, struct NodoTransaccion *ventas) {
    //Recibe un puntero a un struct de un producto y a las ventas de la farmacia recorriendo cada una de ellas y
    //retornando 1 si el producto tiene bajo stock y 0 si no lo tiene
    int promedio;
    promedio = getPromedioDemanda(producto, ventas);
    if (promedio > producto->cantidad) {
        return 1;
    }
    else return 0;
}

int contarProductosDistintos(struct NodoProducto *rootProductos) {
    // Función para contar los productos distintos dentro del inventario, usando recursividad.
    // Retorna un int con el total de productos distintos.
    if (!rootProductos)
        return 0;
    return 1 + contarProductosDistintos(rootProductos->izq) + contarProductosDistintos(rootProductos->der);
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

int totalTransaccionesDeCategoria(struct NodoTransaccion *ventas, char *categoria) {
    // Recibe una lista simplemente enlazada de struct NodoTransaccion y una categoria, busca las transacciones asignadas
    // a la categoría recibida. Retorna el total de transacciones asignadas a la categoría.
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

int totalIngresosEnRegion(struct NodoFarmacia *head, char *region) {
    // Recibe una lista doblemente enlazada de struct NodoFarmacia y una region, calcula el total de ingresos en la region.
    // Retorna un int con el total de ingresos en la region.
    struct NodoFarmacia *rec;
    int totalIngresos = 0;
    if (head != NULL) {
        rec = head;
        do {
            if (strcmp(rec->datosFarmacia->region, region) == 0)
                totalIngresos += totalIngresosFarmacia(rec->datosFarmacia->ventas);
            rec = rec->sig;
        } while (rec != head);
    }
    return totalIngresos;
}

int totalVentasEnRegion(struct NodoFarmacia *head, char *region) {
    // Recibe una lista doblemente enlazada de struct NodoFarmacia y una region, calcula el total de ventas en la region.
    // Retorna un int con el total de ventas en la region.
    struct NodoFarmacia *rec;
    int totalVentas = 0;
    if (head != NULL) {
        rec = head;
        do {
            if (strcmp(rec->datosFarmacia->region, region) == 0)
                totalVentas += totalVentasFarmacia(rec->datosFarmacia->ventas);
            rec = rec->sig;
        } while (rec != head);
    }
    return totalVentas;
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

int getTotalUnidadesVendidas(struct Producto *producto, struct NodoTransaccion *ventas) {
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

int getTotalStockFarmacia(struct NodoProducto *inventario) {
    // Recibe un arbol binario de busqueda de struct NodoProducto.
    // Retorna la suma del campo cantidad de todos los productos en el arbol.
    if (inventario == NULL)
        return 0;
    return getTotalStockFarmacia(inventario->izq) + getTotalStockFarmacia(inventario->der) +
           getCantidadProducto(inventario->datosProducto->lotes);
}

char *getCategoriaMasVendida(struct NodoTransaccion *ventas) {
    // Recibe una lista simplemente enlazada de struct NodoTransaccion, busca la categoria mas vendida.
    // Retorna un puntero a char con el nombre de la categoria más vendida.
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

char *getCategoriaMasVendidaEstacion(struct NodoTransaccion *ventas, char estacion) {
    // Recibe una lista simplemente enlazada de struct NodoTransaccion y una estacion, busca la categoría más vendida
    // en la estacion recibida. Retorna un puntero a char con el nombre de la categoría más vendida.
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

char *getRutConMasTransacciones(struct NodoTransaccion *headTransaccion, char **nombre) {
    // Recibe una lista simplemente enlazada de struct Transaccion y un puntero a char, busca el rut con más transacciones
    // en la lista y asigna el nombre correspondiente al rut con más transacciones al puntero a char recibido.
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

struct Farmacia *getFarmaciaConMasVentas(struct NodoFarmacia *head) {
    // Recibe una lista doblemente enlazada de struct NodoFarmacia, busca la farmacia con más ventas.
    // Retorna un puntero a struct Farmacia si existe la farmacia con más ventas, en caso contrario retorna NULL.
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

struct Farmacia *getFarmaciaConMasIngresos(struct NodoFarmacia *head) {
    // Recibe una lista doblemente enlazada de struct NodoFarmacia, busca la farmacia con más ingresos.
    // Retorna un puntero a struct Farmacia si existe la farmacia con más ingresos, en caso contrario retorna NULL.
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

char *getRegionConMasVentas(struct NodoFarmacia *headVentas) {
    // Recibe una lista doblemente enlazada de struct NodoFarmacia, busca la region con más ventas.
    // Retorna un puntero a char con el nombre de la region con más ventas.
    struct NodoFarmacia *rec;
    char *regionMasVentas = NULL;
    int maxVentas;
    if (headVentas != NULL) {
        rec = headVentas;
        do {
            if (regionMasVentas == NULL || (regionMasVentas != rec->datosFarmacia->region
                                            && totalVentasEnRegion(headVentas, rec->datosFarmacia->region) > maxVentas)) {
                maxVentas = totalVentasEnRegion(headVentas, rec->datosFarmacia->region);
                regionMasVentas = rec->datosFarmacia->region;
            }
            rec = rec->sig;
        } while (rec != headVentas);
    }
    return regionMasVentas;
}

char *getRegionConMasIngresos(struct NodoFarmacia *headVentas) {
    // Recibe una lista doblemente enlazada de struct NodoFarmacia, busca la region con más ingresos.
    // Retorna un puntero a char con el nombre de la region con más ingresos.
    struct NodoFarmacia *rec;
    char *regionMasIngresos = NULL;
    int maxIngresos;
    if (headVentas != NULL) {
        rec = headVentas;
        do {
            if (regionMasIngresos == NULL || (regionMasIngresos != rec->datosFarmacia->region &&
                                              totalIngresosEnRegion(headVentas, rec->datosFarmacia->region) > maxIngresos)) {
                maxIngresos = totalIngresosEnRegion(headVentas, rec->datosFarmacia->region);
                regionMasIngresos = rec->datosFarmacia->region;
            }
            rec = rec->sig;
        } while (rec != headVentas);
    }
    return regionMasIngresos;
}

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

void passProductosAArreglo(struct NodoProducto *root, struct Producto **arregloProductos, int *i) {
    // Recibe un arbol binario de busqueda de struct NodoProducto, un arreglo de punteros a struct Producto y un puntero
    // a un entero. Asigna los elementos del arbol al arreglo, y aumenta el valor del entero en 1.
    if (root != NULL) {
        passProductosAArreglo(root->izq, arregloProductos, i);
        arregloProductos[*i] = root->datosProducto;
        (*i)++;
        passProductosAArreglo(root->der, arregloProductos, i);
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
            passProductosAArreglo(productosTransaccion, arregloProductos, &i);
        }
    }
    return arregloProductos;
}

void passProductoMasVendido(struct NodoProducto *inventario, struct NodoTransaccion *ventas, int *cantidad,
                            struct Producto **masVendido) {
    // Esta funcion recibe un puntero a un struct nodoProducto (el inventario) y un puntero a un struct nodocompraventa
    // (las ventas de una farmacia) a la vez que un puntero a un entero (cantidad) y un puntero a un struct Producto
    // Al ser una funcion tipo void no tiene un retorno claro, ya que se dedica a actualizar las variables de cantidad y
    // producto para encontrar el producto con más ventas dentro de la farmacia
    int cantCandidato;
    if (inventario != NULL) {
        passProductoMasVendido(inventario->izq, ventas, cantidad, masVendido);
        cantCandidato = getTotalUnidadesVendidas(inventario->datosProducto, ventas);
        if (cantCandidato > (*cantidad)) {
            *masVendido = inventario->datosProducto;
            (*cantidad) = cantCandidato;
        }
        passProductoMasVendido(inventario->der, ventas, cantidad, masVendido);
    }
}

struct Producto *getProductoMasVendido(struct NodoProducto *inventario, struct NodoTransaccion *ventas) {
    // Recibe un puntero a un struct de la farmacia y a las ventas de esta misma recorriendo cada una de ellas y
    // retornando el producto más vendido en la farmacia
    int cantidad = 0;
    struct Producto *masVendido = NULL;
    passProductoMasVendido(inventario, ventas, &cantidad, &masVendido);

    return masVendido;
}


void passProductoMenosVendido(struct NodoProducto *inventario, struct NodoTransaccion *ventas, int *cantidad,
                              struct Producto **menosVendido, int *contador) {
    // Esta funcion recibe un puntero a un struct NodoProducto (el inventario) y un puntero a un struct NodoTransaccion
    // (las ventas de una farmacia), a la vez que un puntero a un entero (cantidad), un puntero a un struct Producto y
    // un puntero a un entero. Al ser una funcion tipo void no tiene un retorno claro, ya que se dedica a actualizar las
    // variables de cantidad y producto para encontrar el producto con más ventas dentro de la farmacia.
    int cantCandidato;
    if (inventario != NULL) {
        passProductoMenosVendido(inventario->izq, ventas, cantidad, menosVendido, contador);
        cantCandidato = getTotalUnidadesVendidas(inventario->datosProducto, ventas);
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
        passProductoMenosVendido(inventario->der, ventas, cantidad, menosVendido, contador);
    }
}

struct Producto *getProductoMenosVendido(struct NodoProducto *inventario, struct NodoTransaccion *ventas) {
    // Recibe un puntero a un struct de la farmacia y a las ventas de esta misma recorriendo cada una de ellas y
    // retornando el producto menos vendido en la farmacia
    int cantidad;
    int contador = 0;
    struct Producto *menosVendido = NULL;
    passProductoMenosVendido(inventario, ventas, &cantidad, &menosVendido, &contador);

    return menosVendido;
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

struct FarmaSalud *crearFarmasalud() {
    // Función para crear y asignar memoria al sistema de FarmaSalud
    struct FarmaSalud *nuevoFarmaSalud;
    nuevoFarmaSalud = (struct FarmaSalud *) malloc(sizeof(struct FarmaSalud));
    nuevoFarmaSalud->headFarmacias = NULL;
    return nuevoFarmaSalud;
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

struct Lote *leerDatosLote(int totalUnidades) {
    // Lee datos de un lote por la entrada del usuario.
    // Retorna un puntero al struct Lote que contiene los datos leidos.
    char *fechaCaducidad, *numeroLote;
    int cantidadLote;
    printf("Unidades restantes: %d\n", totalUnidades);
    printf("Número de lote: ");
    numeroLote = leerCadena();
    do {
        printf("Cantidad de productos del lote (valor numérico): ");
        scanf(" %d", &cantidadLote);
        if (cantidadLote > totalUnidades)
            printf("Cantidad de productos del lote no puede ser mayor al total de unidades.\n");
        if (cantidadLote <= 0)
            printf("La cantidad de productos del lote debe ser mayor a 0.\n");
    } while (cantidadLote > totalUnidades || cantidadLote <= 0);
    printf("Fecha de caducidad del lote (DD/MM/AAAA): ");
    fechaCaducidad = leerCadena();
    return crearLote(numeroLote, cantidadLote, fechaCaducidad);
}

struct Producto *leerDatosProducto(char *codigo, char *proveedorCompra) {
    // Recibe un codigo, lee datos de la entrada del usuario acerca del producto correspondientes al codigo recibido.
    // Retorna un puntero al struct Producto que contiene los datos leidos.
    char *nombre, *categoria, *descripcion, *proveedor;
    int precio, requiereReceta;
    printf("Nombre del producto: ");
    nombre = leerCadena();
    printf("Categoría del producto: ");
    categoria = leerCadena();
    printf("Descripción del producto: ");
    descripcion = leerCadena();
    if (proveedorCompra == NULL) {
        printf("Proveedor del producto: ");
        proveedor = leerCadena();
    } else {
        proveedor = proveedorCompra;
    }
    printf("Precio del producto (valor numérico): ");
    scanf(" %d", &precio);
    printf("Producto requiere receta? (0/1): ");
    scanf(" %d", &requiereReceta);
    return crearProducto(codigo, nombre, categoria, descripcion, proveedor, precio, requiereReceta);
}

int agregarNodoProducto(struct NodoProducto **root, struct NodoProducto *nuevoNodo) {
    // Recibe la dirección de la raíz del inventario y un puntero a un nodo de producto
    // Agrega el nodo recibido al inventario
    // Retorna 1 en caso de éxito, 0 en caso contrario
    if (nuevoNodo) {
        if (!(*root)) {
            *root = nuevoNodo;
            return 1;
        }
        if (strcmp(nuevoNodo->datosProducto->codigo, (*root)->datosProducto->codigo) < 0)
            return agregarNodoProducto(&(*root)->izq, nuevoNodo);
        else if (strcmp(nuevoNodo->datosProducto->codigo, (*root)->datosProducto->codigo) > 0)
            return agregarNodoProducto(&(*root)->der, nuevoNodo);
    }
    return 0;
}

struct NodoProducto *leerProductosTransaccion(struct NodoProducto *inventario, char tipoTransaccion,
                                             char *proveedorCompra, int *totalProductosDistintos) {
    // Recibe un arbol binario de busqueda de struct NodoProducto, un char que indica si se realiza una compra o una
    // venta, y un puntero a un entero. Lee datos de la entrada del usuario, los asigna en un arbol binario de busqueda
    // de struct NodoProducto y aumenta en 1 el entero. Retorna un puntero al arbol creado.
    struct NodoProducto *productosTransaccion = NULL;
    struct Producto *producto, *productoTransaccion;
    char *codigoProducto, poseeReceta, opcion;
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
        printf("\nDesea agregar más productos a la transacción? (s/n): ");
        scanf(" %c", &opcion);
    } while (opcion == 's' || opcion == 'S');

    return productosTransaccion;
}

struct Transaccion *leerDatosTransaccion(struct NodoProducto *inventario, char tipoTransaccion) {
    // Recibe el inventario y un char que indica si se crea una compra o una venta, lee datos de la entrada del usuario.
    // Retorna un puntero a un struct Transaccion que contiene los datos leidos.
    struct NodoProducto *productosTransaccion;
    struct Producto **productos;
    int totalProductosDistintos = 0, id;
    char *nombre, *rut, *fechaSolicitud, *fechaLlegada = NULL, estadoEnvio = 'X';

    printf("\nIngrese ID de la transacción (valor numérico): ");
    scanf(" %d", &id);
    printf("Ingrese nombre: ");
    nombre = leerCadena();
    printf("Ingrese rut (formato 12.345.678-9): ");
    rut = leerCadena();
    if (tipoTransaccion == 'C') {
        productosTransaccion = leerProductosTransaccion(inventario, tipoTransaccion, nombre, &totalProductosDistintos);
    } else {
        productosTransaccion = leerProductosTransaccion(inventario, tipoTransaccion, NULL, &totalProductosDistintos);
    }
    productos = getArregloProductos(productosTransaccion, totalProductosDistintos);
    if (productos != NULL) {
        printf("Ingrese fecha de realización (DD/MM/AAAA): ");
        fechaSolicitud = leerCadena();
        if (tipoTransaccion == 'C') {
            printf("Ingrese estado de envío de la compra (R:Recibido/P:Pendiente): ");
            scanf(" %c", &estadoEnvio);
            if (estadoEnvio == 'R' || estadoEnvio == 'r') {
                printf("Fecha de llegada de la compra (DD/MM/AAAA): ");
                fechaLlegada = leerCadena();
            }
        }
    } else {
        printf("No se pudo crear la transaccion.\n");
        return NULL;
    }
    return crearTransaccion(id, nombre, rut, totalProductosDistintos, productos, estadoEnvio, fechaSolicitud, fechaLlegada);
}

struct Farmacia *leerDatosFarmacia() {
    // Lee datos de una farmacia de la entrada del usuario.
    // Retorna un puntero a un struct Farmacia que contiene los datos leidos.
    char *id, *ciudad, *region;
    printf("Ingrese ID de la nueva farmacia: ");
    id = leerCadena();
    printf("Ingrese región de la nueva farmacia: ");
    region = leerCadena();
    printf("Ingrese ciudad de la nueva farmacia: ");
    ciudad = leerCadena();
    return crearFarmacia(id, region, ciudad);
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

int agregarCompraAInventario(struct NodoProducto **inventario, struct Transaccion *compra) {
    // Recibe un arbol binario de busqueda de struct NodoProducto y un puntero a struct Transaccion, agrega los
    // productos de la compra recibida al inventario. Retorna 1 si la compra es valida, en caso contrario retorna 0.
    struct Producto *producto;
    struct NodoLote *rec;
    int i, totalProducto;
    if (compra != NULL && compra->fechaLlegada != NULL) {
        for (i = 0; i < compra->totalProductosDistintos; i++) {
            printf("\nProducto: %s\n", compra->productos[i]->nombre);
            do {
                totalProducto = compra->productos[i]->cantidad;
                agregarNodoLote(&compra->productos[i]->lotes, crearNodoLote(leerDatosLote(totalProducto)));
                totalProducto -= getCantidadProducto(compra->productos[i]->lotes);
            } while (totalProducto > 0);
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

void alertaStock(struct Producto *producto, struct NodoTransaccion *ventas) {
    // Recibe un puntero a struct Producto, muestra una alerta si el stock de un producto es bajo.
    if (producto != NULL && tieneBajoStock(producto, ventas))
        printf("Alerta de stock bajo: %s, código: %s\n", producto->nombre, producto->codigo);
}

int quitarVentaAInventario(struct NodoProducto *inventario, struct Transaccion *venta, struct NodoTransaccion *ventas) {
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
                producto->cantidad -= venta->productos[i]->cantidad;
                alertaStock(producto, ventas);
            }
        }
        return 1;
    }
    return 0;
}

void agregarFarmaciaSistema(struct NodoFarmacia **headFarmacias) {
    // Función para agregar una nueva farmacia al sistema
    // Imprime mensaje de error si no fue posible
    struct NodoFarmacia *nuevoNodo;
    struct Farmacia *nuevaFarmacia;

    printf("\nAgregar una nueva farmacia al sistema\n");
    nuevaFarmacia = leerDatosFarmacia();
    nuevoNodo = crearNodoFarmacia(nuevaFarmacia);
    if (agregarNodoFarmacia(headFarmacias, nuevoNodo))
        printf("La farmacia se agregó exitosamente.\n");
    else
        printf("La farmacia ya existe en el sistema.\n");
}

struct Lote *quitarLote(struct NodoLote **head, char *numeroLote) {
    // Recibe una lista simplemente enlazada de struct NodoLote y numero de lote, elimina y libera la memoria asignada
    // al elemento que posea dicho numero de lote. Retorna 1 en caso de exito, en caso contrario retorna 0.
    struct NodoLote *rec, *nodoAEliminar;
    if (head != NULL) {
        nodoAEliminar = getNodoLote(*head, numeroLote);
        if (nodoAEliminar != NULL) {
            if (nodoAEliminar == *head) {
                *head = (*head)->sig;
            } else {
                rec = *head;
                while (rec->sig != NULL && rec->sig != nodoAEliminar)
                    rec = rec->sig;
                rec->sig = rec->sig->sig;
            }
            return nodoAEliminar->datosLote;
        }
    }
    return NULL;
}

struct Producto *quitarProducto(struct NodoProducto **root, char *codigo) {
    // Función para quitar un producto del inventario
    // Recibe la raíz del inventario y retorna el producto eliminado
    struct Producto *temp, *aux;
    if (!(*root)) {
        printf("\nNo existen productos en el inventario.\n");
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

void eliminarFarmaciaSistema(struct NodoFarmacia **headFarmacias) {
    // Función para eliminar una farmacia del sistema
    // Imprime mensaje de error si no fue posible
    char *idEliminar;

    if (!*headFarmacias) {
        printf("\nNo existen farmacias en el sistema.\n");
        return;
    }

    printf("\nEliminar una farmacia del sistema\n");
    printf("Ingrese el ID de la farmacia que desea eliminar: ");
    idEliminar = leerCadena();
    if (eliminarFarmacia(headFarmacias, idEliminar))
        printf("La farmacia fue eliminada exitosamente.\n");
    else
        printf("Error al eliminar la farmacia o farmacia no encontrada.\n");
}

void registrarVenta(struct Farmacia *farmacia) {
    // Función para registrar una venta en el sistema
    // Crea la venta, la agrega a la lista de ventas y reduce el stock del inventario
    // Imprime mensajes según errores o éxito
    struct Transaccion *venta;
    struct NodoTransaccion *nodoVenta;

    if (!farmacia->inventario) {
        printf("\nNo existen productos en el sistema.\n");
        return;
    }
    venta = leerDatosTransaccion(farmacia->inventario, 'V');
    if (!venta)
        return;
    nodoVenta = crearNodoTransaccion(venta);
    if (agregarNodoTransaccion(&farmacia->ventas, nodoVenta)) {
        quitarVentaAInventario(farmacia->inventario, venta, farmacia->ventas);
        printf("Venta registrada con exito.\n");
    }
    else {
        printf("No se pudo registrar la venta.\n");
    }
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
    if (farmacia->totalProductos + compra->cantidadProductos > farmacia->maxCapacidad) {
        printf("No se puede registrar la orden de compra, capacidad maxima de almacenaje alcanzada.\n");
        return;
    }
    if (compra->estadoEnvio == 'R' || compra->estadoEnvio == 'r')
        agregarCompraAInventario(&farmacia->inventario, compra);
    nodoCompra = crearNodoTransaccion(compra);
    if (agregarNodoTransaccion(&farmacia->compras, nodoCompra)) {
        printf("Orden de compra registrada con exito.\n");
    }
    else {
        printf("No se pudo registrar la orden de compra.\n");
    }
}

struct Lote *seleccionarLote(struct NodoLote *lotes) {
    // Función para seleccionar un lote según número de lote
    // Retorna un puntero al lote seleccionado
    // Imprime mensajes según errores
    struct Lote *lote;
    char *numeroLote;

    if (!lotes) {
        printf("\nNo existen lotes para este producto.\n");
        return NULL;
    }

    printf("\nIngrese el número de lote que desea seleccionar: ");
    numeroLote = leerCadena();
    lote = getLote(lotes, numeroLote);
    if (!lote) {
        printf("Lote no encontrado / número de lote no válido.\n");
        return NULL;
    }
    return lote;
}

struct Producto *seleccionarProducto(struct NodoProducto *root) {
    // Función para seleccionar un producto según ID
    // Retorna un puntero al producto seleccionado
    // Imprime mensajes según errores
    struct Producto *producto;
    char id[11];

    if (!root) {
        printf("\nNo existen productos en el sistema.\n");
        return NULL;
    }

    printf("\nIngrese el código del producto que desea seleccionar (10 caracteres): ");
    scanf(" %s", id);
    producto = getProducto(root, id);
    if (!producto) {
        printf("Producto no encontrado / codigo no válido\n");
        return NULL;
    }
    return producto;
}

struct Farmacia *seleccionarFarmacia(struct NodoFarmacia *headFarmacias) {
    // Función para que usuario seleccione una farmacia según ID
    // Retorna un puntero a la farmacia seleccionada
    // Imprime mensaje según errores
    struct Farmacia *farmacia;
    char *idBuscado;

    if (!headFarmacias) {
        printf("\nNo existen farmacias en el sistema.\n");
        return NULL;
    }

    printf("\nIngrese el ID de la farmacia a la que desea ingresar: ");
    idBuscado = leerCadena();
    farmacia = getFarmacia(headFarmacias, idBuscado);
    if (!farmacia) {
        printf("Farmacia no encontrada / ID no válido.\n");
    }
    return farmacia;
}

struct Transaccion *seleccionarTransaccion(struct NodoTransaccion *headTransaccion, char tipoTransaccion) {
    // Función para seleccionar una orden de compra o venta según ID
    // Retorna un puntero a la orden de compra o venta seleccionada
    // Según el tipo de transacción recibido por parámetro hace un print distinto
    struct Transaccion *transaccion;
    int id;

    if (!headTransaccion && tipoTransaccion == 'C') {
        printf("\nNo existen órdenes de compra en el sistema.\n");
        return NULL;
    }
    if (!headTransaccion && tipoTransaccion == 'V') {
        printf("\nNo existen ventas en el sistema.\n");
        return NULL;
    }

    if (tipoTransaccion == 'C') {
        printf("\nIngrese el ID de la orden de compra que desea seleccionar (valor numérico): ");
    } else {
        printf("\nIngrese el ID de la venta que desea seleccionar (valor numérico): ");
    }

    scanf(" %d", &id);
    transaccion = getTransaccion(headTransaccion, id);
    return transaccion;
}

void actualizarInventarioFarmacia(struct Farmacia *farmacia) {
    // Función para actualizar el inventario de la farmacia
    // Recibe la farmacia y actualiza el total de productos en bodega
    farmacia->totalProductos = getTotalStockFarmacia(farmacia->inventario);
}

void actualizarEstadoOrdenCompra(struct Farmacia *farmacia, struct NodoTransaccion *headTransaccion) {
    struct Transaccion *ordenCompra;

    if (!headTransaccion) {
        printf("\nNo existen órdenes de compra en el sistema.\n");
        return;
    }

    ordenCompra = seleccionarTransaccion(headTransaccion, 'C');
    if (!ordenCompra) {
        printf("\nOrden de compra no encontrada.\n");
    } else {
        ordenCompra->estadoEnvio = 'R';
        printf("\nIngrese fecha de recepción (DD/MM/AAAA): ");
        ordenCompra->fechaLlegada = leerCadena();
        agregarCompraAInventario(&farmacia->inventario, ordenCompra);
    }
}

void mostrarClienteConMasTransacciones(struct NodoTransaccion *headTransaccion) {
    // Recibe una lista simplemente enlazada de struct Transaccion, .
    // Muestra datos del rut con más transacciones en dicha lista.
    char *rutMasTransacciones, *nombre;
    int totalTransaccionesRut;
    rutMasTransacciones = getRutConMasTransacciones(headTransaccion, &nombre);
    totalTransaccionesRut = totalTransaccionesDeRut(headTransaccion, rutMasTransacciones);
    if (rutMasTransacciones != NULL) {
        printf("\nCliente con más transacciones: \n");
        printf("Nombre: %s\n", nombre);
        printf("RUT: %s\n", rutMasTransacciones);
        printf("Compras realizadas: %d\n", totalTransaccionesRut);
    } else {
        printf("\nNo existen transacciones.\n");
    }
}

void mostrarArregloVentas(struct Producto **prodVendidos, int tam) {
    // Recibe un puntero a un arreglo de las ventas y un entero que ilustra el tamaño del arreglo
    // Muestra al usuario los productos vendidos y la cantidad de cada uno
    int i;
    if (prodVendidos != NULL) {
        for (i = 0; i < tam; i++) {
            printf("\n%s, código: %s\n", prodVendidos[i]->nombre, prodVendidos[i]->codigo);
            printf("Cantidad: %d\n", prodVendidos[i]->cantidad);
        }
    }
}

void mostrarProductosRecetaVendidos(struct Producto **prodVendidos, int tam) {
    // Recibe un arreglo de struct Producto y el largo del arreglo, muestra por pantalla los productos vendidos que
    // requieren receta.
    int i;
    if (prodVendidos != NULL) {
        for (i = 0; i < tam; i++) {
            if (prodVendidos[i]->requiereReceta) {
                printf("\n%s, código: %s\n", prodVendidos[i]->nombre, prodVendidos[i]->codigo);
                printf("Cantidad: %d\n", prodVendidos[i]->cantidad);
            }
        }
    }
}

void mostrarVentasReceta(struct NodoTransaccion *ventas) {
    // Recibe una lista simplemente enlazada de struct NodoTransaccion y muestra por pantalla las ventas de productos
    // que requieren receta.
    struct NodoTransaccion *rec;
    if (ventas != NULL) {
        rec = ventas;
        while (rec != NULL) {
            if (hayProductoConReceta(rec->datosTransaccion)) {
                printf("\nVenta %d", rec->datosTransaccion->id);
                mostrarProductosRecetaVendidos(rec->datosTransaccion->productos,
                                               rec->datosTransaccion->totalProductosDistintos);
            }
            rec = rec->sig;
        }
    } else {
        printf("\nNo existen ventas en el sistema.\n");
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
            printf("\nVenta %d\n", rec->datosTransaccion->id);
            printf("Cantidad de productos: %d", rec->datosTransaccion->cantidadProductos);
            tam = rec->datosTransaccion->totalProductosDistintos;
            mostrarArregloVentas(rec->datosTransaccion->productos, tam);
            rec = rec->sig;
        }
    }
    else {
        printf("\nNo existen ventas registradas\n");
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
                printf("%s, código: %s\n", inventario->datosProducto->nombre, inventario->datosProducto->codigo);
                printf("Cantidad: %d\n\n", inventario->datosProducto->cantidad);
                (*contador)++;
            }
        } else { // Productos sin stock
            if (inventario->datosProducto->cantidad == 0) {
                printf("%s, código: %s\n", inventario->datosProducto->nombre, inventario->datosProducto->codigo);
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
        contador = 0;
        printf("\nProductos con stock en la farmacia\n");
        recorrerInventario(inventario, 1, &contador);
        if (contador == 0){
            printf("\nNo se encuentran productos con stock.\n");
        }
    }
    else {
        printf("\nNo existen productos en el inventario.\n");
    }
}

void mostrarProductosSinStock(struct NodoProducto *inventario) {
    // Recibe un puntero a un arreglo de farmacia
    // Imprimiendo por pantalla todos los productos que no tienen stock en ese momento
    int contador;
    if (inventario != NULL) {
        contador = 0;
        printf("\nProductos sin stock en la farmacia\n");
        recorrerInventario(inventario, 0, &contador);
        if (contador == 0){
            printf("\nNo se encuentran productos sin stock.\n");
        }
    }
    else {
        printf("\nNo existen productos en el inventario.\n");
    }
}

void mostrarLotesACaducar(struct NodoLote *head, char *fecha) {
    // Recibe una lista simplemente enlazada de struct NodoLote y una fecha, muestra los lotes de la lista que caducan
    // en la fecha recibida.
    struct NodoLote *rec;
    if (head != NULL) {
        rec = head;
        while (rec != NULL) {
            if (loteACaducarEnFecha(rec->datosLote, fecha)) {
                printf("Número Lote: %s\n", rec->datosLote->numeroLote);
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
            printf("\n%s, código: %s\n", root->datosProducto->nombre, root->datosProducto->codigo);
            mostrarLotesACaducar(root->datosProducto->lotes, fecha);
        }
        mostrarProductosACaducar(root->der, fecha);
    }
}

void mostrarProductosACaducarEnFecha(struct NodoProducto *root) {
    // Función para mostrar los productos a caducar en cierta fecha
    // Recibe la raíz del árbol de productos y lee la fecha a revisar
    // Imprime un mensaje si no hay productos en el sistema
    char fecha[11];
    if (!root) {
        printf("\nNo existen productos en el sistema.\n");
        return;
    }
    printf("\nIngrese la fecha a revisar (DD/MM/AAAA): ");
    scanf(" %s", fecha);
    mostrarProductosACaducar(root, fecha);
}

void mostrarDetalleFarmacia(struct Farmacia *farmacia) {
    // Función para mostrar los detalles de la farmacia
    printf("\nDetalle de la farmacia\n");
    printf("ID: %s\n", farmacia->id);
    printf("Ciudad: %s\n", farmacia->ciudad);
    printf("Región: %s\n", farmacia->region);
    printf("Total de productos distintos: %d\n", contarProductosDistintos(farmacia->inventario));
    printf("Total de productos en bodega: %d\n", farmacia->totalProductos);
    printf("Capacidad máxima de almacenaje: %d\n", farmacia->maxCapacidad);
}

void mostrarDetalleProducto(struct Producto *producto) {
    // Función para mostrar los detalles de un producto
    printf("\nDetalle del producto\n");
    printf("Nombre: %s\n", producto->nombre);
    printf("Código: %s\n", producto->codigo);
    printf("Descripción: %s\n", producto->descripcion);
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
    printf("\nLotes de producto %s, código %s\n", producto->nombre, producto->codigo);
    if (producto->lotes) {
        rec = producto -> lotes;
        while (rec) {
            printf("Número de lote: %s\n", rec->datosLote->numeroLote);
            printf("Cantidad de productos: %d\n", rec->datosLote->cantidadLote);
            printf("Fecha de caducidad: %s\n\n", rec->datosLote->fechaCaducidad);
            rec = rec->sig;
        }
    }
    else {
        printf("\nNo existen lotes para mostrar\n");
    }
}

void mostrarProductosBajoStockAux(struct NodoProducto *inventario, struct NodoTransaccion *ventas) {
    //Recibe un puntero al inventario de la farmacia y a las ventas de la farmacia
    //Se dedica a imprimir por pantalla todos los productos que esten por debajo del promedio de su stock
    if (inventario != NULL && ventas != NULL) {

        mostrarProductosBajoStockAux(inventario->izq, ventas);

        if (tieneBajoStock(inventario->datosProducto, ventas) == 1) {
            printf("%s, código:%s\n", inventario->datosProducto->nombre, inventario->datosProducto->codigo);
            printf("Cantidad: %d\n\n", inventario->datosProducto->cantidad);
        }

        mostrarProductosBajoStockAux(inventario->der, ventas);
    }
}

void mostrarProductosBajoStock(struct NodoProducto *inventario, struct NodoTransaccion *ventas) {
    // Función para mostrar los productos con bajo stock
    // Recibe la raíz del árbol de productos y la lista de ventas
    // Imprime un mensaje si no hay productos en el sistema o si no hay ventas
    // Llama a una función auxiliar para mostrar los productos con bajo stock
    if (!inventario) {
        printf("\nNo existen productos en el sistema.\n");
        return;
    }
    if (!ventas) {
        printf("\nNo existen ventas en el sistema.\n");
        printf("Productos con bajo stock se basa en promedio de ventas.\n");
        return;
    }
    printf("\nProductos con bajo stock\n");
    mostrarProductosBajoStockAux(inventario, ventas);
}

void mostrarInventarioAux(struct NodoProducto *root) {
    // Función para imprimir los productos del inventario de forma recursiva
    // Imprime nombre, código y stock de cada producto
    if (!root)
        return;
    mostrarInventarioAux(root->izq);
    printf("%s, código: %s, stock: %d\n", root->datosProducto->nombre,
           root->datosProducto->codigo, root->datosProducto->cantidad);
    mostrarInventarioAux(root->der);
}

void mostrarInventario(struct NodoProducto *root) {
    // Función para mostrar el inventario de productos
    // Imprime un mensaje si no hay productos en el sistema
    // Llama a una función auxiliar para mostrar los productos
    if (!root) {
        printf("\nNo existen productos en el sistema.\n");
        return;
    }
    printf("\nInventario de productos\n");
    mostrarInventarioAux(root);
}

void mostrarProductosTransaccion(struct Transaccion *transaccion) {
    // Función para mostrar los productos de una transacción
    // Imprime el nombre de cada producto
    int i;
    for (i = 0; i < transaccion->totalProductosDistintos; i++)
        printf("%d. %s\n", i+1, transaccion->productos[i]->nombre);
}

void mostrarDetalleVenta(struct NodoTransaccion *headVentas) {
    // Función para mostrar el detalle de una venta específica
    // Imprime un mensaje si no hay ventas en el sistema o si no fue encontrada la venta
    struct Transaccion *venta;

    if (!headVentas) {
        printf("\nNo existen ventas en el sistema.\n");
        return;
    }

    venta = seleccionarTransaccion(headVentas, 'V');

    if (!venta) {
        printf("\nVenta no encontrada.\n");
        return;
    }
    printf("\nDetalle de la Venta:\n");
    printf("ID: %d\n", venta->id);
    printf("Nombre del cliente: %s\n", venta->nombre);
    printf("Rut del cliente: %s\n", venta->rut);
    printf("Costo total: $%d\n", venta->costoTotal);
    printf("Total de productos distintos: %d\n", venta->totalProductosDistintos);
    printf("Total de productos comprados: %d\n", venta->cantidadProductos);
    mostrarProductosTransaccion(venta);
    printf("Fecha de venta: %s\n", venta->fechaSolicitud);
}

void mostrarDetalleOrdenCompra(struct NodoTransaccion *headCompras) {
    // Función para mostrar el detalle de una orden de compra específica
    // Imprime un mensaje si no hay órdenes de compra o si no fue encontrada la orden de compra
    struct Transaccion *ordenCompra;
    if (!headCompras) {
        printf("\nNo existen órdenes de compra en el sistema.\n");
        return;
    }

    ordenCompra = seleccionarTransaccion(headCompras, 'C');

    if (!ordenCompra) {
        printf("\nOrden de compra no encontrada.\n");
        return;
    }
    printf("\nDetalle de la orden de compra:\n");
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

void mostrarOrdenesCompra(struct NodoTransaccion *headCompras) {
    // Función para listar las órdenes de compra
    // Imprime un mensaje si no hay órdenes de compra en el sistema
    // Imprime id, nombre, costo total y el estado de la orden de compra
    if (!headCompras) {
        printf("\nNo existen órdenes de compra en el sistema. \n");
        return;
    }
    printf("\nÓrdenes de compra (ID, Nombre, Costo total, Estado):\n");
    while (headCompras) {
        printf("%d, %s, $%d, ", headCompras->datosTransaccion->id,
               headCompras->datosTransaccion->nombre, headCompras->datosTransaccion->costoTotal);
        if (headCompras->datosTransaccion->estadoEnvio == 'R' || headCompras->datosTransaccion->estadoEnvio == 'r')
            printf("Recibido\n");
        else
            printf("Pendiente\n");
        headCompras = headCompras->sig;
    }
}

void mostrarCategoriaMasVendida(struct NodoTransaccion *headVentas) {
    // Función para mostrar la categoría más vendida en la farmacia
    // Imprime un mensaje si no hay ventas en el sistema o si no se pudo determinar la categoría
    // Recibe la lista de ventas y llama a una función auxiliar
    char *categoriaMasVendida;
    if (!headVentas) {
        printf("\nNo existen ventas en el sistema.\n");
        return;
    }

    categoriaMasVendida = getCategoriaMasVendida(headVentas);
    if (!categoriaMasVendida) {
        printf("\nNo se pudo determinar la categoría más vendida.\n");
        return;
    }
    printf("\nLa categoría más vendida es: %s\n", categoriaMasVendida);
}

void mostrarCategoriaMasVendidaEstacion(struct NodoTransaccion *headVentas) {
    // Función para mostrar la categoría más vendida en una estación del año
    // Imprime un mensaje si no hay ventas en el sistema o si no se pudo determinar la categoría
    // Recibe la lista de ventas y llama a una función auxiliar
    char estacion, *categoriaMasVendida;
    if (!headVentas) {
        printf("\nNo existen ventas en el sistema.\n");
        return;
    }

    printf("\nIngrese la estación del año (P: Primavera, V: Verano, O: Otoño, I: Invierno): ");
    scanf(" %c", &estacion);

    categoriaMasVendida = getCategoriaMasVendidaEstacion(headVentas, estacion);
    if (!categoriaMasVendida) {
        printf("\nNo se pudo determinar la categoría más vendida en la estación.\n");
        return;
    }
    printf("\nLa categoría más vendida en la estación es: %s\n", categoriaMasVendida);
}

void mostrarProductoMasVendido(struct Farmacia *farmacia) {
    // Función para mostrar el producto más vendido en la farmacia
    // Imprime un mensaje si no hay ventas o productos en el sistema o si no se pudo determinar el producto
    // Recibe la farmacia y llama a una función auxiliar
    struct Producto *productoMasVendido;
    if (!farmacia->ventas) {
        printf("\nNo existen ventas en el sistema.\n");
        return;
    }
    if (!farmacia->inventario) {
        printf("\nNo existen productos en el sistema.\n");
        return;
    }
    productoMasVendido = getProductoMasVendido(farmacia->inventario, farmacia->ventas);
    if (!productoMasVendido) {
        printf("\nNo se pudo determinar el producto más vendido.\n");
        return;
    }
    printf("\nEl producto más vendido es: %s, código: %s\n", productoMasVendido->nombre,
           productoMasVendido->codigo);
}

void mostrarProductoMenosVendido(struct Farmacia *farmacia) {
    // Función para mostrar el producto menos vendido en la farmacia
    // Imprime un mensaje si no hay ventas o productos en el sistema o si no se pudo determinar el producto
    // Recibe la farmacia y llama a una función auxiliar
    struct Producto *productoMenosVendido;
    if (!farmacia->ventas) {
        printf("\nNo existen ventas en el sistema.\n");
        return;
    }
    if (!farmacia->inventario) {
        printf("\nNo existen productos en el sistema.\n");
        return;
    }
    productoMenosVendido = getProductoMenosVendido(farmacia->inventario, farmacia->ventas);
    if (!productoMenosVendido) {
        printf("\nNo se pudo determinar el producto menos vendido.\n");
        return;
    }
    printf("\nEl producto más vendido es: %s, código: %s\n", productoMenosVendido->nombre,
           productoMenosVendido->codigo);
}

void mostrarIngresosFarmacia(struct NodoTransaccion *headVentas) {
    // Función para mostrar los ingresos de la farmacia
    // Recibe la lista de ventas y llama a una función auxiliar
    int totalIngresos;

    totalIngresos = totalIngresosFarmacia(headVentas);
    printf("\nLos ingresos de la farmacia son: $%d\n", totalIngresos);
}

void mostrarVentasFarmacia(struct NodoTransaccion *headVentas) {
    // Función para mostrar el número de ventas de la farmacia
    // Recibe la lista de ventas y llama a una función auxiliar
    int totalVentas;

    totalVentas = totalVentasFarmacia(headVentas);
    printf("\nEl número de ventas de la farmacia es: %d\n\n", totalVentas);
}

void mostrarFarmacias(struct NodoFarmacia *headFarmacias) {
    // Función para mostrar las farmacias registradas
    struct NodoFarmacia *curr;
    if (!headFarmacias) {
        printf("\nNo existen farmacias en el sistema.\n");
        return;
    }
    curr = headFarmacias;
    printf("\nFarmacias (ID, Ciudad, Región):\n");
    do {
        printf("%s, %s, %s\n", curr->datosFarmacia->id, curr->datosFarmacia->ciudad, curr->datosFarmacia->region);
        curr = curr->sig;
    } while (curr != headFarmacias);
}

void mostrarFarmaciaConMasVentas(struct NodoFarmacia *headFarmacias) {
    // Función para mostrar la farmacia con más ventas en el sistema
    // Recibe la lista de farmacias y llama a una función auxiliar
    // Imprime un mensaje si no hay farmacias en el sistema o si no se pudo determinar la farmacia
    struct Farmacia *farmaciaMasVentas;

    if (!headFarmacias) {
        printf("\nNo existen farmacias en el sistema.\n");
        return;
    }

    farmaciaMasVentas = getFarmaciaConMasVentas(headFarmacias);
    if (!farmaciaMasVentas) {
        printf("\nNo se pudo determinar la farmacia con más ventas.\n");
        return;
    }
    printf("\nLa farmacia con más ventas es:\n");
    printf("ID: %s\n", farmaciaMasVentas->id);
    printf("Ciudad: %s\n", farmaciaMasVentas->ciudad);
    printf("Región: %s\n", farmaciaMasVentas->region);
}

void mostrarFarmaciaConMasIngresos(struct NodoFarmacia *headFarmacias) {
    // Función para mostrar la farmacia con más ingresos en el sistema
    // Recibe la lista de farmacias y llama a una función auxiliar
    // Imprime un mensaje si no hay farmacias en el sistema o si no se pudo determinar la farmacia
    struct Farmacia *farmaciaMasIngresos;

    if (!headFarmacias) {
        printf("\nNo existen farmacias en el sistema.\n");
        return;
    }

    farmaciaMasIngresos = getFarmaciaConMasIngresos(headFarmacias);
    if (!farmaciaMasIngresos) {
        printf("\nNo se pudo determinar la farmacia con más ingresos.\n");
        return;
    }
    printf("\nLa farmacia con más ingresos es:\n");
    printf("ID: %s\n", farmaciaMasIngresos->id);
    printf("Ciudad: %s\n", farmaciaMasIngresos->ciudad);
    printf("Región: %s\n", farmaciaMasIngresos->region);
}

void mostrarRegionConMasVentas(struct NodoFarmacia *headFarmacias) {
    // Función para mostrar la región con más ventas
    // Recibe la lista de farmacias y llama a una función auxiliar
    // Imprime un mensaje si no hay farmacias en el sistema o si no se pudo determinar la región
    char *regionMasVentas;

    if (!headFarmacias) {
        printf("\nNo existen farmacias en el sistema.\n");
        return;
    }

    regionMasVentas = getRegionConMasVentas(headFarmacias);
    if (!regionMasVentas) {
        printf("\nNo se pudo determinar la región con más ventas.\n");
        return;
    }
    printf("\nLa región con más ventas es: %s\n", regionMasVentas);
}

void mostrarRegionConMasIngresos(struct NodoFarmacia *headFarmacias) {
    // Función para mostrar la región con más ingresos
    // Recibe la lista de farmacias y llama a una función auxiliar
    // Imprime un mensaje si no hay farmacias en el sistema o si no se pudo determinar la región
    char *regionMasIngresos;

    if (!headFarmacias) {
        printf("\nNo existen farmacias en el sistema.\n");
        return;
    }

    regionMasIngresos = getRegionConMasIngresos(headFarmacias);
    if (!regionMasIngresos) {
        printf("\nNo se pudo determinar la región con más ingresos.\n");
        return;
    }
    printf("\nLa región con más ingresos es: %s\n", regionMasIngresos);
}

int confirmarEliminarLote(struct Lote *lote) {
    // Función para confirmar la eliminación de un lote
    // Recibe un puntero al lote
    // Retorna 1 (true) o 0 (false)
    char opcion;
    printf("\nEstá seguro que desea eliminar el lote %s del sistema? (s/n): ", lote->numeroLote);
    scanf(" %c", &opcion);
    if (opcion == 's' || opcion == 'S')
        return 1;
    return 0;
}

int confirmarEliminarProducto(struct Producto *producto) {
    // Función para confirmar la eliminación de un producto
    // Recibe un puntero al producto
    // Retorna 1 (true) o 0 (false)
    char opcion;
    printf("\nEstá seguro/a que desea eliminar %s, codigo %s del sistema? (s/n): ", producto->nombre, producto->codigo);
    scanf(" %c", &opcion);
    if (opcion == 's' || opcion == 'S')
        return 1;
    return 0;
}

int confirmarSalida() {
    // Función para confirmar la salida del sistema
    // Retorna un 1 (true) si el usuario confirma, 0 (false) si no
    char opcion;
    printf("\n¿Está seguro/a que desea salir del sistema? (s/n): ");
    scanf(" %c", &opcion);
    if (opcion == 's' || opcion == 'S')
        return 1;
    return 0;
}

void menuEditarLote(struct Lote *lote) {
    // Recibe un puntero a un struct Lote y muestra un menú para modificar los datos del lote
    int opcion;
    do {
        printf("\n\nMenú de edición de lote\n");
        printf("1. Cambiar fecha de caducidad del lote\n");
        printf("2. Volver al menú anterior\n");
        printf("Seleccione una opción del menu:\n");

        scanf(" %d", &opcion);

        switch (opcion) {
            case 1:
                printf("Fecha de caducidad actual: %s\n", lote->fechaCaducidad);
                printf("Nueva fecha de caducidad (DD/MM/AAAA): ");
                lote->fechaCaducidad = leerCadena();
                break;
            case 2:
                printf("Volviendo al menú anterior...\n");
                break;
            default:
                printf("Opción no válida, por favor ingrese una opción válida.\n");
                break;
        }
    } while (opcion != 2);
}

void menuEditarProducto(struct Producto *producto) {
    // Recibe un puntero a struct Producto, muestra un menú de edición de producto y permite al usuario editar los campos
    struct Lote *lote;
    int opcion;
    do {
        printf("\n\nMenú de edición de producto\n");
        printf("1. Editar nombre.\n");
        printf("2. Editar categoría.\n");
        printf("3. Editar precio.\n");
        printf("4. Editar descripción.\n");
        printf("5. Editar proveedor.\n");
        printf("6. Editar requerimiento de receta.\n");
        printf("7. Editar lote del producto.\n");
        printf("8. Volver al menú anterior\n");
        printf("Seleccione una opción del menú: ");

        scanf(" %d", &opcion);

        switch (opcion) {
            case 1:
                printf("Nombre actual: %s\n", producto->nombre);
                printf("Nuevo nombre: ");
                producto->nombre = leerCadena();
                break;
            case 2:
                printf("Categoría actual: %s\n", producto->categoria);
                printf("Nueva categoría: ");
                producto->categoria = leerCadena();
                break;
            case 3:
                printf("Precio actual: $%d\n", producto->precio);
                printf("Nuevo precio: ");
                scanf(" %d", &producto->precio);
                break;
            case 4:
                printf("Descripción actual: %s\n", producto->descripcion);
                printf("Nueva descripción: ");
                producto->descripcion = leerCadena();
                break;
            case 5:
                printf("Proveedor actual: %s\n", producto->proveedor);
                printf("Nuevo proveedor: ");
                producto->proveedor = leerCadena();
                break;
            case 6:
                printf("Requerimiento receta actual: ");
                if (producto->requiereReceta)
                    printf("Si\n");
                else
                    printf("No\n");
                printf("Nuevo requerimiento receta (0/1): ");
                scanf(" %d", &producto->requiereReceta);
                break;
            case 7:
                lote = seleccionarLote(producto->lotes);
                if (lote)
                    menuEditarLote(lote);
                else
                    opcion = 0;
                break;
            case 8:
                printf("Volviendo al menú anterior...\n");
                break;
            default:
                printf("Opción no válida, por favor ingrese una opción válida.\n");
                break;
        }
    } while (opcion != 8);
}

void menuEditarTransaccion(struct Transaccion *transaccion) {
    // Recibe un puntero a struct Transaccion, muestra un menú de edición de transacción y permite al usuario editar los campos
    int opcion;
    do {
        printf("\n\nMenú de edición de transacción\n");
        printf("1. Editar nombre del cliente\n");
        printf("2. Editar rut del cliente\n");
        printf("3. Editar fecha de realizacion\n");
        printf("4. Editar fecha de recepcion\n");
        printf("5. Volver al menú anterior\n");
        printf("Seleccione una opción del menú: ");
        scanf(" %d", &opcion);
        switch (opcion) {
            case 1:
                printf("Nombre actual del cliente: %s\n", transaccion->nombre);
                printf("Nuevo nombre del cliente: ");
                transaccion->nombre = leerCadena();
                break;
            case 2:
                printf("Rut actual del cliente: %s\n", transaccion->rut);
                printf("Nuevo rut del cliente: ");
                transaccion->rut = leerCadena();
                break;
            case 3:
                printf("Fecha de realizacion actual: %s\n", transaccion->fechaSolicitud);
                printf("Nueva fecha de realzacion: ");
                transaccion->fechaSolicitud = leerCadena();
                break;
            case 4:
                printf("Fecha de recepcion actual: %s\n", transaccion->fechaLlegada);
                printf("Nueva fecha de recepcion: ");
                transaccion->fechaLlegada = leerCadena();
                break;
            case 5:
                printf("Volviendo al menú anterior...\n");
                break;
            default:
                printf("Opción no válida, por favor ingrese una opción válida.\n");
                break;
        }
    } while (opcion != 5);
}

void menuEditarFarmacia(struct Farmacia *farmacia) {
    // Recibe un puntero a struct Farmacia y muestra un menú de edición de farmacia y permite al usuario editar los campos
    int opcion;
    do {
        printf("\n\nMenú de edición de farmacia\n");
        printf("1. Editar capacidad máxima de almacenaje\n");
        printf("2. Volver al menú anterior\n");
        printf("Seleccione una opción del menú: ");
        scanf(" %d", &opcion);
        switch (opcion) {
            case 1:
                printf("Capacidad máxima de almacenaje actual: %d\n", farmacia->maxCapacidad);
                printf("Nueva capacidad máxima de almacenaje: ");
                scanf(" %d", &farmacia->maxCapacidad);
                break;
            case 2:
                printf("Volviendo al menú anterior...\n");
                break;
            default:
                printf("Opción no válida, por favor ingrese una opción válida.\n");
                break;
        }
    } while (opcion != 2);
}

void menuEliminarLote(struct NodoLote *lotes) {
    // Función con menú de usuario para eliminar un lote de un producto
    // Recibe la lista de lotes de un producto
    // Imprime un mensaje si no hay lotes en el sistema
    // Pregunta por confirmación para eliminar el lote
    struct Lote *lote;
    if (!lotes) {
        printf("\nNo existen lotes en el sistema.\n\n");
        return;
    }
    lote = seleccionarLote(lotes);
    if (lote) {
        if (confirmarEliminarLote(lote)) {
            if (quitarLote(&lotes, lote->numeroLote))
                printf("\nLote eliminado exitosamente.\n");
            else
                printf("\nError al eliminar el lote.\n");
        }
    }
}

void menuEliminarProducto(struct Farmacia *farmacia) {
    // Función con menú de usuario para eliminar un producto del inventario
    // Recibe la referencia a la farmacia
    // Imprime un mensaje si no hay productos en el sistema
    // Pregunta por confirmación para eliminar el producto
    // Llama a función auxiliar para actualizar el stock de la farmacia
    struct Producto *producto;
    if (!farmacia->inventario) {
        printf("\nNo existen productos en el sistema.\n\n");
        return;
    }
    producto = seleccionarProducto(farmacia->inventario);
    if (producto) {
        if (confirmarEliminarProducto(producto)) {
            if (quitarProducto(&(farmacia->inventario), producto->codigo)) {
                actualizarInventarioFarmacia(farmacia);
                printf("Producto: %s, código: %s eliminado exitosamente.\n", producto->nombre, producto->codigo);
            }
            else
                printf("Error al eliminar el producto.\n");
        }
    }
}

void menuProducto(struct Producto *producto) {
    int opcion;

    do {
        printf("\n\nMenú de opciones de producto ID: %s\n", producto->codigo);
        printf("1. Ver detalle del producto\n");
        printf("2. Mostrar lotes del producto\n");
        printf("3. Opciones de edición de datos del producto.\n");
        printf("4. Eliminar lote del producto.\n");
        printf("5. Volver al menú anterior\n");
        printf("Ingrese una opción del menú: ");

        scanf(" %d", &opcion);

        switch (opcion) {
            case 1:
                mostrarDetalleProducto(producto);
                break;
            case 2:
                mostrarLotesProducto(producto);
                break;
            case 3:
                menuEditarProducto(producto);
            case 4:
                menuEliminarLote(producto->lotes);
                break;
            case 5:
                printf("Volviendo al menú anterior...\n");
                break;
            default:
                printf("Opción no válida, por favor ingrese una opción válida.\n");
                break;

        }
    } while (opcion != 5);
}

void menuInventario(struct Farmacia *farmacia) {
    // Función para el menú con opciones relacionadas al inventario de productos
    int opcion;
    struct Producto *producto;

    do {
        printf("\n\nMenú de inventario\n");
        printf("1. Ver inventario\n");
        printf("2. Ver productos a caducar en cierta fecha\n");
        printf("3. Ver productos con bajo stock\n");
        printf("4. Ver productos sin stock\n");
        printf("5. Ver productos con stock\n");
        printf("6. Opciones de un producto\n");
        printf("7. Eliminar un producto\n");
        printf("8. Volver al menú anterior\n");
        printf("Seleccione una opción del menú: ");

        scanf(" %d", &opcion);

        switch (opcion) {
            case 1:
                mostrarInventario(farmacia->inventario);
                break;
            case 2:
                mostrarProductosACaducarEnFecha(farmacia->inventario);
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
                menuEliminarProducto(farmacia);
                break;
            case 8:
                printf("Volviendo al menú anterior...\n");
                break;
            default:
                printf("Opción no válida, por favor ingrese una opción válida.\n");
                break;
        }
    } while (opcion != 8);
}

void menuVentas(struct Farmacia *farmacia) {
    // Función para el menú con opciones relacionadas a ventas de la farmacia
    struct Transaccion *venta;
    int opcion;

    do {
        printf("\n\nMenú de ventas de la farmacia\n");
        printf("1. Registrar venta\n");
        printf("2. Ver ventas\n");
        printf("3. Ver ventas de productos con receta\n");
        printf("4. Ver detalle de una venta\n");
        printf("5. Opciones de edición de la venta\n");
        printf("6. Volver al menú anterior\n");
        printf("Seleccione una opción del menú: ");

        scanf(" %d", &opcion);

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
                venta = seleccionarTransaccion(farmacia->ventas, 'V');
                if (venta)
                    menuEditarTransaccion(venta);
                else
                    opcion = 0;
                break;
            case 6:
                printf("Volviendo al menú anterior...\n");
                break;
            default:
                printf("Opción no válida, por favor ingrese una opción válida.\n");
                break;
        }
    } while (opcion != 6);
}

void menuCompras(struct Farmacia *farmacia) {
    // Función para el menú con opciones relacionadas a órdenes de compra de la farmacia
    struct Transaccion *compra;
    int opcion;

    do {
        printf("\n\nMenú de órdenes de compra de la farmacia\n");
        printf("1. Registrar orden de compra\n");
        printf("2. Ver órdenes de compra\n");
        printf("3. Actualizar estado de orden de compra\n");
        printf("4. Ver detalle de una orden de compra\n");
        printf("5. Opciones de edición de la compra.\n");
        printf("6. Volver al menú anterior\n");
        printf("Seleccione una opción: ");

        scanf(" %d", &opcion);

        switch(opcion) {
            case 1:
                registrarOrdenCompra(farmacia);
                actualizarInventarioFarmacia(farmacia);
                break;
            case 2:
                mostrarOrdenesCompra(farmacia->compras);
                break;
            case 3:
                actualizarEstadoOrdenCompra(farmacia, farmacia->compras);
                actualizarInventarioFarmacia(farmacia);
                break;
            case 4:
                mostrarDetalleOrdenCompra(farmacia->compras);
                break;
            case 5:
                compra = seleccionarTransaccion(farmacia->compras, 'C');
                if (compra)
                    menuEditarTransaccion(compra);
                else
                    opcion = 0;
                break;
            case 6:
                printf("Volviendo al menú anterior...\n");
                break;
            default:
                printf("Opción no válida, por favor ingrese una opción válida.\n");
                break;
        }
    } while (opcion != 6);
}

void menuAnalisisDatosFarmacia(struct Farmacia *farmacia) {
    int opcion;

    do {
        printf("\n\nMenú de análisis de datos de la farmacia\n");
        printf("1. Categoría más vendida\n");
        printf("2. Categoría más vendida por estación\n");
        printf("3. Producto más vendido\n");
        printf("4. Producto menos vendido\n");
        printf("5. Ingresos de la farmacia\n");
        printf("6. Número de ventas de la farmacia\n");
        printf("7. Cliente con más transacciones\n");
        printf("8. Volver al menú anterior\n");
        printf("Seleccione una opción del menú: ");

        scanf(" %d", &opcion);

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
                mostrarClienteConMasTransacciones(farmacia->ventas);
                break;
            case 8:
                printf("Volviendo al menú anterior...\n");
                break;
            default:
                printf("Opción no válida, por favor ingrese una opción válida.\n");
                break;
        }
    } while (opcion != 8);
}

void menuUnaFarmacia(struct Farmacia *farmacia) {
    // Función para el menú principal de una farmacia específica
    int opcion;

    do {
        printf("\n\nMenú de la farmacia\n");
        printf("1. Detalle de la farmacia\n");
        printf("2. Opciones de inventario/productos\n");
        printf("3. Opciones de venta\n");
        printf("4. Opciones de órdenes de compra\n");
        printf("5. Opciones de análisis de datos\n");
        printf("6. Opciones de edición de datos de la farmacia\n");
        printf("7. Volver al menú anterior\n");
        printf("Seleccione una opción del menú: ");

        scanf(" %d", &opcion);

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
                menuEditarFarmacia(farmacia);
                break;
            case 7:
                printf("Volviendo al menú anterior...\n");
                break;
            default:
                printf("Opción no válida, por favor ingrese una opción válida.\n");
                break;
        }
    } while (opcion != 7);
}

void menuFarmacias(struct NodoFarmacia **headFarmacias) {
    // Función para el menú de farmacias del sistema
    int opcion;
    struct Farmacia *farmacia;

    do {
        printf("\n\nMenú de farmacias de FarmaSalud\n");
        printf("1. Listar farmacias\n");
        printf("2. Ingresar a una farmacia\n");
        printf("3. Agregar una farmacia\n");
        printf("4. Eliminar una farmacia\n");
        printf("5. Volver al menú principal\n");
        printf("Seleccione una opción del menú: ");

        scanf(" %d", &opcion);

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
                printf("Volviendo al menú principal...\n");
                break;
            default:
                printf("Opción no válida, por favor ingrese una opción válida.\n");
                break;
        }
    } while (opcion != 5);
}

void menuAnalisisFarmaSalud(struct FarmaSalud *farmaSalud) {
    // Función para el menú de análisis de datos de FarmaSalud
    // Recibe la estructura de FarmaSalud y llama a funciones auxiliares
    int opcion;

    do {
        printf("\n\nMenú de análisis de datos de FarmaSalud\n");
        printf("1. Ver farmacia con más ventas\n");
        printf("2. Ver farmacia con mayores ingresos\n");
        printf("3. Ver región con más ventas\n");
        printf("4. Ver región con mayores ingresos\n");
        printf("5. Volver al menú anterior\n");
        printf("Seleccione una opción: ");

        scanf(" %d", &opcion);

        switch (opcion) {
            case 1:
                mostrarFarmaciaConMasVentas(farmaSalud->headFarmacias);
                break;
            case 2:
                mostrarFarmaciaConMasIngresos(farmaSalud->headFarmacias);
                break;
            case 3:
                mostrarRegionConMasVentas(farmaSalud->headFarmacias);
                break;
            case 4:
                mostrarRegionConMasIngresos(farmaSalud->headFarmacias);
                break;
            case 5:
                printf("Volviendo al menú anterior...\n");
                break;
            default:
                printf("Opción no válida, por favor ingrese una opción válida.\n");
                break;
        }
    } while (opcion != 5);
}

void menuFarmaSalud(struct FarmaSalud *farmaSalud) {
    // Función para el menú principal de usuario de FarmaSalud
    int opcion;

    do {
        printf("\n\nMenú de FarmaSalud\n");
        printf("1. Ingresar a menú de farmacias\n");
        printf("2. Ingresar a menú de análisis de datos\n");
        printf("3. Salir\n");
        printf("Seleccione una opción del menú: ");

        scanf(" %d", &opcion);

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
                printf("Opción no válida, por favor ingrese una opción válida.\n");
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