#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
  int age = 45;
  write (1, "Here is your age:", 17);
  write (1, (void*) &age, sizeof (int*));
}
//look into fseek() and lseek()
//stat and dir traversal
//recursive traversal
