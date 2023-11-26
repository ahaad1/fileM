#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
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

void iNdMkNode(iNode *_mnInd, iNode *child, iNode *parent, _ushrtint o_type, _uint o_size);
int iNdMkObj(const char *path, _ushrtint o_type, _uint o_size);
int mvIndVrt(const char *path);
int vldTkn(const char *tkn);
int iNdCkExst(const char *o_name);
int chkDsk(iNode *iNd);

int main()
{
    create(101010);
    create_dir("/a");
    create_dir("/a/b");
    return 0;
}

int iNdMkObj(const char *path, _ushrtint o_type, _uint o_size)
{
    while (__ind->prnt != NULL) __ind = __ind->prnt;
    char *tkn, *tfr, *newpth;
    if (path[0] == '/') { newpth = strdup(path); }
    if (path[0] != '/')
    {
        newpth = (char *)calloc(sizeof(char), strlen(__cwd) + (strlen(path) + 1));
        strcat(newpth, __cwd);
        strcat(newpth, path);
    }

    if (newpth == NULL || path == NULL) return 0;
    tfr = strdup(newpth);
    while ((tkn = strsep(&newpth, "/")) != NULL)
    {
        if (strcmp(tkn, ".") == 0) continue;
        if (strcmp(tkn, "..") == 0) { if (__ind->prnt != NULL) __ind = __ind->prnt; continue; }
        if (!vldTkn(tkn)) continue;
        if (!iNdCkExst(tkn))
        {
            if(!__ind->is_dir){ fprintf(stderr, "error: impossible to create object in file\n"); continue; }
            iNode *newInd = (iNode*)malloc(sizeof(iNode));
            newInd->chld = NULL;
            newInd->chldCnt = 0;
            newInd->name = strdup(tkn);
            newInd->prnt = __ind;
            newInd->is_dir = o_type;
            newInd->objSz = o_size;
            newInd->fPth = (char*)calloc(sizeof(char), strlen(__ind->fPth) + strlen(newInd->name) + strlen("/") + 1);
            strcat(newInd->fPth, __ind->fPth);
            strcat(newInd->fPth, newInd->name);
            strcat(newInd->fPth, "/");
            iNdMkNode(__ind, newInd, __ind->prnt, o_type, o_size);
        }
    }
    free(tfr);
    return 1;
}

// o_type: 1 - folder, 0 - file
void iNdMkNode(iNode *_mnInd, iNode *child, iNode *parent, _ushrtint o_type, _uint o_size)
{
    _mnInd->prnt = parent;
    _mnInd->objSz = o_size;
    _mnInd->is_dir = o_type;
    if (child != NULL)
    {
        if (_mnInd->chld == NULL)
            _mnInd->chld = (iNode **)malloc(sizeof(iNode *));
        if (_mnInd->chld != NULL)
        {
            _mnInd->chld = (iNode **)realloc(_mnInd->chld, sizeof(iNode *) * (++_mnInd->chldCnt));
            _mnInd->chld[_mnInd->chldCnt - 1] = child;
        }
        printf("created node %s\t%d\t%d\n", child->fPth, o_type, o_size);
    }
    else _mnInd->chld = NULL;
}

int create(int disk_size)
{
    if (chkDsk(__ind))
    {
        fprintf(stderr, "error: disk already exists\n");
        return 0;
    }
    if (disk_size <= 0)
    {
        fprintf(stderr, "error: bad disk size\n");
        return 0;
    }
    __ind = (iNode *)malloc(sizeof(iNode));
    if (__ind == NULL)
    {
        fprintf(stderr, "error: memory not allocated. aborting file_manager\n");
        exit(0);
    }
    iNdMkNode(__ind, NULL, NULL, 1, 0);
    __dsksz = disk_size;
    __ocpdsz = 0;
    __cwd = strdup("/");
    __ind->name = strdup("/");
    __ind->fPth = strdup("/");
    fprintf(stdout, "created file manager with disk size %d bytes\n", __dsksz);
    return 1;
}

void setup_file_manager(file_manager_t *fm)
{
    fm->create = create;
    // fm->create_dir = create_dir;
    // fm->create_file = create_file;
    // fm->change_dir = change_dir;
    // fm->get_cur_dir = get_cur_dir;
    // fm->remove = remove;
    // fm->destroy = destroy;
}
int chkDsk(iNode *iNd) { return iNd == NULL ? 0 : 1; }

// 1 - exist : 0 - not
int iNdCkExst(const char *o_name)
{
    for (_uint i = 0; i < __ind->chldCnt; ++i)
    {
        /* or (strcmp(__ind->name, o_name) > 0*/
        if (__ind->chld != NULL && strcmp(__ind->chld[i]->name, o_name) == 0)
        {
            __ind = __ind->chld[i];
            return 1;
        }
    }
    return 0;
}

int vldTkn(const char *tkn)
{
    if (strlen(tkn) <= 0 || strlen(tkn) > 32 || !strcmp(tkn, ".") || !strcmp(tkn, "..") || tkn[0] == '.' || !strcmp(tkn, "") || tkn[0] == '\0') 
    { return 0; }
    char bdSymb[] = "!#$%&\'()*+,-/:;<=>?@[\\]^`{|}~";
    for (size_t i = 0; i < strlen(bdSymb); i++) { if (strchr(tkn, bdSymb[i]) != NULL) { return 0; } }
    return 1;
}

int create_dir(const char* path){ return chkDsk(__ind) == 0 ? 0 : iNdMkObj(path, 1, 0); }

// int mvIndVrt(const char *path)
// {
//     while (__ind->prnt != NULL)
//         __ind = __ind->prnt;
//     char *tkn, *str, *tfr, *newpth;
//     if (path[0] == '/')
//     {
//         newpth = strdup(path);
//     }
//     if (path[0] != '/')
//     {
//         newpth = (char *)calloc(sizeof(char), strlen(__cwd) + (strlen(path) + 1));
//         strcat(newpth, __cwd);
//         strcat(newpth, path);
//     }
//     if (newpth == NULL || path == NULL)
//         return 0;
//     tfr = newpth;
//     while ((tkn = strsep(&newpth, "/")) != NULL)
//     {
//         if (!vldTkn(tkn))
//             continue;
//         if (strcmp(tkn, ".") == 0)
//             continue;
//         if (strcmp(tkn, "..") == 0)
//         {
//             if (__ind->prnt != NULL)
//                 __ind = __ind->prnt;
//             continue;
//         };
//         if (!iNdCkExst(tkn))
//             return 0;
//     }
//     free(tfr);
//     return 1;
// }