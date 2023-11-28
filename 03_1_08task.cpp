#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os_file.h"

#define mSwp(T, a, b) \
    {                 \
        T tmp = a;    \
        a = b;        \
        b = tmp;      \
    }

typedef unsigned short int _ushrtint;
typedef unsigned int _uint;

typedef struct iNode
{
    _ushrtint is_dir;
    _uint objSz;
    _uint chldCnt;
    char *name;
    char *fPth;
    iNode **chld;
    iNode *prnt;
} iNode;

_uint __dsksz = 0, __ocpdsz = 0;
iNode *__ind = NULL;
char *__cwd = NULL;

void setup_file_manager(file_manager_t *fm);
int create(int disk_size);
int create_dir(const char *path);
int create_file(const char *path, int file_size);
int change_dir(const char *path);
int remove(const char *path, int recursive);
int destroy();
int copy(const char *path, const char *to_path);
void get_cur_dir(char *dst);

int iNdMkObj(const char *path, _ushrtint o_type, _uint o_size);
int iNdChDir(const char *path);
int vldTkn(const char *tkn);
int iNdCkExst(const char *o_name);
int iNdRm(const char *path, int recursive);
int rmNode(iNode *node);
int chkDsk(iNode *iNd);
int indCpy(const char *path, const char *to_path);
int iNdDstr();
int mkCpNode(const char *path, iNode *iNd);

void prnt(iNode **tst1);

// int mkCpNode(iNode *iNd){
//     iNode *cpNode = (iNode*)malloc(sizeof(iNode));
//     for(_uint i = 0; i < iNd->chldCnt; ++i){
//         if(iNd->chld[i] == NULL) continue;
//         else iNd = iNd->chld[i];
//     }
//     return 1;
// }

// int indCpy(const char *path, const char *to_path){
//     if(path == NULL || to_path == NULL || strcmp(path, "") == 0 || strcmp(to_path, "") == 0 || strcmp(path, to_path) == 0) return -1;
//     char *tkn, *str, *tfr;
//     while (__ind->prnt != NULL) __ind = __ind->prnt;
//     if (path[0] == '/') { tfr = str = strdup(path); }
//     if(path[0] != '/'){
//         tfr = str = (char *)calloc(sizeof(char) , strlen(__cwd) + strlen(path) + 1);
//         strcat(str, __cwd);
//         strcat(str, path);
//     }
//     while ((tkn = strsep(&str, "/")) != NULL){
//         if (strcmp(tkn, ".") == 0){ continue; }
//         if (strcmp(tkn, "..") == 0) { if (__ind->prnt != NULL) __ind = __ind->prnt; continue; }
//         if (!vldTkn(tkn)) { continue; }
//         for (_uint i = 0; i < __ind->chldCnt; ++i)
//         {
//             if (__ind->chld != NULL && strcmp(__ind->chld[i]->name, tkn) == 0) {
//                 __ind = __ind->chld[i];
//                 break;
//             }
//         }
//     }
//     iNode *cpyNode = __ind;
//     mkCpNode(path, cpyNode);
//     free(tfr);
//     return 1;
// }

int rmNode(iNode *node)
{
    if (node == NULL)
    {
        return 0;
    }
    for (_uint i = 0; i < node->chldCnt; ++i)
    {
        // printf("RMNODE: %s %d\n", node, i + 1);
        if (__ind->chld != NULL && __ind->chld[i] != NULL && __ind->chld[i]->prnt != NULL 
            && (__ind->chld[i]->is_dir == 1 || __ind->chld[i]->is_dir == 0)){
                rmNode(node->chld[i]);
            }
        free(node->chld[i]);
        node->chld[i] = NULL;
    }
    // fprintf(stdout, "removing node %s\t%s\n", node->name, node->fPth);
    // removing node
    __ocpdsz -= node->objSz;
    free(node->name);
    free(node->fPth);
    free(node->chld);
    node->name = NULL;
    node->fPth = NULL;
    node->chld = NULL;
    node->prnt = NULL;
    // free(node);
    node = NULL;
    return 1;
}

