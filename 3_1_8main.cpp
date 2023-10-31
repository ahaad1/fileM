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

        fm.change_dir("cat");
        fm.create_dir("catcat");
        
        fm.change_dir("catcat");
        fm.change_dir("/");
        fm.change_dir("/cat/catcat");
        // fm.create_dir("catdir");
        // fileNodeGoToRoot();
        // fm.create_dir("cat/says");
        // fm.create_dir("cat/says/meow");
        // fm.create_dir("cat/doesnt");
        // fm.create_dir("cat/doesnt/says");
        // fm.create_dir("cat/doesnt/says/woof");
        // fm.create_dir("/dream_numbers");
        // fm.create_dir("/dream_numbers/420");
        // fm.create_dir("/henpecked_numbers");
        // fm.create_dir("/henpecked_numbers/69");

    // create(100);
    // create_dir("teest");
    // create_dir("teest1");
    // create_dir("teest2");
    // create_dir("teest3");
    // create_file("qweqweqwe.txt", 10);

    // int s_len = 0;
    // char **test = NULL;
    // parseStr((char*)"/qwe/123/zzz/qsq/qwedqweqwe/sdferfjknevjkner/qlkjefe", &test , &s_len);

    // for(int i = 0; i < s_len; ++i) {
    //     printf("%s\n", test[i]);
    // }
    return 0;
}