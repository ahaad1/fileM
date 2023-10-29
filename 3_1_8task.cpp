#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os_file.h"

typedef struct fileNode
{
    unsigned long long int dsksz;
    unsigned long long int size;
    char* name;
    short int isDir;
    int heirsCount;
    struct fileNode* parent; 
    struct fileNode* heirs; //subdirs
    char* absolute_path;
    
} fileNode;

fileNode *FM = NULL, *CUR_DIR = NULL;

void get_cur_dir(char *dst);
int create(unsigned long long int disk_size);
int chkDisk(fileNode *fn);
int create_dir(const char* path);
int create_file(const char* path, int file_size);
int fileNodeMkObj(char *path, int mode, int file_size);

// int fileNodeMkDir(char* path){
//     if(path[0] != '/'){
//         ++FM->heirsCount;
//         if(FM->heirs == NULL) FM->heirs = (fileNode*)malloc(sizeof(fileNode));
//         FM->heirs = (fileNode*)realloc(FM->heirs, sizeof(fileNode) * FM->heirsCount);
//         FM->heirs[FM->heirsCount - 1].name = (char*)malloc(sizeof(char) * strlen(path));
//         FM->heirs[FM->heirsCount - 1].parent = FM;
//         FM->heirs[FM->heirsCount - 1].isDir = 1;
//         FM->heirs[FM->heirsCount - 1].size = 0;
//         FM->heirs[FM->heirsCount - 1].absolute_path = NULL;
//         FM->heirs[FM->heirsCount - 1].heirs = NULL;
//         FM->heirs[FM->heirsCount - 1].heirsCount = 0;
//         fprintf(stdout, "success: created directory %s\n", path);
//         return 1;
//     }
//     return 1;
// }
// int fileNodeTouch(char* path, unsigned long long int size){
// }

int fileNodeMkObj(char *path, int mode, int file_size){
    if(path[0] != '/'){
        ++FM->heirsCount;
        if(FM->heirs == NULL) FM->heirs = (fileNode*)malloc(sizeof(fileNode));
        FM->heirs = (fileNode*)realloc(FM->heirs, sizeof(fileNode) * FM->heirsCount);
        FM->heirs[FM->heirsCount - 1].name = (char*)malloc(sizeof(char) * strlen(path));
        FM->heirs[FM->heirsCount - 1].parent = FM;
        FM->heirs[FM->heirsCount - 1].isDir = mode;
        FM->heirs[FM->heirsCount - 1].size = file_size;
        FM->heirs[FM->heirsCount - 1].absolute_path = NULL;
        FM->heirs[FM->heirsCount - 1].heirs = NULL;
        FM->heirs[FM->heirsCount - 1].heirsCount = 0;
        FM->dsksz -= file_size;
        if(mode == 1) fprintf(stdout, "success: created directory %s\n", path);
        if(mode == 0) fprintf(stdout, "success: created file %s with size %d bytes in %s\n", path, file_size, FM->name);
        return 1;
    }
    return 1;

}

int create_dir(const char* path){ return chkDisk(FM) == 0 ? 0 : fileNodeMkObj((char*)path,1,0); }
int create_file(const char* path, int file_size) { return chkDisk(FM) == 0 ? 0 : fileNodeMkObj((char*)path, 0, file_size); }

int create(unsigned long long int disk_size){
    if(chkDisk(FM)){ fprintf(stdout, "error: file manager already exists\n"); return 0; }
    else if(disk_size <= 0){ fprintf(stdout, "error: wrong disk size\n"); return 0; }
    else{
        FM = (fileNode*)malloc(sizeof(fileNode));
        if(FM == NULL) return 0;
        FM->parent = NULL;
        FM->heirs = NULL;
        FM->heirsCount = 0;
        FM->size = 0;
        FM->dsksz = disk_size;
        FM->absolute_path = (char*)malloc(sizeof(char));
        FM->name = (char*)malloc(sizeof(char));
        FM->absolute_path[0] = '/';
        FM->name[0] = '/';
        FM->isDir = 1;
        fprintf(stdout, "success: file manager created with disk size %d bytes\n", FM->dsksz);
        fprintf(stdout, "root created successfully\n");
        return 1;
    }
}

int chkDisk(fileNode *fn){ return fn == NULL ? 0 : 1; }