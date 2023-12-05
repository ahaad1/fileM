#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
// #include "03_1_03task.cpp"
#include "os_file.h"
using namespace std;

int main()
{
    file_manager_t fm;
    setup_file_manager(&fm);

    // fm.create(2023);
    // fm.create_dir("/a");
    // fm.create_dir("/a");
    // fm.create_dir("/a");
    // fm.create_dir("/a/b");
    // fm.create_file("/a/a.txt", 10);
    // fm.change_dir("/a/");
    // fm.change_dir("/a/a.txt");
    // fm.create_dir("/a/b/c");
    // fm.create_file("/a/qwe.txt", 100);
    // fm.change_dir("/a");
    // fm.change_dir("/a/b/c/a");
    // fm.create(100);
    // fm.create_dir("/Desktop");
    // fm.create_dir("/Documents");
    // fm.create_dir("/Downloads");
    // fm.create_dir("/file_Manager");
    // fm.change_dir("/Desktop");
    // fm.create_dir("./lesson");
    // fm.create_dir("./python_Course_Tinkoff");
    // fm.change_dir("./lesson");
    // fm.create_dir("./a.out");
    // fm.create_file("./a.out", 1);
    // fm.create_file("./FCFS", 1);
    // fm.create_file("./input.txt", 1);
    // fm.change_dir("../python_Course_Tinkoff");
    // fm.create_file("./main.py", 1);
    // fm.create_file("./taxis", 1);
    // fm.create_file("./test.ipynb", 1);
    // fm.change_dir("../../Downloads");
    // fm.create_dir("/Telegram Desktop");
    // fm.change_dir("./Telegram Desktop");
    // fm.create_file("./Compiler Explorer code.cpp", 1);
    // fm.create_file("./FileName(2).cpp", 1);
    // fm.create_file("./makefile", 1);
    // fm.create_file("./New_Sort", 1);
    // fm.change_dir("..");
    // fm.create_file("main.cpp", 1);
    // fm.create_file("os_file.h", 1);
    // fm.change_dir("../file_Manager");
    // fm.create_file("./22_1_2task", 1);
    // fm.create_file("./22_1_2task.c", 1);
    // fm.create_file("./os_file", 1);
    // fm.change_dir("/");
    // fm.create_file("file1.txt",1000);
    // fm.remove("file1.txt",0);
    // fm.remove("file1.txt",0);
    // fm.change_dir("/");
    // fm.create_dir("cat");
    // fm.create_dir("cat/says");
    // fm.create_dir("cat/says/meow");
    // fm.create_dir("cat/doesnt");
    // fm.create_dir("cat/doesnt/says");
    // fm.create_dir("cat/doesnt/says/woof");
    // fm.create_dir("/dream_numbers");
    // fm.create_dir("/dream_numbers/420");
    // fm.create_dir("/henpecked_numbers");
    // fm.create_dir("/henpecked_numbers/69");
    // fm.destroy();
    // // t2.1
    // cout << (fm.create(1000) == 1);                       //, passed
    // cout << (fm.create_file("test_file", 1000 + 1) == 0); //, passed
    // cout << (fm.destroy() == 1);                          //, passed
    // cout << (fm.destroy() == 0);                          //, passed
    // cout << (fm.create(1000) == 1);                       //, passed
    // cout << (fm.create(1000) == 0);                       //, passed
    // cout << (fm.destroy() == 1) << endl;                          //, passed

    // // t2.1
    // cout << (fm.create(1000) == 1);                       //, passed
    // cout << (fm.create_file("file1.txt", 1000) == 1);     //, passed
    // cout << (fm.create_file("file2.txt", 1) == 0);        //, passed
    // cout << (fm.remove("file1.txt", 0) == 1);             //, passed
    // cout << (fm.remove("file1.txt", 0) == 0);             //, passed
    // cout << (fm.create_file("file2.txt", 1000 / 2) == 1); //, passed
    // cout << (fm.create_file("file2.txt", 1000 / 2) == 0); //, passed
    // cout << (fm.create_file("file3.txt", 1000 / 2) == 1); //, passed
    // cout << (fm.create_dir("test_dir") == 1);             //, passed
    // cout << (fm.remove("file2.txt", 0) == 1);             //, passed
    // cout << (fm.destroy() == 1) << endl;                          //, passed

    // // t2.3
    // cout << (fm.create(1000) == 1);                         //, passed
    // cout << (fm.create_dir("dir1") == 1);                   //, passed
    // cout << (fm.create_dir("dir1/dir11") == 1);             //, passed
    // cout << (fm.change_dir("dir1") == 1);                   //, passed
    // cout << (fm.create_dir("../dir2") == 1);                //, passed
    // cout << (fm.create_dir("../dir2/dir3") == 1);           //, passed
    // cout << (fm.remove("/dir2/dir3", 0) == 1);              //, passed
    // cout << (fm.create_dir("/dir3/dir31") == 0);            //, passed
    // cout << (fm.create_dir("../dir3/dir31") == 0);          //, passed
    // cout << (fm.create_dir("../dir2") == 0);                //, passed
    // cout << (fm.create_file("/dir2/file1", 1) == 1);        //, passed
    // cout << (fm.create_dir("/dir2/dir21") == 1);            //, passed
    // cout << (fm.create_dir("/dir2/file1") == 0);            //, passed
    // cout << (fm.create_dir("../dir2/file1") == 0);          //, passed
    // cout << (fm.create_dir("../dir2/file1/dir") == 0);      //, passed
    // cout << (fm.create_dir("../dir2/dir22") == 1);          //, passed
    // cout << (fm.create_dir("..") == 0);                     //, passed
    // cout << (fm.create_dir("../dir2/.") == 0);              //, passed
    // cout << (fm.change_dir("dir2") == 0);                   //, passed ???
    // cout << (fm.change_dir("dir11") == 1);                  //, passed ???
    // cout << (fm.remove("../../dir2/file1", 0) == 1);        //, failed
    // cout << (fm.create_dir("../../dir2/file1") == 1);       //, passed
    // cout << (fm.remove("../../dir2/file1", 0) == 1);        //, passed
    // cout << (fm.create_dir("../../dir2/file1") == 1);       //, passed
    // cout << (fm.remove("../../dir2/file1", 0) == 1);        //, passed
    // cout << (fm.create_file("../../dir2/file1", 1) == 1);   //, passed
    // cout << (fm.change_dir(".") == 1);                      //, passed
    // cout << (fm.change_dir("/dir1/dir11") == 1);            //, passed
    // cout << (fm.change_dir("/dir1/dir11/dir3") == 0);       //, passed
    // cout << (fm.change_dir("/dir1") == 1);                  //, passed
    // cout << (fm.change_dir("./dir11") == 1);                //, passed
    // cout << (fm.change_dir("..") == 1);                     //, passed
    // cout << (fm.create_file("ddir2/file1", 1000 - 1) == 0); //, passed
    // cout << (fm.create_file("./dir11/file.txt", 1) == 1);   //, passed
    // cout << (fm.remove("dir11", 0) == 0);                   //, passed
    // cout << (fm.remove("./dir11", 0) == 0);                 //, passed
    // cout << (fm.remove("./dir11", 1) == 1) << endl;         //, passed
    // fm.list("/ ./dir11/file.txt", 1);
    // fm.destroy();

    fm.create(2023);
    fm.create_dir("/a");
    fm.create_dir("/a");
    fm.create_dir("/a");
    fm.create_file("/qweret.txt", 10);
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
    printf("\n\n");
    // fm.list("/ /a/b /a/qwe.txt /a", 0);
    fm.destroy();
    return 0;
}