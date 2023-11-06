#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "os_file.h"

#define SWAP(T, a, b) do { T tmp = a; a = b; b = tmp; } while (0)

typedef struct fileNode
{
    unsigned long long int size;
    char* name;
    short int isDir;
    int heirsCount;
    fileNode* parent;
    fileNode** heirs; //subdirs
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
int fileNodeMkObjValidated(char *path, int mode, int fileSize);
int fileNodeGoToDirGlobal();
int fileNodeRmMode(char* path, int rec);
int fileNodeMoveFM(int mode, char *path);
void fileNodeGoToRoot();
void printDir();

int create_dir(const char* path){ return chkDsk(FM) == 0 ? 0 : fileNodeMkObjValidated((char*)path,1,0); }
int create_file(const char* path, int file_size) { return chkDsk(FM) == 0 ? 0 : fileNodeMkObjValidated((char*)path, 0, file_size); }
int change_dir(const char* path) { return chkDsk(FM) == 0 ? 0 : fileNodeChDirGlobal((char*)path); }
int remove(const char* path, int recursive){ return chkDsk(FM) == 0 ? 0 : fileNodeRmMode((char*)path, recursive); }

int fileNodeRmMode(char* path, int rec){
    if(path[0] != '/') fileNodeGoToDirGlobal();
    if(path[0] == '/' && strlen(path) == 1) return 0;
    if(path[0] == '/' && strlen(path) > 1) fileNodeGoToRoot();

    // while()

    SWAP(fileNode*, FM->heirs[0], FM->heirs[1]); //???
    return 1;
}

//mode = 0 working with absolute dir : with local
int fileNodeMoveFM(int mode, char *path){
    if(mode == 0 || strcmp(FM->absolute_path, CUR_DIR) != 0 || strlen(path) > 1) { return fileNodeGoToDirGlobal(); }
    if((strcmp(path, "..") == 0 && strlen(path) == 2) && FM->parent != NULL){ FM = FM->parent; return 1; }
    else if(strcmp(path, ".") == 0 && strlen(path) == 2) return 1;
    else if(strcmp(path, "/") == 0 && strlen(path) == 1){ fileNodeGoToRoot(); return 1; }
    else if(strlen(path) > 0){
        int chkdIndx = fileNodeGoDownChk(path);
        if( chkdIndx >= 0) { return fileNodeGoDown(chkdIndx); }
        else return 0;
    }
    else return 0;
}

int fileNodeChDirGlobal(char *path){
    char *token, *string, *toFree;
    if(path == NULL) return 0;
    string = strdup(path);
    if(string == NULL) return 0;
    toFree = string;
    while((token = strsep(&string, "/")) != NULL){ if(!fileNodeMoveFM(0, token)){ fprintf(stdout ,"cd error: %s does not exist in %s\n", token, FM->absolute_path); return 0; } }
    free(toFree);
    free(token);
    free(string);
    free(CUR_DIR);
    CUR_DIR = strdup(FM->absolute_path);

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
        if(chkdTokenIndx >= 0){ fileNodeGoDown(chkdTokenIndx); }
        else{ return 0; }
    }
    free(toFree);
    free(token);
    free(string);
    return 1;
}

//mode: 0 - file , 1 - directory
int fileNodeMkObjValidated(char *path, int mode, int fileSize){
    char *token, *string, *toFree;
    string = strdup(path);
    if(string == NULL) { return 0; }

    if(path[0] == '/') { fileNodeGoToRoot(); }
    if(path[0] != '/') { fileNodeGoToDirGlobal(); }
    toFree = string;
    while((token = strsep(&string, "/")) != NULL){
        if(strcmp(token, "") == 0 || strlen(token) <= 0 || token[0] == '\0') continue;
        int chkdIndx = fileNodeGoDownChk(token);
        if(chkdIndx == -1 && mode == 0 && (fileSize > DskSz || fileSize + OcpdSz > DskSz || fileSize < 0)) {  fprintf(stdout, "error: file not created\nwrong file size %d\n", fileSize); return 0; }
        if(chkdIndx == -1) fileNodeMkObj(token, mode, fileSize); 
        printDir();
        fileNodeGoDown(fileNodeGoDownChk(token));
    }
    free(toFree);
    free(string);
    free(token);
    return 1;
}

//mode: 0 - file , 1 - directory
int fileNodeMkObj(char *objName, int mode, int file_size){
    if(strcmp(objName, "") == 0 || strlen(objName) <= 0 || objName[0] == '\0') return 0;
    if(fileNodeGoDownChk(objName) >= 0) return 0;
    ++FM->heirsCount;
    if(FM->heirs == NULL) FM->heirs = (fileNode**)malloc(sizeof(fileNode*));
    else FM->heirs = (fileNode**)realloc(FM->heirs, sizeof(fileNode*) * FM->heirsCount);
    fileNode *newFm = (fileNode*)malloc(sizeof(fileNode));
    newFm->name = strdup(objName);
    newFm->parent = FM;
    newFm->isDir = mode;
    newFm->absolute_path = strdup(FM->absolute_path);
    strcat(newFm->absolute_path, objName);
    strcat(newFm->absolute_path, "/");
    newFm->heirs = NULL;
    newFm->heirsCount = 0;
    newFm->size = file_size;
    FM->heirs[FM->heirsCount - 1] = newFm;
    OcpdSz += file_size;
    if(mode == 1) fprintf(stdout, "success: created directory %s in %s\n", FM->heirs[FM->heirsCount - 1]->name, FM->absolute_path);
    if(mode == 0) fprintf(stdout, "success: created file %s with size %lld bytes\nFree space: %d bytes\n", FM->heirs[FM->heirsCount - 1]->name, FM->heirs[FM->heirsCount - 1]->size, DskSz - file_size);
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
    if(heirIndx == -1 || FM->heirs[heirIndx]->isDir == 0) return 0;
    FM = FM->heirs[heirIndx];
    return 1;
}

//returns index if child exists : -1 if not
int fileNodeGoDownChk(char *path){
    for(int i = 0; i < FM->heirsCount; ++i){
        if(strcmp(FM->heirs[i]->name, path) == 0){
            return i;
        }
    }
    return -1;
}

void fileNodeGoToRoot(){
    while(FM->parent != NULL){
        fprintf(stdout, "going to / { %s %s }\n", FM->absolute_path, FM->parent->absolute_path);
        FM = FM->parent;
    }
}

void printDir(){
    fprintf(stdout,"======\n");
    for(int i = 0; i < FM->heirsCount; ++i){
        fprintf(stdout, "%s\t%s\n",FM->absolute_path, FM->heirs[i]->absolute_path);
    }
    fprintf(stdout,"======\n");
}

void get_cur_dir(char *dst){ 
    // if(CUR_DIR != NULL) dst = strdup(CUR_DIR); 
    fprintf(stdout, "CUR_DIR: %s\n", CUR_DIR);
}

int chkDsk(fileNode *fn){ return fn == NULL ? 0 : 1; }

void setup_file_manager(file_manager_t *fm) {
    fm->create = create;
    fm->create_dir = create_dir;
    fm->create_file = create_file;
    fm->change_dir = change_dir;
    fm->get_cur_dir = get_cur_dir;
    fm->remove = remove;
    // fm->destroy = destroy;
    // fm->move = move;
}
