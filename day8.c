#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define NUMELEM(a) (sizeof(a) / sizeof(*a))
#define GRID_SIZE 99

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

typedef enum VIEW_DIRECTION
{
    VIEW_LEFT,
    VIEW_RIGHT,
    VIEW_TOP,
    VIEW_BOTTOM
} VIEW_DIRECTION;

typedef struct GridField
{
    short x;
    short y;
    short value;
    struct GridField *up;
    struct GridField *down;
    struct GridField *left;
    struct GridField *right;
    bool flaggedAsVisible;
} GridField;

typedef struct GridCorners
{
    struct GridField *left_top;
    struct GridField *right_top;
    struct GridField *left_bottom;
    struct GridField *right_bottom;
} GridCorners;

GridField *gridStart;
GridCorners *gridCorners;

GridField *createGridField(short x, short y, short value)
{
    GridField *gridField = malloc(sizeof(GridField));
    gridField->up = malloc(sizeof(GridField *));
    gridField->down = malloc(sizeof(GridField *));
    gridField->left = malloc(sizeof(GridField *));
    gridField->right = malloc(sizeof(GridField *));

    gridField->x = x;
    gridField->y = y;
    gridField->value = value;

    gridField->left = NULL;
    gridField->right = NULL;
    gridField->up = NULL;
    gridField->down = NULL;

    gridField->flaggedAsVisible = false;

    return gridField;
}

void printGrid(GridField *start)
{
    GridField *rowStart = start;
    GridField *field = start;

    while (1)
    {
        printf("%s", (field->flaggedAsVisible) ? KGRN : KRED);
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

GridField *getDirection(GridField *field, VIEW_DIRECTION direction)
{
    switch (direction)
    {
    case VIEW_LEFT:
        return field->right;
    case VIEW_RIGHT:
        return field->left;
    case VIEW_TOP:
        return field->down;
    case VIEW_BOTTOM:
        return field->up;
    }
}

int getVisible(GridField *start, VIEW_DIRECTION direction)
{
    int visible = 0;
    short heighestTree = 0;

    GridField *rowStart = start;
    GridField *field = start;

    while (1)
    {
        GridField *gridDirection = getDirection(field, direction);

        if (gridDirection)
        {
            if (field->value > heighestTree)
            {
                if (!field->flaggedAsVisible)
                {
                    //printf("Visible: x=%d y=%d val=%u\n", field->x, field->y, field->value);
                    visible++;
                    field->flaggedAsVisible = true;
                }
                heighestTree = field->value;
            }

            if (field->x == 0 || field->y == 0 || field->x + 1 == GRID_SIZE || field->y + 1 == GRID_SIZE)
            {
                if (!field->flaggedAsVisible)
                {
                    //printf("Visible: x=%d y=%d val=%u\n", field->x, field->y, field->value);
                    visible++;
                    field->flaggedAsVisible = true;
                }
            }

            field = gridDirection;
            continue;
        }

        if (!field->flaggedAsVisible)
        {
            //printf("Visible: x=%d y=%d val=%u\n", field->x, field->y, field->value);
            visible++;
            field->flaggedAsVisible = true;
        }

        bool endOfLine = false;

        switch (direction)
        {
        case VIEW_LEFT:
        case VIEW_RIGHT:
            if (!rowStart->down)
                endOfLine = true;
            else
                rowStart = rowStart->down;

            break;
        case VIEW_TOP:
        case VIEW_BOTTOM:
            if (!rowStart->right)
                endOfLine = true;
            else
                rowStart = rowStart->right;

            break;
        }

        if (endOfLine)
            break;

        field = rowStart;
        heighestTree = 0;
    }

    return visible;
}

int part1()
{
    int visible = 0;

    visible += getVisible(gridCorners->left_top, VIEW_LEFT);
    visible += getVisible(gridCorners->right_top, VIEW_RIGHT);
    visible += getVisible(gridCorners->left_bottom, VIEW_BOTTOM);
    visible += getVisible(gridCorners->left_top, VIEW_TOP);

    return visible;
}

int main(int argc, const char *const argv[])
{
    gridStart = malloc(sizeof(GridField *));
    gridCorners = malloc(sizeof(GridCorners));

    FILE *fp = fopen("day8.txt", "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int x = 0;
    int y = 0;
    int c;

    GridField *lastAdded;
    GridField **lastRow = malloc(sizeof(GridField *) * GRID_SIZE);

    while ((c = fgetc(fp)) != EOF)
    {
        if (c == 10)
        {
            if (y == 0)
                gridCorners->right_top = lastAdded;

            x = 0;
            lastAdded = NULL;
            y++;
            continue;
        }

        GridField *gridField = createGridField(x, y, c - 48);

        if (x == 0 && y == 0)
        {
            gridStart = gridField;
            gridCorners->left_top = gridField;
        }

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
        
        if (x == 0 && y + 1 == GRID_SIZE)
            gridCorners->left_bottom = gridField;

        lastAdded = gridField;
        lastRow[x] = gridField;

        x++;
    }

    gridCorners->right_bottom = lastAdded;


    printf("Visible #1 %d\n\n", part1());

    printGrid(gridStart);

    // while ((read = getline(&line, &len, fp)) != -1)
    //  {
    // }

    // printf("Sum #1 %lld\n", part1Filesize);
    // printf("Dir size #2 %lld\n", part2Dirsize);

    free(line);
    fclose(fp);
}
