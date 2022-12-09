#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

int findUniqueChars(char * line, int count)
{
    int i = 0;
    
    while(1)
    {
        bool unique = true;
        for (int j=0; j < count; j++)
        {
            char c = line[i + j];
            for (int k=0; k < count; k++)
            {
                if (k == j)
                    continue;

                if (c == line[i + k])
                {
                    unique = false;
                    break;
                }
            }

            if (!unique)
                break;
        }

        if (unique)
            break;

        i++;
    }

    return i + count;
}

int main(int argc, const char *const argv[])
{
    FILE * fp = fopen("day6.txt", "r");
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        printf("Result #1: %d\n", findUniqueChars(line, 4));
        printf("Result #2: %d\n", findUniqueChars(line, 14));
    }

    free(line);
    fclose(fp);
}