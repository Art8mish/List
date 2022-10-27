
#include "../include/list.h"

int ListContextCtor(struct ListContext *list)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    list->nodes = (struct Node *) calloc(LIST_MAX_SIZE, sizeof(struct Node));
    ERROR_CHECK(list->nodes == NULL, ERROR_CALLOC);

    list->capacity = LIST_MAX_SIZE;
    list->size = 0;

    list->head = 1;
    list->tale = 1;
    list->free = 1;

    int initialize_err = InitializeNodes(list);
    ERROR_CHECK(initialize_err, ERROR_INITIALIZE_NODES);

    list->error = 0;

    return SUCCESS;
}

int InitializeNodes(struct ListContext *list)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    list->nodes[0].value = 0;
    list->nodes[0].next  = 0;
    list->nodes[0].prev  = 0;

    for (unsigned int index = 1; index < list->capacity; index++)
    {
        list->nodes[index].value = LIST_POISON_VALUE;
        list->nodes[index].next  = (index + 1) % list->capacity;
        list->nodes[index].prev  = index - 1;
    }


    return SUCCESS;
}

int ListContextDtor(struct ListContext *list)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    ERROR_CHECK(list->nodes == NULL, ERROR_NULL_PTR);
    free(list->nodes);

    list->capacity = 0;
    list->size     = 0;

    list->head     = 0;
    list->tale     = 0;
    list->free     = 0;

    list->error |= CODE_DEAD_LIST;

    return SUCCESS;
}