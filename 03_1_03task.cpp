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
iNode *__ind = NULL;
char *__cwd = NULL;

int compareStrings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void setup_file_manager(file_manager_t *fm);
int create(int disk_size);
int create_dir(const char *path);
int create_file(const char *path, int file_size);
int change_dir(const char *path);
void get_cur_dir(char *dst);
int remove(const char *path, int recursive);
int destroy();
int list(const char* path, int dir_first);

int realloc_node(iNode *node, int index);
int check_disk(iNode *node);
int init_tree(int disk_size);
int check_token(const char *tkn);
void make_node(iNode *main_node, _ushrtint is_dir, _uint size, char **name, char **full_path, iNode *parent);
int make_obj(const char *path, _ushrtint mode, _uint size);
int check_exist_move_down(const char *o_name);
int loop_tree(const char *path);
int change_cwd(const char *path);
int remove_node(iNode *node);
int inode_remove(const char *path, int recursive);
int destroy_tree();
void realloc_node(iNode *node);
int list_node(const char* path, int dir_first);
void print_list(char *str, int dir_first);
void sort_str(char ***path, int len);

void print_list(char *str, int dir_first){
    if(!__ind->is_dir){
        return;
    }
    char **files = (char**)calloc(sizeof(char*), 1); int files_size = 0;
    char **dirs = (char**)calloc(sizeof(char*), 1);  int dirs_size = 0;
    for(_uint i = 0; i < __ind->child_count; ++i){
        if(!__ind->child[i]->is_dir){
            files = (char**)realloc(files, sizeof(char*)*(++files_size));
            files[files_size - 1] = strdup(__ind->child[i]->name);
        }
        else{
            dirs = (char**)realloc(dirs, sizeof(char*)*(++dirs_size));
            dirs[dirs_size - 1] = strdup(__ind->child[i]->name);
        }
    }

    if(dir_first){
    
        qsort(dirs, dirs_size, sizeof(dirs[0]), compareStrings);
        qsort(files, files_size, sizeof(files[0]), compareStrings);

        for(int i = 0; i < dirs_size; ++i){
            printf("%s\n", dirs[i]);
        }
        for(int i = 0; i < files_size; ++i){
            printf("%s\n", files[i]);
        }
    }

    else{
        char** full = (char**)calloc(sizeof(char*), dirs_size + files_size);
        for(int i = 0; i < files_size; ++i){
            full[i] = strdup(files[i]);
        }
        for(int i = 0; i < dirs_size; ++i){
            full[i + files_size] = strdup(dirs[i]);
        }
        qsort(full, dirs_size + files_size , sizeof(full[0]), compareStrings);
        for(int i = 0; i < dirs_size + files_size; ++i){
            printf("%s\n", full[i]);
            free(full[i]);
        }
        free(full);
    }

    for(int i = 0; i < files_size; ++i) free(files[i]);
    for(int i = 0; i < dirs_size; ++i) free(dirs[i]);
    free(dirs);
    free(files);
}

int list_node(const char* path, int dir_first){
    int ret_val = 1;
    char *main_token, *main_loop_path, *main_to_free;
    main_to_free = main_loop_path = strdup(path);
    while((main_token = strsep(&main_loop_path, " ")) != NULL){
        
        if (!strcmp(main_token, "") || main_token[0] == '\0')
        {
           continue;
        }
        char bdSymb[] = "!#$%&\'()*+,-:;<=>?@[\\]^`{|}~";
        for (size_t i = 0; i < strlen(bdSymb); i++)
        {
            if (strchr(main_token, bdSymb[i]) != NULL)
            {
                continue;
            }
        }
        if(!loop_tree(main_token)){
            ret_val = 0;
            // continue;
            break;
        }
        __ind->is_dir ? printf("%s:\n", __ind->name) : printf("%s\n", main_token);
        print_list(main_token, dir_first); 
    }

    free(main_to_free);
    return ret_val;
}

