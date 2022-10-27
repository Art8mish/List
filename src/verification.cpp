
#include "../include/list.h"

#define INSPECT_LIST(cond, err_code)                \
            if (cond)                               \
                list->error |= err_code;            \

#define PRINT_ERROR(err)                            \
            if (list->error & err)                  \
                printf("%s\n", #err)

int graph_counter = 1;

int ListVerification(struct ListContext *list)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    INSPECT_LIST(list->nodes    == NULL,       ERROR_CODE_NULL_NODES_PTR);
    INSPECT_LIST(list->capacity <= list->size, ERROR_CODE_SIZE_DISCREPANCY);
    INSPECT_LIST(list->head     <= 0,          ERROR_CODE_INCORRECT_HEAD);
    INSPECT_LIST(list->tale     <= 0,          ERROR_CODE_INCORRECT_TALE);
    INSPECT_LIST(list->free     <= 0,          ERROR_CODE_INCORRECT_FREE);
    //INSPECT_LIST(list->head == list->tale &&
    //             list->size >  1,              ERROR_CODE_HEAD_IS_TALE);
    //INSPECT_LIST(list->head == list->free,     ERROR_CODE_HEAD_IS_FREE);
    //INSPECT_LIST(list->tale == list->free,     ERROR_CODE_TALE_IS_FREE);
    
    if (list->error != 0)
        DUMPLIST(list);

    return SUCCESS;
}

int DumpList(struct ListContext *list, const char *file_name,
             int line_num, const char *func_name)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    ERROR_CHECK(file_name == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(func_name == NULL, ERROR_NULL_PTR);

    FILE *dump_f = fopen(DUMP_FILE, "a");
    ERROR_CHECK(dump_f == NULL, ERROR_OPENING_FILE);

    fprintf(dump_f, "\nList DUMP at %s(line: %d) at %s\n", file_name, line_num, func_name);
    fprintf(dump_f, "List[%p]\n", list);
    fprintf(dump_f, "{\n");
    fprintf(dump_f, "    capacity = %d\n",   list->capacity);
    fprintf(dump_f, "    size     = %d\n\n", list->size);

    fprintf(dump_f, "    head = %d\n",   list->head);
    fprintf(dump_f, "    tale = %d\n",   list->tale);
    fprintf(dump_f, "    free = %d\n\n", list->free);
    fprintf(dump_f, "}\n\n");

    if (list->error != 0)
    {
        fprintf(dump_f, "ERROR in List (%x):\n", list->error);
        PRINT_ERROR(ERROR_CODE_NULL_NODES_PTR);
        PRINT_ERROR(ERROR_CODE_SIZE_DISCREPANCY);
        PRINT_ERROR(ERROR_CODE_INCORRECT_HEAD);
        PRINT_ERROR(ERROR_CODE_INCORRECT_TALE);
        PRINT_ERROR(ERROR_CODE_INCORRECT_FREE);
        PRINT_ERROR(ERROR_CODE_HEAD_IS_TALE);
    }

    else
    {
        fprintf(dump_f, "    Nodes[%p]\n", list->nodes);
        fprintf(dump_f, "    {\n");
        fprintf(dump_f, "        phys_adr  value     next     prev\n");
        for (unsigned int i = 0; i < list->capacity; i++)
        {
            fprintf(dump_f, "        [%d]       " LIST_SPECIFIER "     %-4d     %-4d", i,
                list->nodes[i].value, list->nodes[i].next,
                list->nodes[i].prev);
            if (i == list->head)
                fprintf(dump_f, "  <--- HEAD");
            if (i == list->tale)
                fprintf(dump_f, "  <--- TALE");
            if (i == list->free)
                fprintf(dump_f, "  <--- FREE");   
            fprintf(dump_f, "\n");
        }
        fprintf(dump_f, "    }\n");
    }

    fprintf(dump_f, "\nEnd of DUMP...\n\n");

    int fclose_err = fclose(dump_f);
    ERROR_CHECK(fclose_err, ERROR_CLOSING_FILE);

    return SUCCESS;
}

int CreateGraph(struct ListContext *list)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    FILE *graph_f = fopen(GRAPH_FILE, "w");
    ERROR_CHECK(graph_f == NULL, ERROR_OPENING_FILE);


    fprintf(graph_f, "digraph list{\n\trankdir=LR;\n");
    for (unsigned int i = 0; i < list->capacity; i++)
    {
        const char *color = "red";
        if (list->nodes[i].value != LIST_POISON_VALUE)
            color = "green";
        if(i == 0)
            color = "black";
        fprintf(graph_f, "\t%d[shape=record,color=%s,label=\"<f0>%d|value %d|<f1>next %d|<f2>prev %d\"]\n",
                         i, color, i, list->nodes[i].value, list->nodes[i].next, list->nodes[i].prev);
        fprintf(graph_f, "\t%d:<f1>->%d:<f0>[style=\"dashed\"]\n", i, list->nodes[i].next);
        fprintf(graph_f, "\t%d:<f2>->%d:<f0>[style=\"dashed\"]\n", i, list->nodes[i].prev);
    }
    fprintf(graph_f, "\n\tHEAD[shape=\"hexagon\",label=\"HEAD = %d\",]", list->head);
    fprintf(graph_f, "\tHEAD->%d:<f0>\n", list->head);
    fprintf(graph_f, "\n\tTALE[shape=\"hexagon\",label=\"TALE = %d\",]", list->tale);
    fprintf(graph_f, "\tTALE->%d:<f0>\n", list->tale);
    fprintf(graph_f, "\n\tFREE[shape=\"hexagon\",label=\"FREE = %d\",]", list->free);
    fprintf(graph_f, "\tFREE->%d:<f0>\n", list->free);
    fprintf(graph_f, "}\n");

    int fclose_err = fclose(graph_f);
    ERROR_CHECK(fclose_err, ERROR_CLOSING_FILE);

    SaveGraphPng();

    return SUCCESS;
}

int SaveGraphPng(void)
{
    char command[100] = "dot graph.gv -Tpng -o images/graph";
    char com_part[] = ".png";

    sprintf(command, "%s%d%s",command, graph_counter++, com_part);

    system(command);

    return SUCCESS;
}

int CreateHtmFile(void)
{
    FILE *htm_f = fopen(HTM_FILE, "w");
    ERROR_CHECK(htm_f == NULL, ERROR_OPENING_FILE);

    fprintf(htm_f, "<pre>\n");
    fprintf(htm_f, "<font size=20 color=green>Всем привет, "
                   "меня зовут <font color=red>ВЛАД ШИЛО</font>,"
                   "я люблю физику и ебаться в рот!))</font>\n"
                   "<img width=600 src=\"../images/vlad.png\"><br>\n");
    for (int i = 1; i < graph_counter; i++)
    {
        fprintf(htm_f, "<img width=1300 src=\"../images/graph%d.png\"><br>\n", i);
    }

    int fclose_err = fclose(htm_f);
    ERROR_CHECK(fclose_err, ERROR_CLOSING_FILE);

    return SUCCESS;
}   