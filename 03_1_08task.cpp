#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os_file.h"

typedef unsigned short int _ushrtint;
typedef unsigned int _uint;

typedef struct iNode
{
    _ushrtint is_dir;
    _uint size;
    _uint child_count;
    char *name;
    char *full_path;
    iNode **child;
    iNode *parent;
} iNode;

_uint __dsksz = 0, __ocpdsz = 0;
iNode *__ind = NULL, *__cwd = NULL;

int create(int disk_size);
int create_dir(const char *path);
int create_file(const char *path, int file_size);
void setup_file_manager(file_manager_t *fm);

int check_disk(iNode *node);
int init_tree(int disk_size);
int check_token(const char *tkn);
int realloc_tree(iNode *node);
void make_node(iNode *main_node, _ushrtint is_dir, _uint size, char **name, char **full_path, iNode *parent);
int remove_node(iNode *node);
int make_obj(const char *path, _ushrtint mode, _uint size);

iNode *get_node(const char *path)
{
    iNode *ret_node = NULL;
    while (__ind->parent != NULL)
        __ind = __ind->parent;
    char *token, *loop_path, *to_free, 
         *passed_path = (char *)calloc(sizeof(char), 1), 
         *full_path = (char *)calloc(sizeof(char), 1);
    if (path[0] == '/')
    {
        to_free = loop_path = strdup(path);
        full_path = (char *)realloc(full_path, sizeof(char) * strlen(path) + 1);
        strcpy(full_path, loop_path);
    }
    if (path[0] != '/')
    {
        to_free = loop_path = (char *)calloc(sizeof(char), (strlen(__cwd->full_path) + strlen(path) + 1));
        strcat(loop_path, __cwd->full_path);
        strcat(loop_path, path);
        full_path = (char *)realloc(full_path, sizeof(char) * strlen(path) + 1);
        strcpy(full_path, loop_path);
    }
    if (loop_path == NULL || path == NULL)
        return 0;

    while ((token = strsep(&loop_path, "/")) != NULL)
    {
        if (!strcmp(token, ".") || !check_token(token))
            continue;
        if (!strcmp(token, ".."))
        {
            if (__ind->parent != NULL)
                __ind = __ind->parent;
            continue;
        }
        for(_uint i = 0; i < __ind->child_count; ++i){
            if(!strcmp(__ind->child[i]->name, token)){
                passed_path = (char *)realloc(passed_path, sizeof(char) * (strlen(passed_path) + strlen(token) + strlen("/") + 1));
                strcat(passed_path, "/");
                strcat(full_path, token);
                __ind = __ind->child[i];
            }
        }
    }
    if(!strcmp(full_path, passed_path)){
        ret_node = __ind;
    }
    free(to_free);
    free(full_path);
    free(passed_path);
    return ret_node;
}

int remove_node(iNode *node){
    return 1;
}
// mode: 1 - folder, 0 - file
int make_obj(const char *path, _ushrtint mode, _uint size)
{
    if(get_node(path) != NULL) return 0;
    int is_created = 0;
    iNode *newNode = NULL;
    while (__ind->parent != NULL)
        __ind = __ind->parent;
    char *token, *loop_path, *to_free;
    if (path[0] == '/')
    {
        to_free = loop_path = strdup(path);
    }
    if (path[0] != '/')
    {
        to_free = loop_path = (char *)calloc(sizeof(char), (strlen(__cwd->full_path) + strlen(path) + 1));
        strcat(loop_path, __cwd->full_path);
        strcat(loop_path, path);
    }
    if (loop_path == NULL || path == NULL)
        return 0;
    while ((token = strsep(&loop_path, "/")) != NULL)
    {
        if (!strcmp(token, ".") || !check_token(token))
        continue;
        if (!strcmp(token, ".."))
        {
            if (__ind->parent != NULL)
                __ind = __ind->parent;
            continue;
        }
        if(is_created && newNode != NULL){
            is_created = 0;
            remove_node(newNode);
            break;
        }
        is_created = 1;
        newNode = (iNode*)malloc(sizeof(iNode));
        char *new_node_name = strdup(token);
        char *new_node_full_path = (char*)calloc(sizeof(char), strlen(__ind->full_path) + strlen(token) + strlen("/") + 1);
        strcat(new_node_full_path, __ind->full_path);
        strcat(new_node_full_path, token);
        strcat(new_node_full_path, "/");
        make_node(newNode, mode, size, &new_node_name, &new_node_full_path, __ind);        
        ++__ind->child_count;
        if(__ind->child == NULL){
            __ind->child = (iNode**)malloc(sizeof(iNode*));
        }
        else{
            __ind->child = (iNode **)realloc(__ind->child, sizeof(iNode *) * __ind->child_count);
        } 
        __ind->child[__ind->child_count] = newNode;
    }
    free(to_free);
    return is_created;
}

int realloc_tree(iNode *node){
    return 1;
}

void make_node(iNode *main_node, _ushrtint is_dir, _uint size, char **name, char **full_path, iNode *parent)
{
    main_node->child = NULL;
    main_node->parent = parent;
    main_node->child_count = 0;
    main_node->is_dir = is_dir;
    main_node->size = size;
    main_node->name = *name;
    main_node->full_path = *full_path;
}

int init_tree(int disk_size)
{
    if (disk_size <= 0)
        return 0;
    __ind = (iNode *)malloc(sizeof(iNode));
    if (__ind == NULL)
        exit(0);
    char *full_path = strdup("/");
    char *name = strdup("/");
    make_node(__ind, 1, 0, &name, &full_path, NULL);
    __cwd = __ind;
    return 1;
}

int check_token(const char *tkn)
{
    if (strlen(tkn) <= 0 || strlen(tkn) > 32 || !strcmp(tkn, "") || tkn[0] == '\0' /*!strcmp(tkn, ".") || !strcmp(tkn, "..") || tkn[0] == '.' ||*/)
    {
        return 0;
    }
    char bdSymb[] = "!#$%&\'()*+,-/:;<=>?@[\\]^`{|}~";
    for (size_t i = 0; i < strlen(bdSymb); i++)
    {
        if (strchr(tkn, bdSymb[i]) != NULL)
        {
            return 0;
        }
    }
    return 1;
}

void setup_file_manager(file_manager_t *fm)
{
    fm->create = create;
    fm->create_dir = create_dir;
    fm->create_file = create_file;
    // fm->change_dir = change_dir;
    // fm->get_cur_dir = get_cur_dir;
    // fm->remove = remove;
    // fm->copy = copy;
    // fm->destroy = destroy;
}

int create(int disk_size) { return check_disk(__ind) == 0 ? init_tree(disk_size) : 0; }
int create_dir(const char *path) { return check_disk(__ind) == 0 ? 0 : make_obj(path, 1, 0); }
int create_file(const char *path, int file_size) { return check_disk(__ind) == 0 ? 0 : make_obj(path, 0, file_size); }
int check_disk(iNode *node) { return node == NULL ? 0 : 1; }

void printTree(iNode *node)
{
    printf("is_dir: %hu\n", node->is_dir);
    printf("objSz: %u\n", node->size);
    printf("chldCnt: %u\n", node->child_count);
    printf("name: %s\n", node->name);
    printf("fPth: %s\n\n", node->full_path);
    // Print information about children, if any
    if (node->child_count > 0 && node->child != NULL)
    {
        printf("Children:\n");
        for (_uint i = 0; i < node->child_count; ++i)
        {
            printf("Child %u:\n", i + 1);
            printTree(node->child[i]);
        }
    }
}