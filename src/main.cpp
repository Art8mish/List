
#include "../include/list.h"

int main(void)
{
    printf("Leeeet's go!\n\n");

    struct ListContext list = {};

    int ext_err = ListContextCtor(&list); 
    ERROR_CHECK(ext_err, 1);

    DUMPLIST(&list, "Ctoring");

    unsigned int new_node_ptr = 0;
    
    ext_err = ListInsertAfter(&list, 1, 15, &new_node_ptr);
    ERROR_CHECK(ext_err, 2);
    
    ext_err = ListInsertAfter(&list, 1, 16, &new_node_ptr);
    ERROR_CHECK(ext_err, 2);
    
    ext_err = ListInsertBefore(&list, 1, 14, &new_node_ptr);
    ERROR_CHECK(ext_err, 2);
    
    DUMPLIST(&list, "Add few elements");

    for (int i = 2; i < 5; i++)
    {
        ext_err = ListInsertAfter(&list, i, 16+i, &new_node_ptr);
        ERROR_CHECK(ext_err, 3);

        printf("node[%d]_ptr = %d\n", i, new_node_ptr);
    }

    //DUMPLIST(&list);

    list_t arg = 0; 
    ext_err = ListRemove(&list, 5, &arg);
    ERROR_CHECK(ext_err, 3);
    printf("arg = %d\n",  arg);


    ext_err = ListRemove(&list, 1, &arg);
    ERROR_CHECK(ext_err, 3);
    printf("arg = %d\n",  arg);

    DUMPLIST(&list, "After adding and removing elements");

    ext_err = ListInsertAfter(&list, 2, 31, &new_node_ptr);
    ERROR_CHECK(ext_err, 2);

    ext_err = ListInsertBefore(&list, 4, 32, &new_node_ptr);
    ERROR_CHECK(ext_err, 2);

    DUMPLIST(&list, "Insert two more elements");

    ext_err = ListSlowSort(&list);
    ERROR_CHECK(ext_err, 5);

    DUMPLIST(&list, "After sorting");

    ext_err = ListContextDtor(&list);
    ERROR_CHECK(ext_err, 5);

    return SUCCESS;
}