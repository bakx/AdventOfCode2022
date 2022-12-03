#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

typedef struct elf 
{
    int elfNumber;
    long long calories;
} elf;

void swap(elf* xp, elf* yp)
{
    elf temp = *xp;
    *xp = *yp;
    *yp = temp;
}

int main(int argc, const char *const argv[])
{
    FILE * fp = fopen("day1.txt", "r");
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    int elvesCount = 0;
    elf** elves = malloc(sizeof(elf));
    elves[0] = malloc(sizeof(elf));
    elves[0]->elfNumber = 1;
    elves[0]->calories = 0;

    char *eptr;
    long int result;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (read > 1) // append to existing elf
        {
            result = strtol(line, &eptr, 10);
            elves[elvesCount]->calories += result;
        } 
        else // create new elf
        {
            elvesCount++;

            elf** elvesR = realloc(elves, sizeof(elf) * elvesCount);
            elvesR[elvesCount] = malloc(sizeof(elf));
            elvesR[elvesCount]->elfNumber = elvesCount + 1;
            elvesR[elvesCount]->calories = 0;
            elves = elvesR;
        }
    }

    long maxCalories = 0;
    long elfMostCalories = 0;

    for (int i=0; i < elvesCount; i++)
    {
        if (elves[i]->calories > maxCalories)
        {
            maxCalories = elves[i]->calories;
            elfMostCalories = i;
        }
    }

    int i, j;
    for (i = 0; i < elvesCount - 1; i++)
    {
        for (j = 0; j < elvesCount - i - 1; j++)
        {
            if (elves[j]->calories < elves[j + 1]->calories)
                swap(elves[j], elves[j + 1]);
        }
    }

    printf("Elf %d has most calories (%lld)\n", elves[elfMostCalories]->elfNumber, elves[elfMostCalories]->calories);
    printf("Sum of top 3 elves, %lld\n", elves[0]->calories + elves[1]->calories + elves[2]->calories);
    
     for (int i=0; i < elvesCount + 1; i++)
        free(elves[i]);
    
    free(elves);

    free(line);
    fclose(fp);
}