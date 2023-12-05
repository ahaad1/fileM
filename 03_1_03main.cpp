#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
// #include "03_1_03task.cpp"
#include "os_file.h"
using namespace std;

int main()
{
    // // TEST 2
    // file_manager_t fm;
    // setup_file_manager(&fm);
    // /* Common file manager test 1 */
    // fm.create(1000); expected: 1
    // fm.create_file("test_file",1000+1); expected: 0
    // fm.destroy(); expected: 1
    // fm.destroy(); expected: 0
    // fm.create(1000); expected: 1
    // fm.create(1000); expected: 0
    // fm.destroy(); expected: 1
    // /* Common file manager test 2 */
    // fm.create(1000); expected: 1
    // fm.create_file("file1.txt",1000); expected: 1
    // fm.create_file("file2.txt",1); expected: 0
    // fm.remove("file1.txt",0); expected: 1
    // fm.remove("file1.txt",0); expected: 0
    // fm.create_file("file2.txt",1000/2); expected: 1
    // fm.create_file("file2.txt",1000/2); expected: 0
    // fm.create_file("file3.txt",1000/2); expected: 1
    // fm.create_dir("test_dir"); expected: 1
    // fm.remove("file2.txt",0); expected: 1
    // fm.destroy(); expected: 1
    // /* Common file manager test 3 */
    // fm.create(1000); expected: 1
    // fm.create_dir("dir1"); expected: 1
    // fm.create_dir("dir1/dir11"); expected: 1
    // fm.change_dir("dir1"); expected: 1
    // fm.create_dir("../dir2"); expected: 1
    // fm.create_dir("../dir2/dir3"); expected: 1
    // fm.remove("/dir2/dir3", 0); expected: 1
    // fm.create_dir("/dir3/dir31"); expected: 0
    // fm.create_dir("../dir3/dir31"); expected: 0
    // fm.create_dir("../dir2"); expected: 0
    // fm.create_file("/dir2/file1", 1); expected: 1
    // fm.create_dir("/dir2/dir21"); expected: 1
    // fm.create_dir("/dir2/file1"); expected: 0
    // fm.create_dir("../dir2/file1"); expected: 0
    // fm.create_dir("../dir2/file1/dir"); expected: 0
    // fm.create_dir("../dir2/dir22"); expected: 1
    // fm.create_dir(".."); expected: 0
    // fm.create_dir("../dir2/."); expected: 0
    // fm.change_dir("dir2"); expected: 0
    // fm.change_dir("dir11"); expected: 1
    // fm.remove("../../dir2/file1", 0); expected: 1
    // fm.create_dir("../../dir2/file1"); expected: 1
    // fm.remove("../../dir2/file1", 0); expected: 1
    // fm.create_dir("../../dir2/file1"); expected: 1
    // fm.remove("../../dir2/file1", 0); expected: 1
    // fm.create_file("../../dir2/file1", 1); expected: 1
    // fm.change_dir("."); expected: 1
    // fm.change_dir("/dir1/dir11"); expected: 1
    // fm.change_dir("/dir1/dir11/dir3"); expected: 0
    // fm.change_dir("/dir1"); expected: 1
    // fm.change_dir("./dir11"); expected: 1
    // fm.change_dir(".."); expected: 1
    // fm.create_file("ddir2/file1", 1000-1); expected: 0
    // fm.create_file("./dir11/file.txt", 1); expected: 1
    // fm.remove("dir11", 0); expected: 0
    // fm.remove("./dir11", 0); expected: 0
    // fm.remove("./dir11", 1); expected: 1
    // fm.get_cur_dir(cur_dir)
    // strcmp(cur_dir,"/dir1") == 0 || strcmp(cur_dir, "/dir1/") == 0; expected: 1
    // fm.destroy(); expected: 1

    // // TEST 3
    // /* Test 1 */
    // mm.create(TOTAL_SIZE); expected: 1
    // mm.create_file(b1,TOTAL_SIZE); expected: 1
    // mm.remove(b1,0); expected: 1
    // mm.remove(b1,0); expected: 0
    // mm.create_file(b2,TOTAL_SIZE/2); expected: 1
    // mm.create_file(b2,TOTAL_SIZE/2); expected: 0
    // mm.create_file(b3,TOTAL_SIZE/2); expected: 1
    // mm.list(".",0); expected: 1
    // mm.destroy(); expected: 1
    // /* Test 2 */
    // mm.create(TOTAL_SIZE); expected: 1
    // mm.create_dir("dir1"); expected: 1
    // mm.create_dir("dir1/dir11"); expected: 1
    // mm.change_dir("dir1"); expected: 1
    // mm.create_dir("../dir2"); expected: 1
    // mm.create_dir("../dir2/dir3"); expected: 1
    // mm.remove("/dir2/dir3", 0); expected: 1
    // mm.create_dir("/dir3/dir31"); expected: 0
    // mm.create_dir("../dir3/dir31"); expected: 0
    // mm.create_dir("../dir2"); expected: 0
    // mm.create_file("/dir2/file1", 1); expected: 1
    // mm.create_dir("/dir2/dir21"); expected: 1
    // mm.create_dir("/dir2/file1"); expected: 0
    // mm.create_dir("../dir2/file1"); expected: 0
    // mm.create_dir("../dir2/file1/dir"); expected: 0
    // mm.create_dir("../dir2/dir22"); expected: 1
    // mm.change_dir("dir2"); expected: 0
    // mm.change_dir("dir11"); expected: 1
    // mm.remove("../../dir2/file1", 0); expected: 1
    // mm.create_dir("../../dir2/file1"); expected: 1
    // mm.remove("../../dir2/file1", 0); expected: 1
    // mm.create_file("../../dir2/file1", 1); expected: 1
    // mm.change_dir("."); expected: 1
    // mm.change_dir("/dir1/dir11"); expected: 1
    // mm.change_dir("/dir1/dir11/dir3"); expected: 0
    // mm.change_dir("/dir1"); expected: 1
    // mm.change_dir("./dir11"); expected: 1
    // mm.change_dir(".."); expected: 1
    // mm.create_file("./dir11/file.txt", 1); expected: 1
    // mm.remove("dir11", 0); expected: 0
    // mm.remove("./dir11", 0); expected: 0
    // mm.remove("./dir11", 1); expected: 1
    // mm.list("..",0); expected: 1
    // mm.list("dir1",0); expected: 0
    // mm.list(".",0); expected: 1
    // mm.list("./dir11/file.txt",0); expected: 0
    // mm.create_file("file11.txt",1); expected: 1
    // mm.list("./file11.txt",0); expected: 1
    // mm.list("file11.txt",0); expected: 1
    // mm.create_file("../dir2/a_file22.txt",1); expected: 1
    // mm.list("../dir2",0); expected: 1
    // mm.list("../dir2",1); expected: 1
    // mm.list("../dir1 ../dir2",1); expected: 1
    // mm.list("../dir2 ../dir3",1); expected: 0
    // mm.destroy(); expected: 1
    // /* Test 3 */
    return 0;
}