
#include "list.h"

int listDump(struct List *list, const char *comment, 
             const char *file_name, int line_num, const char *func_name);

int CreateGraph(struct List *list);
int SaveGraphPng(void);
int AddGraphPng(void);