
#include "../include/list.h"

#define VERIFICATION_BEFORE()                                           \
            int verification_err = ListVerification(list);              \
            ERROR_CHECK(verification_err, ERROR_VERIFICATION)          

#define VERIFICATION_AFTER()                                            \
            verification_err = ListVerification(list);                  \
            ERROR_CHECK(verification_err, ERROR_VERIFICATION);

int ListPushBack(struct ListContext *list, list_t arg,
                 unsigned int *new_node_ptr)
{
    ERROR_CHECK(list         == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(new_node_ptr == NULL, ERROR_NULL_PTR);

    VERIFICATION_BEFORE();

    FREE_NODE.value = arg;        

    unsigned int curr_node = list->free;
    list->free = FREE_NODE.next;
    FREE_NODE.prev = 0;

    TAIL_NODE.next  = curr_node;    
    CURR_NODE.prev  = list->tail;   

    list->tail = curr_node;

    TAIL_NODE.next = 0;
    HEAD_NODE.prev = 0;
    
    list->size += 1;
    *new_node_ptr = curr_node;

    list->is_sorted = false;

    VERIFICATION_AFTER();

    return SUCCESS;
}

int ListPushFront(struct ListContext *list, list_t arg,
                  unsigned int *new_node_ptr)
{
    ERROR_CHECK(list         == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(new_node_ptr == NULL, ERROR_NULL_PTR);

    VERIFICATION_BEFORE();

    FREE_NODE.value = arg;        

    unsigned int curr_node = list->free;
    list->free = FREE_NODE.next;
    FREE_NODE.prev = 0;

    HEAD_NODE.prev  = curr_node;    
    CURR_NODE.next  = list->head;   

    list->head = curr_node;

    TAIL_NODE.next = 0;
    HEAD_NODE.prev = 0;
    
    list->size += 1;
    *new_node_ptr = curr_node;

    list->is_sorted = false;

    VERIFICATION_AFTER();

    return SUCCESS;
}

int ListPopBack(struct ListContext *list, list_t *arg)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(arg  == NULL, ERROR_NULL_PTR);

    VERIFICATION_BEFORE();

    *arg = TAIL_NODE.value;
    TAIL_NODE.value = LIST_POISON_VALUE;
    
    FREE_NODE.prev = list->tail;
    TAIL_NODE.next = list->free;
    list->free = list->tail;

    if (list->size != 1)
    {
        list->tail = TAIL_NODE.prev;
        TAIL_NODE.next = 0;
    }

    FREE_NODE.prev = 0;
    
    list->size -= 1;

    if (list->size == 0)
        list->is_sorted = true;
    else
        list->is_sorted = false;

    VERIFICATION_AFTER();

    return SUCCESS;
}

int ListPopFront(struct ListContext *list, list_t *arg)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(arg  == NULL, ERROR_NULL_PTR);

    VERIFICATION_BEFORE();
    
    *arg = HEAD_NODE.value;
    HEAD_NODE.value = LIST_POISON_VALUE;        

    FREE_NODE.prev = list->head;

    unsigned int curr_node = list->head;

    if (list->size != 1)
    {
        list->head = HEAD_NODE.next;
        HEAD_NODE.prev = 0;
    }
    
    CURR_NODE.next = list->free;
    list->free = curr_node;
    FREE_NODE.prev = 0;    
    
    list->size -= 1;

    if (list->size == 0)
        list->is_sorted = true;
    else
        list->is_sorted = false;
    
    VERIFICATION_AFTER();

    return SUCCESS;
}

