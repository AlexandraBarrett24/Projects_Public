#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

int numOfEntry(char* path) {
    int count=0;
    DIR* dir = opendir(path);
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;
        count++;
    }
    closedir(dir);
    return count;
}
int main(int argc, char** argv) {
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
    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;
        if (entry->d_type == DT_DIR) {
            char next[strlen(path) + strlen(entry->d_name) + 2];
            next[0] = '\0';
            strcat(next, path);
            strcat(next, "/");
            strcat(next, entry->d_name);
            printf("Directory: %s\n\tEntries: %d\n", entry->d_name, numOfEntry(next));
        }
        else if (entry->d_type == DT_REG) {
            struct stat* buf = (struct stat*)malloc(sizeof(struct stat));
            stat(entry->d_name, buf);
            printf("Regular File: %s\n\tOwner: %d\n\tSize: %f\n\tInode: %llu\n",
                entry->d_name, buf->st_uid, (double) buf->st_size, buf->st_ino);
            free(buf);
        }
        else {
            printf("File: %s\n\tType:%hhu\n", entry->d_name, entry->d_type);
        }
    }
    closedir(dir);
    return 0;
}
// char *fd = "myfile.txt";
// struct stat buf;
//
// stat(fd, &buf);
// int size = buf.st_size;
//
// printf("%d",size);

//or:
// char *fd = "myfile.txt";
// struct stat *buf;
// buf = malloc(sizeof(struct stat));
//
// stat(fd, buf);
// int size = buf->st_size;
//
// printf("%d",size);
// free(buf);