int loop_tree(const char *path)
{
    int ret_val = 0;
    while (__ind->parent != NULL)
        __ind = __ind->parent;
    if(!strcmp(path, "/")) return 1;
    char *token, *loop_path, *to_free, 
         *passed_path = (char *)calloc(sizeof(char), 1), 
         *full_path = (char *)calloc(sizeof(char), 1);
    if (path[0] == '/')
    {
        to_free = loop_path = strdup(path);
        full_path = (char *)realloc(full_path, sizeof(char) * strlen(loop_path) + 1);
        strcpy(full_path, loop_path);
    }
    if (path[0] != '/')
    {
        to_free = loop_path = (char *)calloc(sizeof(char), (strlen(__cwd) + strlen(path) + 1));
        strcat(loop_path, __cwd);
        strcat(loop_path, path);
        full_path = (char *)realloc(full_path, sizeof(char) * strlen(loop_path) + 1);
        strcpy(full_path, loop_path);
    }

    while ((token = strsep(&loop_path, "/")) != NULL)
    {
        if (!strcmp(token, ".")){
            passed_path = (char *)realloc(passed_path, sizeof(char) * (strlen(passed_path) + strlen(token) + strlen("/") + 1));
            strcat(passed_path, "/");
            strcat(passed_path, token);
            continue;
        }
        if (!strcmp(token, ".."))
        {
            if (__ind->parent != NULL)
                __ind = __ind->parent;
            passed_path = (char *)realloc(passed_path, sizeof(char) * (strlen(passed_path) + strlen(token) + strlen("/") + 1));
            strcat(passed_path, "/");
            strcat(passed_path, token);
            continue;
        }
        if(!check_token(token)){
            continue;
        }
        if(check_exist_move_down(token) != -1){
            passed_path = (char *)realloc(passed_path, sizeof(char) * (strlen(passed_path) + strlen(token) + strlen("/") + 1));
            strcat(passed_path, "/");
            strcat(passed_path, token);
        }
        else{
            ret_val = 0;
            break;
        }
    }
    if(!strcmp(full_path, passed_path)){
        ret_val = 1;
    }
    free(to_free);
    free(full_path);
    free(passed_path);
    return ret_val;
}

int destroy_tree(){
    loop_tree("/");
    remove_node(__ind);
    free(__cwd);
    __ocpdsz = 0;
    __dsksz = 0;
    __ind = NULL;
    __cwd = NULL;
    return 1;
}

int realloc_node(iNode *node, int index){
    node->child[index] = NULL;
    for(_uint i = index; i < node->child_count - 1; ++i){
        iNode *tmp = node->child[i];
        node->child[i] = node->child[i + 1];
        node->child[i + 1] = tmp;
    }
    --node->child_count;
    if(node->child_count == 0) {
        free(node->child);
        node->child = NULL;
        return 1;
    }
    node->child = (iNode**)realloc(node->child, sizeof(iNode*)*node->child_count);
    return 1;
}

int remove_node(iNode *node){
    if(!node) return 0;
    for(_uint i = 0; i < node->child_count; ++i){
        remove_node(node->child[i]);
        node->child[i] = NULL;
    }
    __ocpdsz -= node->size;
    free(node->name);
    free(node->full_path);
    free(node->child);
    free(node);
    node = NULL;
    return 1;
}

int inode_remove(const char *path, int recursive){
    char *token, *loop_path, *to_free;
    int index = -1;
    if (!strcmp(path, "/") || !loop_tree(path)) { return 0; }
    if(!recursive && __ind->is_dir && __ind->child_count > 0) return 0;
    if (path[0] == '/')
    {
        to_free = loop_path = strdup(path);
    }
    if (path[0] != '/')
    {
        to_free = loop_path = (char *)calloc(sizeof(char), (strlen(__cwd) + strlen(path) + 1));
        strcat(loop_path, __cwd);
        strcat(loop_path, path);
    }
    iNode *del_node = __ind;
    iNode *parent_node = del_node->parent;
    loop_tree("/");
    while ((token = strsep(&loop_path, "/")) != NULL){
        if (!strcmp(token, ".") || !check_token(token))
            continue;
        if (!strcmp(token, ".."))
        {
            if (__ind->parent != NULL)
                __ind = __ind->parent;
            continue;
        }
        index = check_exist_move_down(token);
    }
    loop_tree("/");
    if(!strcmp(del_node->full_path, __cwd)){
        change_cwd("/");    
    }
    remove_node(del_node);
    realloc_node(parent_node, index);
    free(to_free);
    return 1;
}

