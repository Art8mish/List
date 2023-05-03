
#include "../include/list.h"

#define VERIFICATION()                                                  \
            do                                                          \
            {                                                           \
                int verification_err = listVerification(list);          \
                ERROR_CHECK(verification_err, ERROR_VERIFICATION);      \
            } while(false)

#define FREE_NODE list->nodes[list->free]
#define TAIL_NODE list->nodes[list->nodes[0].prev]
#define HEAD_NODE list->nodes[list->nodes[0].next]
#define CURR_NODE list->nodes[curr_node]
#define FICT_NODE list->nodes[0]

#define INSPECT_LIST(cond, err_code)                \
            do                                      \
            {                                       \
                if (cond)                           \
                    list->error |= err_code;        \
            } while(false)  



int listPushBack(struct List *list, list_elem_t arg,
                 unsigned int *new_node_indx)
{
    ERROR_CHECK(list         == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(new_node_indx == NULL, ERROR_NULL_PTR);

    int list_insert_err = listInsertAfter(list, FICT_NODE.prev, arg, new_node_indx);
    ERROR_CHECK(list_insert_err, ERROR_LIST_INSERT_AFTER);

    return SUCCESS;
}

int listPushFront(struct List *list, list_elem_t arg,
                  unsigned int *new_node_indx)
{
    ERROR_CHECK(list         == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(new_node_indx == NULL, ERROR_NULL_PTR);

    int list_insert_err = listInsertBefore(list, FICT_NODE.next, arg, new_node_indx);
    ERROR_CHECK(list_insert_err, ERROR_LIST_INSERT_BEFORE);

    return SUCCESS;
}

int listPopFront(struct List *list, list_elem_t *arg)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(arg  == NULL, ERROR_NULL_PTR);

    int remove_err = listRemove(list, FICT_NODE.next, arg);
    ERROR_CHECK(remove_err, ERROR_LIST_REMOVE);

    return SUCCESS;
}

int listPopBack(struct List *list, list_elem_t *arg)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(arg  == NULL, ERROR_NULL_PTR);

    int remove_err = listRemove(list, FICT_NODE.prev, arg);
    ERROR_CHECK(remove_err, ERROR_LIST_REMOVE);

    return SUCCESS;
}

