#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct ElfSection 
{
    char * start;
    char * end;
    int startInt;
    int endInt;
} ElfSection;

ElfSection * createElfSection()
{
    ElfSection * section = malloc(sizeof(ElfSection));
    section->start = malloc(sizeof(char) * 5);
    section->end = malloc(sizeof(char) * 5);
}

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
        ElfSection * section1 = createElfSection();
        ElfSection * section2 = createElfSection();

        bool gotSection1 = false;
        bool foundRange = false;

        for(int i = 0; i < strlen(line); i++)
        {
            if (line[i] == ',')
            {
                gotSection1 = true;
                foundRange = false;
                continue;
            }

            if (line[i] == '-')
            {
                foundRange = true;
                continue;
            }

            if (!gotSection1)
            {
                if (foundRange)
                    strncat(section1->end, &line[i], 1);
                else 
                    strncat(section1->start, &line[i], 1);
            }
            else
            {
                if (foundRange)
                    strncat(section2->end, &line[i], 1);
                else 
                    strncat(section2->start, &line[i], 1);
            }
        }
        populateInt(section1);
        populateInt(section2);

        result += findCompleteOverlap(section1, section2);

        int r = findPartialOverlap(section1, section2);
        printf("%s = %d\n", line, r);
        partialResult +=  findPartialOverlap(section1, section2);
    }

    printf("Total points #1 %ld\n", result);
    printf("Total points #2 %ld\n", partialResult);
    
    free(line);
    fclose(fp);
}