int iNdRm(const char *path, int recursive)
{
    if (strcmp(path, "/") == 0)
    {
        // fprintf(stderr, "impossible to rm /, aborting\n");
        return 0;
    }
    char *tkn, *str, *tfr, *chkpath = (char *)calloc(sizeof(char), 1), *fchkpath = (char *)calloc(sizeof(char), 1);
    while (__ind->prnt != NULL)
        __ind = __ind->prnt;
    if (path[0] == '/')
    {
        tfr = str = strdup(path);
        fchkpath = (char*)realloc(fchkpath, sizeof(char)*strlen(path) + 1);
        strcpy(fchkpath, str);
    }
    if (path[0] != '/')
    {
        tfr = str = (char *)calloc(sizeof(char), strlen(__cwd) + strlen(path) + 1);
        strcat(str, __cwd);
        strcat(str, path);
        fchkpath = (char *)realloc(fchkpath, sizeof(char) * (strlen(str) + 1));
        strcpy(fchkpath, str);
    }
    if (path == NULL || str == NULL)
        return 0;
    while ((tkn = strsep(&str, "/")) != NULL)
    {
        if (strcmp(tkn, ".") == 0)
        {
            continue;
        }
        if (strcmp(tkn, "..") == 0)
        {
            if (__ind->prnt != NULL)
                __ind = __ind->prnt;
            continue;
        }
        if (!vldTkn(tkn))
        {
            continue;
        }
        for (_uint i = 0; i < __ind->chldCnt; ++i)
        {
            if (__ind->chld != NULL && __ind->chld[i] != NULL && __ind->chld[i]->prnt != NULL
            && (__ind->chld[i]->is_dir == 1 || __ind->chld[i]->is_dir == 0) 
            && strcmp(__ind->chld[i]->name, tkn) == 0 )
            {
                chkpath = (char *)realloc(chkpath, sizeof(char) * (strlen(chkpath) + strlen(tkn) + strlen("/") + 1));
                strcat(chkpath, "/");
                strcat(chkpath, tkn);
                __ind = __ind->chld[i];
                break;
            }
        }
    }

    // fprintf(stdout, "CHKPATHHHH %s %s\n", chkpath, fchkpath);
    if (strcmp(chkpath, fchkpath) != 0)
    {
        free(chkpath);
        free(fchkpath);
        free(tfr);
        // fprintf(stderr, "node not removed: not exist %s\n", path);
        return 0;
    }
    iNode *nodeToDelete = __ind;
    if (!recursive && nodeToDelete->is_dir && nodeToDelete->chldCnt > 0)
    {
        free(tfr);
        free(chkpath);
        free(fchkpath);
        // fprintf(stdout, "node %s not removed: bad flag 1\n", nodeToDelete->fPth);
        return 0;
    }

    if (__ind->prnt != NULL)
        __ind = __ind->prnt;
    // removing node , there is no way

    int result = rmNode(nodeToDelete);
    if (result)
    {
        nodeToDelete = NULL;
        free(__cwd);
        __cwd = strdup("/");
        // fprintf(stdout, "node removed\n");
    }
    free(chkpath);
    free(fchkpath);
    free(tfr);
    return result;
}

int iNdChDir(const char *path)
{
    char *tkn, *str, *tfr;
    while (__ind->prnt != NULL)
        __ind = __ind->prnt;
    if (path[0] == '/')
    {
        tfr = str = strdup(path);
    }
    if (path[0] != '/')
    {
        tfr = str = (char *)calloc(sizeof(char), (strlen(__cwd) + strlen(path) + 1));
        strcat(str, __cwd);
        strcat(str, path);
    }
    if (path == NULL || str == NULL)
        return 0;
    while ((tkn = strsep(&str, "/")) != NULL)
    {
        if (strcmp(tkn, ".") == 0)
        {
            continue;
        }
        if (strcmp(tkn, "..") == 0)
        {
            if (__ind->prnt != NULL)
                __ind = __ind->prnt;
            continue;
        }
        if (!vldTkn(tkn))
        {
            continue;
        }
        if (!iNdCkExst(tkn))
        {
            // fprintf(stdout, "chdir error: dir does not exists { %s }\n", tkn);
            free(tfr);
            return 0;
        }
    }
    // printf("cd %s %s\n", __cwd, __ind->fPth);
    free(__cwd);
    __cwd = strdup(__ind->fPth);
    free(tfr);
    return 1;
}

