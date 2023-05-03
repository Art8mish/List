
#include "../include/list.h"

static int InitializeNodes(struct List *list)
{

    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    list->nodes[0].value = 0;
    list->nodes[0].next  = 0;
    list->nodes[0].prev  = 0;

    for (unsigned int index = 1; index < list->capacity; index++)
    {
        list->nodes[index].value = LIST_POISON_VALUE;
        list->nodes[index].next  = index + 1;
        list->nodes[index].prev  = index - 1;
    }

    list->nodes[list->capacity - 1].next = 0;

    return SUCCESS;
}

int listCtor(struct List *list, size_t list_size)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    list->nodes = (struct Node *) calloc(list_size, sizeof(struct Node));
    ERROR_CHECK(list->nodes == NULL, ERROR_CALLOC);

    list->capacity = list_size;
    list->size = 0;

    list->free = 1;

    int initialize_err = InitializeNodes(list);
    ERROR_CHECK(initialize_err, ERROR_INITIALIZE_NODES);

    list->error = 0;

    int verification_err = listVerification(list);         
    ERROR_CHECK(verification_err, ERROR_VERIFICATION);

    return SUCCESS;
}

int listDtor(struct List *list)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    int verify_err = listVerification(list);
    ERROR_CHECK(verify_err, ERROR_VERIFICATION);

    ERROR_CHECK(list->nodes == NULL, ERROR_NULL_PTR);
    free(list->nodes);
    list->nodes = NULL;

    list->capacity = 0;
    list->size     = 0;
    
    list->free     = 0;

    list->error |= CODE_DEAD_LIST;

    return SUCCESS;
}