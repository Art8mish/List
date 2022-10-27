
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

    if (list->size == 0)
    {
        list->head = list->free;
        list->tale = list->free;
    }

    FREE_NODE.value = arg;        

    unsigned int curr_node = list->free;
    list->free = FREE_NODE.next;
    FREE_NODE.prev = 0;

//start changes (regarding pushback)
    TALE_NODE.next  = curr_node;    
    CURR_NODE.prev  = list->tale;   

    list->tale = curr_node;
//end changes

    TALE_NODE.next = 0;
    HEAD_NODE.prev = 0;
    
    list->size += 1;
    *new_node_ptr = curr_node;

    VERIFICATION_AFTER();

    return SUCCESS;
}

int ListPushFront(struct ListContext *list, list_t arg,
                  unsigned int *new_node_ptr)
{
    ERROR_CHECK(list         == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(new_node_ptr == NULL, ERROR_NULL_PTR);

    VERIFICATION_BEFORE();

    if (list->size == 0)
    {
        list->head = list->free;
        list->tale = list->free;
    }

    FREE_NODE.value = arg;        

    unsigned int curr_node = list->free;
    list->free = FREE_NODE.next;
    FREE_NODE.prev = 0;

//start changes (regarding pushback)
    HEAD_NODE.prev  = curr_node;    
    CURR_NODE.next  = list->head;   

    list->head = curr_node;
//end changes

    TALE_NODE.next = 0;
    HEAD_NODE.prev = 0;
    
    list->size += 1;
    *new_node_ptr = curr_node;

    VERIFICATION_AFTER();

    return SUCCESS;
}

int ListPopBack(struct ListContext *list, list_t *arg)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(arg  == NULL, ERROR_NULL_PTR);

    VERIFICATION_BEFORE();

    *arg = TALE_NODE.value;
    TALE_NODE.value = LIST_POISON_VALUE;
    
    FREE_NODE.prev = list->tale;
    TALE_NODE.next = list->free;
    list->free = list->tale;

    if (list->size != 1)
    {
        list->tale = TALE_NODE.prev;
        TALE_NODE.next = 0;
    }

    FREE_NODE.prev = 0;
    
    list->size -= 1;

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

    if (phys_ptr == list->tale)
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

    VERIFICATION_AFTER();    

    return SUCCESS;
}

int ListRemove(struct ListContext *list, unsigned int phys_ptr,
               list_t *arg)
{
    ERROR_CHECK(list     == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(phys_ptr == NULL, ERROR_NULL_PTR);

    VERIFICATION_BEFORE();

    if (phys_ptr == list->tale)
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
    for (unsigned int index = 0; index < logic_ptr; index++)
        curr_node = list->nodes[curr_node].next;

    *phys_ptr = curr_node;

    VERIFICATION_AFTER();

    return SUCCESS;
}