#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os_file.h"

typedef struct fileNode
{
    unsigned long long int size;
    char* name;
    short int isDir;
    int heirsCount;
    struct fileNode* parent;
    struct fileNode* heirs; //subdirs
    char* absolute_path;

} fileNode;

int DskSz = -1, OcpdSz = 0; fileNode *FM = NULL; char *CUR_DIR = NULL;

void setup_file_manager(file_manager_t *fm);
int create_dir(const char* path);
int change_dir(const char* path);
int create_file(const char* path, int file_size);
void get_cur_dir(char *dst);
int create(unsigned long long int disk_size);

int chkDsk(fileNode *fn);
int fileNodeMkObj(char *path, int mode, int file_size);
int fileNodeChDirGlobal(char *path);
int fileNodeGoDown(int heirIndx);
int fileNodeGoDownChk(char *path);
int fileNodeGoUp(char *path);
int fileNodeMkObjValidated(char *path, int mode, int fileSize);
int fileNodeGoToDirGlobal();
void fileNodeGoToRoot();

int create_dir(const char* path){ return chkDsk(FM) == 0 ? 0 : fileNodeMkObjValidated((char*)path,1,0); }
int create_file(const char* path, int file_size) { return chkDsk(FM) == 0 ? 0 : fileNodeMkObjValidated((char*)path, 0, file_size); }
int change_dir(const char* path) { return chkDsk(FM) == 0 ? 0 : fileNodeChDirGlobal((char*)path); }


int fileNodeChDirGlobal(char *path){
    if(FM->)
    fileNodeGoToDirGlobal();
    if(path[0] == '/' && strlen(path) == 1){ fileNodeGoToRoot(); return 1; }
    if(path[0] == '/') fileNodeGoToRoot();
    char *token, *string, *toFree;
    string = strdup(path);
    if(string != NULL){
        toFree = string;
        while((token = strsep(&string, "/")) != NULL){
            if(strcmp(token, "") == 0 || strlen(token) <= 0 || token[0] == '\0') continue;
            int chkdTokenIndx = fileNodeGoDownChk(token);
            if(chkdTokenIndx >= 0){
                fprintf(stdout, "chdir global: %s %s\n", CUR_DIR, FM->heirs[chkdTokenIndx].absolute_path);
                fileNodeGoDown(chkdTokenIndx);
                free(CUR_DIR);
                CUR_DIR = strdup(FM->absolute_path);
            }
            else{
                fprintf(stdout, "chdir error global: no such directory %s in %s\n", token, FM->absolute_path);
            }
        }
        free(toFree);
    }
    free(token);
    free(string);
    return 1;
}

int fileNodeGoToDirGlobal(){
    fileNodeGoToRoot();
    char *token, *string, *toFree;
    string = strdup(CUR_DIR);
    if(string == NULL) return 0;
    toFree = string;
    while((token = strsep(&string, "/")) != NULL){
        if(strcmp(token, "") == 0 || strlen(token) <= 0 || token[0] == '\0') continue;
        int chkdTokenIndx = fileNodeGoDownChk(token);
        if(chkdTokenIndx >= 0){
            fprintf(stdout, "current dir: %s %s\n", CUR_DIR, FM->heirs[chkdTokenIndx].absolute_path);
            fileNodeGoDown(chkdTokenIndx);
        }
        else{
            fprintf(stdout, "current dir error: no such directory %s in %s\n", token, FM->absolute_path);
            return 0;
        }
    }
    free(toFree);
    return 1;
}

//mode: 0 - file , 1 - directory
int fileNodeMkObjValidated(char *path, int mode, int fileSize){
    char *token, *string, *toFree;
    string = strdup(path);
    if(string == NULL) return 0;

    if(path[0] == '/') { fileNodeGoToRoot(); }
    if(path[0] != '/') { fileNodeGoToDirGlobal(); }
    toFree = string;
    while((token = strsep(&string, "/")) != NULL){
        if(strcmp(token, "") == 0 || strlen(token) <= 0 || token[0] == '\0') continue;
        
        int chkdIndx = fileNodeGoDownChk(token);

        if(chkdIndx == -1) { fileNodeMkObj(token, mode, fileSize); }
        if(mode == 1) fileNodeGoDown(fileNodeGoDownChk(token));
    }
    free(toFree);
    return 1;
}

