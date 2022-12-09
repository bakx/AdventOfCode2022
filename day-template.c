#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUMELEM(a) (sizeof(a) / sizeof(*a))

int main(int argc, const char *const argv[])
{
    FILE *fp = fopen("day8.txt", "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fp)) != -1)
    {
    }

    //printf("Sum #1 %lld\n", part1Filesize);
    //printf("Dir size #2 %lld\n", part2Dirsize);

    free(line);
    fclose(fp);
}