int ListInsertAfter(struct ListContext *list, unsigned int phys_ptr,
                    list_t arg, unsigned int *new_node_ptr)
{
    ERROR_CHECK(list         == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(phys_ptr     == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(new_node_ptr == NULL, ERROR_NULL_PTR);

    VERIFICATION_BEFORE();

    if (phys_ptr == list->tail)
    {
        int push_err = ListPushBack(list, arg, new_node_ptr);
        ERROR_CHECK(push_err, ERROR_PUSH_BACK);
        
        return SUCCESS;
    }

    FREE_NODE.value = arg;
    FREE_NODE.prev  = phys_ptr;

    list->nodes[list->nodes[phys_ptr].next].prev = list->free;

    unsigned int curr_node = list->free; 
    list->free = FREE_NODE.next;
    FREE_NODE.prev = 0;

    CURR_NODE.next = list->nodes[phys_ptr].next;
    list->nodes[phys_ptr].next = curr_node;

    *new_node_ptr = curr_node;

    list->size += 1;

    list->is_sorted = false;

    VERIFICATION_AFTER();    

    return SUCCESS;
}

int ListInsertBefore(struct ListContext *list, unsigned int phys_ptr,
                     list_t arg, unsigned int *new_node_ptr)
{
    ERROR_CHECK(list         == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(phys_ptr     == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(new_node_ptr == NULL, ERROR_NULL_PTR);

    VERIFICATION_BEFORE();

    if (phys_ptr == list->head)
    {
        int push_err = ListPushFront(list, arg, new_node_ptr);
        ERROR_CHECK(push_err, ERROR_PUSH_FRONT);

        return SUCCESS;
    }

    FREE_NODE.value = arg;
    FREE_NODE.prev  = list->nodes[phys_ptr].prev;

    list->nodes[list->nodes[phys_ptr].prev].next = list->free;

    unsigned int curr_node = list->free; 
    list->free = FREE_NODE.next;
    FREE_NODE.prev = 0;

    CURR_NODE.next = phys_ptr;
    list->nodes[phys_ptr].prev = curr_node;

    *new_node_ptr = curr_node;

    list->size += 1;

    list->is_sorted = false;

    VERIFICATION_AFTER();    

    return SUCCESS;
}

int ListRemove(struct ListContext *list, unsigned int phys_ptr,
               list_t *arg)
{
    ERROR_CHECK(list     == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(phys_ptr == NULL, ERROR_NULL_PTR);

    VERIFICATION_BEFORE();

    if (phys_ptr == list->tail)
    {
        int pop_err = ListPopBack(list, arg);
        ERROR_CHECK(pop_err, ERROR_POP_BACK);
        
        return SUCCESS;
    }

    if (phys_ptr == list->head)
    {
        int pop_err = ListPopFront(list, arg);
        ERROR_CHECK(pop_err, ERROR_POP_FRONT);
        
        return SUCCESS;
    }

    *arg = list->nodes[phys_ptr].value;
    list->nodes[phys_ptr].value = LIST_POISON_VALUE;

    list->nodes[list->nodes[phys_ptr].next].prev = list->nodes[phys_ptr].prev;
    list->nodes[list->nodes[phys_ptr].prev].next = list->nodes[phys_ptr].next;

    FREE_NODE.prev = phys_ptr;
    list->nodes[phys_ptr].next = list->free;
    list->free = phys_ptr;
    FREE_NODE.prev = 0;

    list->size -= 1;

    if (list->size == 0)
        list->is_sorted = true;
    else
        list->is_sorted = false;

    VERIFICATION_AFTER();
    
    return SUCCESS;
}

int List_Translate_Logical_Position_To_Physical_Adress(struct ListContext *list,
                                                       unsigned int logic_ptr, 
                                                       unsigned int *phys_ptr)
{
    ERROR_CHECK(list      == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(logic_ptr == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(phys_ptr  == NULL, ERROR_NULL_PTR);

    VERIFICATION_BEFORE();

    unsigned int curr_node = list->head;
    for (unsigned int index = 1; index < logic_ptr; index++)
        curr_node = list->nodes[curr_node].next;

    *phys_ptr = curr_node;

    VERIFICATION_AFTER();

    return SUCCESS;
}

int ListSlowSort(struct ListContext *list)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    if (list->is_sorted)
        return SUCCESS;

    list_t *data  = (list_t*) calloc(list->capacity - 1, sizeof(list_t));
    unsigned int curr_node = list->head;
    unsigned int index = 0;

    for (index = 0; index < list->capacity - 1; index++)
    {
        data[index] = list->nodes[curr_node].value;

        if (list->nodes[curr_node].next == 0)
        {
            list->head = 1;
            list->tail = index + 1;
            break;
        }

        curr_node = list->nodes[curr_node].next;
    }

    index += 2;
    
    for (unsigned int i = 1; i < index; i++)
    {
        list->nodes[i].value = data[i - 1];
        list->nodes[i].next  = (i + 1) % index;
        list->nodes[i].prev  = i - 1;
    }

    list->free = index;
    list->nodes[index].value = LIST_POISON_VALUE;
    list->nodes[index].next  = (index + 1) % list->capacity;
    list->nodes[index].prev  = 0;

    for  (int i = index + 1; i <  list->capacity; i++)
    {
        list->nodes[i].value = LIST_POISON_VALUE;
        list->nodes[i].next  = (i + 1) % list->capacity;
        list->nodes[i].prev  = i - 1;
    }

    free(data);

    return SUCCESS;     
}