#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os_file.h"

void get_cur_dir(char *dst);
int create(unsigned long long int disk_size);
int isCreated(fileNode *f);
int create_dir(const char* path);
int fileNodeCreateDir(const char* path, int fnSize, short int isDir);

typedef struct fileNode
{
    unsigned long long int disk_size;
    char* name;
    short int isDir;
    unsigned long long int heirsCount;

    struct fileNode *parent; 
    struct fileNode *heirs; //subdirs
    char* absolute_path;
    
} fileNode;


fileNode *FM = NULL, *CUR_DIR = NULL;

int fileNodeCreateDir(const char* path, int fnSize, short int isDir){
    if(path[0] == '/'){
         
    }
}

int create_dir(const char* path){ return isCreated(FM) == 0 ? 0 : fileNodeCreateDir(path, 0, 1); }

void get_cur_dir(char *dst){ strcpy(dst, CUR_DIR->absolute_path); }

int create(unsigned long long int disk_size){
    if(isCreated(FM) == 1){ fprintf(stdout, "error: file manager already exists\n"); return 0; }
    else if(disk_size <= 0){ fprintf(stdout, "error: wrong disk size\n"); return 0; }
    else{
        FM = (fileNode*)malloc(sizeof(fileNode));
        if(FM == NULL) return 0;
        // FM->name = (char*)malloc(sizeof(char));
        // FM->absolute_path = (char*)malloc(sizeof(char));
        // FM->name[0] = '/';
        // FM->absolute_path[0] = '/';
        FM->parent = NULL;
        FM->heirs = NULL;
        FM->heirsCount = 0;

        FM->disk_size = disk_size;
        fprintf(stdout, "success: file manager created with disk size %d bytes", FM->disk_size);
        return 1;
    }
}

int isCreated(fileNode *fn){ return fn == NULL; }