#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os_file.h"

void get_cur_dir(char *dst);
int create(unsigned long long int disk_size);
int isCreated(fileNode *f);

typedef struct fileNode
{
    unsigned long long int disk_size;
    char* name;
    short int is_dir;

    struct fileNode *left;
    struct fileNode *right;
    struct fileNode *parent;
    struct fileNode *child;
    char* absolute_path;
    
} fileNode;

fileNode *FM = NULL, *CUR_DIR = NULL;

void get_cur_dir(char *dst){
    strcpy(dst, CUR_DIR->absolute_path);
}
int create(unsigned long long int disk_size){
    if(isCreated(FM) == 1){ fprintf(stdout, "error: file manager already exists\n"); return 0; }
    else if(disk_size <= 0){ fprintf(stdout, "error: wrong disk size\n"); return 0; }
    else{
        FM = (fileNode*)malloc(sizeof(fileNode));
        if(FM == NULL) return 0;
        FM->name = (char*)malloc(sizeof(char));
        FM->absolute_path = (char*)malloc(sizeof(char));
        FM->name[0] = '/';
        FM->absolute_path[0] = '/';
        FM->left = NULL;
        FM->right = NULL;
        FM->parent = NULL;
        FM->child = NULL;
        FM->is_dir = 1;

        FM->disk_size = disk_size;
        fprintf(stdout, "success: file manager created with disk size %d bytes", FM->disk_size);
        return 1;
    }
}

int isCreated(fileNode *fn){ return fn == NULL; }