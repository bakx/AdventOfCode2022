#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <regex.h>

typedef struct Stack
{
    struct Node * next;
    struct Stack ** stack;
    int position;
    size_t size;
} Stack;

typedef struct Node 
{
    char * key;
    struct Node * next;
} Node;


Node * pop(Stack * stack)
{
    if (!stack->next)
    {
        printf("Unable to pop, no items");
        return NULL;
    }

    Node * prev = NULL;
    Node * next = stack->next;
    
    while (1)
    {
        if (!next->next)
            break;
        
        prev = next;
        next = next->next;
    }

    prev->next = NULL;
    return next;
}

void push(Stack * stack, Node * n)
{
    if (!stack->next)
    {
        stack->next = n;
        return;
    }

    Node * next = stack->next;
    
    while (1)
    {
        if (!next->next)
            break;
        
        next = next->next;
    }

    next->next = n;
}

Node * createNode(char * key)
{
    Node * n = malloc(sizeof(Node));
    n->key = strdup(key);
    n->next = NULL;
    return n;
}

int main(int argc, const char *const argv[])
{
    FILE * fp = fopen("day5.txt", "r");
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    regex_t regex;

    int reti  = regcomp(&regex, "^move ([0-9]+) from ([0-9]+) to ([0-9]+)\n$", REG_EXTENDED);

    long int result = 0;
    long int partialResult = 0;

    bool loadInstructions = false;
    bool init = false;

    Stack * stack = malloc(sizeof(Stack));

    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (!init) // Must be first line, do some init stuff
        {
            stack->size = read/4;
            stack->stack = malloc(sizeof(Stack*) * stack->size);

            for (int i=0; i < stack->size; i++)
                stack->stack[i] = malloc(sizeof(Stack));

            init = true;
        }

        if (read == 1)
        {
            loadInstructions = true;
            continue;
        }

        if (loadInstructions)
        {
            bool p1, p2, p3 = false;
            char m1[5], m2[5], m3[5];
             for(int i = 5; i < strlen(line); i++)
             {
                char c = line[i];

                if ( (int) c != 32 && ((int) c < 48 || (int) c > 57))
                    continue;

                if (line[i] == ' ')
                {
                    if (!p1) {
                        p1 = true;
                        continue;
                    }

                    if (!p2)
                    {
                        p2 = true;
                        continue;
                    }

                    if (!p3)
                    {
                        p3 = true;
                        continue;
                    }
                }

                if (!p1 && c != ' ')
                {
                    strcat(m1, &c);
                     continue;
                }

                if (!p2 && c != ' ')
                {
                    strcat(m2, &c);
                     continue;
                } 

                if (!p3 && c != ' ')
                {
                    strcat(m3, &c);
                     continue;
                } 
               
                


             }

          
            //move %d from %d to %d
            // todo

            continue;
        }

        for(int i = 0; i < strlen(line); i++)
        {
            if ( (int) line[i] < 65 || (int) line[i] > 90)
                continue;

            // Get position
            int position = i / stack->size;
            if (i == 1)
                position = 0;
            if (read - stack->size == i)
                position -= 1; // final object has no space

            Node * node = createNode(&line[i]);
            push(stack->stack[position], node);
        }
    }

    for (int i = 0; i < stack->size; i++)   // DEBUGGING 
    {
        Node * next = stack->stack[i]->next;
        printf("Stack %d has keys [", i + 1);

        while (1)
        {
            printf(" %c ", next->key[0]);

            if (!next->next)
                break;
        
            next = next->next;
        }
        
        printf("]\n");

    }
    printf("Total points #1 %ld\n", result);
    printf("Total points #2 %ld\n", partialResult);
    
    free(line);
    fclose(fp);
}