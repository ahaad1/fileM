#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os_file.h"

void get_cur_dir(char *dst);
int create(unsigned long long int disk_size);
int isCreated(fileNode *f);
int create_dir(const char* path);
int fileNodeCreateObj(const char* path, int fnSize, short int isDir);
int fileNodeCreateDir(const char* path);

typedef struct fileNode
{
    unsigned long long int disk_size;
    unsigned long long int size;
    char* name;
    short int isDir;
    int heirsCount;
    struct fileNode *parent; 
    struct fileNode *heirs; //subdirs
    char* absolute_path;
    
} fileNode;


fileNode *FM = NULL, *CUR_DIR = NULL;

int fileNodeCreateDir(char* path){
    ++FM->heirsCount;
    if(FM->heirs == NULL) FM->heirs = (fileNode*)malloc(sizeof(fileNode));
    FM->heirs = (fileNode*)realloc(FM->heirs, FM->heirsCount);
    FM->heirs[FM->heirsCount - 1].name = (char*)malloc(sizeof(char) * strlen(path));
    FM->heirs[FM->heirsCount - 1].parent = FM;
    FM->heirs[FM->heirsCount - 1].isDir = 1;
    FM->heirs[FM->heirsCount - 1].size = 0;
    FM->heirs[FM->heirsCount - 1].absolute_path = NULL;
    FM->heirs[FM->heirsCount - 1].heirs = NULL;
    FM->heirs[FM->heirsCount - 1].heirsCount = 0;
    return 1;
}

int create_dir(const char* path){ return isCreated(FM) == 0 ? 0 : fileNodeCreateDir(path); }

void get_cur_dir(char *dst){ strcpy(dst, CUR_DIR->absolute_path); }

int create(unsigned long long int disk_size){
    if(isCreated(FM) == 1){ fprintf(stdout, "error: file manager already exists\n"); return 0; }
    else if(disk_size <= 0){ fprintf(stdout, "error: wrong disk size\n"); return 0; }
    else{
        FM = (fileNode*)malloc(sizeof(fileNode));
        if(FM == NULL) return 0;
        FM->parent = NULL;
        FM->heirs = NULL;
        FM->heirsCount = 0;
        FM->size = 0;
        FM->disk_size = disk_size;
        create_dir("/");
        fprintf(stdout, "success: file manager created with disk size %d bytes", FM->disk_size);
        return 1;
    }
}

int isCreated(fileNode *fn){ return fn == NULL; }