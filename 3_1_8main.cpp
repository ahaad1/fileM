#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "3_1_8task.h"

int main()
{
    file_manager_t fm;
    setup_file_manager(&fm);
    fm.create(101010);
    fm.create_dir("/a");
    fm.create_dir("/a");
    fm.create_dir("/a");
    fm.create_dir("/a/b");
    fm.create_file("/a/a.txt", 10);
    fm.change_dir("/a/");
    fm.change_dir("/a/a.txt");
    fm.remove("/a/", 1);
    // fm.create_dir("/a/b/c");
    // fm.create_file("/a/qwe.txt", 100);
    // fm.remove("/", 0);
    // fm.change_dir("/a");
    // prnt(&__ind);
    return 0;
}