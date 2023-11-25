#include <stdio.h>
#include <stdlib.h>
#include "os_file.h"

#define mSwp(T, a, b) { T tmp = a; a = b; b = tmp; }

typedef unsigned short int _ushrtint; 
typedef unsigned int _uint;


typedef struct mstrrls{
    //function for initilizing & allocating string(char*)
    //dst - destination string
    //str - string with content
    int (*ststr)(char **dst, const char *str);
    
    //returns length of str 
    _uint (*strln)(const char *str);
} mstrrls;

void cnstrct_mstrrls(mstrrls *variable);
int ststr(char **dst, char *str);
_uint strln(const char *str);

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

_uint __dsksz = 0, __ocpdsz = 0; iNode *__ind = NULL; char *__cwd = NULL; mstrrls string;

void setup_file_manager(file_manager_t *fm);
int create(int disk_size);

void iNdMkObj(iNode *_mnInd, iNode *child, iNode *parent, char *objNm, char *fPth, _ushrtint o_type);
int chkDsk(iNode *iNd);

int main(){
    cnstrct_mstrrls(&string);
    create(101010);
    return 0;
}

// o_type: 1 - folder, 0 - file
void iNdMkObj(iNode *_mnInd, iNode *child, iNode *parent, char *objNm, char *fPth, _ushrtint o_type){ 
    _mnInd->prnt = parent;
    _mnInd->is_dir = o_type; 
    string.ststr(&_mnInd->name, objNm);
    string.ststr(&_mnInd->fPth, fPth);
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
    cnstrct_mstrrls(&string);
    iNdMkObj(__ind, NULL, NULL, (char*)"/", (char*)"/", 1);
    __dsksz = disk_size;
    __ocpdsz = 0;
    string.ststr(&__cwd, "/");
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

int ststr(char **dst, const char *str){
    _uint lnstr = -1;
    if(str == NULL || !str || (lnstr = strln(str)) <= 0) return 0;
    *dst = (char*)malloc(sizeof(char)*lnstr);
    while(--lnstr){
        **dst++ = (char)*str++;
    }
    return 1;
}
_uint strln(const char *str){
    const char *s;
    for (s = str; *s; ++s);
    return (s - str);
}

void cnstrct_mstrrls(mstrrls *variable){
    variable->strln = strln;
    variable->ststr = ststr;
}