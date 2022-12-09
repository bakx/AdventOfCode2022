#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

bool hasCharacter(char * line,  char c);

char findDuplicate(char * comp1, char * comp2)
{
    for (int i = 0; i < strlen(comp1); i++)
    {
        for (int j = 0; j < strlen(comp2); j++)
        {
            if (comp1[i]==comp2[j])
                return comp1[i];
        }
    }
}

int getCharScore(char item)
{
    int ascii = (int) item;

    if (ascii > 96) // a starts at 97
        return ascii-96;
    
    return ascii-38; // A starts at 65 - 26 as A scores as 27
}

int processRucksacks(char ** rucksacks, int offset)
{
    for (int i = offset + 1; i < offset + 3; i++)
    {
        for (int j = 0; j < strlen(rucksacks[offset]); j++)
        {
            if (hasCharacter(rucksacks[offset + 1], rucksacks[offset][j]) && hasCharacter(rucksacks[offset + 2], rucksacks[offset][j]))
                return getCharScore(rucksacks[offset][j]);
        }
    }
    return 0;
}

bool hasCharacter(char * line,  char c)
{
    for (int i = 0; i < strlen(line); i++)
    {
        if (line[i] == c) return true;
    }

    return false;
}

int main(int argc, const char *const argv[])
{
    FILE * fp = fopen("day3.txt", "r");
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    long int result = 0;
    long int badgeResult = 0;

    char * rucksacks[6]; 
    
    for (int i = 0; i < 6; i++)
        rucksacks[i] = "";

    while ((read = getline(&line, &len, fp)) != -1)
    {
        char comp1[50] = {0};  
        char comp2[50] = {0};

        for(int i=0; i < read - 1;i++)
        {
            if (i < read / 2)
                comp1[i] = line[i];
            else
                comp2[i - read / 2 ] = line[i];
        }

        char dup = findDuplicate(comp1, comp2);
        result += getCharScore(dup);
        
        for (int i=0; i < 6; i++)
        {
            if (strlen(rucksacks[i]) > 0)
                continue;
            
            rucksacks[i] = strdup(line);
            break;
        }

        if (strlen(rucksacks[5]) > 0) { // Process rucksacks
            badgeResult += processRucksacks(rucksacks, 0);
            badgeResult += processRucksacks(rucksacks, 3);
            
            for (int i = 0; i < 6; i++) {
                free(rucksacks[i]);
                rucksacks[i] = "";
            }
        }
    }

    printf("Total points #1 %ld\n", result);
    printf("Total points #2 %ld\n", badgeResult);
    
    free(line);
    fclose(fp);
}