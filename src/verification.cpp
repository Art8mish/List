
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
    INSPECT_LIST(list->tail     <= 0,          ERROR_CODE_INCORRECT_TAIL);
    INSPECT_LIST(list->free     <= 0,          ERROR_CODE_INCORRECT_FREE);
    //INSPECT_LIST(list->head == list->tail &&
    //             list->size >  1,              ERROR_CODE_HEAD_IS_TAIL);
    //INSPECT_LIST(list->head == list->free,     ERROR_CODE_HEAD_IS_FREE);
    //INSPECT_LIST(list->tail == list->free,     ERROR_CODE_TAIL_IS_FREE);
    
    if (list->error != 0)
        DUMPLIST(list, "ERROR IN VERIFICATION");

    return SUCCESS;
}

int DumpList(struct ListContext *list, const char *comment, 
             const char *file_name, int line_num, const char *func_name)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    ERROR_CHECK(file_name == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(func_name == NULL, ERROR_NULL_PTR);

    FILE *dump_f = fopen(HTM_FILE, "a");
    ERROR_CHECK(dump_f == NULL, ERROR_OPENING_FILE);

    fprintf(dump_f, "\nList DUMP at %s(line: %d) at %s\n", file_name, line_num, func_name);
    fprintf(dump_f, "<b>%s</b>\n", comment);
    fprintf(dump_f, "List[%p]\n", list);
    fprintf(dump_f, "{\n");
    fprintf(dump_f, "\tcapacity = %d\n",   list->capacity);
    fprintf(dump_f, "\tsize     = %d\n\n", list->size);

    fprintf(dump_f, "\thead = %d\n",   list->head);
    fprintf(dump_f, "\ttail = %d\n",   list->tail);
    fprintf(dump_f, "\tfree = %d\n\n", list->free);
    
    fprintf(dump_f, "\tis_sorted = %d\n", list->is_sorted);
    fprintf(dump_f, "}\n\n");

    if (list->error != 0)
    {
        fprintf(dump_f, "ERROR in List (%x):\n", list->error);
        PRINT_ERROR(ERROR_CODE_NULL_NODES_PTR);
        PRINT_ERROR(ERROR_CODE_SIZE_DISCREPANCY);
        PRINT_ERROR(ERROR_CODE_INCORRECT_HEAD);
        PRINT_ERROR(ERROR_CODE_INCORRECT_TAIL);
        PRINT_ERROR(ERROR_CODE_INCORRECT_FREE);
        PRINT_ERROR(ERROR_CODE_HEAD_IS_TAIL);
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
            if (i == list->tail)
                fprintf(dump_f, "  <--- TAIL");
            if (i == list->free)
                fprintf(dump_f, "  <--- FREE");   
            fprintf(dump_f, "\n");
        }
        fprintf(dump_f, "    }\n");
    }

    int fclose_err = fclose(dump_f);
    ERROR_CHECK(fclose_err, ERROR_CLOSING_FILE);

    int create_graph_err = CreateGraph(list);
    ERROR_CHECK(create_graph_err, ERROR_CREATE_GRAPH);
    int add_graph_png_err = AddGraphPng();
    ERROR_CHECK(add_graph_png_err, ERROR_ADD_GRAPH);
    graph_counter++;

    fprintf(dump_f, "<hr>\n");

    fprintf(dump_f, "\nEnd of DUMP...\n\n");

    

    return SUCCESS;
}

int CreateGraph(struct ListContext *list)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    FILE *graph_f = fopen(GRAPH_FILE, "w");
    ERROR_CHECK(graph_f == NULL, ERROR_OPENING_FILE);


    fprintf(graph_f, "digraph list{\n\trankdir=LR;\nspline = ortho;\n");
    fprintf(graph_f, "\t%d[shape=record,color=%s,label=\"<f0>%d|data %d|<f1>next %d|<f2>prev %d\"]\n",
                        0, "black", 0, list->nodes[0].value, list->nodes[0].next, list->nodes[0].prev);
    for (unsigned int i = 1; i < list->capacity; i++)
    {
        const char *color = "seagreen";
        const char *fill_color = "darkolivegreen2";
        if (list->nodes[i].value != LIST_POISON_VALUE)
        {
            color = "red3";
            fill_color = "peachpuff";
        }
        fprintf(graph_f, "\t%d[weight = 1000,style=\"filled\",fillcolor=\"%s\"shape=record,color=%s,"
                         "label=\"<f0>adrs %d|data %d|{<f2>prev %d | <f1>next %d}\"]\n",
                         i, fill_color, color, i, list->nodes[i].value, list->nodes[i].prev, list->nodes[i].next);
        if (list->nodes[i].next != 0)
            fprintf(graph_f, "\t%d:<f1>->%d:<f2>[style=\"dashed\"]\n", i, list->nodes[i].next);
        if (list->nodes[i].prev != 0)  {}
            //fprintf(graph_f, "\t%d:<f2>->%d:<f1>[style=\"dashed\"]\n", i, list->nodes[i].prev);
    }

    //for (unsigned int i = 1; i < list->capacity; i++)
      //  fprintf(graph_f, "\t{rank=same;%d;}", i);

    fprintf(graph_f, "\nHEAD[style=\"filled\",fillcolor=\"skyblue\",label=\"HEAD = %d\"]", list->head);
    fprintf(graph_f, "\tHEAD->%d:<f0>{rank = same; %d; HEAD;}\n", list->head, list->head);

    fprintf(graph_f, "\nTAIL[style=\"filled\",fillcolor=\"skyblue\",label=\"TAIL = %d\"]", list->tail);
    fprintf(graph_f, "\tTAIL->%d:<f0> {rank = same; %d; TAIL;}; \n", list->tail, list->tail);

    fprintf(graph_f, "\nFREE[style=\"filled\",fillcolor=\"skyblue\",label=\"FREE = %d\"]", list->free);
    fprintf(graph_f, "\tFREE->%d:<f0>{rank = same; %d; FREE;};\n", list->free, list->free);

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

    sprintf(command, "%s%d%s",command, graph_counter, com_part);

    system(command);

    return SUCCESS;
}

int AddGraphPng(void)
{
    FILE *htm_f = fopen(HTM_FILE, "a");
    ERROR_CHECK(htm_f == NULL, ERROR_OPENING_FILE);

    //fprintf(htm_f, "<pre>\n");
    
    fprintf(htm_f, "<img width=900 src=\"../images/graph%d.png\"><br><hr>\n", graph_counter);
    
    int fclose_err = fclose(htm_f);
    ERROR_CHECK(fclose_err, ERROR_CLOSING_FILE);

    return SUCCESS;
}   