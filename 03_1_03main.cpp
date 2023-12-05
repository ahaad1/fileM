#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "03_1_03task.cpp"
// #include "os_file.h"
using namespace std;

int main()
{
    file_manager_t fm;
    setup_file_manager(&fm);
    cout << (fm.create(1000) == 1);                         //, passed
    cout << (fm.create_dir("dir1") == 1);                   //, passed
    cout << (fm.create_dir("dir1/dir11") == 1);             //, passed
    cout << (fm.change_dir("dir1") == 1);                   //, passed
    cout << (fm.create_dir("../dir2") == 1);                //, passed
    cout << (fm.create_dir("../dir2/dir3") == 1);           //, passed
    cout << (fm.remove("/dir2/dir3", 0) == 1);              //, passed
    cout << (fm.create_dir("/dir3/dir31") == 0);            //, passed

    // loop_tree("/");
    // printTree(__ind);


    cout << (fm.create_dir("../dir3/dir31") == 0);          //, passed ??? 
    cout << (fm.create_dir("../dir2") == 0);                //, passed
    cout << (fm.create_file("/dir2/file1", 1) == 1);        //, passed ///
    cout << (fm.create_dir("/dir2/dir21") == 1);            //, passed
    

    printf("==============\n");
    loop_tree("/");
    printTree(__ind);
    printf("==============\n");
    
    cout << (fm.create_dir("/dir2/file1") == 0);            //, passed///
    
    printf("----------------\n");
    loop_tree("/");
    printTree(__ind);
       printf("----------------\n");
    
    cout << (fm.create_dir("../dir2/file1") == 0);          //, passed///
    cout << (fm.create_dir("../dir2/file1/dir") == 0);      //, passed
    cout << (fm.create_dir("../dir2/dir22") == 1);          //, passed
    cout << (fm.create_dir("..") == 0);                     //, passed
    cout << (fm.create_dir("../dir2/.") == 0);              //, passed
    cout << (fm.change_dir("dir2") == 0);                   //, passed
    cout << (fm.change_dir("dir11") == 1);                  //, passed

    

    cout << (fm.remove("../../dir2/file1", 0) == 1);        //, failed
    
    // loop_tree("/");
    // printTree(__ind);
    
    cout << (fm.create_dir("../../dir2/file1") == 1);       //, passed
    cout << (fm.remove("../../dir2/file1", 0) == 1);        //, passed
    cout << (fm.create_dir("../../dir2/file1") == 1);       //, passed
    cout << (fm.remove("../../dir2/file1", 0) == 1);        //, passed
    cout << (fm.create_file("../../dir2/file1", 1) == 1);   //, passed
    cout << (fm.change_dir(".") == 1);                      //, passed
    cout << (fm.change_dir("/dir1/dir11") == 1);            //, passed
    cout << (fm.change_dir("/dir1/dir11/dir3") == 0);       //, passed
    cout << (fm.change_dir("/dir1") == 1);                  //, passed
    cout << (fm.change_dir("./dir11") == 1);                //, passed
    cout << (fm.change_dir("..") == 1);                     //, passed
    cout << (fm.create_file("ddir2/file1", 1000 - 1) == 0); //, passed
    cout << (fm.create_file("./dir11/file.txt", 1) == 1);   //, passed
    cout << (fm.remove("dir11", 0) == 0);                   //, passed
    cout << (fm.remove("./dir11", 0) == 0);                 //, passed
    cout << (fm.remove("./dir11", 1) == 1) << endl;         //, passed
    return 0;
}