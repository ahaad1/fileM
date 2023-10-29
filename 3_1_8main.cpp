#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "3_1_8task.h"

int main()
{
    // fileNode fn;
    create(100);
    create_dir("teest");
    create_dir("teest1");
    create_dir("teest2");
    create_dir("teest3");
    create_file("qweqweqwe.txt", 10);
    return 0;
}