int change_cwd(const char *path){
    if(!loop_tree(path) || !__ind->is_dir) return 0;
    free(__cwd);
    __cwd = strdup(__ind->full_path);
    return 1;
}

// mode: 1 - folder, 0 - file
int make_obj(const char *path, _ushrtint mode, _uint size)
{
    if(__ocpdsz + size > __dsksz) return 0;
    if(loop_tree(path)) return 0;
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
        to_free = loop_path = (char *)calloc(sizeof(char), (strlen(__cwd) + strlen(path) + 1));
        strcat(loop_path, __cwd);
        strcat(loop_path, path);
    }
    if (!loop_path || !path)
        return 0;
    while ((token = strsep(&loop_path, "/")) != NULL)
    {
        if (!strcmp(token, ".") || 
            !check_token(token) || 
            check_exist_move_down(token) >= 0)
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
        if(!__ind->is_dir){
            is_created = 0;
            break;
        }
        is_created = 1;
        if(loop_path != NULL){
            is_created = 0;
            break;
        }
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
        __ind->child[__ind->child_count - 1] = newNode;
        __ocpdsz += size;
    }
    free(to_free);
    return is_created;
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
    __cwd = strdup(__ind->full_path);
    __dsksz = disk_size;
    return 1;
}

int check_exist_move_down(const char *token)
{
    if(__ind->child == NULL) return -1;
    for (_uint i = 0; i < __ind->child_count; ++i)
    {
        if (strcmp(__ind->child[i]->name, token) == 0)
        {
            __ind = __ind->child[i];
            return i;
        }
    }
    return -1;
}

int check_token(const char *tkn)
{
    if (strlen(tkn) <= 0 || strlen(tkn) > 32 || !strcmp(tkn, "") || tkn[0] == '\0')
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
    fm->change_dir = change_dir;
    fm->get_cur_dir = get_cur_dir;
    fm->remove = remove;
    fm->list = list;
    fm->destroy = destroy;
}

int create(int disk_size) { return check_disk(__ind) == 0 ? init_tree(disk_size) : 0; }
int create_dir(const char *path) { return check_disk(__ind) == 0 ? 0 : make_obj(path, 1, 0); }
int create_file(const char *path, int file_size) { return check_disk(__ind) == 0 ? 0 : make_obj(path, 0, file_size); }
int change_dir(const char *path) { return check_disk(__ind) == 0 ? 0 : change_cwd((char *)path); }
int check_disk(iNode *node) { return node == NULL ? 0 : 1; }
int remove(const char *path, int recursive) { return check_disk(__ind) == 0 ? 0 : inode_remove(path, recursive); }
int list(const char* path, int dir_first) { return check_disk(__ind) == 0 ? 0 : list_node(path, dir_first); }
int destroy() { return check_disk(__ind) == 0 ? 0 : destroy_tree(); }
void get_cur_dir(char *dst)
{
    if(__cwd){
        strcpy(dst, __cwd);
    } 
    return;
}

void printTree(iNode *node)
{
    printf("is_dir: %hu\n", node->is_dir);
    printf("objSz: %u\n", node->size);
    printf("chldCnt: %u\n", node->child_count);
    printf("name: %s\n", node->name);
    printf("fPth: %s\n\n", node->full_path);
    if (node->child != NULL)
    {
        printf("Children: \n");
        for (_uint i = 0; i < node->child_count; ++i)
        {
            if(node->child[i] == NULL) continue;
            printf("Child %u\t%p:\n", i + 1, &node->child[i]);
            printTree(node->child[i]);
        }
    }
}