#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct ElfSection 
{
    char start[5];
    char end[5];
    int startInt;
    int endInt;
} ElfSection;

void populateInt(ElfSection * section)
{
    char *ptr;
    section->startInt = strtol(section->start, &ptr, 10);
    section->endInt = strtol(section->end, &ptr, 10);
}

int findCompleteOverlap(ElfSection * section1, ElfSection * section2)
{
    if ( section1->startInt > section2->startInt)
        return section1->endInt <= section2->endInt;
    else if (section1->startInt == section2->startInt && section1->endInt == section2->endInt)
        return 1;
    else if ( section1->startInt == section2->startInt)
        return (section1->endInt > section2->endInt) ? section2->endInt < section1->endInt : section1->endInt < section2->endInt;
    else
        return section2->endInt <= section1->endInt;
}

int findPartialOverlap(ElfSection * section1, ElfSection * section2)
{
    if (section1->startInt > section2->startInt)
        return section2->endInt >= section1->startInt;
    else if (section1->startInt == section2->startInt && section1->endInt == section2->endInt)
        return 1;
    else if (section1->startInt == section2->startInt || section1->endInt == section2->endInt)
        return 1;
    else
        return section2->startInt <= section1->endInt;
}

int main(int argc, const char *const argv[])
{
    FILE * fp = fopen("day4.txt", "r");
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    long int result = 0;
    long int partialResult = 0;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        ElfSection section1;
        ElfSection section2;
        char range[5] = {0};

        bool gotSection1 = false;
        bool foundRange = false;

        for(int i = 0; i < strlen(line); i++)
        {
            if (line[i] == ',')
            {
                strcpy(section1.end, range);
                memset(range, 0, sizeof(range));

                gotSection1 = true;
                foundRange = false;
                continue;
            }

            if (line[i] == '-')
            {
                (gotSection1) ? strcpy(section2.start, range) : strcpy(section1.start, range);
                memset(range, 0, sizeof(range));

                foundRange = true;
                continue;
            }

            strncat(range, &line[i], 1);
        }
        
        strcpy(section2.end, range);
        
        populateInt(&section1);
        populateInt(&section2);

        result += findCompleteOverlap(&section1, &section2);
        partialResult +=  findPartialOverlap(&section1, &section2);
    }

    printf("Total points #1 %ld\n", result);
    printf("Total points #2 %ld\n", partialResult);
    
    free(line);
    fclose(fp);
}