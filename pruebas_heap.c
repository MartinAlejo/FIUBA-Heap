#include "heap.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Compara dos claves
// Pre: Los elementos van a ser numeros
// Post: Devuelve 1 si el primer elemento es mayor el segundo, -1 si el primero es menor o 0 si son iguales
int comparar(const void *elem_1, const void *elem_2) {
    int elem_1_aux = *(int*)elem_1;
    int elem_2_aux = *(int*)elem_2;
    //printf("La clave 1: %d La clave 2: %d\n", elem_1_aux, elem_2_aux);
    if (elem_1_aux > elem_2_aux) {
        return 1;
    }
    else if (elem_1_aux < elem_2_aux) {
        return -1;
    }
    return 0;
}

static void prueba_crear_y_destruir_heap() {
    printf("---Inicio de prueba de crear y destruir heap---\n");
    heap_t* heap = heap_crear(comparar);

    print_test("El heap fue creado", heap != NULL);
    heap_destruir(heap, NULL);
    print_test("Se destruyo el heap", 1); // Se destruyo el heap, si la prueba falla valgrind devolveria el error
}

static void prueba_heap_vacio() {
    printf("---Inicio de prueba de heap vacio---\n");
    heap_t* heap = heap_crear(comparar);

    print_test("El heap se encuentra vacio", heap_esta_vacio(heap));
    print_test("La cantidad de elementos en el heap es 0", heap_cantidad(heap) == 0);
    print_test("Ver el maximo con un heap vacio devuelve NULL", !heap_ver_max(heap));
    print_test("Desencolar en un heap vacio es NULL", !heap_desencolar(heap));

    heap_destruir(heap, NULL);
}

static void prueba_heap_encolar() {
    printf("---Inicio de prueba de encolar en el heap---\n");

    heap_t* heap = heap_crear(comparar);
    int aux_1 = 10;
    int aux_2 = 100;
    int aux_3 = 50;

    print_test("Encolo el valor aux",heap_encolar(heap,&aux_1));
    print_test("Ver el maximo con el heap devuelve el elemento recien encolado", heap_ver_max(heap) == &aux_1);
    print_test("El heap no se encuentra vacio",!heap_esta_vacio(heap));
    print_test("La cantidad de elementos en el heap es de 1", heap_cantidad(heap) == 1);
    print_test("Encolo el elemento aux_2",heap_encolar(heap,&aux_2));
    print_test("El heap no se encuentra vacio",!heap_esta_vacio(heap));
    print_test("La cantidad de elementos en el heap es de 1", heap_cantidad(heap) == 2);
    print_test("Ver el maximo con el heap devuelve el elemento recien encolado",heap_ver_max(heap) == &aux_2);
    print_test("Encolo el elemento aux_3",heap_encolar(heap,&aux_3));
    print_test("El heap no se encuentra vacio",!heap_esta_vacio(heap));
    print_test("La cantidad de elementos en el heap es de 1", heap_cantidad(heap) == 3);
    print_test("Al encolar un elemento menor, el maximo no cambia", heap_ver_max(heap) == &aux_2);

    heap_destruir(heap, NULL);
}

static void prueba_heap_desencolar(void) {
    printf("---Inicio de prueba de desencolar el heap---\n");

    // Creamos el heap y declaramos las variables
    heap_t* heap = heap_crear(comparar);
    int aux_1 = 10;
    int aux_2 = 100;
    int aux_3 = 50;
    int aux_4 = 20;
    int aux_5 = 5;
    int aux_6 = 130;
    int aux_7 = 15;

    // Encolamos todos los elementos
    heap_encolar(heap, &aux_1);
    heap_encolar(heap, &aux_2);
    heap_encolar(heap, &aux_3);
    heap_encolar(heap, &aux_4);
    heap_encolar(heap, &aux_5);
    heap_encolar(heap, &aux_6);
    heap_encolar(heap, &aux_7);

    // Desencolamos y ejecutamos las pruebas
    print_test("El maximo del heap es 130", heap_ver_max(heap) == &aux_6);
    print_test("Primero desencolo el 130", heap_desencolar(heap) == &aux_6);
    print_test("Segundo desencolo el 100", heap_desencolar(heap) == &aux_2);
    print_test("Tercero desencolo el 50", heap_desencolar(heap) == &aux_3);
    print_test("Cuarto desencolo el 20", heap_desencolar(heap) == &aux_4);
    print_test("Quinto desencolo el 15", heap_desencolar(heap) == &aux_7);
    print_test("El maximo del heap es 10", heap_ver_max(heap) == &aux_1);
    print_test("Sexto desencolo el 10", heap_desencolar(heap) == &aux_1);
    print_test("Septimo desencolo el 5", heap_desencolar(heap) == &aux_5);
    print_test("Desencolar en un heap que quedo vacio devuelve NULL", !heap_desencolar(heap));
    print_test("El maximo en un heap que quedo vacio es NULL", !heap_ver_max(heap));

    // Liberamos la memoria utilizada
    heap_destruir(heap, NULL);
}