int listInsertAfter(struct List *list, unsigned int phys_indx,
                      list_elem_t arg, unsigned int *new_node_indx)
{
    ERROR_CHECK(list          == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(new_node_indx == NULL, ERROR_NULL_PTR);

    VERIFICATION();

    if (list->size == list->capacity - 2)
    {
        int list_resize_err = listResize(list);
        ERROR_CHECK(list_resize_err, ERROR_LIST_RESIZE);
    }

    FREE_NODE.value = arg;
    FREE_NODE.prev  = phys_indx;

    list->nodes[list->nodes[phys_indx].next].prev = list->free;

    unsigned int curr_node = list->free; 
    list->free = FREE_NODE.next;
    FREE_NODE.prev = 0;

    CURR_NODE.next = list->nodes[phys_indx].next;
    list->nodes[phys_indx].next = curr_node;

    *new_node_indx = curr_node;

    list->size += 1;

    if (phys_indx != FICT_NODE.prev - 1)
        list->is_linear = false;

    VERIFICATION();    

    return SUCCESS;
}

int listInsertBefore(struct List *list, unsigned int phys_indx,
                     list_elem_t arg, unsigned int *new_node_indx)
{
    ERROR_CHECK(list          == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(phys_indx     == 0,    ERROR_NULL_INDX);
    ERROR_CHECK(new_node_indx == NULL, ERROR_NULL_PTR);

    VERIFICATION();

    if (list->size == list->capacity - 2)
    {
        int list_resize_err = listResize(list);
        ERROR_CHECK(list_resize_err, ERROR_LIST_RESIZE);
    }

    FREE_NODE.value = arg;
    FREE_NODE.prev  = list->nodes[phys_indx].prev;

    list->nodes[list->nodes[phys_indx].prev].next = list->free;

    unsigned int curr_node = list->free; 
    list->free = FREE_NODE.next;
    FREE_NODE.prev = 0;

    CURR_NODE.next = phys_indx;
    list->nodes[phys_indx].prev = curr_node;

    *new_node_indx = curr_node;

    list->size += 1;
    
    list->is_linear = false;

    int list_resize_err = listResize(list);
    ERROR_CHECK(list_resize_err, ERROR_LIST_RESIZE);

    VERIFICATION();    

    return SUCCESS;
}

int listRemove(struct List *list, unsigned int phys_indx,
               list_elem_t *arg)
{
    ERROR_CHECK(list     == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(phys_indx == 0,    ERROR_NULL_PTR);

    VERIFICATION();

    ERROR_CHECK(list->size == 0, ERROR_INCORRECT_OPERATION);

    *arg = list->nodes[phys_indx].value;
    list->nodes[phys_indx].value = LIST_POISON_VALUE;

    list->nodes[list->nodes[phys_indx].next].prev = list->nodes[phys_indx].prev;
    list->nodes[list->nodes[phys_indx].prev].next = list->nodes[phys_indx].next;

    FREE_NODE.prev = phys_indx;
    list->nodes[phys_indx].next = list->free;
    list->free = phys_indx;
    FREE_NODE.prev = 0;

    list->size -= 1;

    if (list->size == 0)
        list->is_linear = true;
    else if (phys_indx != FICT_NODE.prev)
        list->is_linear = false;

    int list_resize_err = listResize(list);
    ERROR_CHECK(list_resize_err, ERROR_LIST_RESIZE);

    VERIFICATION();
    
    return SUCCESS;
}

int list_Translate_Logical_Position_To_Physical_Adress(struct List *list,
                                                       unsigned int logic_indx, 
                                                       unsigned int *phys_indx)
{
    ERROR_CHECK(list      == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(logic_indx == 0,    ERROR_NULL_INDX);
    ERROR_CHECK(phys_indx  == NULL, ERROR_NULL_PTR);

    VERIFICATION();

    if (list->is_linear)
    {
        *phys_indx = logic_indx;

        return SUCCESS;
    }

    unsigned int curr_node = FICT_NODE.next;
    for (unsigned int index = 1; index < logic_indx; index++)
        curr_node = CURR_NODE.next;

    *phys_indx = curr_node;

    VERIFICATION();

    return SUCCESS;
}

int list_Slowest_Sort(struct List *list)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    if (list->is_linear)
        return SUCCESS;

    list_elem_t *data  = (list_elem_t*) calloc(list->capacity - 1, sizeof(list_elem_t));
    unsigned int curr_node = FICT_NODE.next;
    unsigned int index = 0;

    for (index = 0; index < list->capacity - 1; index++)
    {
        data[index] = list->nodes[curr_node].value;

        if (list->nodes[curr_node].next == 0)
        {
            FICT_NODE.next = 1;
            FICT_NODE.prev = index + 1;
            break;
        }

        curr_node = list->nodes[curr_node].next;
    }

    index += 2;

    //filling list of non-poison values
    for (unsigned int i = 1; i < index; i++)
    {
        list->nodes[i].value = data[i - 1];
        list->nodes[i].next  = i + 1;
        list->nodes[i].prev  = i - 1;
    }
    list->nodes[index - 1].next = 0;
    list->free = index;

    //filling free list (poison)
    for  (unsigned int i = index; i <  list->capacity; i++)
    {
        list->nodes[i].value = LIST_POISON_VALUE;
        list->nodes[i].next  = i + 1;
        list->nodes[i].prev  = i - 1;
    }
    list->nodes[index].prev  = 0;
    list->nodes[list->capacity - 1].next = 0;

    free(data);

    list->is_linear = true;

    return SUCCESS;     
}

int listVerification(struct List *list)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    int list_check_err = listCheckError(list);
    ERROR_CHECK(list_check_err, ERROR_LIST_CHECK_ERROR);
    
    if (list->error != 0)
        LISTDUMP(list, "ERROR IN VERIFICATION");

    return SUCCESS;
}

int listCheckError(struct List *list)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    INSPECT_LIST(list->capacity <= list->size, ERROR_CODE_SIZE_DISCREPANCY);
    INSPECT_LIST(list->capacity == 0,          ERROR_CODE_INCORRECT_CAPACITY);
    INSPECT_LIST(list->free     == 0,          ERROR_CODE_INCORRECT_FREE);
    INSPECT_LIST(list->nodes    == NULL,       ERROR_CODE_NODES_NULL_PTR);
    INSPECT_LIST(FICT_NODE.next == FICT_NODE.prev &&
                 list->size >  1,              ERROR_CODE_HEAD_IS_TAIL);

    return SUCCESS;
}

int listResize(struct List *list)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    VERIFICATION();

    size_t capacity = list->capacity;

    if (list->capacity <= 1 )
        list->capacity = 10;

    else if (list->size >= list->capacity - 2)
        list->capacity *= 2;

    else //if ((list->size + 5) * 2 < (list->capacity - 2))
        return SUCCESS;

    list->nodes = (struct Node*) realloc(list->nodes, list->capacity * sizeof(struct Node));
    ERROR_CHECK(list->nodes == NULL, ERROR_REALLOC);

    list->free = capacity - 1;
    list->nodes[list->free].value = LIST_POISON_VALUE;
    list->nodes[list->free].next  = capacity;
    list->nodes[list->free].prev  = 0;

    for (unsigned int index = capacity; index < list->capacity; index++)
    {
        list->nodes[index].value = LIST_POISON_VALUE;
        list->nodes[index].next  = index + 1;
        list->nodes[index].prev  = index - 1;
    }

    list->nodes[list->capacity - 1].next = 0;

    VERIFICATION();

    return SUCCESS;
}