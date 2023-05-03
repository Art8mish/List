
#include "../include/list.h"

#define PRINT_ERROR(err)                            \
            do                                      \
            {                                       \
                if (list->error & err)              \
                    printf("%s\n", #err);           \
            } while(false)

int graph_counter = 1;

int listDump(const struct List *list, const char *comment, 
             const char *file_name, int line_num, const char *func_name)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    ERROR_CHECK(file_name == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(func_name == NULL, ERROR_NULL_PTR);

    FILE *dump_f = fopen(HTM_FILE, "a");
    ERROR_CHECK(dump_f == NULL, ERROR_OPENING_FILE);

    fprintf(dump_f, "<b><h2>%s</h2></b>\n", comment);
    fprintf(dump_f, "\nList DUMP at %s(line: %d) at %s\n", file_name, line_num, func_name);
    fprintf(dump_f, "List[%p]\n", list);
    fprintf(dump_f, "{\n");
    fprintf(dump_f, "\tcapacity = %d\n",   list->capacity);
    fprintf(dump_f, "\tsize     = %d\n\n", list->size);

    fprintf(dump_f, "\thead = %d\n",   list->nodes[0].next);
    fprintf(dump_f, "\ttail = %d\n",   list->nodes[0].prev);
    fprintf(dump_f, "\tfree = %d\n\n", list->free);

    fprintf(dump_f, "\tis_sorted = %d\n", list->is_linear);
    fprintf(dump_f, "}\n\n");

    if (list->error != 0)
    {
        fprintf(dump_f, "ERROR in List (%x):\n", list->error);
        PRINT_ERROR(ERROR_CODE_SIZE_DISCREPANCY);
        PRINT_ERROR(ERROR_CODE_INCORRECT_FREE);
        PRINT_ERROR(ERROR_CODE_HEAD_IS_TAIL);
        PRINT_ERROR(ERROR_CODE_NODES_NULL_PTR);
        PRINT_ERROR(ERROR_CODE_INCORRECT_CAPACITY);
        PRINT_ERROR(CODE_DEAD_LIST);
    }

    else
    {
        fprintf(dump_f, "    Nodes[%p]\n", list->nodes);
        fprintf(dump_f, "    {\n");
        fprintf(dump_f, "        indx  value     next     prev\n");
        for (unsigned int i = 0; i < list->capacity; i++)
        {
            fprintf(dump_f, "        [%d]   " LIST_SPECIFIER "     %-4d     %-4d", i,
                list->nodes[i].value, list->nodes[i].next,
                list->nodes[i].prev);
            if (i == list->nodes[0].next)
                fprintf(dump_f, "  <--- HEAD");
            if (i == list->nodes[0].prev)
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

int CreateGraph(const struct List *list)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    FILE *graph_f = fopen(GRAPH_FILE, "w");
    ERROR_CHECK(graph_f == NULL, ERROR_OPENING_FILE);


    fprintf(graph_f, "digraph list{\n\trankdir=LR;\nspline = ortho;\n");
    fprintf(graph_f, "\t%d[shape=record,color=%s,label=\"<f0>indx %d|data %d|{<f2>prev %d | <f1>next %d}\"]\n",
                        0, "black", 0, list->nodes[0].value, list->nodes[0].next, list->nodes[0].prev);

    for (unsigned int i = 0; i < list->capacity - 1; i++)
        fprintf(graph_f, "%d -> %d [style=\"invis\", weight=1000,constraint=true]\n", i, i + 1);
    
    for (unsigned int i = 1; i < list->capacity; i++)
    {
        const char *color = "seagreen";
        const char *fill_color = "darkolivegreen2";

        if (list->nodes[i].value != LIST_POISON_VALUE)
        {
            color = "red3";
            fill_color = "peachpuff";
        }

        if (list->nodes[i].value == LIST_POISON_VALUE)
            fprintf(graph_f, "\t%d[style=\"filled\",fillcolor=\"%s\"shape=record,color=%s,"
                             "label=\"<f0>indx %d|data POISON|{<f2>prev %d | <f1>next %d}\"]\n",
                             i, fill_color, color, i, list->nodes[i].prev, list->nodes[i].next);

        else
            fprintf(graph_f, "\t%d[style=\"filled\",fillcolor=\"%s\"shape=record,color=%s,"
                             "label=\"<f0>indx %d|data %d|{<f2>prev %d | <f1>next %d}\"]\n",
                             i, fill_color, color, i, list->nodes[i].value, list->nodes[i].prev, list->nodes[i].next);

        fprintf(graph_f, "\t%d:<f1>:s->%d:<f2>:s[dir=both,weight = 1, constraint=true,style=\"dashed\"]\n", i, list->nodes[i].next);
        
    }

    fprintf(graph_f, "\t0:<f1>:s->%d:<f2>:s[dir=both,weight = 1, constraint=true,style=\"dashed\"]\n", list->nodes[0].next);

    fprintf(graph_f, "\nHEAD[style=\"filled\",fillcolor=\"skyblue\",label=\"HEAD = %d\"]", list->nodes[0].next);
    fprintf(graph_f, "\tHEAD->%d:<f0>{rank = same; %d; HEAD;}\n", list->nodes[0].next, list->nodes[0].next);

    fprintf(graph_f, "\nTAIL[style=\"filled\",fillcolor=\"skyblue\",label=\"TAIL = %d\"]", list->nodes[0].prev);
    fprintf(graph_f, "\tTAIL->%d:<f0>{rank = same; %d; TAIL;}; \n", list->nodes[0].prev, list->nodes[0].prev);

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
    
    fprintf(htm_f, "<img width=1800 src=\"../images/graph%d.png\"><br><hr>\n", graph_counter);
    
    int fclose_err = fclose(htm_f);
    ERROR_CHECK(fclose_err, ERROR_CLOSING_FILE);

    return SUCCESS;
}   