static void prueba_heap_destruir_datos() {
    printf("---Inicio de prueba de destruir datos del heap---\n");

    // Creamos el heap y declaramos las variables
    heap_t* heap = heap_crear(comparar);
    int *aux_1 = malloc(sizeof(int));
    int *aux_2 = malloc(sizeof(int));
    int *aux_3 = malloc(sizeof(int));
    int *aux_4 = malloc(sizeof(int));
    int *aux_5 = malloc(sizeof(int));
    int *aux_6 = malloc(sizeof(int));
    int *aux_7 = malloc(sizeof(int));

    *aux_1 = 10;
    *aux_2 = 100;
    *aux_3 = 50;
    *aux_4 = 20;
    *aux_5 = 5;
    *aux_6 = 130;
    *aux_7 = 15;

    // Encolamos todos los elementos
    heap_encolar(heap, aux_1);
    heap_encolar(heap, aux_2);
    heap_encolar(heap, aux_3);
    heap_encolar(heap, aux_4);
    heap_encolar(heap, aux_5);
    heap_encolar(heap, aux_6);
    heap_encolar(heap, aux_7);

    // Ejecutamos la prueba
    heap_destruir(heap, free);
    print_test("El heap se destruyo correctamente incluyendo sus datos", 1); // Se destruyo el heap, si la prueba falla valgrind devolveria el error

}

static void prueba_heap_volumen(int tam) {
    printf("---INICIO DE PRUEBAS DE VOLUMEN---\n");
    heap_t* heap = heap_crear(comparar);
    bool ok = true;
    int* valores = malloc(sizeof(int));

    for(int i = 0; i < tam; i++){
        int* num = malloc(sizeof(int));
        *num = i;
        ok &= heap_encolar(heap,num);
    }

    print_test("Se agregaron todos los elementos correctamente ",ok);
    print_test("La cantidad de elementos en el heap es 0", heap_cantidad(heap) == (size_t)tam);

    ok = true;
    for(int i = 0; i < tam; i++){
        int* desencolado = (int*)heap_desencolar(heap);
        if(!desencolado) ok = false;
        free(desencolado);
    }

    print_test("Se desencolaron todos los elementos correctamente",ok);
    print_test("El heap se encuentra vacio", heap_esta_vacio(heap));
    print_test("La cantidad de elementos en el heap es 0", heap_cantidad(heap) == 0);
    
    heap_destruir(heap, NULL);
    free(valores);
}

static void prueba_heapsort() {
    printf("---INICIO DE PRUEBAS DE HEAPSORT---\n");
    int *arr_random[8];
    int largo = 8;

    int aux_1= 7;
    int aux_2 = 30;
    int aux_3 = 5;
    int aux_4 = 2;
    int aux_5 = 9;
    int aux_6 = 15;
    int aux_7 = 37;
    int aux_8 = 4;

    arr_random[0] = &aux_1;
    arr_random[1] = &aux_2;
    arr_random[2] = &aux_3;
    arr_random[3] = &aux_4;
    arr_random[4] = &aux_5;
    arr_random[5] = &aux_6;
    arr_random[6] = &aux_7;
    arr_random[7] = &aux_8;

    print_test("El arreglo con valores random se creo correctamente",true);

    heap_sort((void**)arr_random,(size_t)largo,comparar);

    bool ok = true;
    for (int i = 0; i < largo - 1; i++){
        if(!(*(int*)arr_random[i] < *(int*)arr_random[i+1])){
            ok = false;
            //break;
        }
    }

    print_test("El arreglo se encuentra ordenado",ok);
}

static void prueba_heap_crear_arr() {
    printf("---Inicio de pruebas de array por parametro---\n");
    void *arr_random[8];
    int largo = 8;

    int aux_1= 4;
    int aux_2 = 2;
    int aux_3 = 7;
    int aux_4 = 10;
    int aux_5 = 4;
    int aux_6 = 3;
    int aux_7 = 8;
    int aux_8 = 5;

    arr_random[0] = &aux_1;
    arr_random[1] = &aux_2;
    arr_random[2] = &aux_3;
    arr_random[3] = &aux_4;
    arr_random[4] = &aux_5;
    arr_random[5] = &aux_6;
    arr_random[6] = &aux_7;
    arr_random[7] = &aux_8;


    heap_t* heap = heap_crear_arr((void**)arr_random,(size_t)largo,comparar);

    print_test("El heap se creo correctamente",heap);
    print_test("El heap  no se encuentra vacio",heap_cantidad(heap) != 0);
    print_test("La cantidad de elementos en el heap es de 8",heap_cantidad(heap) == 8);
    print_test("Ver el maximo del heap devuelve 10",*(int*)heap_ver_max(heap) == 10);

    bool ok = true;
    for (int i = 0; i < largo; i++){
        int num = *(int *)heap_desencolar(heap);
        if(!num) ok = false;
    }

    print_test("Se desencolaron todos los elementos correctamente", ok);

    heap_destruir(heap,NULL);
}

void pruebas_heap_estudiante(void){
    prueba_crear_y_destruir_heap();
    prueba_heap_vacio();
    prueba_heap_encolar();
    prueba_heap_desencolar();
    prueba_heap_destruir_datos();
    prueba_heap_volumen(1000);
    prueba_heapsort();
    prueba_heap_crear_arr();
}

#ifndef CORRECTOR
int main(void) {
    pruebas_heap_estudiante();
    return 0;
}
#endif