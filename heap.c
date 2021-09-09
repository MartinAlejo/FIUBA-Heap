#include "heap.h"
#include <stdlib.h>
#include <stdbool.h>  
#include <stddef.h>   
#include <stdio.h>
#define TAM_INICIAL  10
#define FACTOR_REDIMENSION 2

typedef struct heap {
    void **arr;
    size_t cant;
    size_t tam;
    cmp_func_t cmp;
} heap_t;

/* Funciones auxiliares */

void swap(void *arr[], size_t pos_elem_1, size_t pos_elem_2){
    void *aux = arr[pos_elem_1];
    arr[pos_elem_1] = arr[pos_elem_2];
    arr[pos_elem_2] = aux;
}

bool heap_redimensionar(heap_t *heap, size_t tam_nuevo) {
    void **arr_nuevo = realloc(heap->arr, tam_nuevo * sizeof(void*));
    if (!arr_nuevo) return false;

    heap->arr = arr_nuevo;
    heap->tam = tam_nuevo;

    return true;
}

void upheap(heap_t *heap, size_t pos_hijo) {
    if (pos_hijo == 0) return; // El hijo esta al principio del arreglo, no hay que verificar mas
    size_t pos_padre = (pos_hijo - 1) / 2;

    if (heap->cmp(heap->arr[pos_padre], heap->arr[pos_hijo]) >= 0) { 
        return; // En este caso, el padre es mayor que el hijo, se mantuvo la invariante de heap
    }
    swap(heap->arr, pos_padre, pos_hijo); // Swapeamos el hijo con el padre
    upheap(heap, pos_padre);
}

// Devuelve la posicion del maximo entre el padre, el hijo izquierdo y el hijo derecho
int maximo_cmp(int pos_padre, int pos_hijo_izq, int pos_hijo_der, cmp_func_t cmp, void *arr[]) {
    int pos_maximo;
    int pos_hijo_mayor;

    // Primero busco el mayor entre los hijos
    int cmp_hijos = cmp(arr[pos_hijo_izq], arr[pos_hijo_der]); // Mayor a 0 si el hijo izq es mayor que el hijo der
    if (cmp_hijos > 0) {
        pos_hijo_mayor = pos_hijo_izq;
    }
    else {
        pos_hijo_mayor = pos_hijo_der;
    }

    // Comparo el padre con el mayor de los hijos
    int cmp_padre_hijo = cmp(arr[pos_padre], arr[pos_hijo_mayor]); // Mayor a 0 si el padre es mayor que el hijo mas grande
    if (cmp_padre_hijo > 0) {
        pos_maximo = pos_padre;
    }
    else {
        pos_maximo = pos_hijo_mayor;
    }

    return pos_maximo;
}

void downheap(void *arr[], int pos_padre, cmp_func_t cmp, size_t cant) {
    if (pos_padre >= (int)cant) return; // El padre esta al final del arreglo, no hay que verificar mas
    int pos_hijo_izq = 2 * pos_padre + 1;
    int pos_hijo_der = 2 * pos_padre + 2;

    // Trabajamos casos bordes
    if (pos_hijo_izq > ((int)cant - 1)) {
        // En este caso, no tiene hijos (si no tiene hijo izquierdo tampoco tendra hijo derecho). No hay que hacer nada
        return;
    }
    else if (pos_hijo_der > ((int)cant - 1)) {
        // En este caso, existe hijo izquierdo pero no existe hijo derecho. El hijo derecho "pasa a ser" tambien el hijo izquierdo
        pos_hijo_der = pos_hijo_izq;
    }

    // Hallamos el mayor y hacemos el swap si es necesario
    int pos_maximo = maximo_cmp(pos_padre, pos_hijo_izq, pos_hijo_der, cmp, arr); // Nos quedamos con el mas grande de los 3
    if (pos_maximo == pos_padre) return; // El padre es mas grande que ambos hijos, se mantuvo la invariante de heap

    swap(arr, (size_t)pos_padre, (size_t)pos_maximo); // Swapeamos el padre con el hijo mayor
    downheap(arr, pos_maximo, cmp, cant);
}

void heapify(void *arr[],size_t cant, cmp_func_t cmp){
    for (size_t i = 0; i < cant + 1; i++){
        downheap(arr, (int)(cant - i) / 2, cmp, cant);
    }
}   

void _heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
    if (cant == 1) return;
    swap(elementos, 0, cant - 1);
    downheap(elementos, 0, cmp, cant - 1);
    _heap_sort(elementos, cant - 1, cmp);
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
    heapify(elementos,cant,cmp);
    _heap_sort(elementos,cant,cmp);
}

/* Primitivas del heap */

heap_t *heap_crear(cmp_func_t cmp) {
    heap_t* heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;

    heap->cant = 0;
    heap->cmp = cmp;
    heap->tam = TAM_INICIAL;

    void **arr = malloc(sizeof(void*) * heap->tam);
    if (!arr) {
        free(heap);
        return NULL;
    }

    heap->arr = arr;

    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
    heap_t *heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;

    heap->cmp = cmp;

    void **arr = malloc(sizeof(void*) * n);
    if (!arr) {
        free(heap);
        return NULL;
    }

    for (size_t i = 0; i < n; i++) {
        arr[i] = arreglo[i];
    }

    heapify(arr,n,cmp);

    heap->cant = n;
    heap->arr = arr;
    heap->tam = n;

    return heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)) {
    while (!heap_esta_vacio(heap)) {
        void *act = heap_desencolar(heap);
        if (destruir_elemento) {
            destruir_elemento(act);
        }
    }
    free(heap->arr);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap){
    return heap->cant;
}

bool heap_esta_vacio(const heap_t *heap){
    return heap->cant == 0;
}

bool heap_encolar(heap_t *heap, void *elem){
    // Vemos si hay que redimensionar
    if(heap_cantidad(heap) >= heap->tam && !heap_redimensionar(heap, heap->tam * FACTOR_REDIMENSION)) {
        return false;
    }

    // Encolamos
    heap->arr[heap->cant] = elem;
    upheap(heap, heap->cant);
    heap->cant++;

    return true;
}

void *heap_ver_max(const heap_t *heap) {
    return heap_esta_vacio(heap) ? NULL : heap->arr[0];
}

void *heap_desencolar(heap_t *heap) {
    if (heap_esta_vacio(heap)) return NULL; // Si el heap esta vacio devolvemos NULL

    // Vemos si hay que redimensionar
    if (heap_cantidad(heap) * (FACTOR_REDIMENSION * FACTOR_REDIMENSION) <= heap->tam) {
        if (heap->tam != TAM_INICIAL && !heap_redimensionar(heap, heap->tam / FACTOR_REDIMENSION)) {
            return NULL;
        }
    }

    // Desencolamos
    void *aux = heap->arr[0];
    swap(heap->arr, 0,(heap->cant - 1)); // Posicionamos el elemento a desencolar al final del array
    heap->cant--;
    downheap(heap->arr, 0, heap->cmp, heap->cant);

    return aux;
}

