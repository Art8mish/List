
#include "../include/list.h"

int main(void)
{
    struct List list = {};
    
    int ext_err = listCtor(&list, 15); 
    ERROR_CHECK(ext_err, 1);

    LISTDUMP(&list, "Ctoring");

    unsigned int new_node_indx = 0;
    
    ext_err = listPushBack(&list, 15, &new_node_indx);
    ERROR_CHECK(ext_err, 2);

    ext_err = listPushBack(&list, 16, &new_node_indx);
    ERROR_CHECK(ext_err, 2);

    LISTDUMP(&list, "Add few elements");
    
    ext_err = listPushFront(&list, 14, &new_node_indx);
    ERROR_CHECK(ext_err, 2);

    LISTDUMP(&list, "Add few elements");

    for (int i = 2; i < 5; i++)
    {
        ext_err = listInsertAfter(&list, i, 16+i, &new_node_indx);
        ERROR_CHECK(ext_err, 3);
    }

    LISTDUMP(&list, "After adding elements");

    //LISTDUMP(&list); 
    list_elem_t arg = 0; 
    ext_err = listRemove(&list, 5, &arg);
    ERROR_CHECK(ext_err, 3);

     LISTDUMP(&list, "first remove");


    ext_err = listRemove(&list, 1, &arg);
    ERROR_CHECK(ext_err, 3);

    LISTDUMP(&list, "first remove");

    ext_err = listInsertAfter(&list, 2, 31, &new_node_indx);
    ERROR_CHECK(ext_err, 2);

    ext_err = listInsertBefore(&list, 4, 32, &new_node_indx);
    ERROR_CHECK(ext_err, 2);

    LISTDUMP(&list, "Insert two more elements");

    ext_err = list_Slowest_Sort(&list);
    ERROR_CHECK(ext_err, 5);

    LISTDUMP(&list, "After linearizing");

    ext_err = listDtor(&list);
    ERROR_CHECK(ext_err, 5);

    printf("SUCCESSFUL END.\n");

    return SUCCESS;
}