// o_type: 1 - folder, 0 - file
int iNdMkObj(const char *path, _ushrtint o_type, _uint o_size)
{
    while (__ind->prnt != NULL)
        __ind = __ind->prnt;
    char *tkn, *newpth, *tfr;
    if (path[0] == '/')
    {
        tfr = newpth = strdup(path);
    }
    if (path[0] != '/')
    {
        tfr = newpth = (char *)calloc(sizeof(char), (strlen(__cwd) + strlen(path) + 1));
        strcat(newpth, __cwd);
        strcat(newpth, path);
    }

    if (newpth == NULL || path == NULL)
        return 0;
    while ((tkn = strsep(&newpth, "/")) != NULL)
    {
        if (strcmp(tkn, ".") == 0)
            continue;
        if (strcmp(tkn, "..") == 0)
        {
            if (__ind->prnt != NULL)
                __ind = __ind->prnt;
            continue;
        }
        if (!vldTkn(tkn))
            continue;
        if (!iNdCkExst(tkn))
        {
            if (!__ind->is_dir)
            {
                // fprintf(stderr, "error: impossible to create object in file\n");
                continue;
            }
            if (__ocpdsz + o_size > __dsksz || strlen(__ind->fPth) + strlen(tkn) + strlen("/") > 128)
            {
                // fprintf(stderr, "error: bad object name length or not enough space on disk\n");
                free(newpth);
                free(tfr);
                return 0;
            }
            ++__ind->chldCnt;
            if (__ind->chld == NULL)
                __ind->chld = (iNode **)malloc(sizeof(iNode *));
            else
                __ind->chld = (iNode **)realloc(__ind->chld, sizeof(iNode *) * __ind->chldCnt);
            iNode *newFm = (iNode *)malloc(sizeof(iNode));
            newFm->chld = NULL;
            newFm->chldCnt = 0;
            newFm->is_dir = o_type;
            newFm->name = strdup(tkn);
            newFm->objSz = o_size;
            newFm->prnt = __ind;
            newFm->fPth = (char *)calloc(sizeof(char), (strlen(__ind->fPth) + strlen(tkn) + strlen("/") + 1));
            strcat(newFm->fPth, __ind->fPth);
            strcat(newFm->fPth, newFm->name);
            strcat(newFm->fPth, "/");
            __ind->chld[__ind->chldCnt - 1] = newFm;
            __ocpdsz += o_size;
            // fprintf(stdout, "created %s in %s\t%d\t%d\n", newFm->name, __ind->fPth, newFm->is_dir, newFm->objSz);
            free(tfr);
            free(newpth);
            return 1;
        }
    }
    free(tfr);
    free(newpth);
    return 0;
}

int create(int disk_size)
{
    if (chkDsk(__ind))
    {
        // fprintf(stderr, "error: disk already exists\n");
        return 0;
    }
    if (disk_size <= 0)
    {
        // fprintf(stderr, "error: bad disk size\n");
        return 0;
    }
    __ind = (iNode *)malloc(sizeof(iNode));
    if (__ind == NULL)
    {
        // fprintf(stderr, "error: memory not allocated. aborting file_manager\n");
        exit(0);
    }
    __ind->chld = NULL;
    __ind->chldCnt = 0;
    __ind->fPth = strdup("/");
    __ind->is_dir = 1;
    __ind->name = strdup("/");
    __ind->objSz = 0;
    __ind->prnt = NULL;
    __cwd = strdup("/");
    __dsksz = disk_size;
    __ocpdsz = 0;
    // fprintf(stdout, "created file manager with disk size %d bytes\n", __dsksz);
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
    // fm->copy = copy;
    fm->destroy = destroy;
}
int chkDsk(iNode *iNd) { return iNd == NULL ? 0 : 1; }

// 1 - exist : 0 - not
int iNdCkExst(const char *o_name)
{
    for (_uint i = 0; i < __ind->chldCnt; ++i)
    {
        // if (__ind->chld != NULL && strcmp(__ind->chld[i]->name, o_name) == 0 /*&& __ind->chld[i]->is_dir*/)
        if(__ind->chld != NULL && __ind->chld[i] != NULL && __ind->chld[i]->prnt != NULL
            && (__ind->chld[i]->is_dir == 1 || __ind->chld[i]->is_dir == 0) 
            && strcmp(__ind->chld[i]->name, o_name) == 0)
        {
            __ind = __ind->chld[i];
            return 1;
        }
    }
    return 0;
}

int vldTkn(const char *tkn)
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

int iNdDstr()
{
    iNdChDir("/");
    rmNode(__ind);
    free(__cwd);
    free(__ind);
    __ind = NULL;
    __cwd = NULL;
    return 1;
}

int create_dir(const char *path) { return chkDsk(__ind) == 0 ? 0 : iNdMkObj(path, 1, 0); }
int create_file(const char *path, int file_size) { return chkDsk(__ind) == 0 ? 0 : iNdMkObj(path, 0, file_size); }
int change_dir(const char *path) { return chkDsk(__ind) == 0 ? 0 : iNdChDir((char *)path); }
int remove(const char *path, int recursive) { return chkDsk(__ind) == 0 ? 0 : iNdRm(path, recursive); }
// int copy(const char *path, const char *to_path) { return chkDsk(__ind) == 0 ? 0 : indCpy(path, to_path); }
int destroy() { return chkDsk(__ind) == 0 ? 0 : iNdDstr(); }

void get_cur_dir(char *dst)
{
    // fprintf(stdout, "CUR_DIR: %s\n", __cwd);
}

void printTree(iNode *node){
    printf("is_dir: %hu\n", node->is_dir);
    printf("objSz: %u\n", node->objSz);
    printf("chldCnt: %u\n", node->chldCnt);
    printf("name: %s\n", node->name);
    printf("fPth: %s\n\n", node->fPth);
    // Print information about children, if any
    if (node->chldCnt > 0 && node->chld != NULL) {
        printf("Children:\n");
        for (_uint i = 0; i < node->chldCnt; ++i) {
            printf("Child %u:\n", i + 1);
            printTree(node->chld[i]);
        }
    }
}