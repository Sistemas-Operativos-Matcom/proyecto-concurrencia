#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include "list.h"

void init(sem_t *sem){
  if (sem_init(sem, 0, 1)!=0) 
    printf("Error inicializando semáforo\n");
}

void lock(sem_t *sem) {
  if(sem_wait(sem)!=0)
    printf("Error semáforo\n");
}

void unlock(sem_t *sem) {
  if (sem_post(sem)!=0)
    printf("Error semáforo\n");
}

// Init list structure
int init_list(int_ll_t *list)
{   
    //inicialización estructura de la lista
    list->head=NULL;
    list->size=0;

    //inicialización de los semáforos
    list->sem = (sem_t *)malloc(sizeof(sem_t));  
    init(list->sem); 
    
    //list->sem_empty = (sem_t *)malloc(sizeof(sem_t));
    //init(list->sem_empty);
 
    //bloqueo del semáforo de acceso a la lista vacía
    //lock(list->sem_empty);
   
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{   int_ll_node_t *temp;

    //recorremos la lista liberando los nodos
    temp = list->head;
    while (temp != NULL){
      list->head = temp->next;
      free(temp);
      temp = list->head;
    }
     
    free(list->sem);//liberamos el semáforo
    //free(list->sem_empty);//liberamos el semáforo
    free(list);//liberamos la estructura de la lista
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{   int size=0;
    
    //bloqueamos el semáforo de acceso a la estructura de la lista
    lock(list->sem);
   
    size = list->size; //guardamos en variable size el tamaño de la lista

    //desbloqueamos el semáforo de acceso a la estructura de la lista
    unlock(list->sem);

    return size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{   int_ll_node_t *temp = NULL;

    //bloqueamos el semáforo de la lista vacía
    //lock(list->sem_empty);
    
    //bloqueamos el semáforo de acceso a la estructura de la lista
    lock(list->sem);

    if (list->size==0){
      //desbloqueamos el semáforo de acceso a la estructura de la lista
      unlock(list->sem);
      return 1;
    }
    //transformamos el valor del indice al intervalo de 0 a size-1
    index = (index < 0) ? 0 : (index > list->size-1) ? list->size-1 : index;      
    //recorremos la lista buscando el nodo de indice index
    temp = list->head;
    for(int i = 0;i < index;i++)
      temp = temp->next;
 
    *out_value = temp->value; //guardamos en out_value el valor del nodo encontrado

    //desbloqueamos el semáforo de lista vacía
    //unlock(list->sem_empty);
    
    //desbloqueamos el semáforo de acceso a la estructura de la lista
    unlock(list->sem);
    
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{   int_ll_node_t *temp=NULL;
    int_ll_node_t *curr=NULL;

    //bloqueamos el semáforo de acceso a la estructura de la lista
    lock(list->sem);

    //reservamos espacio para el nuevo nodo
    while((temp = (int_ll_node_t *)malloc(sizeof(int_ll_node_t))) == NULL);

    //guardamos el valor del nodo
    temp->value=value;
    temp->next=NULL;

    //transformamos el valor del indice al intervalo de 0 a size para adicionarlo al final de la lista
    index = (index < 0) ? 0 : (index > list->size) ? list->size : index;

    //si el indice es 0, se adiciona al comienzo de la lista
    if (index == 0){
      temp->next=list->head;
      list->head=temp;
    }
    else {//si no, recorremos la lista hasta llegar al nodo anterior al indice
      curr = list->head;
      for(int i=0;i<index-1;i++)
        curr = curr->next;
      temp->next=curr->next;//insertamos detras del nodo anterior el nuevo nodo
      curr->next=temp;
    }
    
    //incrementamos el tamaño de la lista
    list->size++;
    
    //desbloqueamos el semáforo de acceso a la estructura de la lista
    unlock(list->sem);
    
    //desbloqueamos el semáforo de lista vacía
    //unlock(list->sem_empty);
    
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{   int_ll_node_t *temp=NULL;
    int_ll_node_t *curr=NULL;

    //bloqueamos el semáforo de la lista vacía
    //lock(list->sem_empty);
    
    //bloqueamos el semáforo de acceso a la estructura de la lista
    lock(list->sem);
    
    if (list->size==0){
      //desbloqueamos el semáforo de acceso a la estructura de la lista
      unlock(list->sem);
      return 1;
    }
    //transformamos el valor del indice al intervalo de 0 a size-1 
    index = (index < 0) ? 0 : (index > list->size-1) ? list->size-1 : index;

    //si el indice es 0, se elimina al comienzo de la lista
    if (index == 0) {
      temp = list->head;
      list->head = temp->next;
    }
    else {//si no, recorremos la lista hasta llegar al nodo anterior al indice
      curr = list->head;
      for(int i=0;i<index-1;i++)
          curr = curr->next;
      temp = curr->next;//eliminamos el siguiente nodo
      curr->next = temp->next;
    }
 
    *out_value = temp->value;//guardamos en out_value el valor del nodo encontrado
    //decrementamos el tamaño de la lista
    list->size--;
   
    free(temp);//liberamos el espacio del nodo
    if (list->size == 0)//si la lista esta vacía, ponemos a NULL
      list->head = NULL;
        
    //desbloqueamos el semáforo de acceso a la estructura de la lista
    unlock(list->sem);
        
    return 0;
}
