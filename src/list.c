#include "list.h"
#include <malloc.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>

// Init list structure
int init_list(int_ll_t *list)
{
    //inicializamos la cabeza y el size de la lista
    list->head = NULL;
    list->size = 0;
    //inicializamos el lock
    pthread_mutex_init(&list->lock, NULL);
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    //bloqueamos la lista
    pthread_mutex_lock(&list->lock);
    //si la lista es vacia
    if(list->size == 0){
        //le hacemos free al puntero de la lista
        free(list);
        //desbloqueamos y destruimos
        pthread_mutex_unlock(&list->lock);
        pthread_mutex_destroy(&list->lock);
        return 0;
    }
    //si la lista no es vacia
    pos_t* aux = list->head;
    //la recorremos
    while (aux != NULL){
        pos_t* aux2 = aux->next;
        //y vamos liberando todas las posiciones
        free(aux);
        aux = aux2;
    }
    //despues de liberados todos los nodos, liberamos el puntero hacia la lista
    free(list);
    //desbloqueamos y destruimos el lock
    pthread_mutex_unlock(&list->lock);
    pthread_mutex_destroy(&list->lock);
    return 0;
    
}

// Get list size
int size_list(int_ll_t *list)
{ 
    //bloqueamos
    pthread_mutex_lock(&list->lock);
    //guardamos el size
    int size = list->size;
    //desbloqueamos
    pthread_mutex_unlock(&list->lock);
    //devolvemos el valor del size
    return size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    //bloqueamos
    pthread_mutex_lock(&list->lock);
    //si la lista es vacia devolvemos 0
    if(list->size == 0){
        //desbloqueamos
        pthread_mutex_unlock(&list->lock);
        return 1; 
    } 
    //hacemos las comprobaciones pertinentes con respecto al indice
    int aux_index = index;
    if(index < 0){
        aux_index = 0;
    }
    if(index >= list->size){
        aux_index = list->size -1;
    }
    //creamos una posicion para comparar por el indice
    pos_t* aux = list->head;
    int count = 0;
    while (aux != NULL){
        if(count == aux_index){
            //si lo encontramos guardamos su valor y retornamos 0
            *out_value = aux->value;
            //desbloqueamos
            pthread_mutex_unlock(&list->lock);
            return 0;
        }
        count ++;
        aux = aux->next;
    }
    //desbloqueamos
    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    //bloqueamos
    pthread_mutex_lock(&list->lock);
    //si la lista es vacia
    if(list->size == 0){
        //actualizamos su size a 1
        list->size = 1;
        list->head = (pos_t*)malloc(sizeof(pos_t));
        //creamos una posicion y le colocamos su valor, el prev y el next
        list->head->value = value;
        list->head->prev = NULL;
        list->head->next = NULL;
        //desbloqueamos
        pthread_mutex_unlock(&list->lock);
        return 0;
    }

    //si la lista no es vacia, hacemos las comprobaciones pertinentes conr especto al index
    int aux_ind = index;
    if(index < 0) aux_ind = 0;
    if(index >= list->size) aux_ind = list->size-1;

    //creamos una posicion auxiliar para recorrer la lista
    pos_t* aux = list->head;
    int count = 0;
    while (aux != NULL){
        //cuadno encontramos la posicion del index
        if(count == aux_ind){
            //creamos la posicion a insertar
            pos_t* to_insert = (pos_t*)malloc(sizeof(pos_t));
            //le colocamos el valor
            to_insert->value = value;
            //actualizamos su prev y next
            to_insert->prev = aux->prev;
            //actualizamos el next del prev de la posicion que insertamos
            if(to_insert->prev != NULL) (to_insert->prev)->next = to_insert;\
            to_insert->next = aux;
            //actualizamos el prev del next de la posicion que insertamos
            aux->prev = to_insert;
            //actualizamos el tamanno de la lista
            list->size = list->size+ 1;
            //si insertamos en la posicion 0 actualizamos la primera posicion
            if(count == 0) list->head = to_insert;
            //desbloqueamos
            pthread_mutex_unlock(&list->lock);
            return 0;
        }
        count ++;
        aux = aux->next;
    }
    //desbloqueamos
    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    //bloqueamos
    pthread_mutex_lock(&list->lock);
    //si la lista es vacia devolvemos 0
    if(list->size == 0){
        //desbloqueamos
        pthread_mutex_unlock(&list->lock);
        return 1; 
    } 
    //hacemos las comprobaciones pertinentes con respecto al indice
    int aux_ind = index;
    if(index < 0) aux_ind = 0;
    if(index >= list->size) aux_ind = list->size-1;

    //creamos una posicion axiliar para buscar la posicion a eliminar
    pos_t* aux = list->head;
    int count = 0;
    while (aux != NULL){
        //cuando encontramos la posicion
        if(count == aux_ind){
            //actualizamos el prev del nodo a eliminar
            if(aux->prev != NULL) (aux->prev)->next = aux->next;
            //y el next
            if(aux->next != NULL) (aux->next)->prev = aux->prev;
            if(aux == list->head) list->head = aux->next;
            //guardamos el valor eliminado
            *out_value = aux->value;
            //liberamos la posicion
            free(aux);
            //actualizamos el size de la lista
            list->size = list->size- 1;
            //desbloqueamos
            pthread_mutex_unlock(&list->lock);
            return 0;
        }
        count ++;
        aux = aux->next;
    }
    //desbloqueamos
    pthread_mutex_unlock(&list->lock);
    return 0;
}

