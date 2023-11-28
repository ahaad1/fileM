#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "03_1_08task.cpp"
// #include "os_file.h"

int main()
{
    file_manager_t fm;
    setup_file_manager(&fm);

    // // Common file manager test 1
    printf("%d\t", fm.create(1000)); /* expected: 1, passed */
    printf("%d\t", fm.create_file("test_file",1000+1)); /* expected: 0, passed */
    printf("%d\t", fm.destroy()); /* expected: 1, passed */
    printf("%d\t", fm.destroy()); /* expected: 0, passed */
    printf("%d\t", fm.create(1000)); /* expected: 1, passed */
    printf("%d\t", fm.create(1000)); /* expected: 0, passed */
    printf("%d\t", fm.destroy()); /* expected: 1, passed */
    printf("\n\n");
    // // Test 1 passed

    // // Common file manager test 2
    printf("%d\t", fm.create(1000)); /* expected: 1, passed */
    printf("%d\t", fm.create_file("file1.txt",1000)); /* expected: 1, passed */
    printf("%d\t", fm.create_file("file2.txt",1)); /* expected: 0, passed */
    printf("%d\t", fm.remove("file1.txt",0)); /* expected: 1, passed */
    
    // fm.change_dir("/");
    // printTree(__ind);
    // printf("\n=================\n");
    
    printf("%d\t", fm.remove("file1.txt",0)); /* expected: 0, passed */
    printf("%d\t", fm.create_file("file2.txt",1000/2)); /* expected: 1, passed */
    printf("%d\t", fm.create_file("file2.txt",1000/2)); /* expected: 0, passed */
    printf("%d\t", fm.create_file("file3.txt",1000/2)); /* expected: 1, passed */
    printf("%d\t", fm.create_dir("test_dir")); /* expected: 1, passed */
    printf("%d\t", fm.remove("file2.txt",0)); /* expected: 1, passed */
    printf("%d\t", fm.destroy()); /* expected: 1, passed */
    printf("\n\n");
    // // Test 2 passed

    // // Common file manager test 3
    printf("%d\t",  fm.create(1000)); /* expected: 1, passed */
    printf("%d\t",  fm.create_dir("dir1")); /* expected: 1, passed */
    printf("%d\t",  fm.create_dir("dir1/dir11")); /* expected: 1, passed */
    printf("%d\t",  fm.change_dir("dir1")); /* expected: 1, passed */
    printf("%d\t",  fm.create_dir("../dir2")); /* expected: 1, passed */
    printf("%d\t",  fm.create_dir("../dir2/dir3")); /* expected: 1, passed */
    printf("%d\t", fm.remove("/dir2/dir3", 0)); /* expected: 1, passed */
    printf("%d\t", fm.create_dir("/dir3/dir31")); /* expected: 0, */     

    fm.destroy();

    // printf("\n===test3===\n");
    // fm.change_dir("/");
    // printTree(__ind);
    // printf("\n=================\n");
    
    // // Test 3 failed

    fm.create(2023);
    fm.create_dir("/a");
    fm.create_dir("/a");
    fm.create_dir("/a");
    fm.create_dir("/a/b");
    fm.create_file("/a/a.txt", 10);
    fm.change_dir("/a/");
    fm.change_dir("/a/a.txt");
    fm.create_dir("/a/b/c");
    fm.create_file("/a/qwe.txt", 100);
    fm.change_dir("/a");
    fm.change_dir("/a/b/c/a");
    fm.create(100);
    fm.create_dir("/Desktop");
    fm.create_dir("/Documents");
    fm.create_dir("/Downloads");
    fm.create_dir("/file_Manager");
    fm.change_dir("/Desktop");
    fm.create_dir("./lesson");
    fm.create_dir("./python_Course_Tinkoff");
    fm.change_dir("./lesson");
    fm.create_dir("./a.out");
    fm.create_file("./a.out", 1);
    fm.create_file("./FCFS", 1);
    fm.create_file("./input.txt", 1);
    fm.change_dir("../python_Course_Tinkoff");
    fm.create_file("./main.py", 1);
    fm.create_file("./taxis", 1);
    fm.create_file("./test.ipynb", 1);
    fm.change_dir("../../Downloads");
    fm.create_dir("/Telegram Desktop");
    fm.change_dir("./Telegram Desktop");
    fm.create_file("./Compiler Explorer code.cpp", 1);
    fm.create_file("./FileName(2).cpp", 1);
    fm.create_file("./makefile", 1);
    fm.create_file("./New_Sort", 1);
    fm.change_dir("..");
    fm.create_file("main.cpp", 1);
    fm.create_file("os_file.h", 1);
    fm.change_dir("../file_Manager");
    fm.create_file("./22_1_2task", 1);
    fm.create_file("./22_1_2task.c", 1);
    fm.create_file("./os_file", 1);
    fm.change_dir("/");
    fm.create_file("file1.txt",1000);
    fm.remove("file1.txt",0);
    fm.remove("file1.txt",0);
    fm.change_dir("/");
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
    fm.destroy();
    // // prnt(&__ind);
    return 0;
}