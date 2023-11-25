#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "os_file.h"

#define mSwp(T, a, b) { T tmp = a; a = b; b = tmp; }

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

_uint __dsksz = 0, __ocpdsz = 0; iNode *__ind = NULL; char *__cwd = NULL;

void setup_file_manager(file_manager_t *fm);
int create(int disk_size);
int create_dir(const char *path);

void iNdMkObj(iNode *_mnInd, iNode *child, iNode *parent, char *objNm, char *fPth, _ushrtint o_type);
int iNdMkDir(const char *path);
int mvIndVrt(const char *path);
int chkDsk(iNode *iNd);

int main(){
    create(101010);
    return 0;
}

int iNdMkDir(const char *path){
    mvIndVrt(path);
}

int mvIndVrt(const char *path){
    if(path[0] == '/' && strlen(__ind->fPth) == 1){
        while(__ind->prnt != NULL) __ind = __ind->prnt;
        return 0; 
    }
    return 1;
}

// o_type: 1 - folder, 0 - file
void iNdMkObj(iNode *_mnInd, iNode *child, iNode *parent, char *objNm, char *fPth, _ushrtint o_type){ 
    _mnInd->prnt = parent;
    _mnInd->is_dir = o_type; 
    _mnInd->name = strdup(objNm);
    _mnInd->fPth = fPth;
    if(child != NULL){
        if(_mnInd->chld == NULL && _mnInd->chldCnt == 0) _mnInd->chld = (iNode**)malloc(sizeof(iNode*)*(++_mnInd->chldCnt)); 
        if(_mnInd->chld != NULL && _mnInd->chldCnt > 0){
            _mnInd->chld = (iNode**)realloc(_mnInd->chld, sizeof(iNode*)*(++_mnInd->chldCnt));
            _mnInd->chld[_mnInd->chldCnt - 1] = child;
        }
    }
}

int create(int disk_size){
    if(chkDsk(__ind)) { fprintf(stderr, "error: disk already exists\n"); return 0; }
    if(disk_size <= 0) { fprintf(stderr, "error: bad disk size\n"); return 0; }
    __ind = (iNode*)malloc(sizeof(iNode));
    if(__ind == NULL) { fprintf(stderr, "error: memory not allocated. aborting file_manager\n"); exit(0); }
    iNdMkObj(__ind, NULL, NULL, (char*)"/", (char*)"/", 1);
    __dsksz = disk_size;
    __ocpdsz = 0;
    __cwd = strdup("/");
    fprintf(stdout, "created file manager with disk size %d bytes\n", __dsksz);
    return 1;
}

void setup_file_manager(file_manager_t *fm) {
    fm->create = create;
    // fm->create_dir = create_dir;
    // fm->create_file = create_file;
    // fm->change_dir = change_dir;
    // fm->get_cur_dir = get_cur_dir;
    // fm->remove = remove;
    // fm->destroy = destroy;
}
int chkDsk(iNode *iNd){ return iNd == NULL ? 0 : 1; }