
#include "../include/list.h"

int main(void)
{
    printf("Leeeet's go!\n\n");

    struct ListContext list = {};

    int ext_err = ListContextCtor(&list); 
    ERROR_CHECK(ext_err, 1);

    DUMPLIST(&list);

    unsigned int new_node_ptr = 0;
    
    ext_err = ListInsertAfter(&list, 1, 15, &new_node_ptr);
    ERROR_CHECK(ext_err, 2);
    DUMPLIST(&list);
    
    ext_err = ListInsertAfter(&list, 1, 16, &new_node_ptr);
    ERROR_CHECK(ext_err, 2);
    DUMPLIST(&list);
    
    ext_err = ListInsertBefore(&list, 1, 14, &new_node_ptr);
    ERROR_CHECK(ext_err, 2);
    DUMPLIST(&list);

    for (int i = 2; i < 5; i++)
    {
        ext_err = ListInsertAfter(&list, i, 16+i, &new_node_ptr);
        ERROR_CHECK(ext_err, 3);

        printf("node[%d]_ptr = %d\n", i, new_node_ptr);

        DUMPLIST(&list);
    }

    ext_err = CreateGraph(&list);
    ERROR_CHECK(ext_err, 4);

    list_t arg = 0; 
    ext_err = ListRemove(&list, 5, &arg);
    ERROR_CHECK(ext_err, 3);
    printf("arg = %d\n",  arg);
    DUMPLIST(&list);

    ext_err = ListRemove(&list, 1, &arg);
    ERROR_CHECK(ext_err, 3);
    printf("arg = %d\n",  arg);
    DUMPLIST(&list);

    ext_err = ListRemove(&list, 6, &arg);
    ERROR_CHECK(ext_err, 3);
    printf("arg = %d\n",  arg);
    DUMPLIST(&list);

    ext_err = CreateGraph(&list);
    ERROR_CHECK(ext_err, 4);

    ext_err = ListContextDtor(&list);
    ERROR_CHECK(ext_err, 5);

    CreateHtmFile();

    return SUCCESS;
}