//mode: 0 - file , 1 - directory
int fileNodeMkObj(char *objName, int mode, int file_size){
    if(strcmp(objName, "") == 0 || strlen(objName) <= 0 || objName[0] == '\0') return 0;
    if(file_size > DskSz || file_size + OcpdSz > DskSz || file_size < 0) {
        fprintf(stdout, "error: wrong file size %d\n", file_size);
        return 0;
    }
    ++FM->heirsCount;
    if(FM->heirs == NULL) FM->heirs = (fileNode*)malloc(sizeof(fileNode));
    else FM->heirs = (fileNode*)realloc(FM->heirs, sizeof(fileNode) * FM->heirsCount);
    FM->heirs[FM->heirsCount - 1].name = strdup(objName);
    FM->heirs[FM->heirsCount - 1].parent = FM;
    FM->heirs[FM->heirsCount - 1].isDir = mode;
    FM->heirs[FM->heirsCount - 1].size = file_size;
    FM->heirs[FM->heirsCount - 1].absolute_path = strdup(FM->absolute_path);
    strcat(FM->heirs[FM->heirsCount - 1].absolute_path, objName);
    strcat(FM->heirs[FM->heirsCount - 1].absolute_path, "/");
    FM->heirs[FM->heirsCount - 1].heirs = NULL;
    FM->heirs[FM->heirsCount - 1].heirsCount = 0;
    OcpdSz += file_size;
    if(mode == 1) fprintf(stdout, "success: created directory %s in %s\n", FM->heirs[FM->heirsCount - 1].name, FM->absolute_path);
    if(mode == 0) fprintf(stdout, "success: created file %s with size %lld bytes\nFree space: %d bytes\n", FM->heirs[FM->heirsCount - 1].absolute_path, FM->heirs[FM->heirsCount - 1].size, DskSz - file_size);
    return 1;
}

int create(int disk_size){
    if(chkDsk(FM)){ fprintf(stdout, "error: file manager already exists\n"); return 0; }
    else if(disk_size <= 0){ fprintf(stdout, "error: wrong disk size\n"); return 0; }
    else{
        FM = (fileNode*)malloc(sizeof(fileNode));
        if(FM == NULL) return 0;
        FM->parent = NULL;
        FM->heirs = NULL;
        FM->heirsCount = 0;
        FM->size = 0;
        FM->absolute_path = strdup("/");
        FM->name = strdup("/");
        FM->isDir = 1;
        DskSz = disk_size;
        fprintf(stdout, "success: file manager created with disk size %d bytes\n", DskSz);
        fprintf(stdout, "root created successfully\n");
        CUR_DIR = strdup(FM->absolute_path);
        return 1;
    }
}

int fileNodeGoDown(int heirIndx){
    fprintf(stdout, "chdir %s %s\n", FM->absolute_path, FM->heirs[heirIndx].absolute_path);
    FM = &FM->heirs[heirIndx];
    return 1;
}

int fileNodeGoDownChk(char *path){
    // fprintf(stdout, "chk is exists %s in %s\n", path, FM->absolute_path);
    for(int i = 0; i < FM->heirsCount; ++i){
        // fprintf(stdout, "CHECKING ---%s--- ===%s===\n", FM->heirs[i].name, path);
        if(strcmp(FM->heirs[i].name, path) == 0 && FM->heirs[i].isDir == 1){
            // fprintf(stdout, "%s exists in %s\n", path, FM->absolute_path);
            return i;
        }
    }
    // fprintf(stdout, "%s does not exists in %s\n", path, FM->absolute_path);
    return -1;
}

int fileNodeGoUp(char *path){
    if(FM->parent!= NULL){ FM = FM->parent; return 1; }
    return 0;
}

void fileNodeGoToRoot(){
    while(FM->parent != NULL){
        fprintf(stdout, "going to / { %s %s }\n", FM->absolute_path, FM->parent->absolute_path);
        FM = FM->parent;
    }
}

void get_cur_dir(char *dst){ strcpy(dst, CUR_DIR); }

int chkDsk(fileNode *fn){ return fn == NULL ? 0 : 1; }

void setup_file_manager(file_manager_t *fm) {
    fm->create = create;
    fm->create_dir = create_dir;
    fm->create_file = create_file;
    fm->change_dir = change_dir;
    fm->get_cur_dir = get_cur_dir;
    // fm->destroy = fm_destroy;
    // fm->remove = fm_remove;
    // fm->move = fm_move;
}

