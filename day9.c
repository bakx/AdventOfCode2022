#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUMELEM(a) (sizeof(a) / sizeof(*a))
#define GRID_SIZE 10000

typedef struct TailVisit
{
    int x;
    int y;
} TailVisit;

TailVisit *tailVisits[GRID_SIZE];

void checkInsert(int x, int y)
{
    int insertIndex = 0;

    printf("%s-x=%d y=%d\n", __func__, x, y);

    for (int i = 0; i < GRID_SIZE; i++)
    {
        insertIndex = i;

        if (tailVisits[i] == NULL)
            break;

        if (tailVisits[i]->x == x && tailVisits[i]->y == y)
            return;
    }

    TailVisit *tailVisit = malloc(sizeof(TailVisit));
    tailVisit->x = x;
    tailVisit->y = y;
    tailVisits[insertIndex] = tailVisit;
}

void move(char dir, int * x, int * y, int * tailX, int * tailY, int * steps)
{
    int * target = (dir == 'U' || dir == 'D') ? y : x;
    int modifier = (dir == 'D' || dir == 'R') ? 1 : -1;

    for (int i = 0; i < *steps; i++)
    {
        *target += modifier;
        
        printf("step %c-x=%d y=%d\n", dir, *x, *y);

        if (dir == 'U' || dir == 'D')
        {
            if (abs(*x - *tailX) > 0 && abs(*y - *tailY) > 1) // check if not in line
                *tailX = *x;
            
            if (abs(*y - *tailY) > 1)
                *tailY += modifier;
        }
        else // L R
        {
            if (abs(*y - *tailY) > 0 && abs(*x - *tailX) > 1) // check if not in line
                *tailY = *y;
            
            if (abs(*x - *tailX) > 1)
                *tailX += modifier;
        }

        checkInsert(*tailX, *tailY);
    }
}

int main(int argc, const char *const argv[])
{
    FILE *fp = fopen("day9.txt", "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int x = 0;
    int y = 0;
    int tailX = 0;
    int tailY = 0;

    //checkInsert(0, 0); // Start position

    while ((read = getline(&line, &len, fp)) != -1)
    {
        char dir;
        int steps;
        sscanf(line, "%c %d\n", &dir, &steps);

        move(dir, &x, &y, &tailX, &tailY, &steps);
    }

    int visisted = 0;

    for (int i = 0; i < GRID_SIZE; i++)
    {
        if (!tailVisits[i])
            break;

        visisted++;
    }

    printf("#1 %d\n", visisted);
    // printf("Dir size #2 %lld\n", part2Dirsize);

    free(line);
    fclose(fp);
}
