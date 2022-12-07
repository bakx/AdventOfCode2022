#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct DirData
{
    char *name;
    long long int size;
    int depth;
    struct DirData * parent;
    struct DirData **dirs;
    struct FilesData **files;    
} DirData;

typedef struct FilesData
{
    char *name;
    long long int size;

} FilesData;

enum CURRENT_STATE
{
    cd,
    ls
};

#define NUMELEM(a) (sizeof(a) / sizeof(*a))
#define NAME_SIZE 100
#define FILES_SIZE 1000

DirData * root = NULL;

DirData *createDirData(char *name)
{
    DirData *dirData = malloc(sizeof(DirData));
    dirData->name = strdup(name);
    dirData->size = 0;
    dirData->depth = 0;
    dirData->parent = NULL;
    dirData->dirs = malloc(sizeof(DirData) * FILES_SIZE);
    dirData->files = malloc(sizeof(FilesData) * FILES_SIZE);

    return dirData;
}

FilesData *createFilesData(char *name, long int size)
{
    FilesData *filesData = malloc(sizeof(FilesData));
    filesData->name = strdup(name);
    filesData->size = size;
    return filesData;
}

void addFilesize(DirData * dirData, long int filesize)
{
    DirData * curDir = dirData;
    while(curDir)
    {
        curDir->size += filesize;
        if (curDir == root)
            break;

        curDir = curDir->parent;        
    }
}

long long int part1Filesize = 0;
long long int part2Dirsize = 0;
long long int part2Diff = 999999999999;

void part1(DirData * dirData)
{
    if (dirData->size < 100001)
        part1Filesize += dirData->size;

    for (int i = 0; i < FILES_SIZE; i++)
    {
        if (!dirData->dirs[i])
            return;
        
        part1(dirData->dirs[i]);
    }
}

void part2(DirData * dirData, long long int requiredSpace)
{
    long long int diff = requiredSpace - dirData->size;

    if (part2Diff == 999999999999)
    {
        part2Diff = diff;
        part2Dirsize = dirData->size;
    } 
    else
    {
        if (fabs(diff) < fabs(part2Dirsize - requiredSpace) && dirData->size >= requiredSpace)
        {
            printf("%s - %lld\n", dirData->name, dirData->size);
            part2Diff = diff;
            part2Dirsize = dirData->size;
        }
    }

    for (int i = 0; i < FILES_SIZE; i++)
    {
        if (!dirData->dirs[i])
            return;
        
        part2(dirData->dirs[i], requiredSpace);
    }
}

void printTree(DirData * dirData)
{
    if (!dirData->files[0])
        printf("Directory %s at depth %d with a size of %lld has no files\n", dirData->name, dirData->depth, dirData->size);
    else
    {
        printf("Directory %s at depth %d with a size of %lld has the following files:\n", dirData->name, dirData->depth, dirData->size);
        for (int i = 0; i < FILES_SIZE; i++)
        {
            if (!dirData->files[i])
                break;
            
            printf("File %s with size %lld\n", dirData->files[i]->name, dirData->files[i]->size);
        }
    }

    if (!dirData->dirs[0])
    {
        printf("Directory %s has no subdirectories\n", dirData->name);
        return;
    }

    printf("Directory %s has the following subdirectories:\n", dirData->name);
    for (int i = 0; i < FILES_SIZE; i++)
    {
        if (!dirData->dirs[i])
            break;
        
        printTree(dirData->dirs[i]);
    }
}

int main(int argc, const char *const argv[])
{
    FILE *fp = fopen("day7.txt", "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    root = createDirData("/");
    root->parent = root;
    DirData *curDir = root;

    enum CURRENT_STATE currentState;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        char dir[20] = "";

        //printf("curDir=%s | %s", curDir->name, line);
        //printTree(root);

        if (line[0] == '$') // Figure out command
        {
            if (line[2] == 'c' && line[3] == 'd' && line[4] == ' ')
            {
                currentState = cd;
                sscanf(line, "$ cd %s\n", dir);

                if (strcmp(dir, "/") == 0)
                    continue;

                if (strcmp(dir, "..") == 0)
                    curDir = curDir->parent;
                else
                {
                    for (int i = 0; i < FILES_SIZE; i++)
                    {
                        if (!curDir->dirs[i])
                        {
                            printf("ERROR: directory %s not found in %s\n", dir, curDir->name);
                            break;
                        }

                        if (strcmp(curDir->dirs[i]->name, dir) == 0)
                        {
                            curDir = curDir->dirs[i];
                            break;
                        }
                    }
                }
                continue;
            }

            if (line[2] == 'l' && line[3] == 's')
            {
                currentState = ls;
                continue;
            }
        }

        if (currentState == ls)
        {
            if (line[0] == 'd' && line[1] == 'i' && line[2] == 'r' && line[3] == ' ') // Check for dir instruction
            {
                sscanf(line, "dir %s\n", dir);

                DirData * newDir = NULL;

                for (int i = 0; i < FILES_SIZE; i++)
                {
                    if (!curDir->dirs[i])
                        break;

                    if (strcmp(curDir->dirs[i]->name, dir) == 0)
                    {
                        newDir = curDir->dirs[i];
                        break;
                    }
                }

                if (!newDir)
                {
                    newDir = createDirData(dir);
                    newDir->name = strdup(dir);
                    newDir->parent = curDir;
                    newDir->depth = curDir->depth + 1;

                    for (int i = 0; i < FILES_SIZE; i++)
                    {
                        if (!curDir->dirs[i])
                        {
                            curDir->dirs[i] = newDir;
                            break;
                        }
                    }
                }

                continue;
            }

            // Should be file
            char file[30] = "";
            long long int size = 0;
            sscanf(line, "%lld %s", &size, file);

            FilesData *fileData = createFilesData(file, size);
            fileData->name = strdup(file);
            fileData->size = size;

            addFilesize(curDir, size); // append filesize

            for (int i = 0; i < FILES_SIZE; i++)
            {
                if (!curDir->files[i])
                {
                    curDir->files[i] = fileData;
                    break;
                }
            }
        }
    }

    part1(root);
    printf("Sum #1 %lld\n", part1Filesize);

    part2(root, 30000000 - (70000000 - root->size));
    printf("Dir size #2 %lld\n", part2Dirsize);

    //printTree(root);

    free(line);
    fclose(fp);
}

