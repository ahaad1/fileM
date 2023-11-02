#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "3_1_8task.h"

int main()
{
        file_manager_t fm;
        setup_file_manager(&fm);
        fm.create(42069);
        fm.create_dir("cat");
        fm.create_dir("cat/says");
        fm.create_dir("cat/says/meow");
        fm.create_dir("cat/doesnt");
        fm.create_dir("cat/doesnt/says");
        fm.create_dir("cat/doesnt/says/woof");
        fm.create_dir("/dream_numbers");
        fm.create_dir("/dream_numbers/420");
        fm.create_dir("/henpecked_numbers");
        fm.create_dir("/henpecked_numbers/69");

        fm.create_file("cat.meow", 10);
        fm.create_file("cat.meow", 10);
        fm.create_file("cat.meow", 10);
        fm.change_dir("cat/says");
        fm.change_dir("/henpecked_numbers/69");
        fm.change_dir("/");
        fm.get_cur_dir((char*)"");
        

        fm.create_file("/henpecked_numbers/69/poooof.rrr", 1212);
        fm.change_dir("/");
        printDir();
    return 0;
}