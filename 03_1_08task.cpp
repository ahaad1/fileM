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
int rmNode(iNode *node, int rootflag);
int chkDsk(iNode *iNd);
int indCpy(const char *path, const char *to_path);
int iNdDstr();
int mkCpNode(const char *path, iNode *iNd);

int rmNode(iNode *node, int rootflag)
{
    if (node == NULL)
    {
        return 0;
    }
    for (_uint i = 0; i < node->chldCnt; ++i)
    {
        if (__ind->chld != NULL && __ind->chld[i] != NULL && __ind->chld[i]->prnt != NULL && (__ind->chld[i]->is_dir == 1 || __ind->chld[i]->is_dir == 0))
        {
            rmNode(node->chld[i], 0);
        }
        node->chld[i] = NULL;
    }
    __ocpdsz -= node->objSz;
    free(node->name);
    free(node->fPth);
    free(node->chld);
    node->name = NULL;
    node->fPth = NULL;
    node->chld = NULL;
    node->prnt = NULL;
    node = NULL;
    if (!rootflag)
        free(node);
    return 1;
}

int iNdRm(const char *path, int recursive)
{
    if (strcmp(path, "/") == 0)
    {
        return 0;
    }
    char *tkn, *str, *tfr, *chkpath = (char *)calloc(sizeof(char), 1), *fchkpath = (char *)calloc(sizeof(char), 1);
    while (__ind->prnt != NULL)
        __ind = __ind->prnt;
    if (path[0] == '/')
    {
        tfr = str = strdup(path);
        fchkpath = (char *)realloc(fchkpath, sizeof(char) * strlen(path) + 1);
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
            if (__ind->chld != NULL && __ind->chld[i] != NULL && __ind->chld[i]->prnt != NULL && (__ind->chld[i]->is_dir == 1 || __ind->chld[i]->is_dir == 0) && strcmp(__ind->chld[i]->name, tkn) == 0)
            {
                chkpath = (char *)realloc(chkpath, sizeof(char) * (strlen(chkpath) + strlen(tkn) + strlen("/") + 1));
                strcat(chkpath, "/");
                strcat(chkpath, tkn);
                __ind = __ind->chld[i];
                break;
            }
        }
    }

    if (strcmp(chkpath, fchkpath) != 0)
    {
        free(chkpath);
        free(fchkpath);
        free(tfr);
        return 0;
    }
    iNode *nodeToDelete = __ind;
    if (!recursive && nodeToDelete->is_dir && nodeToDelete->chldCnt > 0)
    {
        free(tfr);
        free(chkpath);
        free(fchkpath);
        return 0;
    }

    while (__ind->prnt != NULL)
        __ind = __ind->prnt;
    int result = rmNode(nodeToDelete, 0);
    if (result)
    {
        nodeToDelete = NULL;
        free(__cwd);
        __cwd = strdup("/");
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
    {
        return 0;
    }
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
            free(tfr);
            return 0;
        }
    }
    free(__cwd);
    __cwd = strdup(__ind->fPth);
    free(tfr);
    return 1;
}

// o_type: 1 - folder, 0 - file
int iNdMkObj(const char *path, _ushrtint o_type, _uint o_size)
{
    int isCreated = 0;
    iNode *newNode = NULL;
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
            if (isCreated && newNode != NULL)
            {
                isCreated = 0;
                rmNode(newNode, 0);
                break;
            }
            if (!__ind->is_dir)
            {
                continue;
            }
            if (__ocpdsz + o_size > __dsksz || strlen(__ind->fPth) + strlen(tkn) + strlen("/") > 128)
            {
                isCreated = 0;
                break;
            }
            isCreated = 1;
            ++__ind->chldCnt;
            if (__ind->chld == NULL)
                __ind->chld = (iNode **)malloc(sizeof(iNode *));
            else
                __ind->chld = (iNode **)realloc(__ind->chld, sizeof(iNode *) * __ind->chldCnt);
            newNode = (iNode *)malloc(sizeof(iNode));
            newNode->chld = NULL;
            newNode->chldCnt = 0;
            newNode->is_dir = o_type;
            newNode->name = strdup(tkn);
            newNode->objSz = o_size;
            newNode->prnt = __ind;
            newNode->fPth = (char *)calloc(sizeof(char), (strlen(__ind->fPth) + strlen(tkn) + strlen("/") + 1));
            strcat(newNode->fPth, __ind->fPth);
            strcat(newNode->fPth, newNode->name);
            strcat(newNode->fPth, "/");
            __ind->chld[__ind->chldCnt - 1] = newNode;
            __ocpdsz += o_size;
        }
    }
    free(newpth);
    free(tfr);
    return isCreated;
}

int create(int disk_size)
{
    if (chkDsk(__ind))
    {
        return 0;
    }
    if (disk_size <= 0)
    {
        return 0;
    }
    __ind = (iNode *)malloc(sizeof(iNode));
    if (__ind == NULL)
    {
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
        if (__ind->chld != NULL && __ind->chld[i] != NULL && __ind->chld[i]->prnt != NULL && (__ind->chld[i]->is_dir == 1 || __ind->chld[i]->is_dir == 0) && strcmp(__ind->chld[i]->name, o_name) == 0)
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
    rmNode(__ind, 1);
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

void printTree(iNode *node)
{
    printf("is_dir: %hu\n", node->is_dir);
    printf("objSz: %u\n", node->objSz);
    printf("chldCnt: %u\n", node->chldCnt);
    printf("name: %s\n", node->name);
    printf("fPth: %s\n\n", node->fPth);
    // Print information about children, if any
    if (node->chldCnt > 0 && node->chld != NULL)
    {
        printf("Children:\n");
        for (_uint i = 0; i < node->chldCnt; ++i)
        {
            printf("Child %u:\n", i + 1);
            printTree(node->chld[i]);
        }
    }
}