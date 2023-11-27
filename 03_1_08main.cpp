#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "03_1_08task.cpp"
#include "os_file.h"

int main()
{
    file_manager_t fm;
    setup_file_manager(&fm);

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