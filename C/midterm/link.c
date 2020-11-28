
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



int main(int argc, char** argv){
  symlink (“test.txt”, “bb.txt”);
  unlink (“aa”);
}
