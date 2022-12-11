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

    for (int i = 0; i < GRID_SIZE; i++)
    {
        insertIndex = i;

        if (tailVisits[i] == NULL)
            break;

        if (tailVisits[i]->x == x && tailVisits[i]->y == y)
            return;
    }

    TailVisit * tailVisit = malloc(sizeof(TailVisit));
    tailVisit->x = x;
    tailVisit->y = y;
    tailVisits[insertIndex] = tailVisit;
}

void move(char dir, int * x, int * y, int * tailX, int * tailY, int * steps)
{
    int * target =  (dir == 'U' || dir == 'D') ? y : x;


    for (int i = 0; i < *steps; i++)
    {
        target--;

        if (abs(y - tailY) > 1)
        {
            if (x != tailX)
                tailX = (x > tailX) ? +1 : -1;

            tailY--;
        }
        
        checkInsert(tailX, tailY);
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

    checkInsert(0,0); // Start position

    while ((read = getline(&line, &len, fp)) != -1)
    {
        char dir;
        int steps;
        sscanf(line, "%c %d\n", &dir, &steps);

        switch (dir)
        {
        case 'U':
            for (int i = 0; i < steps; i++)
            {
                y--;

                if (abs(y - tailY) > 1)
                {
                    if (x != tailX)
                        tailX = (x > tailX) ? +1 : -1;
    
                    tailY--;
                }
                
                checkInsert(tailX, tailY);
            }

            break;
        case 'R':
            for (int i = 0; i < steps; i++)
            {
                x++;

                if (abs(x - tailX) > 1)
                {
                    if (y != tailY)
                        tailY = (y > tailY) ? +1 : -1;
                                
                    tailX++;
                }
                
                checkInsert(tailX, tailY);
            }
            break;
        case 'D':
            for (int i = 0; i < steps; i++)
            {
                y++;

                if (abs(y - tailY) > 1)
                {
                    if (x != tailX)
                        tailX = (x > tailX) ? +1 : -1;

                    tailY++;
                }
                
                checkInsert(tailX, tailY);
            }
            break;
        case 'L':
            for (int i = 0; i < steps; i++)
            {
                x--;

                if (abs(x - tailX) > 1)
                {
                    if (y != tailY)
                        tailY = (y > tailY) ? +1 : -1;

                    tailX--;
                }

                checkInsert(tailX, tailY);
            }
            break;
        default:
            break;
        }
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
