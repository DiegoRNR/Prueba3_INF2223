#include <stdio.h>
#include <stdlib.h>

//arrays

//Doblemente Enlazadas

struct Nodo
{
	int elemento;
	struct Nodo *ant, *sig;
};

struct Nodo *crearNodo()
{
	struct Nodo *nuevo;

	/*Se obtiene la memoria al nuevo nodo*/
	nuevo=((struct Nodo*)malloc(sizeof(struct Nodo)));

	/*Se completa con los datos*/
	printf("ingrese numero: ");
	scanf("%d", &nuevo->elemento);
	nuevo->ant=NULL;
	nuevo->sig=NULL;

	return nuevo;
}

void enlazarNodo(struct Nodo **head, struct Nodo *nuevo)
{
	struct Nodo *rec=*head;

	if(*head==NULL)
	{
		*head=nuevo;
	}
	else
	{
		/*Como no era el primer elemento a agregar se busca el ultimo elemento*/
		while(rec!=NULL)
		{
			if(rec->sig==NULL)
			{
				rec->sig=nuevo;
				rec->sig->ant=rec;
				rec=nuevo;
			}
			rec=rec->sig;
		}
	}
}


main()
{
	/*En este ejemplo se agregarán 3 elementos a la lista*/

	/*Declaracion de la lista*/
	struct Nodo *lista=NULL, *aux, *rec;

	/*Primer elemento*/
	aux=crearNodo();
	enlazarNodo(&lista,aux);

	
	/*Segundo elemento sin usar aux*/
	enlazarNodo(&lista,crearNodo());

	/*Tercer elemento sin usar aux*/
	enlazarNodo(&lista,crearNodo());

	/*Mostrar elementos de la lista*/
	rec=lista;
	while(rec!=NULL)
	{
		printf("[%d]->", rec->elemento);
		rec=rec->sig;
	}
	printf("NULL\n\n");
}

//Circulares

struct Nodo
{
	int elemento;
	struct Nodo *sig;
};

struct Nodo *crearNodo()
{
	struct Nodo *nuevo;

	/*Se obtiene la memoria al nuevo nodo*/
	nuevo=((struct Nodo*)malloc(sizeof(struct Nodo)));

	/*Se completa con los datos*/
	printf("ingrese numero: ");
	scanf("%d", &nuevo->elemento);
	nuevo->sig=NULL;

	return nuevo;
}

void enlazarNodo(struct Nodo **head, struct Nodo *nuevo)
{
	struct Nodo *rec=*head;

	if(*head==NULL)
	{
		*head=nuevo;
		(*head)->sig=*head;
	}
	else
	{
		do
		{
			if(rec!=NULL)
			{
				if(rec->sig==*head)
				{
					nuevo->sig=rec->sig;
					rec->sig=nuevo;
					rec=rec->sig;
				}
			}

		}while(rec->sig!=*head);
	}
}

main()
{
	/*En este ejemplo se agregarán 3 elementos a la lista*/

	/*Declaracion de la lista*/
	struct Nodo *listaCircular=NULL, *aux, *rec;

	/*Primer elemento*/
	aux=crearNodo();
	enlazarNodo(&listaCircular,aux);

	
	/*Segundo elemento sin usar aux*/
	enlazarNodo(&listaCircular,crearNodo());

	/*Mostrar elementos de la lista*/
	rec=listaCircular;
	if(rec)
	{
		do
		{
			printf("[%d]->", rec->elemento);
			rec=rec->sig;
		}while(rec!=listaCircular);
	}
	printf("NULL\n\n");
}




//arboles


struct Nodo {
    int dato;
    struct Nodo *izquierda;
    struct Nodo *derecha;
};

struct Nodo *nuevoNodo(int dato) {
    // Solicitar memoria
    size_t tamanioNodo = sizeof(struct Nodo);
    struct Nodo *nodo = (struct Nodo *) malloc(tamanioNodo);
    // Asignar el dato e iniciar hojas
    nodo->dato = dato;
    nodo->izquierda = nodo->derecha = NULL;
    return nodo;
}

void insertar(struct Nodo *nodo, int dato) {
    // ¿Izquierda o derecha?
    // Si es mayor va a la derecha
    if (dato > nodo->dato) {
        // Tienes espacio a la derecha?
        if (nodo->derecha == NULL) {
            nodo->derecha = nuevoNodo(dato);
        } else {
            // Si la derecha ya está ocupada, recursividad ;)
            insertar(nodo->derecha, dato);
        }
    } else {
        // Si no, a la izquierda
        if (nodo->izquierda == NULL) {
            nodo->izquierda = nuevoNodo(dato);
        } else {
            // Si la izquierda ya está ocupada, recursividad ;)
            insertar(nodo->izquierda, dato);
        }
    }
}

void preorden(struct Nodo *nodo) {
    if (nodo != NULL) {
        printf("%d,", nodo->dato);
        preorden(nodo->izquierda);
        preorden(nodo->derecha);
    }
}

void inorden(struct Nodo *nodo) {
    if (nodo != NULL) {
        inorden(nodo->izquierda);
        printf("%d,", nodo->dato);
        inorden(nodo->derecha);
    }
}

void postorden(struct Nodo *nodo) {
    if (nodo != NULL) {
        postorden(nodo->izquierda);
        postorden(nodo->derecha);
        printf("%d,", nodo->dato);
    }
}
