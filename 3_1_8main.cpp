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

        // fm.change_dir("cat");
        fm.create_dir("/cat/meow");
        
        fm.change_dir("/cat");
        fm.create_dir("woof");
        fm.change_dir("/");
        // fm.create_dir("cat/says");
        // fm.create_dir("cat/says/meow");
        // fm.create_dir("cat/doesnt");
        // fm.create_dir("cat/doesnt/says");
        // fm.create_dir("cat/doesnt/says/woof");
        // fm.create_dir("/dream_numbers");
        // fm.create_dir("/dream_numbers/420");
        // fm.create_dir("/henpecked_numbers");
        // fm.create_dir("/henpecked_numbers/69");
    return 0;
}