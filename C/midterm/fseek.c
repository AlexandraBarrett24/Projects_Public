#include <stdio.h>

int main()
{
    FILE *fp;
    fp = fopen("test.txt", "r");

    // Moving pointer to end
    fseek(fp, 0, SEEK_SET);  //SEEK_END(end of file), SEEK_SET(start of file), SEEK_CUR(file pointer's cur position)
    // Printing position of pointer
    printf("%ld\n", ftell(fp));

    return 0;
}
