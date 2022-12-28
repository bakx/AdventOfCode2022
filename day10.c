#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <signal.h>

#define NUMELEM(a) (sizeof(a) / sizeof(*a))

void moveStack(int stack[], int *cycleValue)
{
    if (stack[0] != 0)
    {
        *cycleValue += stack[0];
        stack[0] = stack[1];
        stack[1] = stack[2];
        stack[2] = 0;
    }
    else if (stack[1] != 0)
    {
        stack[0] = stack[1];
        stack[1] = stack[2];
    }
    else if (stack[2] != 0)
    {
        stack[1] = stack[2];
        stack[2] = 0;
    }
}

void checkCycle(bool *isFirstCycle, int *currentCycle, int *result, int *stack, int *cycles, int *cycleValue)
{
    if ((*isFirstCycle && *currentCycle == 20) || (*currentCycle > 0 && *currentCycle % 40 == 0))
    {
        int res = ((*currentCycle == 20) ? 20 : 20 + 40 * *cycles) * *cycleValue;
        *result += res;

        printf("Cycle result, currentCycle=%d cycleValue=%d result=%d\n", *currentCycle, *cycleValue, res);

        *cycles+=1;
        *isFirstCycle = false;
        *currentCycle = 0;
    }
}

int main(int argc, const char *const argv[])
{
    FILE *fp = fopen("day10.txt", "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int cycleValue = 1;
    int currentCycle = 0;
    int result = 0;
    bool isFirstCycle = true;
    int cycles = 0;

    int moves[3] = {0};
    bool isNoop = false;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        moveStack(moves, &cycleValue);
        checkCycle(&isFirstCycle, &currentCycle, &result, moves, &cycles, &cycleValue);

        int inst;

        if (line[0] != 'n')
        {
            sscanf(line, "addx %d\n", &inst);
            moves[2] = inst;
            currentCycle++;
            moveStack(moves, &cycleValue);
            checkCycle(&isFirstCycle, &currentCycle, &result, moves, &cycles, &cycleValue);
        }
        else
            inst = 0;

        printf("%c %d - %d %d\n", line[0], inst, currentCycle, cycleValue);

        currentCycle++;
    }

    printf("#1 %d\n", result);
    // printf("Dir size #2 %lld\n", part2Dirsize);

    free(line);
    fclose(fp);
}
