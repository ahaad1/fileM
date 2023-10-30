#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "3_1_8task.h"

int main()
{
    // create(100);
    // create_dir("teest");
    // create_dir("teest1");
    // create_dir("teest2");
    // create_dir("teest3");
    // create_file("qweqweqwe.txt", 10);
    int s_len = 0;
    char **test = NULL;
    parseStr((char*)"/qwe/123/zzz/qqqeqeqeqeqeqeqeq", &test , &s_len);

    // fprintf(stdout, "%s", test[0]);
    // create(10000);
    return 0;
}