#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUMELEM(a) (sizeof(a) / sizeof(*a))
#define GRID_SIZE 5

typedef struct GridField
{
    short x;
    short y;
    short value;
    struct GridField * up;
    struct GridField * down;
    struct GridField * left;
    struct GridField * right;
} GridField;

GridField * createGridField(short x, short y, short value)
{
    GridField * gridField = malloc(sizeof(GridField));
    gridField->up = malloc(sizeof(GridField*));
    gridField->down = malloc(sizeof(GridField*));
    gridField->left = malloc(sizeof(GridField*));
    gridField->right = malloc(sizeof(GridField*));

    gridField->x = x;
    gridField->y = y;
    gridField->value = value;
    
    gridField->left = NULL;
    gridField->right = NULL;
    gridField->up = NULL;
    gridField->down = NULL;
    
    return gridField;
}

void printGrid(GridField * start)
{
    GridField * rowStart = start;
    GridField * field = start;

    while(1)
    {
        printf("%d", field->value);
        
        if (field->right)
        {
            field = field->right;
            continue;
        }

        printf("\n");
        
        if (rowStart->down)
        {
            field = rowStart->down;
            rowStart = field;
            continue;
        }

        break;
    }
}

int part1(GridField * start)
{
    int visible = 0;
    short heighestTree = 0;


    GridField * rowStart = start;
    GridField * field = start;

    // Left
    while(1)
    {
        if (field->right)
        {
            if (field->value > heighestTree)
            {
                visible++;
                heighestTree = field->value;
            }

            field = field->right;
            continue;
        }

        if (field->value > heighestTree)
            visible++;

        if (!rowStart->down)
            break;
        
        rowStart = rowStart->down;
        field = rowStart;
        heighestTree = 0;
    }

    return visible;
}

int getVisible(GridField * start, )
{
    int visible = 0;
    short heighestTree = 0;


    GridField * rowStart = start;
    GridField * field = start;

    // Left
    while(1)
    {
        if (field->right)
        {
            if (field->value > heighestTree)
            {
                visible++;
                heighestTree = field->value;
            }

            field = field->right;
            continue;
        }

        if (field->value > heighestTree)
            visible++;

        if (!rowStart->down)
            break;
        
        rowStart = rowStart->down;
        field = rowStart;
        heighestTree = 0;
    }

    return visible;
}

GridField * gridStart;

int main(int argc, const char *const argv[])
{
    gridStart = malloc(sizeof(GridField*));
    FILE *fp = fopen("day8.txt", "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int x = 0;
    int y = 0;
    int added = 0;
    int c;

    GridField * lastAdded;
    GridField ** lastRow = malloc(sizeof(GridField*) * GRID_SIZE);

    while ((c = fgetc(fp)) != EOF) {
        if (c == 10)
        {
            x = 0;
            lastAdded = NULL;
            y++;
            continue;
        }

        GridField * gridField = createGridField(x, y, c - 48);

        if (x == 0 && y == 0)
            gridStart = gridField;

        if (x > 0)
        {
            gridField->left = lastAdded;
            lastAdded->right = gridField;
        }

        if (y > 0)
        {
            gridField->up = lastRow[x];
            lastRow[x]->down = gridField;
        }

        lastAdded = gridField;
        lastRow[x] = gridField;
        
        added++;
        x++;
    }

    printGrid(gridStart);

    printf("Visible %d", part1(gridStart));

   // while ((read = getline(&line, &len, fp)) != -1)
  //  {
   // }

    //printf("Sum #1 %lld\n", part1Filesize);
    //printf("Dir size #2 %lld\n", part2Dirsize);

    free(line);
    fclose(fp);
}

