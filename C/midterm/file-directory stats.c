/*
 * Recitation section number: 6
 * Breakout Group Number: 6
 * Jack Fornaro (forna032@umn.edu):
 * Nathan Hostager (hosta034@umn.edu):
 * Cole Thompson (thom6401@umn.edu):
 * Alexandra Barrett (barre684@umn.edu):
 * Member name (email address):
 * Member name (email address):
 * Member name (email address):
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

// int numOfEntries(char* path) {
//     /*
//         Count the number of entries in path
//     */
// }

int main(int argc, char** argv){
    if (argc < 2) {
        printf("Pass the path as an argument to the program");
        exit(1);
    }
    char* path = argv[1];

    DIR* dir = opendir(path);
    if(dir==NULL){
        printf("The path passed is invalid");
        return -1;
    }
    struct dirent* entry;
    /*
        Iterate through the elements in argv[1]
        Refer the ls example in slides if you have any doubts
    */
    while((entry = readdir(dir)) != NULL){

      struct stat entry_stat;
      // char this_entry[1024] = {'\0'};
      // strcat(this_entry, path);
      // strcat(this_entry, "/");
      // strcat(this_entry, entry->d_name);
      // stat(this_entry, &entry_stat);
      lstat(path, &entry_stat);

      if(entry->d_type == DT_DIR){
        printf("*Directory*\n");
        printf("%s\n", entry->d_name);
        printf("%d\n", entry->d_type);
        printf("%ld\n",((long) entry_stat.st_nlink) - 2);
      }
      if(entry->d_type == DT_REG){
        printf("*File*\n");
        printf("%s\n", entry->d_name);
        printf("%d\n", entry->d_type);
        printf("%ld\n", (long)entry_stat.st_uid);
        printf("%lld\n", (long long)entry_stat.st_size);
        printf("%ld\n", (long)entry_stat.st_ino);
      }
      else{
        printf("%s\n", entry->d_name);
      }
    }

    closedir(dir);
    return 0;
}
