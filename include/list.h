#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>

#ifdef SOFT_ASSERT
#undef SOFT_ASSERT
#endif

#define ASSERT_MODE

#ifdef ASSERT_MODE

#define SOFT_ASSERT(condition)                                                    \
            do                                                                    \
            {                                                                     \
                if (condition)                                                 \
                    printf("Error in %s; file: %s; num of line: %d \n",           \
                           #condition, __FILE__, __LINE__);                       \
            } while(false)

#else
#define SOFT_ASSERT(condition) ;
#endif

#define ERROR_CHECK(cond, rtrn)                             \
            do                                              \
            {                                               \
                SOFT_ASSERT(cond);                          \
                if (cond)                                   \
                    return rtrn;                            \
            } while(false)

#define FILE_ERROR_CHECK(cond, rtrn, closing_file)          \
            do                                              \
            {                                               \
                SOFT_ASSERT(cond);                          \
                if (cond)                                   \
                {                                           \
                    fclose(closing_file);                   \
                    return rtrn;                            \
                }                                           \
            } while(false)


#define FREE_NODE list->nodes[list->free]
#define TALE_NODE list->nodes[list->tale]
#define HEAD_NODE list->nodes[list->head]
#define CURR_NODE list->nodes[curr_node]

#define DUMPLIST(list)                                      \
            DumpList(list, __FILE__, __LINE__, __FUNCTION__)

typedef int list_t;
#define LIST_SPECIFIER "%-5d"

const int LIST_POISON_VALUE = -505;

const size_t LIST_MAX_SIZE = 10;

static const char * const GRAPH_FILE = "graph.gv"; 
static const char * const  DUMP_FILE = "io/dump_file.txt";
static const char * const   HTM_FILE = "io/allgraphs.htm"; 

enum ListError
{
    SUCCESS                = 0,
    ERROR_CTOR             = 1,
    ERROR_DTOR             = 2,
    ERROR_CALLOC           = 3,
    ERROR_NULL_PTR         = 4,
    ERROR_NULL_TALE        = 5,
    ERROR_NULL_FREE        = 6,
    ERROR_VERIFICATION     = 7,
    ERROR_INITIALIZE_NODES = 8,
    ERROR_OPENING_FILE     = 9,
    ERROR_CLOSING_FILE     = 10,
    ERROR_PUSH_BACK        = 11,
    ERROR_PUSH_FRONT       = 12,
    ERROR_POP_BACK         = 13,
    ERROR_POP_FRONT        = 14,
};

enum ListErrorCode 
{
    CODE_DEAD_LIST = 1 << 31,
    ERROR_CODE_NULL_NODES_PTR   = (1 << 0),
    ERROR_CODE_SIZE_DISCREPANCY = (1 << 1),
    ERROR_CODE_INCORRECT_HEAD   = (1 << 2),
    ERROR_CODE_INCORRECT_TALE   = (1 << 3),
    ERROR_CODE_INCORRECT_FREE   = (1 << 4),
    ERROR_CODE_HEAD_IS_TALE     = (1 << 5),
    ERROR_CODE_TALE_IS_FREE     = (1 << 6),
    ERROR_CODE_HEAD_IS_FREE     = (1 << 7),
};


struct Node
{
    list_t       value = LIST_POISON_VALUE;
    unsigned int next  = 0;
    unsigned int prev  = 0;
};

struct ListContext
{
    struct Node *nodes = NULL;
    size_t capacity    = 0;
    size_t size        = 0;
    unsigned int head  = 0;
    unsigned int tale  = 0;
    unsigned int free  = 0;
             int error = 0;
};

int ListVerification(struct ListContext *list);
int DumpList(struct ListContext *list, const char *file_name,
             int line_num, const char *func_name);

int ListContextCtor(struct ListContext *list);
int ListContextDtor(struct ListContext *list);
int InitializeNodes(struct ListContext *list);

int ListPushBack(struct ListContext *list, list_t arg,
                 unsigned int *new_node_ptr);

int ListPushFront(struct ListContext *list, list_t arg,
                  unsigned int *new_node_ptr);

int ListPopBack (struct ListContext *list, list_t *arg);
int ListPopFront(struct ListContext *list, list_t *arg);

int ListInsertAfter (struct ListContext *list, unsigned int phys_ptr,
                     list_t arg, unsigned int *new_node_ptr);
int ListInsertBefore(struct ListContext *list, unsigned int phys_ptr,
                     list_t arg, unsigned int *new_node_ptr);

int ListRemove(struct ListContext *list, unsigned int phys_ptr,
               list_t *arg);

int List_Translate_Logical_Position_To_Physical_Adress(struct ListContext *list,
                                                       unsigned int logic_ptr, 
                                                       unsigned int *phys_ptr);
int CreateGraph(struct ListContext *list);
int SaveGraphPng(void);
int CreateHtmFile(void);

#endif //LIST_H_INCLUDED